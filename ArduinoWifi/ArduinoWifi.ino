#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WebSocketsServer.h>

ESP8266WiFiMulti wifiMulti;
ESP8266WebServer server(80);
WebSocketsServer webSocket(81);

const char *OTAName = "ESP8266";          
const char* mdnsName = "_ESP8266";
const char *OTAPassword = "esp8266";

////SETTINGS////
const char *ssid = "ESP8266 Access Point";
const char *password = "0000";
const char *networkName = "OPPOA72"; 
const char *networkPassword = "baftaboss";
bool webSocketIsConnected = false;

#define SPEED_PIN           D1
#define STEERING_PIN        D2
#define STEERING_ENABLE_PIN D3
////SETTINGS////

void setup() {
  pinMode(SPEED_PIN, OUTPUT);    // the pins with LEDs connected are outputs
  pinMode(STEERING_PIN, OUTPUT);
  pinMode(STEERING_ENABLE_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  pinMode(D4, OUTPUT);

  Serial.begin(115200);        // Start the Serial communication to send messages to the computer
  delay(10);
  Serial.println("\r\n");

  startWiFi();                 // Start a Wi-Fi access point, and try to connect to some given access points. Then wait for either an AP or STA connection
  startOTA();                  // Start the OTA service
  startWebSocket();            // Start a WebSocket server
  startMDNS();                 // Start the mDNS responder
  startServer();               // Start a HTTP server with a file read handler and an upload handler
}

void loop() {
  analogWrite(D4, 250);
  webSocket.loop();                           // constantly check for websocket events
  server.handleClient();                      // run the server
  ArduinoOTA.handle();                        // listen for OTA events
  digitalWrite(LED_BUILTIN, webSocketIsConnected? LOW : HIGH);
}

void startWiFi() {
  WiFi.softAP(ssid, password);
  Serial.print("Access Point \"");
  Serial.print(ssid);
  Serial.println("\" started\r\n");

  wifiMulti.addAP(networkName, networkPassword);   // add Wi-Fi networks you want to connect to //////////////////////////////////////

  Serial.println("Connecting");
  while (wifiMulti.run() != WL_CONNECTED && WiFi.softAPgetStationNum() < 1) {  // Wait for the Wi-Fi to connect
    delay(250);
    Serial.print('.');
  }
  Serial.println("\r\n");
  if(WiFi.softAPgetStationNum() == 0) {      // If the ESP is connected to an AP
    Serial.print("Connected to ");
    Serial.println(WiFi.SSID());             // Tell us what network we're connected to
    Serial.print("IP address:\t");
    Serial.print(WiFi.localIP());            // Send the IP address of the ESP8266 to the computer
  } else {                                   // If a station is connected to the ESP SoftAP
    Serial.print("Station connected to ESP8266 AP");
  }
  Serial.println("\r\n");
}

void startOTA() { // Start the OTA service
  ArduinoOTA.setHostname(OTAName);
  ArduinoOTA.setPassword(OTAPassword);

  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\r\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("OTA ready\r\n");
}

void startWebSocket() {
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  Serial.println("WebSocket server started.");
}

void startMDNS() {
  MDNS.begin(mdnsName);
  Serial.print("mDNS responder started: http://");
  Serial.print(mdnsName);
  Serial.println(".local");
}

void startServer() {
  server.on("/", HTTP_GET, handleRoot);     

  server.onNotFound([](){
    server.send(404, "text/plain", "404: Not found");
  });

  server.begin();
  Serial.println("HTTP server started.");
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) { // When a WebSocket message is received
  switch (type) {
    case WStype_DISCONNECTED:             // if the websocket is disconnected
      Serial.printf("[%u] Disconnected!\n", num);
      webSocketIsConnected = false;
      break;
    case WStype_CONNECTED: {              // if a new websocket connection is established
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        webSocketIsConnected = true;
      }
      break;
    case WStype_TEXT:
    /////////CODE HERE
      if (payload[0] == '#') {
        String package = (char *)payload;
        package.remove(0, 1);

        int speed = package.substring(0, package.indexOf(" ")).toInt();
        int steering = package.substring(package.indexOf(" ") + 1).toInt();
        Serial.print(speed);
        Serial.print(" - ");
        Serial.println(steering);

        analogWrite(SPEED_PIN, map(speed, -100, 100, 5, 250));
        analogWrite(STEERING_PIN, map(steering, -100, 100, 5, 250));
      } else if (payload[0] == '@') { 
        String package = (char *)payload;
        Serial.println(package);
        package.remove(0, 1);

        if(package.equals("true"))
          analogWrite(STEERING_ENABLE_PIN, 250);
        else
          analogWrite(STEERING_ENABLE_PIN, 5);
      } 
    /////////////////
    break;
  }
}

const char MAIN_page[] PROGMEM = 
R"=====(
  
<!DOCTYPE html>
<html>
<head>
<title>RC Car Control</title>
<meta charset="utf-8">
<!--<link rel="stylesheet" href="style.css">-->
<style>
@charset "utf-8";
* {
    margin: 0;
    padding: 0;
    font-family: Segoe, "Segoe UI";
    color: #042940;
    outline: none;
}
body {
    background-color: #005C53;
}
#mainContainer {
    background-color: #9FC131;
    margin: auto;
	  margin-top: 10vh;
    width: 50%;
    height: 600px;
}
header {
    text-align: center;
    text-transform: uppercase;
    font-size: 18px;
    padding: 25px 0 25px 0;
}
#sliderDiv {
    position: relative;
    width: 250px;
    margin: auto;
}
#speed {
    width: 250px;
    margin-top: 50%;
    position: absolute;
    background: none;
}
#steering {
    width: 250px;
    margin-top: 50%;
    position: absolute;
    -webkit-transform: rotate(270deg);
    -moz-transform: rotate(270deg);
    -ms-transform: rotate(270deg);
    -o-transform: rotate(270deg);
    transform: rotate(270deg);
}
#fakeSliderDiv {
    width: 250px;
    margin-top: 50%;
    position: absolute;
    height: 20px;
    background: #D6D58E;
    border-radius: 15px;
}
.slider {
    -webkit-appearance: none;
    background: #D6D58E;
    border-radius: 15px;
}
.slider::-webkit-slider-thumb {
-webkit-appearance: none;
appearance: none;
border-radius: 15px;
background: #042940;
width: 20px;
height: 20px;
}
#dataDiv {
    width: 50%;
    height: 100px;
    margin: auto;
    margin-top: 300px;
    font-weight: bolder;
}
#rotationButton {
    border: 1px solid #D6D58E;
    background: #042940;
    border-radius: 4px;
    text-align: center;
    font-weight: 500;
    color: #D6D58E;
    width: 80%;
    height: 23px;
    margin-left: 3%;
}
#rotationButton:hover {
    color: #A0A0A0;
    border-color: #A0A0A0;
    transition-duration: 0.1s;
}
#rotationButton:active {
    color: #7C7C7C;
    border-color: #7C7C7C;
}
#buttonData {
    width: 20px;
    height: 20px;
    background: red;
    margin-right: 3%;
    border-radius: 15px;
    border: 2px solid #042940;
}
#speedData, #steeringData, #connectionData, #controllerData {
    width: 100%;
    height: 100%;
    padding-top: 5px;
    text-align: center;
}
</style>
</head>

