# Garage Door Controller System

A C++ implementation of a garage door control system using a Raspberry Pi Pico (RP2040). The system provides both local button control and remote MQTT control capabilities.

## Features

- Local control via physical buttons
- Remote control through MQTT
- Automatic calibration system
- Position tracking using rotary encoder
- Limit switch safety controls
- Status reporting via MQTT
- Error detection and handling

## Hardware Requirements

- Raspberry Pi Pico (RP2040)
- Stepper motor
- Rotary encoder
- 2x Limit switches
- 3x Control buttons
- Power supply
- WiFi connection (for MQTT)

## Software Dependencies

- C++17 or later
- CMake 3.12 or later
- Pico SDK
- libmosquitto
- Docker (for MQTT broker)

## Project Structure

```
garage_door_system/
├── src/                # Source files
├── include/            # Public headers
├── tests/             # Test files
├── docs/              # Documentation
└── tools/             # Utility scripts
```

## Building the Project

1. Clone the repository:
```bash
git clone [repository-url]
cd garage_door_system
```

2. Create build directory:
```bash
mkdir build
cd build
```

3. Configure and build:
```bash
cmake ..
make
```

## MQTT Setup

1. Start the MQTT broker using Docker:
```bash
docker-compose up -d
```

2. MQTT topics:
- Commands: `garage/door/command`
- Status: `garage/door/status`
- Errors: `garage/door/error`

## Usage

### Local Control
- Single press of SW1: Open/Close door
- Press while moving: Stop door
- SW0 + SW2 (2 seconds): Start calibration

### MQTT Control
Send commands to `garage/door/command`:
```json
{
    "command": "OPEN|CLOSE|STOP|CALIBRATE",
    "id": "unique-command-id",
    "timestamp": "2024-02-23T10:30:00Z"
}
```

## Pin Configuration

- Stepper Motor: GP2, GP3, GP6, GP13
- Limit Switches: Grove connector (2 GPIOs)
- Rotary Encoder: Grove connector (2 GPIOs)
- Control Buttons: GP20, GP21, GP22

## State Machine

The system implements the following states:
- UNCALIBRATED: Initial state, needs calibration
- IDLE: Door fully open or closed
- OPENING: Door moving up
- CLOSING: Door moving down
- STOPPED: Door stopped mid-movement
- ERROR: System error state
- CALIBRATING: System performing calibration


## License

