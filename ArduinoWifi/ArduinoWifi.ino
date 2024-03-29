#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server(80);

int speed = 0;
int steering = 0;
bool stop = false;
bool steeringLock = false;
bool honk = false;

extern const char mainPage[] PROGMEM;
const char *ssid = "RC-CAR_ESP8266";  //wi-fi network name
const char *password = "password";    //wi-fi password

///////////////SETTINGS
//The website url is: http://192.168.4.1
#define SPEED_PIN D5          //speed pin
#define STEERING_PIN D6       //steering pin
#define STEERING_LOCK_PIN D7  //steering lock pin
#define HONK_PIN D3           //honk pin

void setup() {
  pinMode(SPEED_PIN, OUTPUT);
  pinMode(STEERING_PIN, OUTPUT);
  pinMode(STEERING_LOCK_PIN, OUTPUT);
  pinMode(HONK_PIN, OUTPUT);

  Serial.begin(115200);
  delay(100);

  startServer();
}

void loop() {
  server.handleClient();

  if (stop == 1) {
    speed = 0;
    steering = 0;
  }
  
  analogWrite(SPEED_PIN, (int)map(speed, -100, 100, 5, 250));
  analogWrite(STEERING_PIN, (int)map(steering, -100, 100, 5, 250));
  analogWrite(STEERING_LOCK_PIN, steeringLock ? 250 : 5);

  if (honk)
    tone(HONK_PIN, 1000);
  else
    noTone(HONK_PIN);
}

void startServer() {
  WiFi.softAP(ssid, password);

  server.on("/", HTTP_GET, handleRoot);

  server.on("/data", HTTP_POST, handleData);

  server.onNotFound([]() {
    server.send(404, "text/plain", "404: Not found");
  });

  server.begin();
  Serial.println("Server started");
}

void handleRoot() {
  server.send(200, "text/html", mainPage);
}

void handleData() {
  if (server.hasArg("data")) {
    int nr = 0, aux = 0;
    String data = server.arg("data");
    String packets[6];
    
    for (int i = 0; i < data.length(); i++)
      if (data.charAt(i) == ' ') {
        packets[nr++] = data.substring(aux, i);
        aux = i + 1;
      }
    packets[4] = data.substring(aux, data.length());

    stop = packets[0] == "1" ? true : false;
    speed = packets[1].toInt();
    steering = packets[2].toInt();
    steeringLock = packets[3] == "1" ? true : false;
    honk = packets[4] == "1" ? true : false;

    server.send(200, "text/plain", "Data received: " + data);
  } else {
    Serial.println("Data not recieved.");
    server.send(400, "text/plain", "Bad Request");
  }
}

////////////////Web Page
const char mainPage[] PROGMEM =
  R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<meta http-equiv="X-UA-Compatible" content="ie=edge">
<title>RC Control Panel</title>
	
<style>
* {
  margin: 0;
  padding: 0;
  border-width: 0;
  text-align: center;
  color: #042940;
  font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, "Helvetica Neue", Arial, "Noto Sans", sans-serif, "Apple Color Emoji", "Segoe UI Emoji", "Segoe UI Symbol", "Noto Color Emoji";
  transition-duration: 0.2s;
  -webkit-touch-callout: none;
  -webkit-user-select: none;
  -khtml-user-select: none;
  -moz-user-select: none;
  -ms-user-select: none;
  user-select: none;
  font-weight: bold;
  overflow: hidden;
}
#mainContainer {
  width: 100%;
  height: 100vh;
  background: #005C53;
  display: flex;
  justify-content: center;
  align-items: center;
}
#mainDiv {
  min-width: 40vw;
  height: 100vh;
  background: #9FC131;
  border-radius: 3px;
  padding: 10px 0;
}
.paragraph {
  font-size: 18px;
  display: flex;
  justify-content: center;
  align-items: center;
  padding: 2%;
  min-height: 30px;
}
.paragraphRight {
  padding: 10%;
}
.dot {
  height: 25px;
  width: 25px;
  background-color: red;
  border: 2px solid #042940;
  border-radius: 30%;
  display: inline-block;
}
#sLock {
  margin-left: 10px;
  padding: 3% 6%;
  border-radius: 10px;
  border: 2px solid black;
  background: none;
  font-size: 18px;
}
#sLock:hover {
  background: rgba(0, 0, 0, 0.2);
}
#sLock:active {
  margin-left: 10px;
  padding: 3% 6%;
  background: rgba(0, 0, 0, 0.3);
}
#stop {
  font-size: 18px;
  padding: 5% 15%;
  border-radius: 5px;
  background: green;
  border: 3px solid #042940;
}
#honk {
  font-size: 18px;
  padding: 5% 5%;
  margin-right: 15px;
  border-radius: 5px;
  background: #0FB1BC;
  border: 3px solid #042940;
}
#honk:active {
  background: #0C919A;
}	
/* --------------------ProgressBars-------------------- */
#verticalBarContainer {
  position: relative;
  background-color: #9BC9C7;
  width: 50px;
  max-width: 100%;
  height: 80%;
  margin-left: calc(50% - 25px);
  margin-bottom: 5px;
}
#verticalBar {
  position: absolute;
  background-color: #199c85;
  bottom: 50%;
  width: 100%;
  height: 0px;
  transition-duration: 0s;
}
#horizontalBarContainer {
  position: relative;
  background-color: #9BC9C7;
  width: 80%;
  margin-left: 10%;
  height: 50px;
}
#horizontalBar {
  position: absolute;
  background-color: #199c85;
  left: 50%;
  width: 0px;
  height: 100%;
  transition-duration: 0s;
}
/* --------------------/ProgressBars-------------------- */
@media screen and (max-width: 900px) {
	#mainDiv {
	  min-width: 70vw;
	}
	#mainTitle{
		display: none;
	}
}
@media screen and (max-width: 600px) {
	#mainDiv {
	  min-width: 100vw;
	}
	#mainTitle{
		display: none;
	}
}
</style>
</head>

