# Garage Door Control System

## Overview
This project implements a garage door control system using a Raspberry Pi Pico (RP2040) microcontroller. The system provides both local and remote control of a garage door through physical buttons and MQTT communication, with status reporting, error detection, and persistence across power cycles.

## Features
- **Dual Control Modes**: Operate the door locally using buttons or remotely via MQTT
- **Automatic Calibration**: System calibrates door movement ranges using limit switches
- **Status Monitoring**: Real-time reporting of door status via MQTT
- **Error Detection**: Detects when the door gets stuck and enters error state
- **Persistent State**: Preserves calibration and door state across power cycles
- **Visual Feedback**: LED indicators for different system states

## Hardware Components
- Raspberry Pi Pico (RP2040)
- Stepper motor for door movement
- Rotary encoder for movement detection
- Two limit switches to detect door position limits
- Three buttons for local control
- Three LEDs for status indication
- EEPROM for state persistence

## Pin Assignments
- **Stepper Motor Controller**: GP2, GP3, GP6, GP13
- **LEDs**: GP20, GP21, GP22
- **Buttons**: SW0 (GP9), SW1 (GP8), SW2 (GP7)
- **Limit Switches**: Upper (GP16), Lower (GP17)
- **Rotary Encoder**: A (GP28), B (GP27)
- **EEPROM**: SDA (GP14), SCL (GP15)

## System Architecture
The software follows an object-oriented design with clear separation of responsibilities:

1. **Hardware Layer**:
   - `StepperMotor`: Controls the motor that moves the door
   - `RotaryEncoder`: Detects movement direction and position
   - `Button`/`LimitSwitch`: Handles input detection
   - `LED`: Provides visual feedback
   - `EEPROM`: Persistent storage

2. **Controller Layer**:
   - `DoorController`: Manages the hardware components
   - `Storage`: Handles saving/loading state from EEPROM

3. **Handler Layer**:
   - `ButtonHandler`: Processes button inputs
   - `MQTTHandler`: Handles remote communication
   - `GPIOInterrupt`: Manages hardware interrupts

4. **System Layer**:
   - `GarageDoorSystem`: Main system controller coordinating all components

## Operation Guide

### Local Operation
- **Calibration**: Press SW0 and SW2 simultaneously to start calibration
- **Open/Close**: Press SW1 to operate the door:
  - When door is closed → Opens the door
  - When door is open → Closes the door
  - While door is moving → Stops the door
  - When door has been stopped → Moves in the opposite direction

### Remote Operation via MQTT
- **Subscribe to**: `garage/door/command` to send commands
- **Publish to**: `garage/door/status` to receive status updates
- **Response Topic**: `garage/door/response` for command responses

### Status Indicators
- **Not Calibrated**: LED1 blinking fast
- **Opening/Closing**: LED1 constant on
- **Idle (not moving)**: LED1 slowly blinking
- **Calibrating**: All three LEDs on

## MQTT Commands
- `open`: Opens the door
- `close`: Closes the door
- `stop`: Stops door movement
- `calib`: Initiates calibration procedure

## Status Reporting
The system reports status in the following format:
```
Door:[OPENED/CLOSED/OPENING/CLOSING/IDLE],Error:[NORMAL/STUCK],Calib:[CALIBRATED/UNCALIBRATED]
```

## Building and Flashing
This project is built using CMake and the Pico SDK. To build:

1. Set up the Pico SDK environment
2. Configure with CMake:
   ```
   mkdir build
   cd build
   cmake ..
   ```
3. Build the project:
   ```
   make
   ```
4. Flash the binary to the Pico (connect while holding BOOTSEL button)

## Important Notes
- Never turn the stepper motor or move the belt by hand!
- Door will not operate in uncalibrated state
- Remote control does not work in uncalibrated state
- If the door gets stuck, it will enter error state and require recalibration

## Dependencies
- Pico SDK
- MQTT Client library
- EEPROM I2C library
