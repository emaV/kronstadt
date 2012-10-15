<?php
// $Id: gigya.inc,v 1.1.2.16 2009/10/21 04:53:58 japerry Exp $

/**
 * @file
 * This module provides a link to Gigya profiles from a users' drupal account.
 *
 * Development of this module is sponsered by gigya, Inc and implimented by Acquia.
 *
 * @version 1.0
 * @package gigya
 */

/**
 * Main user authentication function.
 *
 * If successful, sets the global $user object.
 */
function gigya_authenticate($form_values, $gigya_loggedin = FALSE) {
  include_once($base_path .'modules/user/user.pages.inc');
  global $user, $_gigya_uid;

  $dest = $base_path . variable_get('gigya_login_uiconfig_urlredirect', 'user');
  //we've logged in with a user through the normal login block. We need to check to see if they're a gigya user as well
  if (!$gigya_loggedin) {
    if (user_authenticate($form_values)) {
      return "I'm logged in";
    }
  }
  $localkey = _gigya_verify_signature($form_values['timestamp'], $form_values['gigya_uid'], variable_get('gigya_SecretKey', FALSE));
  //first, verify signature
  if ($localkey != $form_values['signature']) {
    //print $localkey ."    and     ". $form_values['signature'];
    drupal_set_message(t('Unable to authenticate. Gigya signature does not match.', 'error'));
      if ($user->uid == 1) {
        drupal_set_message(t('Signature is %gigya, Site sig is %site.', array('%gigya' => $form_values['signature'], '%site' => $localkey)), 'error');
      }
  }

  //check to see if the gigya_user is linked to a drupal user
  if ($drupal_uid = _gigya_get_uid_by_gid($form_values['gigya_uid'])) {
    $user = user_load($drupal_uid);
    $_gigya_uid = $form_values['gigya_uid'];
    user_authenticate_finalize($form_values);
    $_SESSION['gigya_uid'] = $form_values['gigya_uid'];
    drupal_goto($dest);
  }
  else { //otherwise, lets give them a registration page
    //$output .= drupal_get_form('user_login');
    $output .= drupal_get_form('user_register');
    return $output;
  }
}

/**
 * Storage function to link the gigya GID+Provider to a drupal username
 *
 * @param array &$edit contains the uid, gigya_uid, and a provider to be stored
 */
function gigya_save_profile($edit) {
  if (!$edit['uid'] || $edit['gigya_uid'] == '') {
    return;
  }
  //print_r("Socialize ID:" . $_SESSION['gigya_uid']);
  if (!isset($edit['loginProvider'])) {
    $bio = gigya_getUserInfo($_SESSION['gigya_uid']);
    //print_r($bio);
    $edit['loginProvider'] = $bio['loginProvider']; 
  }
  if($edit['loginProvider'] && $edit['uid'] && $edit['gigya_uid']) {
    db_query("UPDATE {gigya} SET uid = %d, gigya_uid = '%s', loginProvider = '%s' WHERE uid = %d", $edit['uid'], $edit['gigya_uid'], $edit['loginProvider'], $edit['uid']);
    if (!db_affected_rows()) {
      db_query("INSERT INTO {gigya} (uid, gigya_uid, loginProvider) VALUES (%d, '%s', '%s')", $edit['uid'], $edit['gigya_uid'], $edit['loginProvider']);
    }
    return TRUE;
  } else {
    drupal_set_message("Error saving your socialize profile info. Please contact the system administrator",'error');
    return FALSE;
  }
}

function gigya_delete_profile($uid) {
  db_query("DELETE FROM {gigya} WHERE uid = %d", $uid);
  return TRUE;
}

function gigya_get_loginprovider($uid) {
  if($login_provider = db_result(db_query("SELECT loginProvider FROM {gigya} WHERE uid = %d",$uid)))
    return $login_provider;
  return false;
}

function gigya_avatar_status($uid) {
  $res = db_result("SELECT use_avatar, avatar_timestamp FROM {gigya} WHERE uid=%d",$uid);
  while($data = db_fetch_array($res)) {
    if(!$data['use_avatar']) {
      return FALSE;
    } 
    return $data;
  }
}

function gigya_avatar_check($uid) {
  $bio = gigya_getUserInfo(_gigya_get_gid_by_uid($uid));
  if ($bio['thumbnailURL'])
    return $bio['thumbnailURL'];
  else if ($bio['photoURL'])
    return $bio['photoURL'];
  else { 
    return FALSE;
  }
}
  

