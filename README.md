# ⚡ Quick Draw Button Game

An Arduino-based two-player reaction game inspired by classic quick-draw duels.

Two players wait through a countdown sequence. Once the draw signal is given, both players race to press their button. The first valid button press wins the round.

This project demonstrates fundamental Arduino concepts including digital inputs and outputs, timing, button state detection, and basic game logic.

---

## 🎮 Project Overview

**Quick Draw** is a simple reaction-based game built using an Arduino, push buttons, LEDs, and a breadboard.

The game works as follows:

1. The game waits for the start button to be pressed.
2. A startup animation and countdown sequence begins.
3. Once the countdown finishes, both player LEDs turn on.
4. Players must press their buttons as quickly as possible.
5. The first valid button press wins.
6. The winning player's LED flashes while the losing player's LED turns off.
7. The game automatically resets to the idle state, ready for another round.

Pressing a button before the draw signal triggers a false-start penalty, immediately awarding the round victory to the other player.

---

## 🎯 Game Objective

The objective is simple:

> **Be the first player to press your button after the draw signal.**

### Players

* **Player 1:** Button on pin 7
* **Player 2:** Button on pin 6

### Start

* The game is started using the button connected to pin 10.

---

## 🧰 Hardware Components

| Component      |    Quantity |
| -------------- | ----------: |
| Arduino board  |           1 |
| Countdown LEDs |           3 |
| Player LEDs    |           2 |
| Push buttons   |           3 |
| LED resistors  |           5 |
| Jumper wires   | As required |
| Breadboard     |           1 |

---

## 🔌 Pin Configuration

| Component       | Arduino Pin |
| --------------- | ----------: |
| Start Button    |        `10` |
| Countdown LED 1 |        `13` |
| Countdown LED 2 |        `12` |
| Countdown LED 3 |        `11` |
| Player 1 LED    |         `2` |
| Player 2 LED    |         `3` |
| Player 1 Button |         `7` |
| Player 2 Button |         `6` |

### Button Configuration

All buttons use Arduino's internal pull-up resistors:

```cpp
pinMode(buttonPin, INPUT_PULLUP);
```

This means:

| Button State | Arduino Reads |
| ------------ | ------------- |
| Not pressed  | `HIGH`        |
| Pressed      | `LOW`         |

---

## 🔌 Circuit Wiring Instructions

<details>
<summary><strong>Click to expand / collapse wiring instructions</strong></summary>

<br>

Build the circuit using an Arduino board, a breadboard, five LEDs, five current-limiting resistors, and three push buttons.

### 1. Connect the Countdown LEDs

Each countdown LED requires a current-limiting resistor, typically between 220 Ω and 330 Ω.

| Component | Arduino pin |
|---|---:|
| Countdown LED 1 | 13 |
| Countdown LED 2 | 12 |
| Countdown LED 3 | 11 |

For each LED:

1. Connect the assigned Arduino output pin to a resistor.
2. Connect the other end of the resistor to the LED's anode (longer leg).
3. Connect the LED's cathode (shorter leg) to GND.

### 2. Connect the Player LEDs

| Component | Arduino pin |
|---|---:|
| Player 1 LED | 2 |
| Player 2 LED | 3 |

Wire each player LED in the same way as the countdown LEDs, using its own current-limiting resistor.

### 3. Connect the Push Buttons

The game uses the Arduino's internal pull-up resistors through `INPUT_PULLUP`.

| Button | Arduino pin | Ground connection |
|---|---:|---|
| Start button | 10 | GND |
| Player 1 button | 7 | GND |
| Player 2 button | 6 | GND |

For each button, connect one terminal to the assigned Arduino pin and the other terminal to GND.

No external pull-up resistors are required.

When a button is released, the Arduino reads `HIGH`. When pressed, the Arduino reads `LOW`.

**Important:** If using four-legged tactile push buttons, place each button across the breadboard's centre gap and use terminals on opposite sides of the switch.

### 4. Connect Ground

Connect Arduino GND to the breadboard's ground rail. Connect the cathode of every LED and one terminal of every push button to this common ground.

### 5. Check the Circuit

Before powering the circuit, verify the following:

- All five LEDs have their own current-limiting resistors.
- Each LED is connected to the correct Arduino pin.
- Each button connects its assigned input pin to GND when pressed.
- The breadboard ground rail is connected to Arduino GND.
- No Arduino output pin is connected directly to GND or 5 V.

