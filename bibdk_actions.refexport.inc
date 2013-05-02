<?php

/** Page that will generate a file with reference information
 * @param $ids
 * @return null The proces is killed after the file has been echoed
 */
function bibdk_actions_refexport_page($ids){
  $ids = preg_replace('@;@', ',', $ids);
  $ids = explode(',', $ids);

  try{
    bibdk_actions_refexport_file($ids);
  } catch (Exception $e){
    drupal_set_message(t('export_cannot_be_generated'), 'error');
  }

}

/** link for cart actions hook
 * @param $actions
 * @return mixed
 */
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

  drupal_add_http_header('Content-Type', 'text/plain; charset=utf-8');
  drupal_add_http_header('Content-Disposition', 'attachment; filename='.$filename);

  echo $result;
  die();
}

/** Get references with ids $ids
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

/** extract reference format from webservice result
 * @param $result xml
 * @return array
 */
function _bibdk_actions_refexport_convert_result_to_ref_format($result, $format){
  $dom = new DomDocument();

  if (!@$dom->loadXML($result)) {
    throw new Exception();
  }
  $xpath = new DomXPATH($dom);
  $xpath->registerNamespace('ns', 'http://oss.dbc.dk/ns/opensearch');

  $nodelist = $xpath->query('//ns:'.$format);
  $ref_nodes = array();
  foreach ($nodelist as $node) {
    $ref_nodes[] = $node->nodeValue;
  }
  return implode("\n\n", $ref_nodes);
}
