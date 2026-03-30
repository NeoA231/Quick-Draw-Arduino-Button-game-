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
}

void loop() {

  bool currentStartState = digitalRead(start);

  // detect button press (edge detection)
  if (startState == HIGH && currentStartState == LOW) {
    startFunction();
  }

  startState = currentStartState;
}

void startFunction() {

  // startup blink animation
  for (int x = 3; x > 0; x--) {

    digitalWrite(count1, HIGH);
    delay(333);
    digitalWrite(count1, LOW);

    digitalWrite(count2, HIGH);
    delay(333);
    digitalWrite(count2, LOW);

    digitalWrite(count3, HIGH);
    delay(333);
    digitalWrite(count3, LOW);
  }

  // countdown
  digitalWrite(count1, HIGH);
  delay(1000);
  digitalWrite(count1, LOW);

  digitalWrite(count2, HIGH);
  delay(1000);
  digitalWrite(count2, LOW);

  digitalWrite(count3, HIGH);
  delay(1000);
  digitalWrite(count3, LOW);

  // draw signal
  digitalWrite(p1LED, HIGH);
  digitalWrite(p2LED, HIGH);

  playerPress();
}

void playerPress() {

  // continuously check until someone presses
  while (true) {

    bool currentP1BtnState = digitalRead(p1Btn);
    bool currentP2BtnState = digitalRead(p2Btn);

    if (p1BtnState == HIGH && currentP1BtnState == LOW && currentP2BtnState == HIGH) {
      player1Win();
      break;
    }

    else if (p2BtnState == HIGH && currentP2BtnState == LOW && currentP1BtnState == HIGH) {
      player2Win();
      break;
    }

    // update stored states
    p1BtnState = currentP1BtnState;
    p2BtnState = currentP2BtnState;
  }
}

void player1Win() {

  // Player 1 wins
  digitalWrite(p2LED, LOW);

  digitalWrite(p1LED, HIGH);
  delay(1010);

  digitalWrite(p1LED, LOW);
  delay(500);

  for (int i = 0; i < 7; i++) {
    digitalWrite(p1LED, HIGH);
    delay(167);
    digitalWrite(p1LED, LOW);
    delay(167);
  }
}

void player2Win() {

  // Player 2 wins
  digitalWrite(p1LED, LOW);

  digitalWrite(p2LED, HIGH);
  delay(1010);

  digitalWrite(p2LED, LOW);
  delay(500);

  for (int i = 0; i < 7; i++) {
    digitalWrite(p2LED, HIGH);
    delay(167);
    digitalWrite(p2LED, LOW);
    delay(167);
  }
}