function gigya_save_userpic($uid) {
  // Get facebook user picture's url.

  if($avatar = gigya_avatar_check($uid)) {
    $result = drupal_http_request($avatar);
    
    if ($result->code != 200) {
      watchdog('gigya', 'Failed importing user avatar for @uid, code : @code', 
      array('@uid' => $uid, '@code' => $result->code));
      return FALSE;
    }  
    
    $filename = 'picture-gigya_'. $uid .'.jpg';
    $dest = file_directory_path();  
    if (variable_get('user_picture_path', '')) {
      $dest .= '/'. variable_get('user_picture_path', '');
    }
    $dest .= '/'. $filename;
    
    $file = file_save_data($result->data, $dest, FILE_EXISTS_REPLACE);
    // Resizing
    list($width, $height) = sscanf(variable_get('user_picture_dimensions', '85x85'), '%dx%d');
    image_scale_and_crop($file, $dest, $width, $height);
    drupal_set_message("user pic saved successfully!");

    return $file;
  } else {
    drupal_set_message("Your social network does not have a profile picture");
    return FALSE;
  }
}

/**
 *  Gigya parameter functions below
 */

function gigya_get_capabilities() {
  //this function retrieves services available on this account
  //currently the module only supports login, notifications, and friends until the Gigya REST API comes out.
  $services = array(
    'login' => 'login',
    'notifications' => 'notifications',
    'friends' => 'friends',
    'status' => 'status',
    'newsfeed' => 'newsfeed',
  );
  return $services;
}

function gigya_status_form($account = NULL) {
  drupal_add_js(drupal_get_path('module', 'gigya') .'/gigya_updatestatus.js', 'module');

  if (empty($account)) {
    global $user;
    $account = $user;
  }

  if ($capabilities = _gigya_getUserCapabilities($_SESSION['gigya_uid'])) {
    if (array_search('Status', $capabilities)) {
      $form = array();
      $form['status'] = array(
        '#type' => 'textfield',
        '#id' => 'gigya-textfield',
      );
      return $form;
    }
  }
}

function gigya_get_networks() {
  //this function gets the list of services that can be connected to
  $services = array(
    'facebook' => 'Facebook',
    'myspace' => 'MySpace',
    'twitter' => 'Twitter',
    'google' => 'Google',
    'yahoo' => 'Yahoo',
    'aol' => 'AOL',
  );
  return $services;
}


function _gigya_get_profile_fields() {
  //todo add the ability to specify the field type
  return array(
    "nickname" => "nickname",
    "firstName" => "firstName",
    "lastName" => "lastName",
    "gender" => "gender",
    "age" => "age",
    "birthDay" => "birthDay",
    "birthMonth" => "birthMonth",
    "birthYear" => "birthYear",
    "email" => "email",
    "country" => "country",
    "state" => "state",
    "city" => "city",
    "zip" => "zip",
  );
}

/**
 * Internal gigya function to get the Gigya User id when the drupal uid is known
 * @param integer $uid The drupal user id
 * @return integer gigya user id if it exists in the link table
 */
function _gigya_get_gid_by_uid($uid) {
  if ($gid = db_result(db_query("SELECT gigya_uid FROM {gigya} WHERE uid = %d", $uid))) {
    return $gid;
  }
  else {
    return FALSE;
  }
}

function _gigya_getUserCapabilities($uid) {
  if (empty($uid)) {
    return;
  }
  if ($bio = gigya_getUserInfo($uid)) {
    $capabilities = split(', ', $bio['capabilities']);
    array_walk($capabilities, '_gigya_trim_value');
    return $capabilities;
  }
  else {
    return FALSE;
  }
}

/**
 * Internal gigya function to get the Gigya User id when the drupal uid is known
 * @param integer $uid The drupal user id
 * @return integer gigya user id if it exists in the link table
 */
function _gigya_get_linked_networks($uid) {
  $network = array();
  //TODO: REST API call here
  $res = db_query("SELECT gigya_uid,loginProvider FROM {gigya} WHERE uid = %d", $uid);
  while ($gid = db_fetch_array($res)) {
    $network[$gid['loginProvider']] = $gid['gigya_uid'];
  }
  return $network;
}

/**
 * Internal gigya function to get the drupal user id when the gigya uid is known
 * @param integer $uid The gigya user id
 * @return integer drupal user id if it exists in the link table
 */
