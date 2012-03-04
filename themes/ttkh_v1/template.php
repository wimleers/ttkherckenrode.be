<?php

/**
 * Override or insert variables into the node template.
 */
function ttkh_v1_preprocess_node(&$variables) {
  if ($variables['view_mode'] == 'full' && node_is_page($variables['node'])) {
    $variables['classes_array'][] = 'node-full';
  }

  $node = $variables['node'];

  // Topics images.
  $topics_images = drupal_get_path('theme', 'ttkh_v1') . '/images/topics/';
  if (isset($node->field_tags) && count($node->field_tags)) {
    // When showing teasers, the taxonomy term object may not be loaded yet.
    if (!isset($node->field_tags['und'][0]['taxonomy_term'])) {
      $node->field_tags['und'][0]['taxonomy_term'] = taxonomy_term_load($node->field_tags['und'][0]['tid']);
    }
    $topic = $node->field_tags['und'][0]['taxonomy_term']->name;
    $path = $topics_images . $topic . '.gif';
    // TODO: don't assume the image exists! (Will require either filesystem
    // checks or a hardcoded array of available images.)
    $title = t('Topic: @topic', array('@topic' => $topic));
    $variables['topic'] = theme('image', array(
      'path'   => $path,
      'width'  => 90,
      'height' => 60,
      'alt'    => $title,
      'title'  => $title,
      'attributes' => array(
        'class'  => 'topic_image',
      ),
    ));
  }
  else {
    $variables['topic'] = NULL;
  }
}