<body>
<div id="mainContainer">
  <div id="mainDiv">
    <table id="mainTitle" style="height: 10%; width: 100%;">
      <tr>
        <td style="border-bottom: 2px solid #042940;"><p class="paragraph" style="padding-bottom: calc(3% + 5px);">RC-CAR CONTROL PANEL</p></td>
      </tr>
    </table>
    <table style="height: 50%; width: 100%;">
      <tr style="height: 50%; width: 100%;">
        <td style="width: 30%; border-bottom: 2px solid #042940;"><div id="verticalBarContainer">
            <div id="verticalBar"> </div>
          </div>
          Speed: <span id="speed"> </span>%</td>
        <td style="width: 70%; border-bottom: 2px solid #042940; border-left: 2px solid #042940;"><div class = "paragraph paragraphRight"> <span class="dot" id="sLockSpan"></span>
            <button id="sLock" onClick="setSLock();">STEERING LOCK</button>
          </div>
          <div id="horizontalBarContainer">
            <div id="horizontalBar"> </div>
          </div>
          Steering: <span id="steering"></span>%
          <div class = "paragraph paragraphRight">
            <button id="honk" onclick="setHonk()">Honk</button>
            <button id="stop" onclick="setStop()">STOP</button>
          </div></td>
      </tr>
    </table>
    <table style="height: 40%; width: 100%;">
      <tr style="height: 10%; width: 100%;">
        <td><div class = "paragraph" style="height: 10%;"> <span class="dot" id="serverDot"></span>
            <p id="serverConnection">&nbsp;Server is not connected.</p>
          </div></td>
      </tr>
      <tr style="height: 10%; width: 100%;">
        <td><div class = "paragraph" style="height: 50%;"> <span class="dot" id="controllerDot"></span>
            <p id="controllerConnection">&nbsp;Controller is not connected.</p>
          </div></td>
      </tr>
      <tr style="height: 50%; width: 100%;">
        <td style="border-top: 2px solid #042940;margin-bottom: 10%;"><canvas id="canvas" style=" height:100%;"></canvas></td>
      </tr>
    </table>
  </div>
</div>
</body>
	
<script>
/* --------------------Controller-------------------- */
var activeGamepad = null;
var controllerSteering = 0;
var controllerSpeed = 0;
var buttonPressed = false;
var buttonPressed2 = false;

window.addEventListener("gamepadconnected", (event) => {
  console.log("A gamepad connected:");
  activeGamepad = null;
  controllerSteering = 0;
  controllerSpeed = 0;
});

window.addEventListener("gamepaddisconnected", (event) => {
  console.log("A gamepad disconnected:");
  activeGamepad = null;
  controllerSteering = 0;
  controllerSpeed = 0;
});

function checkGamepads() {
  var gamepads = navigator.getGamepads();

  if (gamepads[0] != null && activeGamepad == null) {
    activeGamepad = gamepads[0];
    console.log(gamepads[0]);
  } else if (gamepads[0] == null) {
    activeGamepad = null;
  }

  if (activeGamepad != null) {
    activeGamepad = gamepads[0];
    document.getElementById("controllerConnection").innerHTML = "&nbsp " + activeGamepad.id.split(' (')[0];
    document.getElementById("controllerDot").style.backgroundColor = "green";

    if (activeGamepad.buttons[0].pressed && !buttonPressed) {
      setSLock();
      buttonPressed = true;
    } else if (!activeGamepad.buttons[0].pressed)
      buttonPressed = false;

    if (activeGamepad.buttons[1].pressed) {
      enableHonk();
    } else
      disableHonk();

    if (activeGamepad.buttons[3].pressed && !buttonPressed2) {
      setStop();
      buttonPressed2 = true;
    } else if (!activeGamepad.buttons[3].pressed)
      buttonPressed2 = false;

    controllerSpeed = Math.floor(activeGamepad.axes[1] * 100);
    controllerSteering = Math.floor(activeGamepad.axes[2] * 100);
  } else {
    document.getElementById("controllerConnection").innerHTML = "&nbsp;Controller is not connected.";
    document.getElementById("controllerDot").style.backgroundColor = "red";
  }

  handleValues();

  requestAnimationFrame(checkGamepads);
}