Once the wiring is complete, upload the Quick Draw program and press the start button to begin a round.

</details>

---

## 🕹️ How the Game Works

### 1. Idle State

When the Arduino starts or finishes a round, the game remains in an idle state waiting for the start button.

The program uses **edge detection** to detect a change in the button state (`HIGH` to `LOW`). This ensures that the game starts once for each valid button press rather than repeatedly triggering while the button is held down.

---

### 2. Startup Animation

After the start button is pressed, the three countdown LEDs perform a non-blocking startup blink animation (3 cycles cycling across the three LEDs).

This provides visual feedback that the round is about to begin. **False-start detection is active** during this animation: pressing a player button early will trigger an immediate penalty.

---

### 3. Countdown

A three-step countdown then runs using the countdown LEDs:

```text
LED 1 → LED 2 → LED 3
```

Each countdown LED illuminates sequentially for 1 second. Players must wait through the countdown without pressing their buttons.

---

### 4. False-Start Penalties

Throughout both the startup animation and the countdown sequence, the Arduino continuously checks player button states before the draw signal is given.

If a player presses their button before the draw signal (or holds their button down early):

* The early press is detected as a **false start**.
* The countdown sequence immediately halts, and all countdown LEDs are turned off.
* The offending player is penalized by forfeiting the round: the **opposing player is automatically awarded the win** and their win animation begins.
* If both players press early in the exact same cycle, the round safely resets to the idle state.

---

### 5. Draw Signal

Once the countdown is complete, both player LEDs turn on simultaneously:

```text
Player 1 LED → ON
Player 2 LED → ON
```

This indicates that the draw signal is active and players are now allowed to press their buttons.

---

### 6. Player Input Detection

While in the draw state, the Arduino continuously monitors both player buttons for the first valid press (`HIGH` to `LOW`).

The first player to press their button after the draw signal wins the duel.

---

### 7. Win Output

