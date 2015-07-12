<?php
/**
 * @file
 * Default theme to display image with description using showcase slider.
 *
 * Available variables:
 * - $items: Having information about slider content.
 *   - $title: An element having node title.
 *   - $description: An element having description about that slide.
 *   - $image: Image of slider.
 */
?>
<style type="text/css">
  #slideshow .slides { background-color: <?php print $options['background_color']; ?>; }
</style>
<div class="slideshow" id="slideshow">
  <ol class="slides">
    <?php foreach ($items as $key => $item): ?>
      <?php if ($key == 0): ?>
        <li class="current">
      <?php else: ?><li>
      <?php endif; ?>
        <div class="description">
          <?php if ($item['title']): ?>
            <h2><?php print $item['title']; ?></h2>
          <?php endif; ?>
          <?php if ($item['description']): ?>
            <p><?php print $item['description']; ?></p>
          <?php endif; ?>
        </div>
        <div class="tiltview col">
          <?php print $item['image']; ?>
        </div>
      </li>
    <?php endforeach; ?>
  </ol>
</div>
