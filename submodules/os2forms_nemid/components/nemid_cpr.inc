<?php

/**
 * @file
 * Webform module NemID CPR component.
 */

/**
 * Implements _webform_defaults_component().
 */
function _webform_defaults_nemid_cpr() {
  return array_merge_recursive(array(
    'extra' => array(
      'maxlength' => '10',
      'placeholder' => '070593-1234',
      'nemid_component_type' => OS2FORMS_NEMID_WEBFORM_COMPONENT_TYPE_PERSONAL,
    ),
  ), os2forms_nemid_webform_defaults_component());
}

/**
 * Implements _webform_edit_component().
 */
function _webform_edit_nemid_cpr($component) {
  return os2forms_nemid_webform_edit_component($component);
}

/**
 * Implements _webform_render_component().
 */
function _webform_render_nemid_cpr($component, $value = NULL, $filter = TRUE, $submission = NULL) {
  $element = os2forms_nemid_webform_render_component($component, $value, $filter, $submission);

  if (isset($_SESSION['nemid_login']['cpr'])) {
    $element['#default_value'] = $_SESSION['nemid_login']['cpr'];
  }
  elseif (isset($value[0])) {
    $element['#default_value'] = $value[0];
  }

  return $element;
}

/**
 * Implements _webform_display_component().
 */
function _webform_display_nemid_cpr($component, $value, $format = 'html', $submission = array()) {
  return os2forms_nemid_webform_display_component($component, $value, $format, $submission);
}

/**
 * Implements _webform_analysis_component().
 */
function _webform_analysis_nemid_cpr($component, $sids = array(), $single = FALSE, $join = NULL) {
  return os2forms_nemid_webform_analysis_component($component, $sids, $single, $join);
}

/**
 * Implements _webform_table_component().
 */
function _webform_table_nemid_cpr($component, $value) {
  return os2forms_nemid_webform_table_component($component, $value);
}

/**
 * Implements _webform_action_set_component().
 */
function _webform_action_set_nemid_cpr($component, &$element, &$form_state, $value) {
  os2forms_nemid_webform_action_set_component($component, $element, $form_state, $value);
}

/**
 * Implements _webform_csv_headers_component().
 */
function _webform_csv_headers_nemid_cpr($component, $export_options) {
  return os2forms_nemid_webform_csv_headers_component($component, $export_options);
}

/**
 * Implements _webform_csv_data_component().
 */
function _webform_csv_data_nemid_cpr($component, $export_options, $value) {
  return os2forms_nemid_webform_csv_data_component($component, $export_options, $value);
}

/**
 * Implements _form_builder_webform_form_builder_types_component().
 */
function _form_builder_webform_form_builder_types_nemid_cpr() {
  return os2forms_nemid_form_builder_webform_form_builder_types_nemid_com_name('nemid_cpr', t('CPR'));
}

/**
 * Implements _form_builder_webform_form_builder_map_component().
 */
function _form_builder_webform_form_builder_map_nemid_cpr() {
  return array(
    'form_builder_type' => 'nemid_cpr',
  ) + os2forms_nemid_form_builder_webform_form_builder_map_component();
}

/**
 * Implements _form_builder_webform_form_builder_properties_component().
 */
function _form_builder_webform_form_builder_properties_nemid_cpr() {
  return os2forms_nemid_form_builder_webform_form_builder_properties_component();
}
