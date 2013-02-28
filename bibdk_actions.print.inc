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

function bibdk_actions_print_page($ids){
  $ids = explode(',', $ids);
  $view = "";
  $manifestations = bibdk_actions_get_manifestations($ids);
  foreach($manifestations as $manifestation){
   $work = new BibdkWork($manifestation->getWork());
    $view = ting_openformat_manifestation_view($manifestation, 'print');
    $view['#theme'] = 'bibdk_actions_print';
    $view += array(
      '#title' => _ting_openformat_parse_element($work->getTitle(), ',', TRUE, TRUE),
      '#abstract' => _ting_openformat_parse_element($work->getAbstract(), ',', TRUE, TRUE),
      '#subjects' => _ting_openformat_parse_element($work->getSubjects(), ',', TRUE, TRUE),
      '#author' => _ting_openformat_parse_element($manifestation->getCreator(), ',', TRUE, FALSE),
      '#type' => _ting_openformat_parse_element($manifestation->getType(), ',', TRUE, TRUE),
      '#pid' => drupal_strtolower(preg_replace('/[^a-zA-Z0-9-]+/', '-', $manifestation->id)),

    );
    $views[] = $view;
  }
  drupal_add_js(drupal_get_path('module', 'bibdk_actions') . '/js/bibdk_actions.print.js');
  drupal_add_css(drupal_get_path('module', 'bibdk_actions') . '/css/bibdk_actions.css', array('media' => 'all'));
  return drupal_render($views);
}
