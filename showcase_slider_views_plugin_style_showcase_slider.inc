<?php

/**
 * @file
 * Showcase slider style plugin for the Views module.
 */

 /**
  * Implements a style type plugin for the Views module.
  */
class showcase_slider_views_plugin_style_showcase_slider extends views_plugin_style {

  /**
   * Set default options.
   */
  function option_definition() {
    $options = parent::option_definition();
    $options += array(
      'title' => array('default' => ''),
      'description' => array('default' => ''),
      'image' => array('default' => ''),
      'autoplay' => array('default' => TRUE),
      'interval' => array('default' => 4000),
      'navigation' => array('default' => TRUE),
      'background_color' => array('default' => '#ddd'),
    );

    return $options;
  }

  /**
   * Show a form to edit the style options.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['showcase_slider'] = array(
      '#type' => 'fieldset',
      '#title' => t('Showcase Slider'),
    );

    $field_options = array('' => t('None'));
    foreach ($this->view->display_handler->get_handlers('field') as $field => $handler) {
      $field_options[$field] = $handler->ui_name();
    }

    $form['showcase_slider']['title'] = array(
      '#type' => 'select',
      '#title' => t('Showcase Slider Title'),
      '#description' => t("Select a field to be used as the title."),
      '#options' => $field_options,
      '#default_value' => $this->options['title'],
    );

    $form['showcase_slider']['description'] = array(
      '#type' => 'select',
      '#title' => t('Showcase Slider Description'),
      '#description' => t("Select a field to be used as the description."),
      '#options' => $field_options,
      '#default_value' => $this->options['description'],
    );

    $form['showcase_slider']['image'] = array(
      '#type' => 'select',
      '#title' => t('Showcase Slider Image'),
      '#description' => t("Select a field to be used as the image."),
      '#options' => $field_options,
      '#default_value' => $this->options['image'],
    );

    $form['showcase_slider']['autoplay'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable Autoplay'),
      '#default_value' => $this->options['autoplay'],
    );

    $form['showcase_slider']['interval'] = array(
      '#type' => 'textfield',
      '#title' => t('Time Interval'),
      '#maxlength' => 5,
      '#description' => t("Enter time interval between slides in millisecond i.e 1sec = 1000, interval must be greater than 2500"),
      '#default_value' => $this->options['interval'],
      '#element_validate' => array('element_validate_showcase_slider_interval'),
    );

    $form['showcase_slider']['navigation'] = array(
      '#type' => 'checkbox',
      '#title' => t('Navigation'),
      '#description' => t("Check for hide navigation from slider"),
      '#default_value' => $this->options['navigation'],
    );

    $form['showcase_slider']['background_color'] = array(
      '#type' => 'textfield',
      '#title' => t('Background Color'),
      '#description' => t("Enter Background Color of slider starts with #"),
      '#default_value' => $this->options['background_color'],
    );
  }

  /**
   * Performs some cleanup tasks on the options array before saving it.
   */
  function options_submit(&$form, &$form_state) {
    $options =& $form_state['values']['style_options'];

    if (!empty($options['showcase_slider'])) {
      // Pull the fieldset values one level up
      $options += $options['showcase_slider'];
      unset($options['showcase_slider']);
    }
  }

  /**
   * Render the display in this style.
   */
  function render() {

    // Group the rows according to the grouping field, if specified.
    $sets = $this->render_grouping($this->view->result, $this->options['grouping']);

    // Render each group separately and concatenate.
    $output = '';

    foreach ($sets as $title => $rows) {
      // Add caption field if chosen.
      if (!empty($this->options['title'])) {
        $title_field = $this->options['title'];
        $description_field = $this->options['description'];
        $image_field = $this->options['image'];
        $image_field_name = 'field_' . $image_field;
        foreach ($rows as $index => $row) {
          $rows[$index]->title = $this->rendered_fields[$index][$title_field];
          $rows[$index]->description = $this->rendered_fields[$index][$description_field];
          $rows[$index]->image = $this->rendered_fields[$index][$image_field];
        }
      }
      $output .= theme($this->theme_functions(),
        array(
          'view' => $this->view,
          'options' => $this->options,
          'rows' => $rows,
          'title' => $title,
        )
      );
    }

    return $output;
  }
}

/**
 * Validate showcase slider interval.
 */
function element_validate_showcase_slider_interval($element, &$form_state) {
  $value = $element['#value'];
  if ($value < 2500 || $value !== '' && (!is_numeric($value) || intval($value) != $value || $value <= 0)) {
    form_error($element, t('%name must be a positive integer and greater than 2500.', array('%name' => $element ['#title'])));
  }
}
