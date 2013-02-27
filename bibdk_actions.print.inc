<?php

/**
 * @param array $actions
 * @return array
 */
function bibdk_actions_get_print_actions($actions){
  $actions['print'] = array(
    '#type' => 'submit',
    '#value' => t('print_manifestation', array(), array('context' => 'bibdk_actions')),
    '#submit' => array('bibdk_actions_print_items'),
    '#weight' => 0,
  );

  return $actions;
}

/**
 * @param array $form_id
 * @param array $form_state
 */
function bibdk_actions_print_items($form_id, &$form_state){
  dpm('printing...');
}
