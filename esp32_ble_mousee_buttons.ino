#include <BleMouse.h>

// =====================================================
// PIN DEFINITIONS - CLICK BUTTONS
// =====================================================

#define LEFT_BTN_PIN     27   // LEFT CLICK button
#define RIGHT_BTN_PIN    26   // RIGHT CLICK / MENU button


// =====================================================
// PIN DEFINITIONS - DIRECTION BUTTONS
// =====================================================

#define UP_BTN_PIN       33
#define DOWN_BTN_PIN     32
#define LEFT_MOVE_PIN    25
#define RIGHT_MOVE_PIN    4


// =====================================================
// SETTINGS
// =====================================================

#define DEBOUNCE_MS        20

// Movement repeat interval (ms) - how often a movement step is sent
#define MOVE_REPEAT_MS      15

// Starting speed (pixels/step)
#define MOVE_SPEED_MIN        2

// Maximum speed (pixels/step)
#define MOVE_SPEED_MAX       14

// While a direction button is held, speed ramps up linearly from
// MOVE_SPEED_MIN to MOVE_SPEED_MAX over this duration (ms),
// giving an acceleration feel
#define ACCEL_RAMP_MS      800


BleMouse bleMouse("ESP32 Mouse", "DIY", 100);

// Click button states
bool lastLeftState   = false;
bool lastRightState  = false;
unsigned long lastLeftChange  = 0;
unsigned long lastRightChange = 0;

// Direction buttons - press start timestamps (0 = not pressed)
unsigned long upPressStart    = 0;
unsigned long downPressStart  = 0;
unsigned long leftPressStart  = 0;
unsigned long rightPressStart = 0;

unsigned long lastMoveTime = 0;


// =====================================================
// SETUP
// =====================================================

void setup() {

  Serial.begin(115200);

  pinMode(LEFT_BTN_PIN, INPUT_PULLUP);
  pinMode(RIGHT_BTN_PIN, INPUT_PULLUP);

  pinMode(UP_BTN_PIN, INPUT_PULLUP);
  pinMode(DOWN_BTN_PIN, INPUT_PULLUP);
  pinMode(LEFT_MOVE_PIN, INPUT_PULLUP);
  pinMode(RIGHT_MOVE_PIN, INPUT_PULLUP);

  Serial.println("Starting ESP32 BLE Mouse...");
  bleMouse.begin();
}


// =====================================================
// LOOP
// =====================================================

void loop() {

  if (!bleMouse.isConnected()) {

    lastLeftState  = false;
    lastRightState = false;

    upPressStart    = 0;
    downPressStart  = 0;
    leftPressStart  = 0;
    rightPressStart = 0;

    delay(200);
    return;
  }

  handleLeftClick();
  handleRightClick();
  handleDirectionMovement();

  delay(5);
}


// =====================================================
// LEFT CLICK BUTTON (debounced)
// =====================================================

void handleLeftClick() {

  bool raw = (digitalRead(LEFT_BTN_PIN) == LOW);
  unsigned long now = millis();

  if (raw != lastLeftState) {
    lastLeftChange = now;
  }

  if (now - lastLeftChange > DEBOUNCE_MS) {

    static bool stable = false;

    if (raw != stable) {

      stable = raw;

      if (stable) {
        bleMouse.press(MOUSE_LEFT);
      } else {
        bleMouse.release(MOUSE_LEFT);
      }
    }
  }

  lastLeftState = raw;
}


// =====================================================
// RIGHT CLICK / MENU BUTTON (debounced)
// =====================================================

void handleRightClick() {

  bool raw = (digitalRead(RIGHT_BTN_PIN) == LOW);
  unsigned long now = millis();

  if (raw != lastRightState) {
    lastRightChange = now;
  }

  if (now - lastRightChange > DEBOUNCE_MS) {

    static bool stable = false;

    if (raw != stable) {

      stable = raw;

      if (stable) {
        bleMouse.press(MOUSE_RIGHT);
      } else {
        bleMouse.release(MOUSE_RIGHT);
      }
    }
  }

  lastRightState = raw;
}


// =====================================================
// COMPUTES THE CURRENT SPEED FOR A DIRECTION BUTTON
// BASED ON HOW LONG IT HAS BEEN HELD (0 = not pressed)
// =====================================================

int speedFor(unsigned long pressStart, unsigned long now) {

  if (pressStart == 0) {
    return 0;
  }

  unsigned long held = now - pressStart;

  if (held >= ACCEL_RAMP_MS) {
    return MOVE_SPEED_MAX;
  }

  // Linear ramp: MOVE_SPEED_MIN -> MOVE_SPEED_MAX
  long speed = MOVE_SPEED_MIN +
    (long)(MOVE_SPEED_MAX - MOVE_SPEED_MIN) * held / ACCEL_RAMP_MS;

  return (int)speed;
}


// =====================================================
// MOVEMENT VIA 4 DIRECTION BUTTONS
// =====================================================

void handleDirectionMovement() {

  unsigned long now = millis();

  bool upPressed    = (digitalRead(UP_BTN_PIN)    == LOW);
  bool downPressed   = (digitalRead(DOWN_BTN_PIN)   == LOW);
  bool leftPressed   = (digitalRead(LEFT_MOVE_PIN)  == LOW);
  bool rightPressed  = (digitalRead(RIGHT_MOVE_PIN) == LOW);

  // Update press-start timestamps
  if (upPressed    && upPressStart    == 0) upPressStart    = now;
  if (downPressed  && downPressStart  == 0) downPressStart  = now;
  if (leftPressed  && leftPressStart  == 0) leftPressStart  = now;
  if (rightPressed && rightPressStart == 0) rightPressStart = now;

  if (!upPressed)    upPressStart    = 0;
  if (!downPressed)  downPressStart  = 0;
  if (!leftPressed)  leftPressStart  = 0;
  if (!rightPressed) rightPressStart = 0;


  if (now - lastMoveTime < MOVE_REPEAT_MS) {
    return;
  }


  int dx = 0;
  int dy = 0;

  if (upPressed)    dy -= speedFor(upPressStart, now);
  if (downPressed)  dy += speedFor(downPressStart, now);
  if (leftPressed)  dx -= speedFor(leftPressStart, now);
  if (rightPressed) dx += speedFor(rightPressStart, now);


  if (dx != 0 || dy != 0) {

    bleMouse.move(dx, dy, 0);

    lastMoveTime = now;
  }
}
