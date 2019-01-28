<?php

/**
 * @file
 * Bellcom webform address autocomplete component.
 */

module_load_include('inc', 'webform', 'components/textfield');

/**
 * Implements _webform_defaults_component().
 */
function _webform_defaults_address_autocomp() {
  $defaults = _webform_defaults_textfield();
  $defaults['extra']['mode'] = '';
  return $defaults;
}

/**
 * Implements _webform_edit_component().
 *
 * Used in on component edit page.
 */
function _webform_edit_address_autocomp($component) {
  $form = _webform_edit_textfield($component);

  $form['mode'] = array(
    '#type' => 'radios',
    '#title' => t('Autocomplete mode'),
    '#required' => TRUE,
    '#default_value' => ($component['extra']['mode']) ? $component['extra']['mode'] : 'address',
    '#options' => array(
      'address' => t('Adresse'),
      'block' => t('Ejerlav'),
      'matrikula' => t('Jordstykker'),
    ),
    '#parents' => array('extra', 'mode'),
  );

  return $form;
}

/**
 * Configuration form for the "mode" property.
 *
 * Used in form_builder.
 */
function bellcom_webform_address_autocomplete_property_mode_form(&$form_state, $form_type, $element, $property) {
  $form = array();

  $form['mode'] = array(
    '#form_builder' => array('property_group' => 'options'),
    '#type' => 'radios',
    '#title' => t('Autocomplete mode'),
    '#required' => TRUE,
    '#default_value' => ($element['#mode']) ? $element['#mode'] : 'address',
    '#options' => array(
      'address' => t('Adresse'),
      'block' => t('Ejerlav'),
      'matrikula' => t('Jordstykker'),
    ),
    '#weight' => 2,
  );

  return $form;
}

/**
 * Implements _webform_render_component().
 */
function _webform_render_address_autocomp($component, $value = NULL, $filter = TRUE, $submission = NULL) {
  $element = _webform_render_textfield($component, $value, $filter, $submission);

  $mode = $component['extra']['mode'];
  if (!$mode) {
    $mode = 'address';
  }

  $element['#autocomplete_path'] = "bellcom_webform_address_autocomplete/$mode";
  $element['#element_validate'] = array('bellcom_webform_address_address_autocomplete_validate');

  return $element;
}

/**
 * Implements _webform_display_component().
 */
function _webform_display_address_autocomp($component, $value, $format = 'html', $submission = array()) {
  return _webform_display_textfield($component, $value, $format, $submission);
}

/**
 * Implements _webform_analysis_component().
 */
function _webform_analysis_address_autocomp($component, $sids = array(), $single = FALSE, $join = NULL) {
  return _webform_analysis_textfield($component, $sids, $single, $join);
}

/**
 * Implements _webform_table_component().
 */
function _webform_table_address_autocomp($component, $value) {
  return _webform_table_textfield($component, $value);
}

/**
 * Implements _webform_action_set_component().
 */
function _webform_action_set_address_autocomp($component, &$element, &$form_state, $value) {
  _webform_action_set_textfield($component, $element, $form_state, $value);
}

/**
 * Implements _webform_csv_headers_component().
 */
function _webform_csv_headers_address_autocomp($component, $export_options) {
  return _webform_csv_headers_textfield($component, $export_options);
}

/**
 * Implements _webform_csv_data_component().
 */
function _webform_csv_data_address_autocomp($component, $export_options, $value) {
  return _webform_csv_data_textfield($component, $export_options, $value);
}

/**
 * A Drupal Form API Validation function.
 *
 * Validates the entered value against the values returned by webservice.
 *
 * @param array $element
 *   The form element.
 * @param array $form_state
 *   The full form state for the webform.
 */
function bellcom_webform_address_address_autocomplete_validate($element, &$form_state) {
  // Trim spaces for basic cleanup.
  $value = trim($element['#value']);
  form_set_value($element, $value, $form_state);

  if ($value != '') {
    $mode = $element['#webform_component']['extra']['mode'];

    $search_value = $value;

    // Special validation for 'block' mode.
    if ($mode === 'block') {
      // Making a search value from e.g. '1070455 Testrup, Kattrup' to be
      // 'Testrup, Kattrup', because the webservice is not capable of searching,
      // when code is present in the search string.
      $search_value = preg_replace('/^\d* /', '', $value);
    }

    if (!in_array($value, bellcom_webform_address_perform_request($mode, $search_value))) {
      form_error($element, t('!name: "!value" has been changed. Only values from list are allowed.', array('!name' => $element['#title'], '!value' => $value)));
    }
  }
}

/**
 * Implements _form_builder_webform_form_builder_types_component().
 */
function _form_builder_webform_form_builder_types_address_autocomp() {
  $fields = array();

  $fields['address_autocomp'] = array(
    'title' => t('Address autocomplete'),
    'weight' => -20,
  );
  $component['name'] = t('New address');
  $fields['address_autocomp']['default'] = _form_builder_webform_default('address_autocomp', array(), $component);


  return $fields;
}

/**
 * Implements _form_builder_webform_form_builder_map_component().
 */
function _form_builder_webform_form_builder_map_address_autocomp() {
  $mapping = _form_builder_webform_form_builder_map_textfield();
  $mapping['form_builder_type'] = 'address_autocomp';
  $mapping['properties']['mode'] = array(
    'storage_parents' => array('extra', 'mode'),
  );

  return $mapping;
}

/**
 * Implements _form_builder_webform_form_builder_properties_component().
 */
function _form_builder_webform_form_builder_properties_address_autocomp() {
  return _form_builder_webform_form_builder_properties_textfield();
}
