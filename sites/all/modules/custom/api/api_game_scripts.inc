<?php

function api_game_scripts($id = NULL) {
  if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $game_script = api_get_post_data();
    if ($game_script) {
      $response = api_save_game_script($game_script);
    }
  } else {
    if ($id) {
      $response = api_get_game_script($id);
    } else if ($_GET['token']) {
      $response = api_load_game_scripts_by_token($_GET['token']);
    }
  }
	api_resond($response);
}

function api_load_game_scripts_by_token($token) {
	$rslt = db_query("select * from game_saves where token = '%s'", $token);
  $game_save = db_fetch_object($rslt);
  if (!$game_save) {
    return;
  }
  $rslt = db_query("select * from game_scripts where game_save_id = %d", $game_save->id);
	while ($obj = db_fetch_object($rslt)) {
		$obj->data = json_decode($obj->data);
		$records[] = $obj;
	}
	return $records;
}

function api_save_game_script($game_script) {
  if (!isset($game_script->game_save_id)) {
    api_error_response(400, 'game script must have a game_save_id field');
  }
  if (!isset($game_script->data)) {
    api_error_response(400, 'game script must have a data field');
  }
  if (isset($game_script->id)) {
    db_query("UPDATE {game_scripts} SET data='%s'", json_encode($game_script->data));
  } else {
    $sql = "INSERT INTO {game_scripts} (game_save_id, data) VALUES (%d, '%s')";
    db_query($sql, $game_script->game_save_id, json_encode($game_script->data));
    $game_script->id = db_last_insert_id('game_scripts');
  }
  return $game_script;
}
