<?php


/**
 * Defines the link for
 *
 * @param array $actions
 * @return array
 */
function bibdk_actions_get_email_actions($actions) {
  $actions['email'] = array(
    '#theme' => 'link',
    '#text' => t('email_manifestation', array(), array('context' => 'bibdk_actions')),
    '#path' => 'email/cart',
    '#weight' => 0,
    '#options' => array(
      'query' => NULL,
      'attributes' => array(
        'class' => array(
          'cart-action-btn', 'inactive'
        ),
      ),
      'html' => FALSE,
    ),
  );

  return $actions;
}

/**
 * Callback function when user clicks print button in cart view
 *
 * @param $ids
 * @return bool|string
 */
function bibdk_actions_email_page($ids) {
  if (empty($ids)) {
    return FALSE;
  }

  $ids = explode(',', $ids);
  $manifestations = bibdk_actions_get_manifestations($ids);

  $title = '';
  $content = array();
  foreach ($manifestations as $manifestation) {
    $view = ting_openformat_manifestation_view($manifestation, 'email');
    $view['display'] = bibdk_actions_get_fieldinfo($view);
    $view['#theme'] = 'bibdk_actions_email';
    //TODO no hest!
    $content[]['#markup'] = theme('bibdk_actions_email', array('hest' => $view['display']));
    if(empty($title)){
      $title = $manifestation->getTitle();
    }
  }

  $form_values['content'] = $content;
  $form_values['title'] = $title;

  $mail_form = drupal_get_form('bibdk_actions_mail_form', $form_values);
  return $mail_form;
}

function bibdk_actions_get_fieldinfo($view){
  $display_strings = array();
  if (isset($view['#fields']) && is_array($view['#fields'])) {
    foreach ($view['#fields'] as $key => $field) {
      if (isset($field[0]['#markup']) && isset($field['#title'])) {
        $display_strings[] = $field['#title'] . ': ' . $field[0]['#markup'];
      }
    }
  }
  return $display_strings;
}

/**
 * Creates the bibdk_actions_mail_form form
 *
 * @param $form_id
 * @param $form_state
 * @internal param $form
 * @return mixed
 */
function bibdk_actions_mail_form($form_id, $form_state) {
  global $user;
  $form['recipient'] = array(
    '#type' => 'textfield',
    '#title' => t('recipients_email_adress', array(), array('context' => 'bibdk_actions')),
    '#default_value' => ($user->uid != 0) ? $user->mail : '',
    '#required' => TRUE,
  );

  $form['subject'] = array(
    '#type' => 'textarea',
    '#title' => t('message_for_recipient', array(), array('context' => 'bibdk_actions')),
    '#required' => FALSE,
  );

  $form['note'] = array(
    '#type' => 'textfield',
    '#title' => t('note_for_recipient', array(), array('context' => 'bibdk_actions')),
    '#required' => FALSE,
  );

  $form['send'] = array(
    '#type' => 'submit',
    '#submit' => array('bibdk_actions_mail_form_submit'),
    '#value' => t('send_email', array(), array('context' => 'bibdk_actions')),
  );

  return $form;
}

/**
 * Validates form elements in bibdk_actions_mail_form
 *
 * @param $form
 * @param $form_state
 */
function bibdk_actions_mail_form_validate($form, &$form_state) {
  if (!isset($form_state['values']['recipient']) || !valid_email_address($form_state['values']['recipient'])) {
    $error_msg = t('email_adress_not_formatted_properly', array(), array('context' => 'bibdk_actions'));
    form_set_error('recipient', $error_msg);
  }
}

/**
 * @param $form_id
 * @param $form_state
 */
function bibdk_actions_mail_form_submit($form_id, &$form_state) {
  $params['form_values'] = $form_state['values'];
  $params['manifestations'] = $form_state['build_info']['args'][0]['content'];
  $params['title'] = $form_state['build_info']['args'][0]['title'];
  bibdk_actions_mail_send($params);
}

/**
 * @param $params
 * @internal param $form_values
 * @internal param $manifestations
 */
function bibdk_actions_mail_send($params) {
  $module = 'bibdk_actions';
  $key = 'mail_cart_items';
  $to = 'mmj@dbc.dk'; #$form_values['email'];

  global $language;

  $from = 'noreply@bibliotek.dk';
  $send = TRUE;

  $result = drupal_mail($module, $key, $to, $language, $params, $from, $send);
  if ($result['result'] == TRUE) {
    drupal_set_message(t('Your message has been sent.'));
  }
  else {
    drupal_set_message(t('There was a problem sending your message and it was not sent.'), 'error');
  }
}

/**
 * Implements hook_mail().
 */
function bibdk_actions_mail($key, &$message, $params) {
  //TODO include notes and custom messages from form into final message
  $options = array(
    'langcode' => $message['language']->language,
    'context' => 'bibdk_actions'
  );

  $subject_intro = t('manifestations_from @site-name', array('@site-name' => variable_get('site_name', t('email_site_name', array(), array('context' => 'bibdk_actions')))), $options);
  $subject_title = isset($params['title']) ? ' - ' . $params['title'] : '';
  $subject_more = count($params['manifestations']) ? ' - ' . t('and_other_titles', array(), array('context' => 'bibdk_actions')) : '';

  $subject = $subject_intro . $subject_title . $subject_more;

  switch ($key) {
    case 'mail_cart_items':
      $message['subject'] = $subject;
      $message['body'][] = drupal_render($params['manifestations']);
      break;
  }
}