function _gigya_get_uid_by_gid($gid) {
  if ($uid = db_result(db_query("SELECT uid FROM {gigya} WHERE gigya_uid = '%s'", $gid))) {
    return $uid;
  }
  else {
    return FALSE;
  }
}

/**
 * Internal gigya function to get the drupal user id when the gigya uid is known
 * @param integer $uid The gigya user id
 * @return integer drupal user id if it exists in the link table
 */
function _gigya_network_lookup($gid) {
  if ($provider = db_result(db_query("SELECT loginProvider FROM {gigya} WHERE gigya_uid = '%s'", $gid))) {
    return $provider;
  }
  else {
    return FALSE;
  }
}

function _gigya_trim_value(&$value) {
    $value = trim($value);
}

/**
 *  Internal gigya signature verification functions
 */

/**
 * Wrapper around the hmacsha1 signature function
 * @param string $timestamp The current timestamp for the request being made or received
 * @param string $uid The gigya GID being passed to/from gigya
 * @param string $privatekey The private key set in the admin settings. Fetch the key from the gigya socialize site
 */
function _gigya_verify_signature($timestamp, $uid, $privatekey) {

  $ch1data = $timestamp .'_'. $uid;
  $keydata = base64_decode($privatekey);
  return _gigya_encodedata($keydata, $ch1data);
}

/**
 * Wrapper for the base64 encoding of the hmacsha1 function
 * @param string $key The base64 decoded version of the private key set in the gigya settings
 * @param string $data The timestamp+uid being passed for comparison
 * @return The signature created by the hmacsha1 function. Should be the same as the signature retrieved from gigya socialize
 */
function _gigya_encodedata($key, $data) {
    return base64_encode(_gigya_hmacsha1($key, $data));
}

/**
 *  Internal gigya signature verification function
 */
function _gigya_hmacsha1($key, $data) {
    $blocksize=64;
    $hashfunc='sha1';
    if (drupal_strlen($key)>$blocksize)
        $key=pack('H*', $hashfunc($key));
    $key = str_pad($key, $blocksize, chr(0x00));
    $ipad = str_repeat(chr(0x36), $blocksize);
    $opad = str_repeat(chr(0x5c), $blocksize);
    $hmac = pack(
                'H*', $hashfunc(
                    ($key^$opad) . pack(
                        'H*', $hashfunc(
                            ($key^$ipad) . $data
                        )
                    )
                )
            );
    return $hmac;
}

/**
 * Very lightweight helper function to generate a TinyURL for a given post.
 */
function gigya_shorten_url($url) {
  $response = drupal_http_request("http://tinyurl.com/api-create.php?url=". $url);
  if ($response->code == 200) {
    return $response->data;
  }
  else {
    return $url;
  }
}

/**
 * Used to validate the gigya session keys. We use the site 'admin' username to find out the status. 
 * If it shows the user logged out, thats good, if it returns an error then our keys are most likely bad
 */ 
function gigya_validate($update = FALSE) {
  $gigya_params = gigya_load_params();
  $gigya_params['uid'] = 'admin';
  $signature = _gigya_calc_signature(variable_get('gigya_SecretKey', FALSE), 'GET', 'getUserInfo', $gigya_params);
  $url = "http://". variable_get('gigya_api_url', 'socialize.api.gigya.com') ."/socialize.getUserInfo".
            "?apiKey=". drupal_urlencode($gigya_params['apiKey']) .
            "&timestamp=". drupal_urlencode($gigya_params['timestamp']) .
            "&nonce=". drupal_urlencode($gigya_params['nonce']) .
            "&sig=". drupal_urlencode($signature) .
            "&uid=". urlencode($gigya_params['uid']); //for some reason drupal_urlencode doesn't take out slashes
  $headers = array('Content-type' => 'application/x-www-form-urlencoded');
  $results = drupal_http_request($url, $headers, 'GET');
  //print_r($results);
  if ($error = _gigya_request_failure($results,true)) {
    variable_set("gigya_validate", FALSE);
    if($error == 403003) {
      if(!$update) 
        drupal_set_message("Your request signature is invalid. This is most likely caused by invalid site and/or private keys. Gigya is disabled until this error message is cleared",'error');
    } else {
      if(!$update) 
        drupal_set_message("Gigya encountered an error while validating itself. Please check watchdog and try later. If this message keeps appearing please contact gigya or file a bug in the gigya project issue queue",'error');
    }
    return FALSE;
  }
  if(!$update) 
    drupal_set_message("Gigya validated properly. This site is authorized to use Gigya services");  
  variable_set("gigya_validate", TRUE);
  return TRUE;
}
  
