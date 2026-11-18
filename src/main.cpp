#include "main.h"

namespace {
constexpr int32_t liftPositionStep        = 100;
bool              liftPositionInitialized = false;
} // namespace


/**
 * Sets up all GPIO, and anything connected to physical pins
 */
void initGPIO() {
  robot.servo.wrist.attach(gpio.out1);

  robot.servo.claw.attach(gpio.out2);

  pinMode(gpio.out3, INPUT);
  pinMode(gpio.out4, INPUT);
  pinMode(gpio.out5, INPUT);
  pinMode(gpio.out6, INPUT);
  pinMode(gpio.out7, INPUT);
  pinMode(gpio.out8, INPUT);
  pinMode(gpio.out9, INPUT);
  pinMode(gpio.out10, INPUT);
  pinMode(gpio.out11, INPUT);
  pinMode(gpio.out12, INPUT);
}


/***/
void setColorBar(int R, int G, int B) {
  gamepad.colorBar[0] = R;
  gamepad.colorBar[1] = G;
  gamepad.colorBar[2] = B;
}


/**
 *  Adjust drive speed based on controller shoulder buttons
 */
void adjustDriveSpeed() {
  if (gamepad.L1 || gamepad.R1) {
    // Left trigger - Fast speed
    if (gamepad.L1 && !robot.slowSpeed) {
      robot.fastSpeed = true;
      setColorBar(255, 0, 0);

      // Right Trigger - Slow speed
    } else if (gamepad.R1 && !robot.fastSpeed) {
      robot.slowSpeed = true;
      setColorBar(0, 0, 255);
    }

    // No trigger pressed - Medium speed
  } else {
    robot.slowSpeed = false;
    robot.fastSpeed = false;
    setColorBar(0, 255, 0);
  }
}


/**
 * Calculate the power for mechanum drive systems
 */
void calcMech(int y, int x, int rot) {
  // Applies deadzones to inputs
  if (y > -gamepad.deadzone && y < gamepad.deadzone)
    y = 0;
  if (x > -gamepad.deadzone && x < gamepad.deadzone)
    x = 0;
  if (rot > -gamepad.deadzone && rot < gamepad.deadzone)
    rot = 0;

  float speedMulti = 1;
  if (robot.slowSpeed) {
    speedMulti = 0.3;
  } else if (robot.fastSpeed) {
    speedMulti = 2;
  }

  // Mechanum calculation matrix
  robot.motor.FL_Motor = (int)((y + x + rot) * speedMulti);
  robot.motor.FR_Motor = (int)((y - x - rot) * speedMulti);
  robot.motor.BL_Motor = (int)((y - x + rot) * speedMulti);
  robot.motor.BR_Motor = (int)((y + x - rot) * speedMulti);
}


/**
 * Send calculated wheel powers to Roboclaws
 */
void driveMotors(int powFL, int powFR, int powBL, int powBR) {
  /*
    // Working on this
    // Trying to make it so that if any of the wheels are commanded to go more
    than max speed, it scales back the other ones to the appropriate speed int
    powMax = max(max(powFL, powFR), max(powBL, powBR));

    if (powFL > maxSpeed || powFR > maxSpeed || powBL > maxSpeed || powBR >
    maxSpeed) {

    }
    */


  motorDriver.SpeedM1(driver1Addr, powFL);

  // FRMotor
  motorDriver.SpeedM2(driver1Addr, powFR);

  // BLMotor
  motorDriver.SpeedM1(driver2Addr, powBL);

  // BRMotor
  motorDriver.SpeedM2(driver2Addr, powBR);
}


/***/
void liftCtl(int pow) {
  motorDriver.SpeedM1(driver3Addr, pow);
}


void autoMode() {
  while (!gamepad.cross && !gamepad.circle && !gamepad.square &&
         !gamepad.triangle) {
    // driveMotors();
  }

  return;
}


void setServos() {
  robot.servo.wrist.write(robot.servo.wristPos);
  robot.servo.claw.write(robot.servo.clawPos);
}


/**
 * Runs once when the controller is booted
 */
void setup() {
  Serial.begin(115200);
  motorDriver.begin(38400); // Initialize the motor drivers
  Wire.begin(gpio.out11, gpio.out12);

  Serial.printf("Firmware: %s\n", BP32.firmwareVersion());
  const uint8_t *addr = BP32.localBdAddress();
  Serial.printf("BD Addr: %2X:%2X:%2X:%2X:%2X:%2X\n",
                addr[0],
                addr[1],
                addr[2],
                addr[3],
                addr[4],
                addr[5]);

  // Setup the Bluepad32 callbacks
  BP32.setup(&onConnectedController, &onDisconnectedController);

  BP32.enableVirtualDevice(true);
  // BP32.forgetBluetoothKeys(); // Call this if you want to clear paired
  // gamepads.

  initColor();
  initGPIO();
}


