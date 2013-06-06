<?php

/** Page that will generate a file with reference information
 * @param $ids
 * @return null The proces is killed after the file has been echoed
 */
function bibdk_actions_refexport_page($ids){
  $pids = preg_replace('@;@', ',', $ids);
  $pids = explode(',', $pids);

  try{
    bibdk_actions_refexport_file($pids);
    $ids = explode(';', $ids);
    foreach($ids as $id){
      bibdk_cart_reservation_update_status('export_complete', $id);
    }
    die();
  } catch (Exception $e){
    drupal_set_message(t('export_cannot_be_generated'), 'error');
  }

}


/**
 * Implements hook_manifestation_actions
 */

function bibdk_actions_manifestation_actions($manifestation){
  dpm(func_get_args());
  $actions['export'] = array(
    '#theme' => 'links',
    '#links' => bibdk_actions_refexport_links(),
    '#weight' => 0,
  );
  return $actions;
}

/** link for cart actions hook
 * @param $actions
 * @return mixed
 */
function bibdk_actions_get_refexport_actions($actions) {

  $attributes = array(
    'class' => array(
      'cart-action-btn', 'inactive'
    ),
  );

  $actions['export'] = array(
    '#theme' => 'links',
    '#links' => bibdk_actions_refexport_links(null, $attributes),
    '#weight' => 0,
  );

  return $actions;
}


/** Return links for export of manifestations
 * @return array
 */
function bibdk_actions_refexport_links($pid = null, $attributes = array()){
  $paths = array(
    'direct' => '',
    'refworks' => 'http://www.refworks.com/express/ExpressImport.asp?vendor=bibliotek.dk&filter=RefWorks Tagged Format&encoding=28591&url=',
    'endnote' => 'http://www.myendnoteweb.com/EndNoteWeb.html?func=directExport& partnerName=bibliiotek.dk&dataIdentifier=1&dataRequestUrl='
  );

  $links = array();

  foreach ($paths as $type => $path) {
    $path = $path . url('export/cart', array('absolute' => true)) . $pid;
    $links[$type] = array(
      'title' => t('export_manifestation_' . $type, array(), array('context' => 'bibdk_actions')),
      'href' => $path,
      'attributes' => $attributes,
      'html' => FALSE,
    );
  }

  return $links;
}


/**
 * @param $ids
 * @param string $format
 */
function bibdk_actions_refexport_file($ids, $format = 'ris'){

  $ris = _bibdk_actions_refexport_get_manifestations_from_webservice($ids, $format);
  $result = _bibdk_actions_refexport_convert_result_to_ref_format($ris, $format);

  $filename = 'bibdk_'.date('Ymd_His').'.'.$format;

  drupal_add_http_header('Content-Type', 'application/x-endnote-refer; charset=utf-8');
  drupal_add_http_header('Content-Disposition', 'attachment; filename='.$filename);

  echo $result;
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
