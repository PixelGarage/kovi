<?php

/**
 * @file
 * Customize confirmation screen after successful submission.
 *
 * This file may be renamed "webform-confirmation-[nid].tpl.php" to target a
 * specific webform e-mail on your site. Or you can leave it
 * "webform-confirmation.tpl.php" to affect all webform confirmations on your
 * site.
 *
 * Available variables:
 * - $node: The node object for this webform.
 * - $progressbar: The progress bar 100% filled (if configured). This may not
 *   print out anything if a progress bar is not enabled for this node.
 * - $confirmation_message: The confirmation message input by the webform
 *   author.
 * - $sid: The unique submission ID of this submission.
 * - $url: The URL of the form (or for in-block confirmations, the same page).
 */

// load webform submission includes
module_load_include('inc', 'webform', 'includes/webform.submissions');

$master = postcard_webform_master_form($node);
$submission = webform_get_submissions(array('nid' => $master->nid, 'sid' => $sid))[$sid];
$post_nid = _webform_submission_value($master, 'post_nid', $submission);
$post = node_load($post_nid);
$rendered_post = node_view($post);

$url = '/';
?>

<div class="links">
  <a href="<?php print $url; ?>"><?php print t('Go back to home') ?></a>
</div>
<div class="rendered-node"><?php print render($rendered_post); ?></div>

