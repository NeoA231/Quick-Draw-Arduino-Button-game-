int start = 10;
bool startState = HIGH;

int count1 = 13;
int count2 = 12;
int count3 = 11;

int p1LED = 2;
int p1Btn = 7;
bool p1BtnState = HIGH;

int p2LED = 3;
int p2Btn = 6;
bool p2BtnState = HIGH;

// Game states for non-blocking state machine
enum GameState {
  STATE_IDLE,
  STATE_STARTUP,
  STATE_COUNTDOWN,
  STATE_DRAW,
  STATE_WIN
};

GameState gameState = STATE_IDLE;
unsigned long stateStartTime = 0;
int winner = 0; // 1 for Player 1, 2 for Player 2

void setup() {

  // Buttons idle HIGH because of INPUT_PULLUP
  pinMode(start, INPUT_PULLUP);

  pinMode(count1, OUTPUT);
  pinMode(count2, OUTPUT);
  pinMode(count3, OUTPUT);

  pinMode(p1LED, OUTPUT);
  pinMode(p2LED, OUTPUT);

  pinMode(p1Btn, INPUT_PULLUP);
  pinMode(p2Btn, INPUT_PULLUP);

  resetToIdle();
}

void loop() {

  switch (gameState) {

    case STATE_IDLE: {
      bool currentStartState = digitalRead(start);

      // detect button press (edge detection)
      if (startState == HIGH && currentStartState == LOW) {
        startFunction();
      }

      startState = currentStartState;
      break;
    }

    case STATE_STARTUP:
      checkFalseStart();
      if (gameState == STATE_STARTUP) {
        updateStartupAnimation();
      }
      break;

    case STATE_COUNTDOWN:
      checkFalseStart();
      if (gameState == STATE_COUNTDOWN) {
        updateCountdown();
      }
      break;

    case STATE_DRAW:
      playerPress();
      break;

    case STATE_WIN:
      updateWinDisplay();
      break;
  }
}

// Resets all LEDs, updates button states, and returns the game to idle
void resetToIdle() {

  // Turn off all LEDs
  digitalWrite(count1, LOW);
  digitalWrite(count2, LOW);
  digitalWrite(count3, LOW);
  digitalWrite(p1LED, LOW);
  digitalWrite(p2LED, LOW);

  // Sync stored button states to prevent false triggers
  startState = digitalRead(start);
  p1BtnState = digitalRead(p1Btn);
  p2BtnState = digitalRead(p2Btn);

  winner = 0;
  gameState = STATE_IDLE;
}

// Starts the round sequence
void startFunction() {

  gameState = STATE_STARTUP;
  stateStartTime = millis();

  // Ensure all LEDs start off
  digitalWrite(count1, LOW);
  digitalWrite(count2, LOW);
  digitalWrite(count3, LOW);
  digitalWrite(p1LED, LOW);
  digitalWrite(p2LED, LOW);

  p1BtnState = digitalRead(p1Btn);
  p2BtnState = digitalRead(p2Btn);
}

// Detects false starts before the draw signal and penalizes early pressers
void checkFalseStart() {

  bool currentP1BtnState = digitalRead(p1Btn);
  bool currentP2BtnState = digitalRead(p2Btn);

  bool p1Early = (currentP1BtnState == LOW);
  bool p2Early = (currentP2BtnState == LOW);

  if (p1Early && !p2Early) {
    // Player 1 false start -> Player 2 wins
    player2Win();
  } else if (p2Early && !p1Early) {
    // Player 2 false start -> Player 1 wins
    player1Win();
  } else if (p1Early && p2Early) {
    // Both pressed before draw signal -> reset to idle
    resetToIdle();
  }
}

