<?php
function api_get_photos($id = NULL) {
  if ($id) {
		$response = _api_load_photo($id);
		if ( ! $response) {
			api_error_response(404, 'photo with id '.$id.' does not exist');
		}
	} else {
		$input = api_validate_list_input();
		$response = _api_query_photos($input->page, $input->limit);
	}
	api_resond($response);
}


function _api_query_photos($page = 1, $limit = 20) {
	$sql = _api_get_photos_query()." and n.type <> 'project' limit $limit offset ".($page * $limit);
	$rslt = db_query($sql);
	$records = array();
	while ($obj = db_fetch_object($rslt)) {
		$obj->meta = unserialize($obj->meta);
		$records[] = $obj;
	}
	return $records;
}

function _api_load_photo($id) {
	$sql = _api_get_photos_query()." and f.fid = ".mysql_real_escape_string($id);
	$rslt = db_query($sql);
	$obj = db_fetch_object($rslt);
	$obj->meta = unserialize($obj->meta);
	return $obj;
}

function _api_transform_photo($photo) {
	$photo->meta = unserialize($photo->meta);
	return $photo;
}

function _api_get_photos_query() {
	return "select f.fid, f.filepath, n.title, ci.field_images_data as meta from files f
		join content_field_images ci on ci.field_images_fid = f.fid
		join node n on n.nid = ci.nid
		where ci.field_images_fid is not null";
}
