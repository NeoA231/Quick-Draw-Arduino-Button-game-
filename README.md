Quick Draw Button Game – Arduino Project Documentation

Project Overview
Quick Draw is a two player reaction game built using an Arduino and simple digital
components. The idea is inspired by classic quick draw duels. Both players wait through a
countdown sequence and then must press their button as fast as possible once the signal is
given. The first valid button press wins the round.

The project focuses on understanding digital inputs, outputs, timing, and basic state change
detection using Arduino.

Game Objective
Two players compete to press their button first after a countdown finishes. Pressing early
does nothing. Pressing first after the signal causes the winning player’s LED to flash while
the losing player’s LED turns off.

Hardware Components
Arduino board, 
3 countdown LEDs, 
2 player LEDs, 
3 push buttons (start, player 1, player 2)
Resistors for LEDs,
Jumper wires, 
breadboard

Pin Configuration
Start button: Pin 10
Countdown LEDs: Pins 13, 12, 11
Player 1 LED: Pin 2
Player 2 LED: Pin 3
Player 1 button: Pin 7
Player 2 button: Pin 6
All buttons use INPUT_PULLUP, meaning they idle HIGH and go LOW when pressed.

How the Game Works
1. Idle State
The game waits for the start button to be pressed. Button state changes are detected using
edge detection to ensure the game only starts once per press.
2. Countdown Sequence
When the start button is pressed: The three countdown LEDs flash quickly as a startup
animation. A proper countdown then runs from LED 1 to LED 3 with one second delays.
3. Draw Signal
Once the countdown finishes, both player LEDs turn on. This signals that players are
allowed to press their buttons.
4. Player Input Detection
The program checks both player buttons. The first button that changes from HIGH to LOW is
declared the winner, as long as the other player has not pressed yet.
5. Win Output
The winning player’s LED flashes in a pattern. The losing player’s LED turns off.


Code Structure

setup()
Initializes all pin modes. Buttons are configured using INPUT_PULLUP. LEDs are set as
OUTPUT.

loop()
Continuously monitors the start button. Detects a valid press using state comparison. Calls
startFunction() when the game begins.

startFunction()
Runs startup blink animation. Runs the countdown sequence. Turn on player LEDs. Calls
playerPress() to check for button presses.

playerPress()
Reads both player button states. Determines which player pressed first. Call the appropriate
win function.

player1Win() and player2Win()
Control LED behavior for win indication. Flash the winning player’s LED.

Known Issues and Limitations

Button debouncing is not implemented, which may cause false triggers in rare cases. The
game flow is blocked due to use of delay(), meaning no other inputs are processed during
delays.

Possible Improvements

Add button debouncing using timing or software filtering. Replace delay() with millis() to
make the game non-blocking. Add a reset state after a win. Add sound feedback using a
buzzer. Add score tracking for multiple rounds.

Conclusion

This project demonstrates core Arduino concepts such as digital I/O, state tracking, timing,
and basic game logic. It is a solid foundation for learning how real time input handling works
and can be expanded into a more complex reaction based game with improved
responsiveness and structure.
Through this project, I have recently learned about blocking code and how excessive use of
delay() can prevent the Arduino from responding to inputs in real time.
