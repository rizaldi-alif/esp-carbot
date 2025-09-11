let websocket;
let isConnected = false;
const obsDistContainer = document.getElementById("obs-dist");
const lmsContainer = document.getElementById("left-motor-speed");
const rmsContainer = document.getElementById("right-motor-speed");
const dirContainer = document.getElementById("car-direction");

window.addEventListener("load", () => {
  initWebSocket();
});

window.addEventListener(
  "touchstart",
  (e) => {
    if (e.touches.length > 1) {
      e.preventDefault(); // Prevent multi-touch
    }
  },
  false
);

window.addEventListener("beforeunload", () => {
  if (websocket) {
    websocket.close();
  }
});

function initWebSocket() {
  websocket = new WebSocket(`ws://${window.location.hostname}/ws`);
  websocket.onopen = onOpen;
  // websocket.onclose = onClose;
  websocket.onmessage = onMessage;
}

function onOpen(event) {
  console.log("Connected to WebSocket server");
  isConnected = true;
}

function onClose(event) {
  console.log("Disconnected from WebSocket server");
  isConnected = false;
  setTimeout(initWebSocket, 5000); // Try to reconnect every 5 seconds
}

function onMessage(event) {
  try {
    const message = JSON.parse(event.data);
    obsDistContainer.innerText = message.distance;
  } catch (e) {
    console.error("Error parsing message:", e);
  }
}

function setMotorSpeed(leftMotors, rightMotors) {
  if (!leftMotors) leftMotors = 0;
  if (!rightMotors) rightMotors = 0;

  lmsContainer.innerText = Math.abs(leftMotors);
  rmsContainer.innerText = Math.abs(rightMotors);
}

function setDirection(angle) {
  let direction = "Standby";
  if (!angle) angle = 0;
  if (angle > 0 && angle <= 15) direction = "Kanan";
  else if (angle >= 15 && angle < 75) direction = "Maju Kanan";
  else if (angle >= 75 && angle <= 105) direction = "Maju";
  else if (angle > 105 && angle < 165) direction = "Maju Kiri";
  else if (angle > 165 && angle < 195) direction = "Kiri";
  else if (angle > 195 && angle < 255) direction = "Mundur Kiri";
  else if (angle >= 255 && angle <= 285) direction = "Mundur";
  else if (angle > 285 && angle < 360) direction = "Mundur Kanan";
  else if (angle === 0 || angle === 360) direction = "Standby";
  dirContainer.innerText = direction;
}
