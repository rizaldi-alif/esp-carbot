// const leftJoystick = nipplejs.create({
//   zone: leftJoystickContainer,
//   mode: "static",
//   position: { left: "26%", top: "50%" },
//   color: "oklch(62.3% 0.214 259.815)",
//   size: 150,
//   restOpacity: 0.5,
//   lockY: true,
// });

// const rightJoystick = nipplejs.create({
//   zone: rightJoystickContainer,
//   mode: "static",
//   position: { right: "26%", top: "50%" },
//   color: "oklch(62.3% 0.214 259.815)",
//   size: 150,
//   restOpacity: 0.5,
//   lockX: true,
// });

// const maxDistance = 150 / 2;
// // ==================== CONFIGURASI ====================
// const CONFIG = {
//   deadzone: 12, // % area mati biar joystick nggak gerak sendiri
//   throttleScale: 1.0, // skala kecepatan (0.0 - 1.5, tergantung motor & supply)
//   steeringScale: 1.0, // skala belok (0.0 - 1.5)
//   maxOutput: 100, // batas maksimum output motor
// };
// // =====================================================

// // Helper untuk normalisasi input joystick (0 - 100)
// function normalizeInput(value, maxDistance, deadzone) {
//   if (value < deadzone) return 0;
//   const range = maxDistance - deadzone;
//   return Math.min(1, (value - deadzone) / range) * 100;
// }

// // Variabel motor terakhir (biar bisa dipakai untuk optimisasi)
// let lastLeftMotor = 0;
// let lastRightMotor = 0;

// // Fungsi hitung kecepatan motor
// function calculateMotorSpeeds(throttle, steering) {
//   throttle = throttle * CONFIG.throttleScale;
//   steering = steering * CONFIG.steeringScale;

//   let leftMotor = throttle - steering;
//   let rightMotor = throttle + steering;

//   if (throttle < 0) {
//     leftMotor = throttle + steering;
//     rightMotor = throttle - steering;
//   }

//   // Batasi output agar tidak lebih dari maxOutput
//   leftMotor = Math.max(
//     -CONFIG.maxOutput,
//     Math.min(CONFIG.maxOutput, Math.round(leftMotor))
//   );
//   rightMotor = Math.max(
//     -CONFIG.maxOutput,
//     Math.min(CONFIG.maxOutput, Math.round(rightMotor))
//   );

//   return { leftMotor, rightMotor };
// }

// // Event Joystick Kiri (Throttle)
// leftJoystick.on("move", (evt, data) => {
//   if (data && data.distance !== undefined) {
//     const throttle = normalizeInput(
//       data.distance,
//       maxDistance,
//       (CONFIG.deadzone / 100) * maxDistance
//     );

//     lastThrottle = data.angle.degree < 180 ? throttle : -throttle; // atas = maju, bawah = mundur
//   }
// });

// leftJoystick.on("end", () => {
//   lastThrottle = 0;
//   sendMotorCommand(0, 0);
// });

// // Event Joystick Kanan (Steering)
// rightJoystick.on("move", (evt, data) => {
//   if (data && data.distance !== undefined) {
//     const steering = normalizeInput(
//       data.distance,
//       maxDistance,
//       (CONFIG.deadzone / 100) * maxDistance
//     );

//     lastSteering =
//       data.angle.degree > 90 && data.angle.degree < 270 ? steering : -steering; // kiri = -, kanan = +
//   }
// });

// rightJoystick.on("end", () => {
//   lastSteering = 0;
//   sendMotorCommand(0, 0);
// });

// // Variabel global throttle & steering
// let lastThrottle = 0;
// let lastSteering = 0;

// // Kirim perintah motor
// function sendMotorCommand(throttle, steering) {
//   const { leftMotor, rightMotor } = calculateMotorSpeeds(throttle, steering);

//   if (leftMotor !== lastLeftMotor || rightMotor !== lastRightMotor) {
//     const message = `S${rightMotor},${leftMotor}`;
//     sendWebSocketMessage(message);
//     console.log(message);

//     lastLeftMotor = leftMotor;
//     lastRightMotor = rightMotor;
//   }
// }

// // Loop update motor (sinkron throttle & steering)
// setInterval(() => {
//   sendMotorCommand(lastThrottle, lastSteering);
// }, 50); // update tiap 100ms

// // Update CONFIG ketika slider digeser
// deadzoneSlider.addEventListener("input", () => {
//   CONFIG.deadzone = parseInt(deadzoneSlider.value);
//   deadzoneValue.textContent = CONFIG.deadzone;
// });

// throttleSlider.addEventListener("input", () => {
//   CONFIG.throttleScale = parseFloat(throttleSlider.value);
//   throttleValue.textContent = CONFIG.throttleScale.toFixed(1);
// });

// steeringSlider.addEventListener("input", () => {
//   CONFIG.steeringScale = parseFloat(steeringSlider.value);
//   steeringValue.textContent = CONFIG.steeringScale.toFixed(1);
// });

function sendWebSocketMessage(message) {
  if (websocket && websocket.readyState === WebSocket.OPEN) {
    websocket.send(message);
  }
}

const manager = nipplejs.create({
  zone: joystickContainer,
  mode: "static",
  position: { left: "50%", top: "75%" },
  color: "oklch(62.3% 0.214 259.815)",
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
  }
});

manager.on("end", () => {
  const message = `S0,0`;
  sendWebSocketMessage(message);
});