/**
 *  GIGYA REST API functions
 */

 /**
 * Fetch information about users following a given socialize account.
 */
function gigya_getUserInfo($gigya_uid) {
  if (empty($gigya_uid))
    return FALSE;

  $gigya_params = gigya_load_params();
  $gigya_params['uid'] = $gigya_uid;
  $signature = _gigya_calc_signature(variable_get('gigya_SecretKey', FALSE), 'GET', 'getUserInfo', $gigya_params);
  $url = "http://". variable_get('gigya_api_url', 'socialize.api.gigya.com') ."/socialize.getUserInfo".
            "?apiKey=". drupal_urlencode($gigya_params['apiKey']) .
            "&timestamp=". drupal_urlencode($gigya_params['timestamp']) .
            "&nonce=". drupal_urlencode($gigya_params['nonce']) .
            "&sig=". drupal_urlencode($signature) .
            "&uid=". urlencode($gigya_params['uid']); //for some reason drupal_urlencode doesn't take out slashes
  $headers = array('Content-type' => 'application/x-www-form-urlencoded');
  $results = drupal_http_request($url, $headers, 'GET');
  //print_r($results);
  if (_gigya_request_failure($results)) {
    return FALSE;
  }
  return _gigya_convert_xml_to_array($results->data);
}

function gigya_getFriendsInfo($gigya_uid) {
  if (!$gigya_uid)
    return FALSE;

  $gigya_params = gigya_load_params();
  $gigya_params['uid'] = $gigya_uid;
  $signature = _gigya_calc_signature(variable_get('gigya_SecretKey', FALSE), 'GET', 'getFriendsInfo', $gigya_params);
  $url = "http://". variable_get('gigya_api_url', 'socialize.api.gigya.com') ."/socialize.getFriendsInfo".
            "?apiKey=". drupal_urlencode($gigya_params['apiKey']) .
            "&timestamp=". drupal_urlencode($gigya_params['timestamp']) .
            "&nonce=". drupal_urlencode($gigya_params['nonce']) .
            "&sig=". drupal_urlencode($signature) .
            "&uid=". urlencode($gigya_params['uid']);
  $headers = array('Content-type' => 'application/x-www-form-urlencoded');
  $results = drupal_http_request($url, $headers, 'GET');
  if (_gigya_request_failure($results)) {
    return FALSE;
  }
  return _gigya_convert_xml_to_array($results->data);
}

function gigya_setStatus($gigya_uid, $status) {
  $gigya_params = gigya_load_params();
  $gigya_params['uid'] = $gigya_uid;
  $gigya_params['status'] = $status;

  $signature = _gigya_calc_signature(variable_get('gigya_SecretKey', FALSE), 'GET', 'setStatus', $gigya_params);
  $url = "http://". variable_get('gigya_api_url', 'socialize.api.gigya.com') ."/socialize.setStatus".
            "?apiKey=". drupal_urlencode($gigya_params['apiKey']) .
            "&timestamp=". drupal_urlencode($gigya_params['timestamp']) .
            "&nonce=". drupal_urlencode($gigya_params['nonce']) .
            "&sig=". drupal_urlencode($signature) .
            "&uid=". urlencode($gigya_params['uid']) .
            "&status=". rawurlencode($gigya_params['status']);
  $headers = array('Content-type' => 'application/x-www-form-urlencoded');
  $results = drupal_http_request($url, $headers, 'GET');

  if (_gigya_request_failure($results)) {
    return FALSE;
  }

  return _gigya_convert_xml_to_array($results->data);
}

function gigya_publish_user_action($gigya_uid, $content) {
  $gigya_params = gigya_load_params();
  $gigya_params['uid'] = $gigya_uid;
  $gigya_params['userAction'] = _gigya_get_useraction_xml($content);
  $signature = _gigya_calc_signature(variable_get('gigya_SecretKey', FALSE), 'GET', 'publishUserAction', $gigya_params);

  $url = "http://". variable_get('gigya_api_url', 'socialize.api.gigya.com') ."/socialize.publishUserAction".
            "?apiKey=". drupal_urlencode($gigya_params['apiKey']) .
            "&timestamp=". drupal_urlencode($gigya_params['timestamp']) .
            "&nonce=". drupal_urlencode($gigya_params['nonce']) .
            "&sig=". drupal_urlencode($signature) .
            "&uid=". urlencode($gigya_params['uid']) .
            "&userAction=". rawurlencode($gigya_params['userAction']);

  $headers = array('Content-type' => 'application/x-www-form-urlencoded');
  $results = drupal_http_request($url, $headers, 'GET');


  if (_gigya_request_failure($results)) {
    return FALSE;
  }
  return _gigya_convert_xml_to_array($results->data);
}

