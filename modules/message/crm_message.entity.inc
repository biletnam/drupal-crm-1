<?php

/**
 * @file
 *
 */

/**
 * Implements hook_entity_info().
 */
function crm_message_entity_info() {
  $entities['crm_message'] = array(
    'label' => t('Message', array(), array('context' => 'crm')),
    'controller class' => 'CRMMessageController',
    'base table' => 'crm_message',
    'revision table' => 'crm_message_revision',
    'load hook' => 'crm_message_load',
    'uri callback' => 'crm_message_uri',
    'label callback' => 'crm_message_label',
    'module' => 'crm_message',
    'fieldable' => TRUE,
    'entity keys' => array(
      'id' => 'message_id',
      'bundle' => 'type',
      'revision' => 'revision_id',
    ),
    'bundle keys' => array(
      'bundle' => 'type',
    ),
    'bundles' => array(),
    'view modes' => array(
      'administrator' => array(
        'label' => t('Administrator'),
        'custom settings' => FALSE,
      ),
      'customer' => array(
        'label' => t('Customer'),
        'custom settings' => FALSE,
      ),
    ),
    'access callback' => 'crm_message_access',
  );
  return $entities;
}

/**
 * Check entity access.
 */
function crm_message_access($op, $entity = NULL, $account = NULL) {
  if (is_null($account)) {
    global $user;
    $account = clone $user;
  }
  $access = array();
  $access = array_merge_recursive($access, module_invoke_all('crm_message_access', $op, $entity, $account));
  $access[] = entity_get_controller('crm_message')->access($op, $entity, $account);
  return (in_array(CRM_ACCESS_ALLOW, $access, TRUE) && !in_array(CRM_ACCESS_DENY, $access, TRUE));
}

/**
 *
 */
function crm_message_load($id) {
  if (!is_numeric($id)) {
    return FALSE;
  }
  $entities = crm_message_load_multiple(array($id));
  if ($entities) {
    return reset($entities);
  }
}

/**
 *
 */
function crm_message_load_multiple($ids) {
  return entity_load('crm_message', $ids);
}

/**
 * @param $ids
 */
function crm_message_delete($id) {
  crm_message_delete(array($id));
}

/**
 * @param $ids
 */
function crm_message_delete_multiple($ids) {
  entity_delete_multiple('crm_message', $ids);
}

/**
 *
 */
function crm_message_view($message, $view_mode = 'customer') {
  if (!is_array($message)) {
    $message = array($message->message_id => $message);
  }
  return entity_get_controller('crm_message')->view($message, $view_mode);
}

/**
 * @todo: Move uri to UI module.
 */
function crm_message_uri($message) {
  return array(
    'path' => 'admin/crm/message/' . $message->message_id,
  );
}

/**
 *
 */
function crm_message_label($message) {
  $wrapper = entity_metadata_wrapper('crm_message', $message);
  if ($wrapper->__isset('crm_channel_single') && $label = $wrapper->crm_channel_single->label()) {
    return t('Message to @channel', array('@channel' => $label));
  }
  return '';
}
