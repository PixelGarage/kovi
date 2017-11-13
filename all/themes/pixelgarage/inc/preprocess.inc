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
      'data-mail-subject' => variable_get('shariff_mail_subject', t("Die Kirchen sagen JA zu KOVI...")),
      'data-mail-body' => variable_get('shariff_mail_body', t("Die Kirchen sagen JA zu KOVI. " . $url)),
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


