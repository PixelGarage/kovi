<?php
/**
 * @file
 * Bootstrap 12 template for Display Suite.
 */
//
// shariff button definition
global $base_url, $language;

libraries_load('shariff', 'naked');
$lang = '/' . $language->language;
$node_url = $base_url . $lang . '/node/' . $node->nid;
$mail_subject = t("@title sagt JA zur KOVI", array('@title' => $node->field_your_name[LANGUAGE_NONE][0]['value']));
$html_body =  t('... weil ') . $node->field_quote[LANGUAGE_NONE][0]['value'];
$mail_descr = drupal_html_to_text($html_body);

$shariff_attrs = array(
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
global $language;

$img_path = drupal_get_path('theme', 'pixelgarage') . '/images/';
switch ($language->language) {
  case 'de':
    $icon_path = file_create_url($img_path . 'zweimal-ja_banner.png');
    break;
  case 'fr':
    $icon_path = file_create_url($img_path . 'zweimal-ja_banner_fr.png');
    break;
}

//
// set language dependent heading
$content = $variables['content'];
$post_title = t('Support campaign!');
$post_heading = t('Share this post with your friends and family members!');
$has_age = isset($content['field_age'][0]['#markup']) ? $content['field_age'][0]['#markup'] != '0' : false;
$post_claim = null;
?>


<?php if (!$teaser): ?>
<div class="node-post-page-wrapper">
    <div class="node-post-header">
      <div class="post-title">
        <?php print $post_title ?>
      </div>
      <div class="post-heading">
        <?php print $post_heading ?>
      </div>
      <div class="social-buttons">
        <div class="shariff" <?php print drupal_attributes($shariff_attrs); ?>></div>
      </div>
    </div>
    <div class="node-post-wrapper">
<?php endif; ?>

<<?php print $layout_wrapper; print $layout_attributes; ?> class="<?php print $classes; ?>">
  <?php if (isset($title_suffix['contextual_links'])): ?>
    <?php print render($title_suffix['contextual_links']); ?>
  <?php endif; ?>
  <div class="row">
    <<?php print $central_wrapper; ?> class="col-sm-12 <?php print $central_classes; ?>">
      <?php print render($content['field_image']); ?>
      <div class="logo-wrapper"><img class="icon-quote" src="<?php print $icon_path; ?>"/></div>
      <div class="name-age-wrapper">
        <?php print render($content['field_your_name']); ?><?php if ($has_age): print render($content['field_age']); endif; ?>
      </div>
      <?php print render($content['field_profession']); ?>
      <div class="quote-wrapper">
        <?php print render($content['field_quote']); ?>
      </div>
    </<?php print $central_wrapper; ?>>
  </div>
</<?php print $layout_wrapper ?>>

<?php if (!$teaser): ?>
  <!-- close node wrapper and node-page-wrapper-->
  </div>
</div>
<?php endif; ?>



<!-- Needed to activate display suite support on forms -->
<?php if (!empty($drupal_render_children)): ?>
  <?php print $drupal_render_children ?>
<?php endif; ?>
