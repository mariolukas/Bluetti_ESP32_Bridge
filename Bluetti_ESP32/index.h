const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>Bluetti MQTT Bridge</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    p { font-size: 1.2rem;}
    body {  margin: 0;}
    .topnav { overflow: hidden; background-color: #f09a10; color: white; font-size: 1rem; }
    .content { padding: 15px; ; text-align: center;}
    .card { background-color: white; box-shadow: 2px 2px 12px 1px rgba(135, 105, 105, 0.5); }
    .cards { max-width: 800px; margin: 0 auto; display: grid; grid-gap: 2rem; grid-template-columns: repeat(auto-fit, minmax(250px, 1fr)); }
    .reading { font-size: 1.4rem; }
    .version { margin-bottom: 1em; }
  </style>
</head>
<body>
  <div class="topnav">
    <h1>Bluetti MQTT Bridge web server</h1>
    <div class="version">
      Firmware Version: 0.1.0 (<a href='/update' target='_blank'>Update</a>)
    </div>
  </div>
  <div class="content">
    <div class="cards">
      <div class="card">
        <p>System</p>
          <p><span class="reading">
          <p>IP: <span id="ip">%IP% </span><span><a href="/rebootDevice" target="_blank">reboot</a></span></p>
          <p>MAC: <span id="mac">%MAC%</span></p>
          <p>SSID: <span id="ssid">%SSID% </span><span><a href="/resetConfig" target="_blank">reset</a></span></p>
          <p>RSSI: <span id="rssi">%RSSI%</span></p>
        </span></p>
      </div>
      <div class="card">
        <p>Runtime</p>
          <p><span class="reading"></p>
          <p><span id="run">%RUNTIME%</span> ms</span></p>
          <p><span id="run_h">%RUNTIME_H%</span>  h</span></p>
          <p><span id="run_d">%RUNTIME_D%</span>  d</span></p>
        </span></p>
      </div>
      <div class="card">
        <p>MQTT</p>
          <p><span class="reading"></p>
          <p>server: <span id="mqtt_ip">%MQTT_IP%</span></p>
          <p>port: <span id="mqtt_port">%MQTT_PORT%</span></p>
          <p>connected: <span id="mqtt_connected">%MQTT_CONNECTED%</span></p>
          <p>last msg time: <span id="mqtt_last_msg_time">%LAST_MQTT_MSG_TIME%</span></p>
        </span></p>
      </div>
      <div class="card">
        <p>Bluetooth</p>
          <p><span class="reading"></p>
          <p>Bluetti device id: <span id="device_id">%DEVICE_ID%</span></p>
          <p>connected: <span id="bt_connected">%BT_CONNECTED%</span></p>
          <p>last msg time: <span id="bt_last_msg_time">%LAST_BT_MSG_TIME%</span></p>
          <p>publish errors: <span id="bt_error">%BT_ERROR%</span></p>
          </span></p>
        </div>
      </div>
      <div class="card">
        <p>last messages (time / value):</p>
          <p><span class="reading"></p>
          <p><span id="last_msg">%LAST_MSG%</span></p>
          </span></p>
        </div>
      </div>
    </div>   
  </div>
<script>
if (!!window.EventSource) {
 var source = new EventSource('/events');
 
 source.addEventListener('open', function(e) {
  console.log("Events Connected");
 }, false);

 source.addEventListener('error', function(e) {
  if (e.target.readyState != EventSource.OPEN) {
    console.log("Events Disconnected");
  }
 }, false);
 
 source.addEventListener('message', function(e) {
  console.log("message", e.data);
 }, false);
 
 source.addEventListener('runtime', function(e) {
  console.log("runtime", e.data);
  document.getElementById("run").innerHTML = e.data;
  document.getElementById("run_h").innerHTML = Math.round(e.data/ 3600000,1);
  document.getElementById("run_d").innerHTML = Math.round(e.data/ 3600000/24,1);
 }, false);
 
 source.addEventListener('ip', function(e) {
  console.log("ip", e.data);
  document.getElementById("ip").innerHTML = e.data;
 }, false);
 
 source.addEventListener('mac', function(e) {
  console.log("mac", e.data);
  document.getElementById("mac").innerHTML = e.data;
 }, false);
 
 source.addEventListener('ssid', function(e) {
  console.log("ssid", e.data);
  document.getElementById("ssid").innerHTML = e.data;
 }, false);

 source.addEventListener('rssi', function(e) {
  console.log("rssi", e.data);
  document.getElementById("rssi").innerHTML = e.data;
 }, false);
 
 source.addEventListener('mqtt_ip', function(e) {
  console.log("mqtt_ip", e.data);
  document.getElementById("mqtt_ip").innerHTML = e.data;
 }, false);

  source.addEventListener('mqtt_port', function(e) {
  console.log("mqtt_port", e.data);
  document.getElementById("mqtt_port").innerHTML = e.data;
 }, false);

  source.addEventListener('mqtt_connected', function(e) {
  console.log("mqtt_connected", e.data);
  document.getElementById("mqtt_connected").innerHTML = e.data;
 }, false);

  source.addEventListener('mqtt_last_msg_time', function(e) {
  console.log("mqtt_last_msg_time", e.data);
  document.getElementById("mqtt_last_msg_time").innerHTML = e.data;
 }, false);
 
 source.addEventListener('device_id', function(e) {
  console.log("device_id", e.data);
  document.getElementById("device_id").innerHTML = e.data;
 }, false);
 
 source.addEventListener('bt_connected', function(e) {
  console.log("bt_connected", e.data);
  document.getElementById("bt_connected").innerHTML = e.data;
 }, false);

source.addEventListener('bt_last_msg_time', function(e) {
  console.log("bt_last_msg_time", e.data);
  document.getElementById("bt_last_msg_time").innerHTML = e.data;
 }, false);

 source.addEventListener('last_msg', function(e) {
  console.log("last_msg", e.data);
  document.getElementById("last_msg").innerHTML = e.data;
 }, false);
}
</script>
</body>
</html>)rawliteral";