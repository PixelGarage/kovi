<?php

/**
 * @file
 * Customize the navigation shown when editing or viewing submissions.
 *
 * Available variables:
 * - $node: The node object for this webform.
 * - $mode: Either "form" or "display". May be other modes provided by other
 *          modules, such as "print" or "pdf".
 * - $submission: The Webform submission array.
 * - $submission_content: The contents of the webform submission.
 * - $submission_navigation: The previous submission ID.
 * - $submission_information: The next submission ID.
 */

//
// get post node from submission
$is_postcard_delivery_form = false;
$img_html = false;
$master = postcard_webform_master_form($node);

if ($master->nid == 18) {
  //
  // POSTCARD DATA webform, get example image from webform
  global $language;
  $transl_arr = translation_node_get_translations($master->nid);
  $transl_nid = !empty($transl_arr) ? $transl_arr[$language->language]->nid : $master->nid;
  $transl_node = node_load($transl_nid);

  $img_uri = !empty($transl_node->field_image) ? $transl_node->field_image[LANGUAGE_NONE][0]['uri'] : false;
  if ($img_uri) {
    $img_url = file_create_url($img_uri);
    $vars = array (
      'path' => $img_uri,
      'alt' => 'Model image',
      'attributes' => array('class'=>array('img-responsive', 'model-image')),
    );
    $img_html = theme_image($vars);
  }
}
else if ($master->nid == 789) {
  //
  // POSTCARD DELIVERY webform, get postcard image from post
  $post_nid = false;
  $is_postcard_delivery_form = true;
  foreach ($master->webform['components'] as $key => $data) {
    if ($data['form_key'] == 'post_nid') {
      $post_nid = $submission->data[$key][0];
      continue;
    }
  }

  //
  // get postcard image from post node, if available
  if ($post_nid) {
    $post = node_load($post_nid);
    $img_uri = !empty($post->field_hires_image) ? $post->field_hires_image[$post->language][0]['uri'] : false;
    if ($img_uri) {
      $img_url = file_create_url($img_uri);
      $vars = array (
        'path' => $img_uri,
        'alt' => 'Postcard image',
        'attributes' => array('class'=>array('img-responsive', 'postcard-image')),
      );
      $img_html = theme_image($vars);
    }
  }
}

?>

<?php if ($mode == 'display' || $mode == 'form'): ?>
  <div class="submission-actions clearfix">
    <?php print $submission_actions; ?>
  </div>
<?php endif; ?>

<div class="webform-submission">
  <?php if ($img_html): ?>
    <div class="row">
      <?php if ($is_postcard_delivery_form): ?>
        <div class="col-sm-6"><?php print $img_html; ?></div>
        <div class="col-sm-6"><?php print render($submission_content); ?></div>
      <?php else: ?>
        <div class="col-sm-6"><?php print render($submission_content); ?></div>
        <div class="col-sm-6"><?php print $img_html; ?></div>
      <?php endif; ?>
    </div>
  <?php else: ?>
    <div class="row">
      <div class="col-sm-12">
        <?php print render($submission_content); ?>
      </div>
    <div>
  <?php endif; ?>
</div>