<body>
<div id="mainContainer">
  <header>
    <h1>rc car control</h1>
  </header>
  <div id="sliderDiv">
    <div id="fakeSliderDiv"></div>
    <input class="slider" id="steering" type="range" min="-100" max="100" value="0" oninput="sendPackage();">
    <input class="slider"  id="speed" type="range" min="-100" max="100" value="0" oninput="sendPackage();">
  </div>
  <div id="dataDiv">
    <table style="width: 100%; height: 100%;">
      <tr style="height: 20%;">
        <td><input id="rotationButton" type="button" value="Enable Rotation" onclick="buttonPressed();">
          <div id= "buttonData" style="float: right;"></div></td>
      </tr>
      <tr style="height: 20%;">
        <td><p id="speedData">Speed: 0%</p></td>
      </tr>
      <tr style="height: 20%;">
        <td><p id="steeringData">Steering: 0%</p></td>
      </tr>
      <tr style="height: 20%;">
        <td><p id="connectionData">Connection: Not connected</p></td>
      </tr>
      <tr style="height: 20%;">
        <td><p id="controllerData">Controller: Not connected</p></td>
      </tr>
    </table>
  </div>
</div>
<script>
	//////////////////////////////////controller handling
var gamepad;
var buttonEnable = false, isButtonPressed = false;
var isControllerConnected = null;
setInterval(updateStatus, 10);