// Start checking gamepads
requestAnimationFrame(checkGamepads);
/* --------------------/Controller-------------------- */

/* --------------------Joystick-------------------- */
var canvas, ctx;

window.addEventListener('DOMContentLoaded', () => {
  //server function
  setInterval(function () {
    checkServerConnection();
  }, 1000);

  setInterval(function () {
    sendData();
  }, 100);

  //joystick funtions
  canvas = document.getElementById('canvas');
  ctx = canvas.getContext('2d');
  resize();

  document.addEventListener('mousedown', startDrawing);
  document.addEventListener('mouseup', stopDrawing);
  document.addEventListener('mousemove', Draw);

  document.addEventListener('touchstart', startDrawing);
  document.addEventListener('touchend', stopDrawing);
  document.addEventListener('touchcancel', stopDrawing);
  document.addEventListener('touchmove', Draw);
  window.addEventListener('resize', resize);

  document.getElementById("speed").innerText = 0;
  document.getElementById("steering").innerText = 0;
  document.getElementById("verticalBar").style.height = "0px";
  document.getElementById("horizontalBar").style.width = "0px";
  speed = 0;
  steering = 0;
});


var width, height, radius, x_orig, y_orig, steering = 0,
  speed = 0;

function resize() {
  width = canvas.scrollWidth;
  height = canvas.scrollHeight;
  radius = height / 6.2;
  ctx.canvas.width = width;
  ctx.canvas.height = height;
  background();
  joystick(width / 2, height / 2, false);
}

function background() {
  x_orig = width / 2;
  y_orig = height / 2;

  ctx.beginPath();
  ctx.arc(x_orig, y_orig, radius * 2, 0, Math.PI * 2, true);
  ctx.fillStyle = '#9BC9C7';
  ctx.fill();
}

function joystick(x, y, fromController) {
  if (fromController) {
    var angle = Math.atan2(y, x);

    var x2 = mapValue(x, -100, 100, -radius, radius);
    var y2 = mapValue(y, -100, 100, -radius, radius);

    x = radius * Math.cos(angle);
    y = radius * Math.sin(angle);

    x = y2 + x_orig;
    y = x2 + y_orig;

    ctx.clearRect(0, 0, canvas.width, canvas.height);
    background();
  }

  ctx.beginPath();
  ctx.arc(x, y, radius, 0, Math.PI * 2, true);
  ctx.fillStyle = '#199c85';
  ctx.fill();
}

let coord = {
  x: 0,
  y: 0
};
let paint = false;

function getPosition(event) {
  var mouse_x = event.clientX || event.touches[0].clientX;
  var mouse_y = event.clientY || event.touches[0].clientY;

  var box = canvas.getBoundingClientRect();

  coord.x = mouse_x - box.left;
  coord.y = mouse_y - box.top;
}

function is_it_in_the_circle() {
  var current_radius = Math.sqrt(Math.pow(coord.x - x_orig, 2) + Math.pow(coord.y - y_orig, 2));
  if (radius >= current_radius) return true
  else return false
}


function startDrawing(event) {
  paint = true;
  getPosition(event);
  if (is_it_in_the_circle()) {
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    background();
    joystick(coord.x, coord.y, false);
    Draw();
  }
}


function stopDrawing() {
  paint = false;
  ctx.clearRect(0, 0, canvas.width, canvas.height);
  background();
  joystick(width / 2, height / 2, false);
  document.getElementById("speed").innerText = 0;
  document.getElementById("steering").innerText = 0;
  document.getElementById("verticalBar").style.height = "0px";
  document.getElementById("horizontalBar").style.width = "0px";
  speed = 0;
  steering = 0;
}

function Draw(event) {

  if (paint) {
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    background();
    var angle_in_degrees, x, y;
    var angle = Math.atan2((coord.y - y_orig), (coord.x - x_orig));

    if (Math.sign(angle) == -1) {
      angle_in_degrees = Math.round(-angle * 180 / Math.PI);
    } else {
      angle_in_degrees = Math.round(360 - angle * 180 / Math.PI);
    }

    if (is_it_in_the_circle()) {
      joystick(coord.x, coord.y, false);
      x = coord.x;
      y = coord.y;
    } else {
      x = radius * Math.cos(angle) + x_orig;
      y = radius * Math.sin(angle) + y_orig;
      joystick(x, y, false);
    }


    getPosition(event);

    steering = Math.round(mapValue(Math.round(x - x_orig), -24, 24, -100, 100));
    speed = Math.round(mapValue(Math.round(y - y_orig), -24, 24, -100, 100));

    handleValues();
  }
}

