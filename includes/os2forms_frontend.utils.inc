<?php

/**
 * @file
 * Os2forms_frontend.utils.inc.
 */

/**
 * Adds submission to the list of unanonymized submissions.
 *
 * @param mixed $submission
 *   Submission object.
 */
function os2forms_frontend_add_unanonymized_submission($submission) {
  $data = array();

  $data['sid'] = $submission->sid;
  $data['uuid'] = $submission->uuid;
  $data['webform_nid'] = $submission->nid;

  $uuid = entity_get_uuid_by_id('node', array($submission->nid));
  if ($uuid && is_array($uuid)) {
    $data['webform_uuid'] = reset($uuid);
  }

  $data['synched'] = 0;

  db_insert('os2forms_frontend_unanonymized_submissions')
    ->fields($data)
    ->execute();
}

/**
 * Deletes submission from the of unanonymized submissions.
 *
 * @param string $uuid
 *   The uuid of the submission.
 *
 * @return int number of affected rows.
 */
function os2forms_frontend_delete_unanonymized_submission($uuid) {
  $row_affected = db_delete('os2forms_frontend_unanonymized_submissions')
    ->condition('uuid', $uuid)
    ->execute();

  return $row_affected;
}

/**
 * Marks unanonymized submission as synched.
 *
 * @param string $uuid
 *   The uuid of the submission.
 *
 * @return int number of affected rows.
 */
function os2forms_frontend_mark_submission_synched($uuid) {
  $data = array();
  $data['synched'] = 1;

  $row_affected = db_update('os2forms_frontend_unanonymized_submissions')
    ->fields($data)
    ->condition('uuid', $uuid)
    ->execute();

  return $row_affected;
}

/**
 * Returns the list of unanonymized submissions.
 *
 * @param int $synched
 *   Which submission to return, 0 indicated unsynched, 1 indicates synched.
 * @param array $pager. If empty all entries will be returned.
 *   Format:
 *   array(
 *     'page' => Number of the page.
 *     'pagesize' => Size of the page.
 *   )
 *
 * * @return array of submissions.
 *
 */
function os2forms_frontend_fetch_unanonymized_submissions($synched = 0, $pager = array()) {
  $query = db_select('os2forms_frontend_unanonymized_submissions', 'o')
    ->fields('o')
    ->condition('synched', $synched);

  if ($pager) {
    $page = $pager['page'];
    $pagesize = $pager['pagesize'];
    $query->range($page * $pagesize, $pagesize);
  }

  $result = $query
    ->execute()
    ->fetchAll();

  return $result;
}

/**
 * Anonymizes a submission of a webform.
 *
 * @param int $webform_nid
 *   Nid of the webform.
 * @param $sid
 *   Sid of the submission.
 */
function os2forms_frontend_anonymize_submission($webform_nid, $sid) {
  $webform = node_load($webform_nid);
  $submission = webform_menu_submission_load($sid, $webform_nid);

  // Iterate through all components and let each do cleanup if necessary.
  foreach ($webform->webform['components'] as $cid => $component) {
    if (isset($submission->data[$cid])) {
      webform_component_invoke($component['type'], 'delete', $component, $submission->data[$cid]);
    }
  }

  // Emptying the fields.
  foreach($submission->data as $key => $element) {
    $submission->data[$key] = array();
  }

  // Updating the submission with empty data.
  webform_submission_update($webform, $submission);

  // Deleting the submission from a queue.
  os2forms_frontend_delete_unanonymized_submission($submission->uuid);
}

/**
 * Get os2forms_frontend setting for the specified webform.
 *
 * @param int $nid
 *   Node is of the node.
 * @param string $submodule
 *   Name of the submodule.
 *
 * @return array
 *   Settings stored for this submodule.
 */
function os2forms_frontend_webform_get_setting($nid, $submodule) {
  $default = db_select('os2forms_frontend_webform_settings', 'of')
    ->fields('of')
    ->condition('nid', $nid, '=')
    ->condition('submodule', $submodule, '=')
    ->execute()
    ->fetchAssoc();

  if ($default) {
    $data = unserialize($default['data']);
    return $data;
  }

  return array();
}

/**
 * Update os2forms_frontend setting for the specified webform.
 *
 * If data is empty, setting entry will be deleted instead.
 *
 * @param int $nid
 *   Node is of the node.
 * @param string $submodule
 *   Name of the submodule.
 * @param array $data
 *   Array of data to be saved.
 */
function os2forms_frontend_webform_save_setting($nid, $submodule, $data) {
  $query = db_select('os2forms_frontend_webform_settings');
  $query->condition('nid', $nid);
  $query->condition('submodule', $submodule);
  $query->addExpression('COUNT(*)');
  $rows = $query->execute()->fetchField();

  $fields = array(
    'nid' => $nid,
    'submodule' => $submodule,
    'data' => serialize($data),
  );

  if ($rows > 0) {
    if (!empty($data)) {
      db_update('os2forms_frontend_webform_settings')
        ->fields($fields)
        ->condition('nid', $nid)
        ->condition('submodule', $submodule)
        ->execute();
    }
    else {
      os2forms_frontend_webform_delete_setting($nid, $submodule);
    }
  }
  else {
    if (!empty($data)) {
      db_insert('os2forms_frontend_webform_settings')
        ->fields($fields)
        ->execute();
    }
  }
}

/**
 * Removes os2forms_frontend setting for the specified webform.
 *
 * @param int $nid
 *   Node is of the node.
 * @param string $submodule
 *   Name of the submodule.
 */
function os2forms_frontend_webform_delete_setting($nid, $submodule = NULL) {
  $query = db_delete('os2forms_frontend_webform_settings')
    ->condition('nid', $nid, '=');

  if ($submodule) {
    $query->condition('submodule', $submodule, '=');
  }

  $query->execute();
}

/**
 * Removes os2forms_frontend setting for submodule.
 *
 * @param string $submodule
 *   Name of the submodule.
 */
function os2forms_frontend_submodule_delete_setting($submodule) {
  $query = db_delete('os2forms_frontend_webform_settings')
    ->condition('submodule', $submodule, '=');
  $query->execute();
}
