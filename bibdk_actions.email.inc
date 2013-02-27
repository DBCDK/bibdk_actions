<?php


/**
 * @param array $actions
 * @return array
 */
function bibdk_actions_get_email_actions($actions){
  $actions['email'] = array(
    '#type' => 'submit',
    '#value' => t('email_manifestation', array(), array('context' => 'bibdk_actions')),
    '#submit' => array('bibdk_actions_email_items'),
    '#weight' => 0,
    '#attributes' => array(
      'class' => array(
        'email',
      ),
    ),
  );

  return $actions;
}

/**
 * @param $form_id
 * @param $form_state
 */
function bibdk_actions_email_items($form_id, &$form_state){
  dpm('emailing...');
  drupal_set_message('hello world', 'status');
}

