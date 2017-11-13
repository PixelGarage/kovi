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
  $vars['title'] = '';


  //
  // define shariff buttons
  if ($vars['is_front']) {
    global $language;
    $url = 'http://zweimal-ja.ch/' . $language->language;

    $shariff_attrs = array(
      'data-services' => '["twitter","facebook","mail","whatsapp"]',
      'data-orientation' => "horizontally",
      'data-mail-url' => "mailto:",
      'data-mail-subject' => variable_get('shariff_mail_subject', t("Die Kirchen sagen JA zu KoVI...")),
      'data-mail-body' => variable_get('shariff_mail_body', t("Die Kirchen sagen JA zu KoVI. " . $url)),
      'data-lang' => "de",
      'data-url' => $url,
    );
    $vars['shariff_attributes'] = drupal_attributes($shariff_attrs);

    // load library
    $variant = 'naked'; //variable_get('shariff_css', 'complete');
    libraries_load('shariff', $variant);
  }
}

/* =============================================================================
 *      Preprocess post nodes
 * ========================================================================== */

function pixelgarage_ds_pre_render_alter(&$layout_render_array, $context, &$vars) {
  // check node type
  if ($context['entity_type'] == 'node' && $context['bundle'] == 'post') {
    $view_mode = $context['view_mode'];
    _preprocess_node_post($vars, $view_mode);
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
  $mail_subject = t("@title sagt JA zur KOVI", array('@title' => $node->field_your_name[LANGUAGE_NONE][0]['value']));
  $html_body =  t('... weil ') . $node->field_quote[LANGUAGE_NONE][0]['value'];
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
      $vars['icon_path'] = file_create_url($img_path . 'quote_image.png');
      break;
    case 'fr':
      $vars['icon_path'] = file_create_url($img_path . 'quote_image.png');
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

