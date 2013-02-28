<?php


/**
 * @param array $actions
 * @return array
 */
function bibdk_actions_get_email_actions($actions) {
  $actions['email'] = array(
    '#type' => 'submit',
    '#value' => t('email_manifestation', array(), array('context' => 'bibdk_actions')),
    '#submit' => array('bibdk_actions_email_items'),
    '#weight' => 0,
    '#attributes' => array(
      'class' => array(
        'cart-action-btn', 'disabled'
      ),
    ),
  );

  return $actions;
}

/**
 * @param $form_id
 * @param $form_state
 */
function bibdk_actions_email_items($form_id, &$form_state) {
  $manifestations = bibdk_actions_parse_form_state($form_state);
  bibdk_action_email_generate_email($manifestations, $view);
}

/**
 * @param $manifestations
 * @return bool
 */
function bibdk_action_email_generate_email($manifestations) {
  if (empty($manifestations)) {
    return FALSE;
  }

  $view = array();
  foreach ($manifestations as $key => $manifestion) {
    $view[] = ting_openformat_manifestation_view($manifestion, 'email');
  }
  #dpm($view);
  #dpm(drupal_render($view));
  return 'hest';
}

function bibdk_action_send_email(){

}

