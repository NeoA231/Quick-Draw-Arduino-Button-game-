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

Pressing a player button before the draw signal does not count as a valid move.

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

## 🕹️ How the Game Works

### 1. Idle State

When the Arduino starts, the game remains in an idle state waiting for the start button.

The program uses **edge detection** to detect a change in the button state. This ensures that the game starts once for each valid button press rather than repeatedly triggering while the button is held down.

---

### 2. Startup Animation

After the start button is pressed, the three countdown LEDs perform a quick startup blink animation.

This provides visual feedback that the game is about to begin.

---

### 3. Countdown

A three-step countdown then runs using the countdown LEDs.

Each LED activates sequentially with a one-second delay between steps.

```text
LED 1 → LED 2 → LED 3
```

This gives the players time to prepare.

---

### 4. Draw Signal

Once the countdown is complete, both player LEDs turn on.

```text
Player 1 LED → ON
Player 2 LED → ON
```

This indicates that players are now allowed to press their buttons.

---

### 5. Player Input Detection

The Arduino continuously checks both player buttons.

The program looks for the first button that changes from:

```text
HIGH → LOW
```

The first valid button press is declared the winner.

---

### 6. Win Output

When a player wins:

* The winning player's LED flashes.
* The losing player's LED turns off.

For example:

```text
Player 1 wins

Player 1 LED → FLASHING
Player 2 LED → OFF
```

---

## 🧠 Code Structure

The program is divided into several functions to separate the different parts of the game.

### `setup()`

Initializes the Arduino pins.

Responsibilities include:

* Configuring LEDs as `OUTPUT`
* Configuring buttons as `INPUT_PULLUP`
* Setting the initial LED states

---

### `loop()`

The main program loop continuously monitors the start button.

It checks for a valid state change and calls:

```cpp
startFunction();
```

when the game begins.

---

### `startFunction()`

Controls the beginning of a round.

Responsibilities include:

* Running the startup blink animation
* Running the countdown
* Turning on both player LEDs
* Starting player input detection

It then calls:

```cpp
playerPress();
```

---

### `playerPress()`

Reads the player buttons and determines which player pressed first.

The function checks the state of both buttons and calls the appropriate win function.

```cpp
player1Win();
```

or

```cpp
player2Win();
```

---

### `player1Win()` and `player2Win()`

These functions control the winning LED.

The winning player's LED flashes while the other player's LED is turned off.

---

## ⚠️ Known Issues and Limitations

### Button Debouncing

The current implementation does not use software button debouncing.

Mechanical push buttons can sometimes produce several rapid electrical transitions when pressed. This can potentially result in false triggers.

A debouncing system could be added using timing or software filtering.

---

### Blocking `delay()`

The game currently relies on Arduino's:

```cpp
delay();
```

function.

For example:

```cpp
delay(1000);
```

While `delay()` is running, the Arduino does not process the game logic normally.

This means that inputs cannot be handled in real time during these delays.

This project helped demonstrate an important Arduino programming concept: **blocking code**.

---

## 🚀 Possible Improvements

Several improvements could make the game more responsive and feature-rich.

### Software Debouncing

Implement button debouncing using a timer or state filtering.

### Replace `delay()` with `millis()`

Using:

```cpp
millis();
```

would allow the program to perform timing without completely blocking the main loop.

This would make it possible to monitor inputs continuously while timing operations are running.

### Reset After a Win

Automatically return the game to the idle state after displaying the winner.

### Add a Buzzer

A buzzer could provide an audible countdown and draw signal.

For example:

```text
Countdown → Beep
Draw → Long Beep
Winner → Beep pattern
```

### Score Tracking

Add a scoring system that keeps track of wins across multiple rounds.

Example:

```text
Player 1: 3
Player 2: 2
```

### False Start Detection

Instead of simply ignoring early button presses, the game could detect false starts and penalize the player who presses before the draw signal.

---

## 📚 What I Learned

This project helped reinforce several fundamental Arduino and programming concepts:

* Digital input and output
* `INPUT_PULLUP`
* LED control
* Button state detection
* Edge detection
* Timing and sequencing
* Function-based program structure
* Basic game logic
* Real-time input handling
* Blocking vs non-blocking code

One of the most important things I learned was how **blocking code** affects real-time systems.

Using too many `delay()` calls can prevent an Arduino from responding to inputs while it is waiting. This showed me why techniques such as `millis()` are useful when building systems that need to monitor multiple inputs continuously.

---

## 🔧 Future Development

The project can be expanded beyond a simple reaction game.

Potential future versions could include:

* 🏆 Multiple-round tournaments
* 📊 Persistent score tracking
* 🔊 Buzzer and sound effects
* ⏱️ Randomized countdown timing
* 🚨 False-start detection
* 💡 More advanced LED animations
* 🖥️ LCD/OLED score display
* 🎮 Different game modes
* ⚡ Non-blocking game logic using `millis()`

---

## 📁 Project Structure

A possible project structure is:

```text
Quick-Draw/
│
├── Quick-Draw.ino
├── README.md
└── images/
    └── circuit-diagram.png
```

---

## 📝 Conclusion

The Quick Draw Button Game is a simple Arduino project that demonstrates how digital inputs, outputs, timing, and state changes can be combined to create an interactive system.

Although the current implementation uses blocking `delay()` calls and does not include button debouncing, these limitations provide useful learning opportunities.

The project serves as a foundation for understanding real-time input handling and can be extended with non-blocking timing, scoring, sound feedback, false-start detection, and additional game modes.

---

## 👨‍💻 Author

**Neo**

Arduino project focused on learning embedded programming, digital electronics, and real-time input handling.
