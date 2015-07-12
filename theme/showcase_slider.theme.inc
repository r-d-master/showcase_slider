<?php

/**
 * @file
 * Theme function for modernslide Views.
 *
 * @author Akash Jain <akashjain132@gmail.com>
 */

/**
 * Add a preprocessor to prepare data from ModernSlide.
 */
function template_process_showcase_slider(&$vars) {

  // Only run the preprocessor if it is a view.
  if (!empty($vars['view'])) {
    // Prepare all data to be passed to theme_modernslide().
    $items = array();
    $view = $vars['view'];
    $settings = array();
    global $base_url;

    $row_plugin = $view->style_plugin->row_plugin;

    foreach ($vars['rows'] as $id => $row) {
      // Render the row into a slide.
      $view->row_index = $id;
      $item['title'] = (isset($row->title) && !empty($row->title)) ? $row->title : NULL;
      $item['description'] = (isset($row->description) && !empty($row->description)) ? $row->description : NULL;
      $item['image'] = (isset($row->image) && !empty($row->image)) ? $row->image : NULL;
      $item['pager'] = (isset($row->pager) && !empty($row->pager)) ? $row->pager : NULL;

      $items[] = $item;
    }

    $autoplay = (isset($vars['options']['autoplay']) && !empty($vars['options']['autoplay'])) ? TRUE : FALSE;
    $interval = isset($vars['options']['interval']) ? $vars['options']['interval'] : 4000;
    $navigation = (isset($vars['options']['navigation']) && !empty($vars['options']['navigation'])) ? TRUE : FALSE;

    // Add the slide items to the variables.
    $vars['items'] = $items;

    $showcase_slider_path = drupal_get_path('module', 'showcase_slider');
    drupal_add_js(array('showcase_slider' => array('autoplay' => $autoplay, 'interval' => $interval, 'navigation' => $navigation)), array('type' => 'setting'));
    drupal_add_js($showcase_slider_path . '/js/classie.js');
    drupal_add_js($showcase_slider_path . '/js/modernizr.custom.js');
    drupal_add_js($showcase_slider_path . '/js/tiltSlider.js');
    drupal_add_js($showcase_slider_path . '/js/wait.js');

    drupal_add_css($showcase_slider_path . '/css/component.css');
    drupal_add_css($showcase_slider_path . '/css/demo.css');
    drupal_add_css($showcase_slider_path . '/css/normalize.css');
  }

  return $vars;
}
