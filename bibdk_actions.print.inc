<?php

/**
 * @param array $actions
 * @return array
 */
function bibdk_actions_get_print_actions($actions){
  $actions['print'] = array(
    '#theme' => 'link',
    '#text' => t('print_manifestation', array(), array('context' => 'bibdk_actions')),
    '#path' => 'print/cart',
    '#weight' => 0,
    '#options' => array(
      'query' => null,
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
 * @param array $form_id
 * @param array $form_state
 */
function bibdk_actions_print_items($form_id, &$form_state){
  bibdk_actions_parse_form_state($form_state, 'print');
  dpm('printing...');
}

function bibdk_actions_print_page($ids){
  $ids = explode(',', $ids);
  $view = "";
  $manifestations = bibdk_actions_get_manifestations($ids);
  foreach($manifestations as $manifestation){
    $view .= ting_openformat_manifestation_view($manifestation, 'full');
  }
  drupal_add_js(drupal_get_path('module', 'bibdk_actions') . '/js/bibdk_actions.print.js');
  drupal_add_css(drupal_get_path('module', 'bibdk_actions') . '/css/bibdk_actions.css', array('media' => 'print'));
  return $view;
}
