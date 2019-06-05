<?php
/**
 * Created by PhpStorm.
 * User: ralph
 * Date: 06.02.15
 * Time: 17:23
 */

/**
 * Preprocess variables for the html template.
 */
function pixelgarage_preprocess_html(&$vars) {
  // make sure jQuery UI and effects is loaded for anonymous users
  drupal_add_library('system', 'ui');
  drupal_add_library('system', 'effects');

}

/**
 * Override or insert variables for the page templates.
 */
function pixelgarage_preprocess_page(&$vars) {
  // hide titles on login forms
  pg_login_preprocess_page($vars);

  // replace logo with .svg
  //$vars['logo'] = str_replace('.jpg', '.svg', $vars['logo']);

  //
  // define shariff buttons
  global $language, $base_url;

  if ($vars['is_front']) {
    $url = 'https://kirchefuerkovi.ch/' . $language->language;

    $shariff_attrs = array(
      'data-services' => '["twitter","facebook","mail","whatsapp"]',
      'data-orientation' => "horizontally",
      'data-mail-url' => "mailto:",
      'data-mail-subject' => variable_get('shariff_mail_subject', t("Kirche für KoVI, weil...")),
      'data-mail-body' => variable_get('shariff_mail_body', t('<p>Die Kirche sagt JA zu KoVI.</p><p>Komm doch auf unsere Webseite und schau, ob du auch mitmachen möchtest.<br><a href="@link">Hier geht es zu unserer Webseite</a></p><p>Liebe Grüsse</p>', array('@link' => $url))),
      'data-lang' => "de",
      'data-url' => $url,
    );
    $vars['shariff_attributes'] = drupal_attributes($shariff_attrs);

    // load library
    $variant = 'naked'; //variable_get('shariff_css', 'complete');
    libraries_load('shariff', $variant);
  }

  //
  // adapt primary menu
  $vars['search_block'] = $vars['primary_nav'][3469];
  unset($vars['primary_nav'][3469]);

  //
  // calculate the different counts for secondary menu
  $cat_orga = 86;
  $cat_canton = 87;
  switch ($language->language) {
    case 'de':
    default:
      $organisation = 3474;
      $canton = 3533;
      $community = 3476;
    break;
    case 'fr':
      $organisation = 3475;
      $canton = 3534;
      $community = 3477;
      break;
    case 'it':
      $organisation = 3508;
      $canton = 3535;
      $community = 3509;
      break;
  }
  $vars['secondary_nav'][3472]['#title'] .= _get_count('post', $language->language);  // personally
  $vars['secondary_nav'][$organisation]['#title'] .= _get_count('organisation', $language->language, $cat_orga);
  $vars['secondary_nav'][$canton]['#title'] .= _get_count('organisation', $language->language, $cat_canton);

}

function _get_count($bundle, $lang, $category = FALSE) {
  // get the published posts
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', $bundle)
    ->propertyCondition('status', NODE_PUBLISHED)
    ->propertyCondition('language', array($lang, LANGUAGE_NONE), 'IN');
  if ($category) {
    $query->fieldCondition('field_kategorie', 'tid', $category);
  }
  $result = $query->count()->execute();

  return $result ? ' (' . $result  . ')' : ' (0)';
}

/*
function __update_language() {
  $nids = [2035, 2036, 1932, 1606, 2012, 1984, 2118, 1746, 1951, 2005, 1954, 2079, 2096, 2085, 2130, 1830, 1926, 1927, 1959, 1963, 2014, 2119, 2030, 2024, 1985, 1968, 1854, 2016, 2002, 1923, 1825, 1938, 1961, 1969, 2004, 1930, 1974, 1941, 2020, 1569];

  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'post')
    ->propertyCondition('status', NODE_PUBLISHED);
  $result = $query->execute();
  if ($result && !empty($result['node'])) {
    $nodes = node_load_multiple(array_keys($result['node']));
    foreach ($nodes as $nid => $node) {
      $node->field_language[LANGUAGE_NONE][0]['value'] = in_array($nid, $nids) ? 'fr' : 'de';
      node_save($node);
    }
  }
}
*/

/* =============================================================================
 *      Front Masonry
 * ========================================================================== */
/**
 * Adapts the proximity view, so that always two randomly chosen testimonials are visible
 * in the masonry.
 */
function pixelgarage_preprocess_views_view_proximity (&$vars) {
  $view = $vars['view'];
  if ($view->name != 'post_grid') return;

  // add testimonials for the post masonry
  $vars['testimonials'] = _random_nodes_of_bundle('testimonial');

  // add important person at specific position
  $vars['important_posts'] = _random_nodes_of_bundle('post');

  // add french person at specific position (to show multilingual aspect)
  $vars['french_posts'] = _random_nodes_of_bundle('post-french');
}

