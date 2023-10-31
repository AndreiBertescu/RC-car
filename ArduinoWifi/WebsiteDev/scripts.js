/* --------------------Controller-------------------- */
var activeGamepad = null;
var controllerSteering = 0;
var controllerSpeed = 0;

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

    controllerSpeed = Math.floor(activeGamepad.axes[1] * 100);
    controllerSteering = Math.floor(activeGamepad.axes[0] * 100);
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

  //steering lock
  document.getElementById("sLockSpan").style.backgroundColor = steeringLock ? "green" : "red";
  document.getElementById("stop").style.backgroundColor = stop ? "green" : "#dc3545";
  document.getElementById("stop").innerHTML = stop ? "START" : "STOP";

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

    if (finalSteering > 0) {
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
  if(!isConnected)
	  return;
	
  var data = stop ? "1" : "0";
  data += " " + finalSpeed;
  data += " " + finalSteering;
  data += " " + (steeringLock ? "1" : "0");

  var xhr = new XMLHttpRequest();
  xhr.open("POST", "/data", true);
  xhr.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");

  // xhr.onreadystatechange = function() {
  //   if (xhr.readyState == 4 && xhr.status == 200) {
  //     // Handle the response from the server if needed
  //     console.log(xhr.responseText);
  //   }
  // };
  
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