# Digital-voting-machine-with-secure-memory-

Project Overview

This project implements an Arduino-based Electronic Voting Machine (EVM) using push buttons, a Finite State Machine (FSM), EEPROM storage, and an LCD display. The system allows users to cast votes, stores vote counts in EEPROM memory, and displays voting status and results.

Objectives

- Design an electronic voting system using Arduino.
- Implement vote-counting logic using a Finite State Machine (FSM).
- Store vote counts in EEPROM for persistence after power loss.
- Display voting information and results using an LCD.
- Implement push-button debouncing for reliable vote registration.

Hardware Components

- Arduino Uno R3 (ATmega328P)
- 16×2 LCD with I2C module
- Push buttons (candidate selection and result/admin)
- LEDs (status indication)
- 220Ω resistors
- Breadboard
- Jumper wires (Male-to-Male and Male-to-Female)
- USB cable

Software and Technologies

- Arduino IDE
- Embedded C/C++
- FSM (Finite State Machine)
- EEPROM (Non-Volatile Memory)
- I2C communication

System Features

- Push-button-based voting
- FSM-controlled voting process
- EEPROM-based vote storage
- LCD-based status and result display
- Button debouncing
- LED status indication
- Vote retention after power-off

Working Principle

1. The system initializes the Arduino, LCD, buttons, and EEPROM.
2. The LCD displays the welcome message and voting instructions.
3. A voter selects a candidate using the corresponding push button.
4. The FSM processes the button input and validates the voting operation.
5. The vote count is updated and stored in EEPROM.
6. The LCD displays confirmation and voting status.
7. Authorized result mode displays the candidate-wise vote counts.

FSM Design

The voting system uses a Finite State Machine to manage the voting process.

- IDLE: Waits for user input.
- VOTING: Detects and processes a candidate selection.
- CONFIRMATION: Confirms that the vote has been recorded.
- RESULTS: Displays vote counts when results mode is activated.

EEPROM Storage

The Arduino's built-in EEPROM is used to store vote counts. This allows the stored voting data to remain available even when the system is powered off.

Button Debouncing

Software debouncing is implemented to reduce false or repeated button detections caused by mechanical switch bouncing.

Display

A 16×2 I2C LCD is used to display:

- Welcome messages
- Candidate selection information
- Vote confirmation
- Voting status
- Candidate-wise results

Testing

The system is tested for:

- Correct button detection
- Accurate vote counting
- FSM operation
- LCD message display
- EEPROM data retention after power-off
- Button debouncing

Applications

- Embedded systems learning
- Digital electronics demonstrations
- Voting system prototypes
- Arduino-based educational projects

Project Status

Prototype development, hardware integration, programming, and testing.

Author

Nikita Tambe
