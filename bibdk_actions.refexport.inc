<?php

function bibdk_actions_refexport_page($ids){
  $id = explode(';',$ids);
  bibdk_actions_refexport_file($id);
  return '';
}


function bibdk_actions_get_refexport_actions($actions) {
  $actions['export'] = array(
    '#theme' => 'link',
    '#text' => t('export_manifestation', array(), array('context' => 'bibdk_actions')),
    '#path' => 'export/cart',
    '#weight' => 0,
    '#options' => array(
      'query' => NULL,
      'attributes' => array(
        'class' => array(
          'cart-action-btn', 'inactive',
        ),
      ),
      'html' => FALSE,
    ),
  );

  return $actions;
}


/**
 * @param $ids
 * @param string $format
 */
function bibdk_actions_refexport_file($ids, $format = 'ris'){

  $ris = _bibdk_actions_refexport_get_manifestations_from_webservice($ids, $format);
  $result = _bibdk_actions_refexport_convert_result_to_ref_format($ris, $format);

  $filename = 'bibdk_'.date('Ymd_His').'.txt';

  drupal_add_http_header('Content-Type', 'text/xml; charset=utf-8');
  drupal_add_http_header('Content-Disposition', 'attachment; filename='.$filename);

  echo implode("\n\n", $result);
  die();
}

/** Get reviews with ids $ids
 * @param $ids
 * @return xml
 */
function _bibdk_actions_refexport_get_manifestations_from_webservice($ids, $objectFormat){
  $params = array(
    'objectFormat' => $objectFormat,
    'objectId' => $ids,
    'outputType' => 'xml',
  );

  $client = new ting_client_class();

  return $client->do_get_object($params);

}

/** Converts result from webservice to BibdkReview Objects
 * @param $result xml
 * @return array
 */
function _bibdk_actions_refexport_convert_result_to_ref_format($result, $format){
  $dom = new DomDocument();

  if (!@$dom->loadXML($result)) {
    //watchdog('bibdk_reviews', t('BIBDK material review could not load response: %xml', array('%xml' => var_export($xml, TRUE))), array(), WATCHDOG_ERROR);
  }
  $xpath = new DomXPATH($dom);
  $xpath->registerNamespace('ns', 'http://oss.dbc.dk/ns/opensearch');

  $nodelist = $xpath->query('//ns:'.$format);
  $ref_nodes = array();
  foreach ($nodelist as $node) {
    $ref_nodes[] = $node->nodeValue;
  }
  return $ref_nodes;
}
