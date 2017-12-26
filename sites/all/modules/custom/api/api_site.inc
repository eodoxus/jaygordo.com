<?php

function api_get_site($id = NULL) {
	global $base_url;
	$response = new stdClass();
	$response->avatar = $base_url.'/sites/default/files/images/green_instgrm.JPG';
	$response->email 	= variable_get('site_mail');
	$response->footer = variable_get('site_footer');
	$response->home 	= variable_get('site_frontpage');
	$response->name 	= variable_get('site_name');
	$response->phone 	= variable_get('site_phone');
	$response->slogan = variable_get('site_slogan');
	api_resond($response);
}