var rAF = window.mozRequestAnimationFrame
  || window.webkitRequestAnimationFrame
  || window.requestAnimationFrame;

function connecthandler(e) {
  gamepad = e.gamepad;
  isControllerConnected = gamepad.id;
  document.getElementById('controllerData').innerHTML = "Controller: " + isControllerConnected;
  console.log(gamepad);
}

function disconnecthandler(e) {
  gamepad = null;
  isControllerConnected = null;
  document.getElementById('controllerData').innerHTML = "Controller: Not connected";
  console.log("Controller disconected");
}

function updateStatus() {
  scangamepads();
  if (!isControllerConnected)
    return;

  if (gamepad.buttons[2].value && !isButtonPressed){
    buttonPressed();
	isButtonPressed = true;
  }
  else if(!gamepad.buttons[2].value)
	isButtonPressed = false;

  var speedAxis = gamepad.buttons[7].value > gamepad.buttons[6].value ? gamepad.buttons[7].value : -gamepad.buttons[6].value;
  var steeringAxis = Math.abs(gamepad.axes[0]) > 0.1 ? gamepad.axes[0] : 0;

  Math.round(speedAxis * 100) / 100;
  Math.round(steeringAxis * 100) / 100;
  document.getElementById('steering').value = speedAxis * 100;
  document.getElementById('speed').value = steeringAxis * 100;
  sendPackage();
}

if ('GamepadEvent' in window) {
  window.addEventListener("gamepadconnected", connecthandler);
  window.addEventListener("gamepaddisconnected", disconnecthandler);
} else if ('WebKitGamepadEvent' in window) {
  window.addEventListener("webkitgamepadconnected", connecthandler);
  window.addEventListener("webkitgamepaddisconnected", disconnecthandler);
} else {
  setInterval(scangamepads, 500);
}

function scangamepads() {
  var gamepads = navigator.getGamepads ? navigator.getGamepads() : (navigator.webkitGetGamepads ? navigator.webkitGetGamepads() : []);
  gamepad = gamepads[0];
}

//////////////////////////////////arduino handling
var isConnected = false;
var connection = new WebSocket('ws://' + location.hostname + ':81/', ['arduino']);
connection.onopen = function () {
  document.getElementById('connectionData').innerHTML = "Connection: Connected";
  isConnected = true;
};
connection.onerror = function (error) {
  document.getElementById('connectionData').innerHTML = "Connection: Error";
  isConnected = false;
};
connection.onmessage = function (e) {
  console.log('Server: ', e.data);
};
connection.onclose = function () {
  document.getElementById('connectionData').innerHTML = "Connection: Terminated";
  isConnected = false;
};

function buttonPressed() {
  buttonEnable = !buttonEnable;
  document.getElementById('buttonData').style.backgroundColor = buttonEnable ? "green" : "red";
  if (isConnected)
    connection.send("@" + buttonEnable);
}

function sendPackage() {
  var speed = document.getElementById('speed').value;
  var steering = document.getElementById('steering').value;
  var package = "#" + speed + " " + steering;
  if (isConnected)
    connection.send(package);

  document.getElementById('speedData').innerHTML = "Speed: " + steering + "%";
  document.getElementById('steeringData').innerHTML = "Steering: " + speed + "%";
}
</script>
</body>
</html>

)=====";

void handleRoot() {
  server.send(200, "text/html", MAIN_page);
}