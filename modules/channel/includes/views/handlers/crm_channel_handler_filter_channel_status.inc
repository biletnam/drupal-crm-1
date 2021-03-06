<?php

/**
 * Filter by product type.
 */
class crm_channel_handler_filter_channel_status extends views_handler_filter_in_operator {
  // Display a list of product types in the filter's options.
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Status type');
      $this->value_options = crm_channel_status_option_list();
    }
  }

  function expose_form(&$form, &$form_state) {
    parent::expose_form($form, $form_state);
    $form['value']['#type'] = 'select';
  }
}
