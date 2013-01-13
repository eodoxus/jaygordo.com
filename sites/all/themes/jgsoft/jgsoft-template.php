<?php

function interactive_media_preprocess_user_picture(&$variables) {
  if (variable_get('user_pictures', 0)) {
    $account = $variables['account'];
    if (!empty($account->picture) && file_exists($account->picture)) {
      $picture = file_create_url($account->picture);
    }
    else if (variable_get('user_picture_default', '')) {
      $picture = variable_get('user_picture_default', '');
    }

    if (isset($picture)) {
      $alt = $account->name ? $account->name : variable_get('anonymous', t('Anonymous'));
      $variables['picture'] = theme('imagecache', '85', $picture, $alt, $alt, '', FALSE);
      if (!empty($account->uid) && user_access('access user profiles')) {
        $attributes = array('attributes' => array('title' => t('View user profile.')), 'html' => TRUE);
        $variables['picture'] = l($variables['picture'], "user/$account->uid", $attributes);
      }
    }
  }
}