function _random_nodes_of_bundle($bundle) {
  global $language;
  $lang = $language->language;
  $result = FALSE;

  if ($bundle == 'testimonial') {
    // get the published testimonials
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'node')
      ->entityCondition('bundle', 'testimonial')
      ->propertyCondition('status', NODE_PUBLISHED)
      ->propertyCondition('language', array($lang, LANGUAGE_NONE), 'IN');
    $result = $query->execute();
  }
  else if ($bundle == 'post') {
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'node')
      ->entityCondition('bundle', 'post')
      ->propertyCondition('status', NODE_PUBLISHED)
      ->fieldCondition('field_important', 'value', '1');
    $result = $query->execute();
  }
  else if ($bundle == 'post-french') {
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'node')
      ->entityCondition('bundle', 'post')
      ->propertyCondition('status', NODE_PUBLISHED)
      ->fieldCondition('field_language', 'value', 'fr');
    $result = $query->execute();
  }
  if ($result && !empty($result['node'])) {
    $nodes = node_load_multiple(array_keys($result['node']));
  }
  else return array();

  // return a random node list
  $rand_key = array_rand($nodes, 1);
  $pos = ($bundle == 'testimonial') ? 3 : 1;
  return array(
    $pos => array($rand_key => node_view($nodes[$rand_key], 'teaser'))
  );
}


/* =============================================================================
 *      Preprocess post nodes
 * ========================================================================== */

function pixelgarage_ds_pre_render_alter(&$layout_render_array, $context, &$vars) {
  // check node type
  if ($context['entity_type'] != 'node') return;

  $view_mode = $context['view_mode'];
  if ($context['bundle'] == 'post') {
    _preprocess_node_post($vars, $view_mode);
  }
  else if ($context['bundle'] == 'event') {
    _preprocess_node_event($vars, $view_mode);
  }
}

function _preprocess_node_post(&$vars, $view_mode) {
  //
  // shariff button definition
  global $base_url, $language;

  libraries_load('shariff', 'naked');
  $node = $vars['node'];
  $lang = '/' . $language->language;
  $node_url = $base_url . $lang . '/node/' . $node->nid;
  $mail_subject = t("@title sagt JA zur KoVI, weil...", array('@title' => $node->field_your_name[LANGUAGE_NONE][0]['value']));
  $html_body =  '...' . $node->field_quote[LANGUAGE_NONE][0]['value'] . t('<p></p><p>Komm auf die Seite und schau, ob du auch mitmachen möchtest.<br><a href="@link">Hier geht es zu meinem Beitrag</a></p><p>Liebe Grüsse</p>', array('@link' => $node_url));
  $mail_descr = drupal_html_to_text($html_body);

  $vars['shariff_attrs'] = array(
    'data-services' => '["facebook","twitter","mail","whatsapp"]',
    'data-orientation' => "horizontal",
    'data-mail-url' => "mailto:",
    'data-mail-subject' => variable_get('shariff_mail_subject', $mail_subject),
    'data-mail-body' => variable_get('shariff_mail_body', $mail_descr),
    'data-lang' => "de",
    'data-url' => $node_url,
  );

  //
  // get logo
  $img_path = drupal_get_path('theme', 'pixelgarage') . '/images/';
  switch ($language->language) {
    case 'de':
    default:
      $vars['icon_path'] = file_create_url($img_path . 'KOVI_weil_de.svg');
      break;
    case 'fr':
      $vars['icon_path'] = file_create_url($img_path . 'KOVI_weil_fr.svg');
      break;
    case 'it':
      $vars['icon_path'] = file_create_url($img_path . 'KOVI_weil_it.svg');
      break;
  }

  //
  // set language dependent heading
  $content = $vars['content'];
  $vars['post_title'] = t('Support campaign!');
  $vars['post_heading'] = t('Share this post with your friends and family members!');

  $vars['field_funktion_ort'] = isset($content['field_funktion'][0]['#markup']) ? $content['field_funktion'][0]['#markup'] : '';
  $field_ort = isset($content['field_ort'][0]['#markup']) ? $content['field_ort'][0]['#markup'] : '';
  if ($vars['field_funktion_ort']) {
    $vars['field_funktion_ort'] .= (isset($field_ort) ? ' | ' . $field_ort : '');
  }
  else {
    $vars['field_funktion_ort'] .= (isset($field_ort) ? $field_ort : '');
  }
}

function _preprocess_node_event(&$vars, $view_mode) {
  //
  // set variables
  $content = $vars['content'];
  $vars['field_date_location'] = isset($content['field_full_date'][0]['#markup']) ? $content['field_full_date'][0]['#markup'] : '';
  $field_ort = isset($content['field_ort'][0]['#markup']) ? $content['field_ort'][0]['#markup'] : '';
  if ($vars['field_date_location']) {
    $vars['field_date_location'] .= (isset($field_ort) ? ' | ' . $field_ort : '');
  }
  else {
    $vars['field_date_location'] .= (isset($field_ort) ? $field_ort : '');
  }
}


/* =============================================================================
 *      Exposed form
 * ========================================================================== */
/**
 * Implements template_preprocess_views_exposed_form().
 *
 * Adds total result count to exposed form and the placeholder for the search field.
 */
function pixelgarage_preprocess_views_exposed_form(&$vars) {
  if ($filter = &$vars['widgets']['filter-combine']) {
    $placeholder = 'placeholder="' . $filter->label . '"/>';
    $filter->widget = str_replace('/>', $placeholder, $filter->widget);
  }
}


