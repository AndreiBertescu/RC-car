//////////////////////////////////controller handling
var gamepad;
var buttonEnable = false;
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

  if (gamepad.buttons[2].value == 1)
    buttonPressed();

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
  sendPackage();
  isConnected = true;
  connection.send("@" + buttonEnable);
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
