<?php
// $Id: display-render.inc,v 1.4.2.2 2008/10/24 00:06:10 merlinofchaos Exp $

/**
 * @file
 *
 * Contains Panels display rendering functions.
 */

/**
 * Render a display by loading the content into an appropriate
 * array and then passing through to panels_render_layout.
 *
 * if $incoming_content is NULL, default content will be applied. Use
 * an empty string to indicate no content.
 * @render
 * @ingroup hook_invocations
 */
function _panels_render_display(&$display) {
  $layout = panels_get_layout($display->layout);
  if (!$layout) {
    return NULL;
  }

  // TODO: This may not be necessary now. Check this.
  panels_sanitize_display($display);

  $output = '';

  // Let modules act just prior to render.
  foreach (module_implements('panels_pre_render') as $module) {
    $function = $module . '_panels_pre_render';
    $output .= $function($display);
  }

  $output .= panels_render_layout($layout, $display, $display->css_id, $display->layout_settings);

  // Let modules act just after render.
  foreach (module_implements('panels_post_render') as $module) {
    $function = $module . '_panels_post_render';
    $output .= $function($display);
  }
  return $output;
}

/**
 * For external use: Given a layout ID and a $content array, return the
 * panel display. The content array is filled in based upon the content
 * available in the layout. If it's a two column with a content
 * array defined like array('left' => t('Left side'), 'right' =>
 * t('Right side')), then the $content array should be array('left' =>
 * $output_left, 'right' => $output_right)
 * @render
 */
function _panels_print_layout($id, $content) {
  $layout = panels_get_layout($id);
  if (!$layout) {
    return;
  }

  return panels_render_layout($layout, $content);
}

/**
 * Given a full layout structure and a content array, render a panel display.
 * @render
 */
function panels_render_layout($layout, $content, $css_id = NULL, $settings = array()) {
  if (!empty($layout['css'])) {
    if (file_exists(path_to_theme() . '/' . $layout['css'])) {
      drupal_add_css(path_to_theme() . '/' . $layout['css']);
    }
    else {
      drupal_add_css($layout['path'] . '/' . $layout['css']);
    }
  }
  $display = NULL;

  // This now comes after the CSS is added, because panels-within-panels must
  // have their CSS added in the right order; inner content before outer content.

  // If $content is an object, it's a $display and we have to render its panes.
  if (is_object($content)) {
    $display = $content;
    if (empty($display->cache['method'])) {
      $content = panels_render_panes($display);
    }
    else {
      $cache = panels_get_cached_content($display, $display->args, $display->context);
      if ($cache === FALSE) {
        $cache = new panels_cache_object();
        $cache->set_content(panels_render_panes($display));
        panels_set_cached_content($cache, $display, $display->args, $display->context);
      }
      $content = $cache->content;
    }
  }

  $output = theme($layout['theme'], check_plain($css_id), $content, $settings, $display);

  return $output;
}

/**
 * Render all the panes in a display into a $content array to be used by
 * the display theme function.
 */
function panels_render_panes($display) {
  // Safety check.
  if (empty($display->content)) {
    return array();
  }

  // First, render all the panes into little boxes. We do this here because
  // some panes request to be rendered after other panes (primarily so they
  // can do the leftovers of forms).
  $panes = array();
  $later = array();

  foreach ($display->content as $pid => $pane) {
    $pane->shown = isset($pane->shown) ? $pane->shown : TRUE;
    // TODO Really ought to design a method for creating a quick-access set of content_type (and other plugin) data to help optimize render performance
    // If the user can't see this pane, do not render it.
    if (!$pane->shown || !panels_pane_access($pane, $display)) {
      continue;
    }

    // If this pane wants to render last, add it to the $later array.
    $content_type = panels_get_content_type($pane->type);

    if (!empty($content_type['render last'])) {
      $later[$pid] = $pane;
      continue;
    }

    $panes[$pid] = panels_render_pane_content($display, $pane);
  }

  foreach ($later as $pid => $pane) {
    $panes[$pid] = panels_render_pane_content($display, $pane);
  }

  // Loop through all panels, put all panes that belong to the current panel
  // in an array, then render the panel. Primarily this ensures that the
  // panes are in the proper order.
  $content = array();
  foreach ($display->panels as $panel_name => $pids) {
    $panel = array();
    foreach ($pids as $pid) {
      if (!empty($panes[$pid])) {
        $panel[$pid] = $panes[$pid];
      }
    }
    $content[$panel_name] = panels_render_panel($display, $panel_name, $panel);
  }

  // Prevent notices by making sure that all panels at least have an entry:
  $layout = panels_get_layout($display->layout);
  $panels = panels_get_panels($layout, $display);
  foreach ($panels as $id => $panel) {
    if (!isset($content[$id])) {
      $content[$id] = NULL;
    }
  }

  return $content;
}

/**
 * Render a single pane, identifying its context, and put it into
 * the $panes array.
 */