When a player wins (either by quick reaction or via an opponent's false start):

* The losing player's LED turns off.
* The winning player's LED turns on solid for 1010 ms, pauses off for 500 ms, and then flashes 7 times (167 ms on / 167 ms off).

```text
Winner LED → SOLID (1010 ms) → OFF (500 ms) → 7 FLASHES
Loser LED  → OFF
```

---

### 8. Automatic Reset

After the win display animation completes:

* All LEDs are turned off (`LOW`).
* Button states are re-synchronized to prevent any held or released buttons from causing accidental triggers.
* The game automatically returns to the **idle state** (`STATE_IDLE`).
* Players can start a new round immediately by pressing the start button again, with no manual reset or power cycling required.

---

## 🧠 Code Structure

The program is structured as a non-blocking **Finite State Machine (FSM)** driven by `millis()` timing functions rather than blocking `delay()` calls. This allows the Arduino to continuously monitor player inputs, catch false starts in real time, and smoothly transition between states.

### Finite State Machine (FSM) States

| State | Description |
|---|---|
| `STATE_IDLE` | Waiting for the start button to be pressed using falling edge detection. All LEDs remain off. |
| `STATE_STARTUP` | Runs the non-blocking 9-step countdown LED sequence (333 ms per step) while monitoring for early button presses. |
| `STATE_COUNTDOWN` | Sequentially lights countdown LEDs (1000 ms per step) while actively checking for false starts. |
| `STATE_DRAW` | Both player LEDs turn `HIGH`. Continuously monitors player buttons for reaction presses. |
| `STATE_WIN` | Non-blockingly animates the winner's LED (solid pulse + 7 flashes), then automatically calls `resetToIdle()`. |

---

### Key Functions & `millis()` Timing

#### `setup()`

Initializes the Arduino pins, configures inputs with `INPUT_PULLUP`, sets output pins for LEDs, and calls `resetToIdle()` to prepare the initial state.

#### `loop()`

The main program loop continuously executes a `switch(gameState)` statement without blocking. Because there are no `delay()` calls, `loop()` runs thousands of times per second, ensuring sub-millisecond reaction time detection.

#### `resetToIdle()`

Resets all five LEDs to `LOW`, synchronizes stored button states to prevent stray edge triggers, clears the winner status, and returns `gameState` to `STATE_IDLE`.

#### `startFunction()`

Transitions the game to `STATE_STARTUP`, records the start timestamp (`stateStartTime = millis()`), clears all LEDs, and reads initial button states.

#### `checkFalseStart()`

Actively runs during `STATE_STARTUP` and `STATE_COUNTDOWN`. If either player's button reads `LOW` before the draw signal, the countdown sequence aborts immediately, and the opponent is awarded the win.

#### `updateStartupAnimation()`

Calculates elapsed time with `millis() - stateStartTime`. Every 333 ms, it advances through the 9 steps of the LED animation without blocking. Once complete, it transitions to `STATE_COUNTDOWN`.

#### `updateCountdown()`

Calculates elapsed time with `millis() - stateStartTime`. Advances through the 3 countdown LEDs at 1000 ms intervals. When 3000 ms elapses, it illuminates both player LEDs (`p1LED` and `p2LED`) and transitions to `STATE_DRAW`.

#### `playerPress()`

Active during `STATE_DRAW`. Uses edge detection to check which player pressed their button first while the opposing button remains unpressed, then triggers either `player1Win()` or `player2Win()`.

#### `player1Win()` & `player2Win()`

Sets the winner, turns off all countdown LEDs and the losing player's LED, sets `gameState = STATE_WIN`, and timestamps `stateStartTime = millis()`.

#### `updateWinDisplay()`

Controls the winner's flashing LED animation using elapsed `millis()` intervals:
- **0–1010 ms:** Winner LED on solid.
- **1010–1510 ms:** Winner LED off pause (500 ms).
- **1510–3848 ms:** 7 flashing cycles (167 ms on / 167 ms off).
- **>3848 ms:** Animation completes and automatically calls `resetToIdle()`.

---

## ⚠️ Known Issues and Limitations

### Button Debouncing

The current implementation does not use software button debouncing.

Mechanical push buttons can sometimes produce several rapid electrical transitions (contact bounce) when pressed. While edge detection and input pull-ups mitigate most issues, debouncing filters could further improve switch stability.

---

### Simultaneous Button Presses

If both players press their buttons at the exact same moment during the draw phase, the logic checks Player 1 and Player 2 sequentially within that loop iteration. If both players press early at the exact same microsecond before the draw signal, the round safely resets to idle.

---

## 🚀 Possible Improvements

Several improvements could make the game even more feature-rich:

### Software Debouncing

Implement software button debouncing using timestamp filtering or state integration to prevent mechanical bounce artifacts.

### Add a Buzzer

A buzzer could provide audible countdown and draw signals:

```text
Countdown → Short Beep
Draw → High Beep
Winner → Victory Melody
```

### Score Tracking

Add a scoring system that keeps track of wins across multiple rounds:

```text
Player 1: 3
Player 2: 2
```

### Randomized Countdown Timing

Introduce a variable or randomized delay for the countdown or draw signal to prevent players from anticipating the draw timing.

---

## 📚 What I Learned

This project helped reinforce several fundamental Arduino and programming concepts:

* Digital input and output
* `INPUT_PULLUP`
* LED control
* Button state detection & edge detection
* Non-blocking timing with `millis()`
* Finite State Machine (FSM) architecture
* Real-time input handling & false-start detection
* Automatic state resets and multi-round loops

Learning how to replace blocking `delay()` calls with non-blocking `millis()` timing was a major milestone. It demonstrated how embedded systems can monitor inputs continuously while maintaining precise timing schedules in parallel.

---

## 🔧 Future Development

The project can be expanded beyond a simple reaction game:

* 🏆 Multiple-round tournaments
* 📊 Persistent score tracking
* 🔊 Buzzer and sound effects
* ⏱️ Randomized countdown timing
* 💡 More advanced LED animations
* 🖥️ LCD/OLED score display
* 🎮 Different game modes

---

## 📁 Project Structure

A possible project structure is:

```text
Quick-Draw/
│
├── QuickDraw_Arduino.ino
├── README.md
└── images/
    └── circuit-diagram.png
```

---

## 📝 Conclusion

The Quick Draw Button Game is an Arduino project that demonstrates how digital inputs, outputs, non-blocking timing with `millis()`, and a finite state machine can be combined to create a responsive, real-time interactive game.

With non-blocking execution, real-time false-start detection, and automatic post-win resetting, the game provides a fast-paced duel experience while serving as a practical demonstration of embedded systems programming.

---

## 👨‍💻 Author

**Neo**

Arduino project focused on learning embedded programming, digital electronics, and real-time input handling.
