<?php

/**
 * @file
 * Default entity form.
 */

/**
 * Form wrapper for create, update and delete.
 *
 * @param array $channel
 *   Channel object or array of default values.
 * @param bool $delete
 *   Boolean indicator if create/edit or delete form should be displayed.
 *
 * @return array
 */
function crm_channel_ui_form_wrapper($channel = array(), $delete = FALSE, $type = NULL) {
  if ($delete == TRUE && isset($channel->channel_id)) {
    return drupal_get_form('crm_channel_ui_delete_form', $channel);
  }
  if (empty($channel))  {
    $channel = entity_create('crm_channel', array(
      'type' => $type,
    ));
  }
  return drupal_get_form('crm_channel_ui_form', $channel);
}

/**
 * Channel form.
 */
function crm_channel_ui_form($form, &$form_state, $channel) {

  $form['channel'] = array(
    '#type' => 'value',
    '#value' => $channel,
  );

  field_attach_form('crm_channel', $channel, $form, $form_state);

  $form['settings'] = array(
    '#type' => 'vertical_tabs',
    '#weight' => 98,
  );
  $form['settings']['options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Options'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['settings']['options']['status'] = array(
    '#type' => 'select',
    '#title' => t('Status'),
    '#required' => TRUE,
    '#default_value' => $channel->status,
    '#options' => crm_channel_status_option_list(),
  );

  $form['settings']['revision'] = array(
    '#type' => 'fieldset',
    '#title' => t('Revision'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['settings']['revision']['log'] = array(
    '#type' => 'textarea',
    '#title' => t('Revision log'),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

/**
 * Channel form validation
 */
function crm_channel_ui_form_validate($form, &$form_state) {
  field_attach_form_validate('crm_channel', $form_state['values']['channel'], $form, $form_state);
}

/**
 * Channel form submit callback.
 */
function crm_channel_ui_form_submit($form, &$form_state) {
  $channel = $form_state['values']['channel'];

  $channel->status = $form_state['values']['status'];
  $channel->log = $form_state['values']['log'];
  if (!isset($channel->revision)) {
    $channel->revision = TRUE;
  }
  try {
    field_attach_submit('crm_channel', $channel, $form, $form_state);
    entity_save('crm_channel', $channel);
    $uri = entity_uri('crm_channel', $channel);
    $form_state['redirect'] = $uri['path'];
    drupal_set_message(t('Channel @label has been saved.', array('@label' => entity_label('crm_channel', $channel))));
  }
  catch (Exception $e) {
    watchdog_exception('crm_channel', $e);
    $form_state['rebuild'] = TRUE;
    drupal_set_message(t('Unable to save channel @label.', array('@label' => entity_label('crm_channel', $channel))), 'error');
  }

}

/**
 * Delete confirmation form.
 */
function crm_channel_ui_delete_form($form, &$form_state, $channel) {
  $form['channel'] = array(
    '#type' => 'value',
    '#value' => $channel,
  );
  return confirm_form(
    $form,
    t('Do you want to delete channel?'),
    'admin/crm/channel'
  );
}

/**
 * Delete form submit handler.
 */
function crm_channel_ui_delete_form_submit($form, &$form_state) {
  $channel_id = entity_id('crm_channel', $form_state['values']['channel']);
  $label = entity_label('crm_channel', $form_state['values']['channel']);
  if (entity_delete('crm_channel', $channel_id) !== FALSE) {
    drupal_set_message(t('Channel @label has been deleted.', array('@label' => $label)));
    $form_state['redirect'] = 'admin/crm/channel';
  }
  else {
    drupal_set_message(t('Unable to delete chanel @label.', array('@label' => $label)), 'error');
    $form_state['rebuild'] = TRUE;
  }
}

/**
 *
 */
function crm_channel_ui_add() {
  $build = array();
  $items = array();
  $options = array();
  if (isset($_GET['destination'])) {
    $options['query'] = drupal_get_destination();
  }

  foreach (crm_channel_type_option_list() as $type => $name) {
    $type_arg = strtr($type, '_', '-');
    $items[] = array(
      'data' => l($name, 'admin/crm/channel/add/' . $type_arg, $options),
      'class' => array('crm-channel', $type_arg),
    );
  }

  $build['list'] = array(
    '#theme' => 'item_list',
    '#items' => $items,
    '#attributes' => array(
      'class' => array(
        'crm-channel-ui-type-list',
      ),
    ),
  );

  $build['#attached']['css'][] = drupal_get_path('module', 'crm_channel_ui') . '/assets/crm_channel_ui.types.css';

  return $build;
}
