<?php
function api_get_projects($id = NULL) {
  if ($id) {
		if ( ! $node = node_load(array('nid' => $id, 'type' => 'project'))) {
			api_error_response(404, 'project with id '.$id.' does not exist');
		}
		$response = api_transform_node($node);
	} else {
		$input = api_validate_list_input();
		$response = api_get_view_results('projects', NULL, $input->page, $input->limit);
	}
	api_resond($response);
}
