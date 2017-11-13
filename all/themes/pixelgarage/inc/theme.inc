<?php
/**
 * Created by PhpStorm.
 * User: ralph
 * Date: 06.02.15
 * Time: 17:22
 */

/* =============================================================================
 *      Theme language block
 * ========================================================================== */

function pixelgarage_links__locale_block(&$vars) {
  foreach($vars['links'] as $language => $langInfo) {
    $abbr = substr($langInfo['language']->language, 0, 1);
    $vars['links'][$language]['title'] = $abbr;
  }
  $content = theme_links($vars);
  return $content;
}

