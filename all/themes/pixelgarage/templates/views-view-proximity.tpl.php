<?php
/**
 * @file
 * View theme template to layout the proximity items and add the bootstrap modal dialog functionality to each item.
 */
?>

<div id="<?php print $pe_container_id; ?>" class="pe-container">
  <div class="pe-background-image"></div>
  <div class="grid-sizer"></div>
  <div class="gutter-sizer"></div>
  <div class="stamp stamp1"></div>

  <?php foreach ($rows as $id => $row): ?>
    <?php if (isset($row)): ?>
      <!--
     Add non-ajax posts to the list
     -->
      <div
        class="pe-item pe-item-no-ajax <?php print 'pe-item-' . $ajax_load_params[$id]; ?> <?php if ($classes_array[$id]) print $classes_array[$id]; ?>"
        style="margin: 0 -2px">
        <div class="pe-item-inner">
          <?php print $row; ?>
        </div>
      </div>
    <?php endif; ?>

  <?php endforeach; ?>


  <?php if ($use_modal_dlg): ?>
    <!--
    Modal dialog displaying the item content
    The item content is retrieved via AJAX or added directly on full page loads
    -->
    <div id="pe-modal-dialog-<?php print $container_index; ?>" class="modal" tabindex="-1" role="dialog"
         aria-labelledby="pe-modal-label" aria-hidden="true">
      <div class="modal-dialog" role="document">
        <div class="modal-content">
          <!-- Header -->
          <div class="modal-header">
            <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span
                aria-hidden="true">&times;</span></button>
            <?php if ($title): ?>
              <h2 class="modal-title" id="pe-modal-label"><?php print $title; ?></h2>
            <?php endif; ?>
            <div class="body-fading body-fading-top"></div>
          </div>
          <!-- Body -->
          <div class="modal-body">
            <?php if ($rendered_item) print $rendered_item; ?>
          </div>
          <!-- Footer -->
          <div class="modal-footer">
            <div class="body-fading body-fading-bottom"></div>
            <button type="button" class="btn btn-default" data-dismiss="modal"><?php print $close_text; ?></button>
          </div>

        </div>
      </div>
    </div>
  <?php endif; ?>

</div>
