<?php
/**
 * @file
 * Contains core functions for the Views module support.
 */

/*
 * Implements hook_views_plugins().
 *
 * This function annnounces the style plugin for showcase slider.
 */
function showcase_slider_views_plugins() {
  return array(
    'style' => array(
      // Style plugin for the ModernSlide.
      'showcase_slider' => array(
        'title' => t('Showcase Slider'),
        'help' => t('Display the results in a Showcase Slide widget.'),
        'handler' => 'showcase_slider_views_plugin_style_showcase_slider',
        'theme' => 'showcase_slider',
        'theme file' => 'showcase_slider.theme.inc',
        'theme path' => drupal_get_path('module', 'showcase_slider') . '/theme',
        'uses row plugin' => TRUE,
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'normal',
        'even empty' => FALSE,
      ),
    ),
  );
}
