const joystickContainer = document.getElementById("joystick-container");

function sendWebSocketMessage(message) {
  if (websocket && websocket.readyState === WebSocket.OPEN) {
    websocket.send(message);
  }
}

const manager = nipplejs.create({
  zone: joystickContainer,
  mode: "static",
  position: { left: "50%", top: "70%" },
  color: "blue",
  size: 180,
  restOpacity: 0.5,
});

const maxDistance = 90;
const centerDeadzone = 25;
const lateralDeadzone = 10;
const minDistance = (centerDeadzone / 100) * maxDistance;
const range = maxDistance - minDistance;

let lastLeftMotor = 0;
let lastRightMotor = 0;

function calculateMotorSpeeds(distance, angle) {
  angle = (angle + 360) % 360;

  if (distance < (centerDeadzone / 100) * maxDistance) {
    return { rightMotor: 0, leftMotor: 0 };
  }

  // const speed = (Math.min(distance, maxDistance) / maxDistance) * 100;
  const speed =
    Math.min(Math.max((distance - minDistance) / range, 0), 1) * 100;
  let rightMotor = 0;
  let leftMotor = 0;

  const edgeDeadzone = 5;
  const horizontalDeadzone = 10;

  if (angle >= 90 - horizontalDeadzone && angle <= 90 + horizontalDeadzone) {
    leftMotor = speed;
    rightMotor = speed;
  } else if (
    angle >= 270 - horizontalDeadzone &&
    angle <= 270 + horizontalDeadzone
  ) {
    leftMotor = -speed;
    rightMotor = -speed;
  } else if (angle >= edgeDeadzone && angle <= 180 - edgeDeadzone) {
    const turnFactor = Math.abs((angle - 90) / 90);
    if (angle <= 90) {
      rightMotor = speed * (1 - turnFactor);
      leftMotor = speed;
    } else {
      leftMotor = speed * (1 - turnFactor);
      rightMotor = speed;
    }
  } else if (angle >= 180 + edgeDeadzone && angle <= 360 - edgeDeadzone) {
    const turnFactor = Math.abs((angle - 270) / 90);
    if (angle <= 270) {
      leftMotor = -speed * (1 - turnFactor);
      rightMotor = -speed;
    } else {
      leftMotor = -speed;
      rightMotor = -speed * (1 - turnFactor);
    }
  }

  rightMotor = Math.max(-100, Math.min(100, Math.round(rightMotor)));
  leftMotor = Math.max(-100, Math.min(100, Math.round(leftMotor)));

  return { rightMotor, leftMotor };
}

manager.on("move", (evt, data) => {
  if (
    data &&
    data.angle &&
    data.angle.degree !== undefined &&
    data.distance !== undefined
  ) {
    const distance = data.distance;
    const angle = data.angle.degree;

    const { rightMotor, leftMotor } = calculateMotorSpeeds(distance, angle);

    const message = `S${rightMotor},${leftMotor}`;

    sendWebSocketMessage(message);
    setMotorSpeed(leftMotor, rightMotor);
    setDirection(angle);
  }
});

manager.on("end", () => {
  const message = `S0,0`;
  sendWebSocketMessage(message);
  console.log("end");
});