// Non-blocking startup blink animation using millis()
void updateStartupAnimation() {

  unsigned long elapsed = millis() - stateStartTime;

  // 9 steps total: 3 cycles of count1, count2, count3 (333ms each = 2997ms)
  if (elapsed >= 9 * 333) {
    digitalWrite(count1, LOW);
    digitalWrite(count2, LOW);
    digitalWrite(count3, LOW);

    gameState = STATE_COUNTDOWN;
    stateStartTime = millis();
    return;
  }

  int step = elapsed / 333; // 0 to 8
  int ledIndex = step % 3;  // 0 -> count1, 1 -> count2, 2 -> count3

  digitalWrite(count1, (ledIndex == 0) ? HIGH : LOW);
  digitalWrite(count2, (ledIndex == 1) ? HIGH : LOW);
  digitalWrite(count3, (ledIndex == 2) ? HIGH : LOW);
}

// Non-blocking countdown using millis()
void updateCountdown() {

  unsigned long elapsed = millis() - stateStartTime;

  // 3 steps: 1000ms each (3000ms total)
  if (elapsed >= 3000) {
    digitalWrite(count1, LOW);
    digitalWrite(count2, LOW);
    digitalWrite(count3, LOW);

    // Draw signal: both player LEDs turn ON
    digitalWrite(p1LED, HIGH);
    digitalWrite(p2LED, HIGH);

    gameState = STATE_DRAW;
    p1BtnState = digitalRead(p1Btn);
    p2BtnState = digitalRead(p2Btn);
    return;
  }

  int step = elapsed / 1000; // 0, 1, or 2
  digitalWrite(count1, (step == 0) ? HIGH : LOW);
  digitalWrite(count2, (step == 1) ? HIGH : LOW);
  digitalWrite(count3, (step == 2) ? HIGH : LOW);
}

// Checks player reaction presses during the draw state
void playerPress() {

  bool currentP1BtnState = digitalRead(p1Btn);
  bool currentP2BtnState = digitalRead(p2Btn);

  if (p1BtnState == HIGH && currentP1BtnState == LOW && currentP2BtnState == HIGH) {
    player1Win();
  } else if (p2BtnState == HIGH && currentP2BtnState == LOW && currentP1BtnState == HIGH) {
    player2Win();
  }

  p1BtnState = currentP1BtnState;
  p2BtnState = currentP2BtnState;
}

// Sets Player 1 as winner and starts win sequence
void player1Win() {

  winner = 1;
  gameState = STATE_WIN;
  stateStartTime = millis();

  // Loser LED and countdown LEDs off
  digitalWrite(count1, LOW);
  digitalWrite(count2, LOW);
  digitalWrite(count3, LOW);
  digitalWrite(p2LED, LOW);
}

// Sets Player 2 as winner and starts win sequence
void player2Win() {

  winner = 2;
  gameState = STATE_WIN;
  stateStartTime = millis();

  // Loser LED and countdown LEDs off
  digitalWrite(count1, LOW);
  digitalWrite(count2, LOW);
  digitalWrite(count3, LOW);
  digitalWrite(p1LED, LOW);
}

// Non-blocking win animation display using millis()
void updateWinDisplay() {

  unsigned long elapsed = millis() - stateStartTime;
  int winLED = (winner == 1) ? p1LED : p2LED;
  int loseLED = (winner == 1) ? p2LED : p1LED;

  digitalWrite(loseLED, LOW);

  // Initial solid ON (1010ms)
  if (elapsed < 1010) {
    digitalWrite(winLED, HIGH);
  }
  // Pause OFF (500ms, from 1010ms to 1510ms)
  else if (elapsed < 1510) {
    digitalWrite(winLED, LOW);
  }
  // 7 flashes: 167ms ON / 167ms OFF (cycle = 334ms, total = 2338ms)
  else if (elapsed < 1510 + (7 * 334)) {
    unsigned long blinkElapsed = elapsed - 1510;
    if ((blinkElapsed % 334) < 167) {
      digitalWrite(winLED, HIGH);
    } else {
      digitalWrite(winLED, LOW);
    }
  }
  // Automatically return to idle state after displaying the winner
  else {
    resetToIdle();
  }
}
