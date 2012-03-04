<?php

//----------------------------------------------------------------------------
// Menu system callbacks.

/**
 * Menu system callback; ranking.
 */
function mc_ranking($week = NULL) {
  $output = '';

  $max_week = mc_get_last_week();
  if ($week == NULL) {
    $week = $max_week;
  }
  else {
    $week = intval($week);
  }

  $path = drupal_get_path('module', 'mc') . '/images/';
  $header = array(
    'Stand',
    'Vst.',
    'Naam',
    'Manager',
    'Punten',
    'Stijgpl.'
  );
  $rows = array();

  // Query the DB + store the data.
  $query = db_select('mc_ranking', 'r')
    ->fields('r')
    ->condition('week', $week);
  $result = $query->execute();
  foreach ($result as $record) {
    // "movement" apparently is 0 when the ranking has decreased.
    if ($record->rank > $record->previous_rank) {
      $record->movement = $record->previous_rank - $record->rank;
    }

    // Fancify the output for the "movement" field.
    $delta = '';
    if ($record->movement > 0) {
      $delta .= theme('image', array('path' => "$path/stijger.gif"));
    }
    else if ($record->movement < 0) {
      $delta .= theme('image', array('path' => "$path/daler.gif"));
    }
    else {
      $delta = theme('image', array('path' => "$path/blijver.gif"));
    }
    $delta .= '&nbsp;' . $record->movement;

    // Build the rows.
    $rows[] = array(
      $record->rank,
      $record->previous_rank,
      $record->name,
      $record->manager,
      $record->points,
      $delta
    );
  }

  // Generate actual output.
  $output .= '<h2 align=center>' . t('Week !week', array('!week' => $week)) . '</h2>';
  if ($week > 1) {
    $output .= '<div style="float:left;">' . l(t('Vorige week'), 'mc/' . ($week - 1)) . '</div>';
  }
  if ($week < $max_week) {
    $output .= '<div style="float:right;">' . l(t('Volgende week'), 'mc/' . ($week + 1)) . '</div>';
  }
  $output .= '<div class="clearfix"></div>';

  $output .= theme('table', array(
    'header' => $header,
    'rows' => $rows
  ));

  return $output;
}