function _gigya_get_useraction_xml($content) {
  $template = $content['template'];
  $action = $content['action'];
  //$icon = $content['node']
  $title = htmlspecialchars($content['title']);
  $path = htmlspecialchars(url($content['path'], array('absolute' => TRUE)));
  $xml = "<userAction><template>$template</template><actionName>$action</actionName><templateFields><templateField><name>thing</name><text>$title</text><href>$path</href></templateField></templateFields></userAction>";
  return $xml;
}

function _gigya_calc_signature($secretkey, $httpmethod, $apimethod, $parameters) {
  require_once("OAuth.inc");
  $req = OAuthRequest::from_request($httpmethod, 'http://socialize.api.gigya.com/socialize.'. $apimethod, $parameters);
  //print_r("Base String.... ");
  //print_r($req->get_signature_base_string());
  //print_r("<br /><br />");
  return base64_encode(hash_hmac('sha1', $req->get_signature_base_string(), base64_decode($secretkey), TRUE));
}

/**
  * Internal helper function to deal cleanly with various HTTP response codes.
  */
function _gigya_request_failure($results, $return_error = FALSE) {
  //print_r($results);
  $code = $results->code;
  $errormsg = '';
  if($code == 200) {
    $result = _gigya_convert_xml_to_array($results->data);
    $code = intval($result['statusCode']);
    $errormsg = $result['errorMessage'];
    $errorcode = $result['errorCode'];
    if(!$errorcode) 
      $errorcode = $code;
  }
  switch ($code) { //see if there is a issue with the request on the http level
    case 302:
      watchdog('gigya', $errorcode . ' Missing Schema. ' . $errormsg);
      if ($return_error)
        return $errorcode;
      else
        return TRUE;
    case 304:
      // 304 Not Modified: there was no new data to return.
      if ($return_error)
        return $errorcode;
      else
        return TRUE;
    case 400:
      // 400 Bad Request: your request is invalid, and we'll return an error message that tells you why. This is the status code returned if you've exceeded the rate limit
      watchdog('gigya', $errorcode . ' Bad Request. ' . $errormsg);
      if ($return_error)
        return $errorcode;
      else
        return TRUE;
    case 401:
      // 401 Not Authorized: either you need to provide authentication credentials, or the credentials provided aren't valid.
      watchdog('gigya', $errorcode . ' Not Authorized. ' . $errormsg);
      if ($return_error)
        return $errorcode;
      else
        return TRUE;
    case 403:
      // 403 Forbidden: we understand your request, but are refusing to fulfill it.  An accompanying error message should explain why.
      watchdog('gigya', $errorcode . ' Forbidden. ' . $errormsg);
      if ($return_error)
        return $errorcode;
      else
        return TRUE;
    case 404:
      // 404 Not Found: either you're requesting an invalid URI or the resource in question doesn't exist (ex: no such user)
      watchdog('gigya', $errorcode . ' Not Found. ' . $errormsg);
      if ($return_error)
        return $errorcode;
      else
        return TRUE;
    case 500:
      // 500 Internal Server Error: we did something wrong.  Please post to the group about it and the Gigya team will investigate.
      watchdog('gigya', $errorcode . ' Internal Server Error. ' . $errormsg);
      if ($return_error)
        return $errorcode;
      else
        return TRUE;
    case 502:
      // 502 Bad Gateway: returned if Gigya is down or being upgraded.
      watchdog('gigya', $errorcode . ' Bad Gateway. ' . $errormsg);
      if ($return_error)
        return $errorcode;
      else
        return TRUE;
    case 503:
      // 503 Service Unavailable: the Gigya servers are up, but are overloaded with requests.  Try again later.
      watchdog('gigya', $errorcode . ' Service Unavailable. ' . $errormsg);
      if ($return_error)
        return $errorcode;
      else
        return TRUE;
  }
  // 200 OK: everything went awesome.
  return FALSE;
}

/**
 * Internal XML munging code
 */

function _gigya_convert_xml_to_array($data) {
  $xml = new SimpleXMLElement($data);
  return get_object_vars($xml);
}

