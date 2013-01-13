<?php
require_once("jgsoft-template.php");

/**
 * Force refresh of theme registry.
 * DEVELOPMENT USE ONLY - COMMENT OUT FOR PRODUCTION
 */
//drupal_rebuild_theme_registry();


/**
 * Return a themed mission trail.
 *
 * @return
 *   a string containing the mission output, or execute PHP code snippet if
 *   mission is enclosed with <?php ?>.
 */
function phptemplate_mission() {
  $mission = theme_get_setting('mission');
  if (preg_match('/^<\?php/', $mission)) {
    $mission = drupal_eval($mission);
  }
  else {
    $mission = filter_xss_admin($mission);
  }
  return isset($mission) ? $mission : '';
}

/**
 * Generates IE CSS links for LTR and RTL languages.
 */
function phptemplate_get_ie_styles() {
  global $language;

  $iecss = '<link type="text/css" rel="stylesheet" media="all" href="'. base_path() . path_to_theme() .'/fix-ie.css" />';
  if (defined('LANGUAGE_RTL') && $language->direction == LANGUAGE_RTL) {
    $iecss .= '<style type="text/css" media="all">@import "'. base_path() . path_to_theme() .'/fix-ie-rtl.css";</style>';
  }

  return $iecss;
}

/**
 * Override or insert PHPTemplate variables into the templates.
 */
function phptemplate_preprocess_page(&$vars) {

  // Allow other modules to override menu location
  drupal_alter('menu_location', $vars);

  $vars['primary_links'] = menu_primary_links();//theme('primary_links', $vars['primary_links'], array('class' => 'links', 'id' => 'primary-links'));
  $vars['breadcrumb'] = theme('breadcrumb',drupal_get_breadcrumb());
}
