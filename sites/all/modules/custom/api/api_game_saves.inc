<?php

function api_game_saves($id = NULL) {
  if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $game_save = api_get_post_data();
    if ($game_save) {
      $response = api_save_game_save($game_save);
    }
  } else {
    if ($id) {
      $response = api_get_game_save($id);
    } else if ($_GET['token']) {
      $response = api_load_game_saves_by_token($_GET['token']);
    }
  }
	api_resond($response);
}

function api_get_game_save($id) {
  $rslt = db_query("select * from game_saves where id = %d", $id);
	$obj = db_fetch_object($rslt);
  if (!$obj) {
    api_error_response(404, 'game save with id '.$id.' does not exist');
  }
  return $response;
}

function api_load_game_saves_by_token($token) {
	$rslt = db_query("select * from game_saves where token = '%s'", $token);
	$records = array();
	while ($obj = db_fetch_object($rslt)) {
		$obj->data = json_decode($obj->data);
		$records[] = $obj;
	}
	return $records;
}

function api_save_game_save($game_save) {
  if (!isset($game_save->token)) {
    api_error_response(400, 'game save must have a token field');
  }
  if (!isset($game_save->slot)) {
    api_error_response(400, 'game save must have a slot field');
  }
  if (!isset($game_save->data)) {
    api_error_response(400, 'game save must have a data field');
  }
  if (isset($game_save->id)) {
    db_query("UPDATE {game_saves} SET data='%s'", json_encode($game_save->data));
  } else {
    $sql = "INSERT INTO {game_saves} (token, slot, data) VALUES ('%s', %d, '%s')";
    db_query($sql, $game_save->token, $game_save->slot, json_encode($game_save->data));
    $game_save->id = db_last_insert_id('game_saves');
  }
  return $game_save;
}