/**
 * Runs continuously
 */
void loop() {
  // Updates the controller data
  if (BP32.update()) {
    processControllers();
  }


  // Move the lift up and down based on the dpad
  liftCtl(gamepad.dpad_up ? 511 : (gamepad.dpad_down ? -512 : 0));


  // Control for the servos on the wrist and claw
  robot.servo.wristPos = map(gamepad.L2, 0, 1023, 100, 45);
  robot.servo.clawPos  = map(gamepad.R2, 0, 1023, 135, 0);

  // runColor();

  /*
    if (gamepad.share && gamepad.options) {
      autoMode();
    }
  */


  adjustDriveSpeed();
  calcMech(gamepad.LY, gamepad.LX, gamepad.RX);
  driveMotors(robot.motor.FL_Motor,
              robot.motor.FR_Motor,
              robot.motor.BL_Motor,
              robot.motor.BR_Motor);


  // Hold the robot in starting position until Circle is pressed
  if (gamepad.square) {
    robot.goodToGo = true;
  } else if (gamepad.triangle) {
    robot.goodToGo = false;
  }
  if (!robot.goodToGo) {
    robot.servo.wristPos = 150;
    robot.servo.clawPos  = 75;
  }


  if (gamepad.circle) {
    robot.servo.wristPos = 80;
  }


  // Write current servo positions
  setServos();

  Serial.print(gamepad.L2);
  Serial.print(", ");
  Serial.print(robot.servo.wristPos);
  Serial.print(", ");
  Serial.print(gamepad.R2);
  Serial.print(", ");
  Serial.println(robot.servo.clawPos);


  // printDebugController();
  // getCycleTime();
}


/**
 * To debug timing, prints out the cycle time between calls
 */
void getCycleTime() {
  static unsigned long lastRun = 0;
  const unsigned long  now     = millis();
  Serial.print("Time per cycle: ");
  Serial.println(now - lastRun);
  lastRun = now;
}


/**
 * Prints out a graphic of the controller for debugging
 */
void printDebugController() {
  // Only print every 100 ms
  static unsigned long lastRun    = 0;
  const unsigned long  intervalMs = 400;
  const unsigned long  now        = millis();
  if (now - lastRun < intervalMs) {
    return;
  }
  lastRun = now;

  static bool firstFrame = true;
  if (firstFrame) {
    Serial.write(0x1B);
    Serial.print("[2J");
    firstFrame = false;
  }
  Serial.write(0x1B);
  Serial.print("[H");
  Serial.printf("                      GAMEPAD: %s\n",
                gamepad.connected ? "CONNECTED" : "DISCONNECTED");
  Serial.printf("          __________/                        \\__________\n");
  Serial.printf("         /                                              \\\n");
  Serial.printf(
      "        /                                                \\\n");
  Serial.printf("       |  L1 [%s]  L2: %4d             R2: %4d  R1 [%s]  |\n",
                gamepad.connected && gamepad.L1 ? "ON " : "   ",
                gamepad.connected ? gamepad.L2 : 0,
                gamepad.connected ? gamepad.R2 : 0,
                gamepad.connected && gamepad.R1 ? "ON " : "   ");
  Serial.printf("       |                                                |\n");
  Serial.printf(
      "       |      %c                                    [%c]   |\n",
      gamepad.connected && gamepad.dpad_up ? '^' : ' ',
      gamepad.connected && gamepad.triangle ? 'T' : ' ');
  Serial.printf(
      "       |   %c  +  %c                               [%c] [%c]  |\n",
      gamepad.connected && gamepad.dpad_left ? '<' : ' ',
      gamepad.connected && gamepad.dpad_right ? '>' : ' ',
      gamepad.connected && gamepad.square ? 'S' : ' ',
      gamepad.connected && gamepad.circle ? 'O' : ' ');
  Serial.printf(
      "       |      %c                                    [%c]   |\n",
      gamepad.connected && gamepad.dpad_down ? 'v' : ' ',
      gamepad.connected && gamepad.cross ? 'X' : ' ');
  Serial.printf("       |                                                |\n");
  Serial.printf("       |   Left stick: X %4d Y %4d   Right: X %4d Y %4d |\n",
                gamepad.connected ? gamepad.LX : 0,
                gamepad.connected ? gamepad.LY : 0,
                gamepad.connected ? gamepad.RX : 0,
                gamepad.connected ? gamepad.RY : 0);
  Serial.printf("        \\                                              /\n");
  Serial.printf("         \\__________                        __________/\n");
  Serial.printf("                    \\______________________/\n");
}