function panels_render_pane_content(&$display, &$pane) {
  if (empty($pane->context)) {
    $pane->context = panels_pane_select_context($pane, $display->context);
    if ($pane->context === FALSE) {
      return FALSE;
    }
  }

  $content = panels_get_pane_content($display, $pane, $display->args, $pane->context, $display->incoming_content);

  $keywords = !empty($display->keywords) ? $display->keywords : array();
  // Override the title if configured to
  if (!empty($pane->configuration['override_title'])) {
    // Give previous title as an available substitution here.
    $keywords['%title'] = $content->title;
    $content->title = $pane->configuration['override_title_text'];
  }

  // Pass long the css_id that is usually available.
  if (!empty($pane->configuration['css_id'])) {
    $content->css_id = $pane->configuration['css_id'];
  }

  // Pass long the css_class that is usually available.
  if (!empty($pane->configuration['css_class'])) {
    $content->css_class = $pane->configuration['css_class'];
  }

  if (!empty($content->title)) {
    // Perform substitutions
    if (!empty($keywords)) {
      $content->title = strtr($content->title, $keywords);
    }

    // Sterilize the title
    $content->title = filter_xss_admin($content->title);

    // If a link is specified, populate.
    if (!empty($content->title_link)) {
      if (!is_array($content->title_link)) {
        $url = array('href' => $content->title_link);
      }
      else {
        $url = $content->title_link;
      }
      // set defaults so we don't bring up notices
      $url += array('href' => '', 'attributes' => NULL, 'query' => NULL, 'fragment' => NULL, 'absolute' => NULL, 'html' => TRUE);
      $content->title = l($content->title, $url['href'], $url);
    }
  }
  return $content;
}

/**
 * Render a pane using the appropriate style.
 *
 * $content
 *   The already rendered content via panels_render_pane_content()
 * $pane
 *   The $pane information from the display
 * $display
 *   The display.
 */
function panels_render_pane($content, $pane, $display) {
  if (!empty($pane->configuration['style'])) {
    $style = panels_get_style($pane->configuration['style']);

    if (isset($style) && isset($style['render pane'])) {
      $output = theme($style['render pane'], $content, $pane, $display);

      // This could be null if no theme function existed.
      if (isset($output)) {
        return $output;
      }
    }
  }

  if (!empty($content)) {
    // fallback
    return theme('panels_pane', $content, $pane, $display);
  }
}

/**
 * Given a display and the id of a panel, get the style in which to render
 * that panel.
 */
function panels_get_panel_style_and_settings($panel_settings, $panel) {
  if (empty($panel_settings)) {
    return array(panels_get_style('default'), array());
  }

  if (empty($panel_settings['individual']) || empty($panel_settings['panel'][$panel]['style'])) {
    $style = panels_get_style($panel_settings['style']);
    $style_settings = isset($panel_settings['style_settings']['default']) ? $panel_settings['style_settings']['default'] : array();
  }
  else {
    $style = panels_get_style($panel_settings['panel'][$panel]['style']);
    $style_settings = isset($panel_settings['style_settings'][$panel]) ? $panel_settings['style_settings'][$panel] : array();
  }

  return array($style, $style_settings);
}

/**
 * Render a panel, by storing the content of each pane in an appropriate array
 * and then passing through to the theme function that will render the panel
 * in the configured panel style.
 *
 * @param $display
 *   A display object.
 * @param $panel
 *   The ID of the panel being rendered
 * @param $panes
 *   An array of panes that are assigned to the panel that's being rendered.
 *
 * @return
 *   The rendered HTML for a panel.
 * @render
 */
function panels_render_panel($display, $panel, $panes) {
  list($style, $style_settings) = panels_get_panel_style_and_settings($display->panel_settings, $panel);

  // Retrieve the pid (can be a panel page id, a mini panel id, etc.), this
  // might be used (or even necessary) for some panel display styles.
  // TODO: Got to fix this to use panel page name instead of pid, since pid is
  // no longer guaranteed. This needs an API to be able to set the final id.
  $owner_id = 0;
  if (isset($display->owner) && is_object($display->owner) && isset($display->owner->id)) {
    $owner_id = $display->owner->id;
  }

  return theme($style['render panel'], $display, $owner_id, $panes, $style_settings, $panel);
}

/**
 * @file panels.theme.inc
 * Core theme functions for Panels.
 */

/**
 * Render a panel pane like a block.
 *
 * A panel pane can have the following fields:
 *
 * $pane->type -- the content type inside this pane
 * $pane->subtype -- The subtype, if applicable. If a view it will be the
 *   view name; if a node it will be the nid, etc.
 * $content->title -- The title of the content
 * $content->content -- The actual content
 * $content->links -- Any links associated with the content
 * $content->more -- An optional 'more' link (destination only)
 * $content->admin_links -- Administrative links associated with the content
 * $content->feeds -- Any feed icons or associated with the content
 * $content->subject -- A legacy setting for block compatibility
 * $content->module -- A legacy setting for block compatibility
 * $content->delta -- A legacy setting for block compatibility
 */
function theme_panels_pane($content, $pane, $display) {
  if (!empty($content->content)) {
    $idstr = $classstr = '';
    if (!empty($content->css_id)) {
      $idstr = ' id="' . $content->css_id . '"';
    }
    if (!empty($content->css_class)) {
      $classstr = ' ' . $content->css_class;
    }

    $output = "<div class=\"panel-pane$classstr\"$idstr>\n";
    if (user_access('view pane admin links') && !empty($content->admin_links)) {
      $output .= "<div class=\"admin-links panel-hide\">" . theme('links', $content->admin_links) . "</div>\n";
    }
    if (!empty($content->title)) {
      $output .= "<h2 class=\"title\">$content->title</h2>\n";
    }

    if (!empty($content->feeds)) {
      $output .= "<div class=\"feed\">" . implode(' ', $content->feeds) . "</div>\n";
    }

    $output .= "<div class=\"content\">$content->content</div>\n";

    if (!empty($content->links)) {
      $output .= "<div class=\"links\">" . theme('links', $content->links) . "</div>\n";
    }


    if (!empty($content->more)) {
      if (empty($content->more['title'])) {
        $content->more['title'] = t('more');
      }
      $output .= "<div class=\"more-link\">" . l($content->more['title'], $content->more['href']) . "</div>\n";
    }

    $output .= "</div>\n";
    return $output;
  }
}