function mapValue(x, inMin, inMax, outMin, outMax) {
  return (x - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
}
/* --------------------/Joystick-------------------- */

/* --------------------DataHandling-------------------- */
var steeringLock = false;
var honk = false;
var stop = true;
var isConnected = false;
var serverMessage = "Connection not checked.";
var finalSpeed = 0;
var finalSteering = 0;
var prevaux = false;

function setSLock() {
  steeringLock = !steeringLock;
}

function setStop() {
  stop = !stop;
}

function setHonk() {
  honk = !honk;
}

function enableHonk() {
  honk = true;
}

function disableHonk() {
  honk = false;
}

function handleValues() {
  //server Stuff
  if (!isConnected) {
    //    stop = true;
    document.getElementById("serverConnection").innerHTML = "&nbsp;" + serverMessage;
    document.getElementById("serverDot").style.backgroundColor = "red";
  } else {
    document.getElementById("serverConnection").innerHTML = "&nbsp;Server is connected.";
    document.getElementById("serverDot").style.backgroundColor = "green";
  }

  document.getElementById("sLockSpan").style.backgroundColor = steeringLock ? "green" : "red";
  document.getElementById("stop").style.backgroundColor = stop ? "green" : "#dc3545";
  document.getElementById("stop").innerHTML = stop ? "START" : "STOP";
  document.getElementById("honk").style.backgroundColor = honk ? "#0F60BC" : "#0FB1BC";

  //speed and steering
  if (!stop) {
    var aux = false;

    if (Math.abs(speed) < Math.abs(controllerSpeed) && Math.abs(controllerSpeed) > 5 && Math.abs(controllerSpeed) <= 100) {
      finalSpeed = -controllerSpeed;
      aux = true;
    } else
      finalSpeed = -speed;

    if (Math.abs(steering) < Math.abs(controllerSteering) && Math.abs(controllerSteering) > 5 && Math.abs(controllerSteering) <= 100) {
      finalSteering = controllerSteering;
      aux = true;
    } else
      finalSteering = steering;

    if (aux) {
      background();
      joystick(controllerSpeed, controllerSteering, true);
      prevaux = true;
    }

    if (!aux && prevaux) {
      background();
      joystick(width / 2, height / 2, false);
      prevaux = false;
    }

    if (finalSpeed > 0) {
      document.getElementById("verticalBar").style.bottom = "50%";
      document.getElementById("verticalBar").style.top = "inherit";
    } else {
      document.getElementById("verticalBar").style.bottom = "inherit";
      document.getElementById("verticalBar").style.top = "50%";
    }

    if (-finalSteering > 0) {
      document.getElementById("horizontalBar").style.right = "50%";
      document.getElementById("horizontalBar").style.left = "inherit";
    } else {
      document.getElementById("horizontalBar").style.right = "inherit";
      document.getElementById("horizontalBar").style.left = "50%";
    }

    document.getElementById("verticalBar").style.height = Math.abs(finalSpeed / 2) + "%";
    document.getElementById("horizontalBar").style.width = Math.abs(finalSteering / 2) + "%";
    document.getElementById("speed").innerText = finalSpeed;
    document.getElementById("steering").innerText = finalSteering;
  }
}
/* --------------------/DataHandling-------------------- */

/* --------------------ServerHandling-------------------- */
function sendData() {
  if (!isConnected)
    return;

  var data = stop ? "1" : "0";
  data += " " + finalSpeed;
  data += " " + finalSteering;
  data += " " + (steeringLock ? "1" : "0");
  data += " " + (honk ? "1" : "0");

  var xhr = new XMLHttpRequest();
  xhr.open("POST", "/data", true);
  xhr.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");

  xhr.send("data=" + data);
}

function checkServerConnection() {
  var xhr = new XMLHttpRequest();
  var url = "http://192.168.4.1";
  xhr.open("GET", url, true);
  xhr.timeout = 2000;

  xhr.onreadystatechange = function () {
    if (xhr.readyState === 4)
      if (xhr.status === 200)
        isConnected = true;
      else {
        isConnected = false;
        serverMessage = "Request encountered an error: " + xhr.status + ".";
      }
  };

  xhr.ontimeout = function () {
    serverMessage = "Request timed out."; // Request timed out
  };

  xhr.onerror = function () {
    serverMessage = "Request encountered an error."; // Request encountered an error
  };

  xhr.send();
}
/* --------------------/ServerHandling-------------------- */
</script>
</html>
)=====";
