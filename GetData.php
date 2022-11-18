<?php
header('Access-Control-Allow-Origin: *');

header('Access-Control-Allow-Methods: GET, POST');

header("Access-Control-Allow-Headers: X-Requested-With");

$servername = "128.100.119.14";
$username = "namiki";
$password = "namiki";

try {
  $conn = new PDO("mysql:host=$servername;dbname=maipping_temp", $username, $password);
  // set the PDO error mode to exception
  $conn->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
  //echo "Connected successfully";
} catch(PDOException $e) {
  echo "Connection failed: " . $e->getMessage();
 }
// $device = $_GET["deviceID"];
 $sql = "select * from realtime where device_id = '6'";
 $rmt = $conn->query($sql);
 $rmt->execute();
 $row = $rmt->fetch(PDO::FETCH_ASSOC);
 echo json_encode($row);
 $conn = null;

?>