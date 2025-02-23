# Garage Door System - Complete Design

## Project Structure
```
garage_door_system/
│
├── CMakeLists.txt
│
├── src/
│   ├── main.cpp
│   ├── GarageDoorsystem.cpp
│   ├── GarageDoorsystem.h
│   │
│   ├── hardware/
│   │   ├── limit_switch.h
│   │   ├── limit_switch.cpp
│   │   ├── rotary_encoder.h
│   │   ├── rotary_encoder.cpp
│   │   ├── stepper_motor.h
│   │   └── stepper_motor.cpp
│   │
│   ├── controller/
│   │   ├── door_controller.h
│   │   ├── door_controller.cpp
│   │   └── door_status.h
│   │
│   ├── commands/
│   │   ├── command.h
│   │   ├── open_command.h
│   │   ├── close_command.h
│   │   ├── stop_command.h
│   │   └── calibrate_command.h
│   │
│   ├── state_machine/
│   │   ├── state_machine.h
│   │   ├── state_machine.cpp
│   │   ├── states.h
│   │   └── state_observer.h
│   │
│   └── input/
│       ├── input_handler.h
│       ├── button_handler.h
│       ├── button_handler.cpp
│       ├── mqtt_handler.h
│       └── mqtt_handler.cpp
```

## 1. Hardware Layer

### LimitSwitch (hardware/limit_switch.h)
```cpp
#pragma once

class LimitSwitch {
public:
    explicit LimitSwitch(uint8_t pin);
    
    // Returns true if switch is activated
    bool isHit() const;

private:
    uint8_t pin;
};
```

### RotaryEncoder (hardware/rotary_encoder.h)
```cpp
#pragma once
#include <cstdint>

class RotaryEncoder {
public:
    RotaryEncoder(uint8_t pinA, uint8_t pinB);
    
    // Get current position
    int32_t getPosition() const;
    
    // Check if position changed since last check
    bool hasMovedSinceLastCheck();
    
    // Reset position counter
    void reset();

private:
    uint8_t pinA, pinB;
    volatile int32_t position;
    int32_t lastCheckedPosition;
    
    static void handleInterrupt(uint gpio, uint32_t events, void* ctx);
};
```

### StepperMotor (hardware/stepper_motor.h)
```cpp
#pragma once
#include <array>

class StepperMotor {
public:
    StepperMotor(uint8_t in1, uint8_t in2, uint8_t in3, uint8_t in4);
    
    void moveUp();
    void moveDown();
    void stop();

private:
    enum class Direction { UP, DOWN, STOP };
    std::array<uint8_t, 4> pins;
    Direction currentDirection;
    
    void setDirection(Direction dir);
};
```

## 2. Controller Layer

### DoorStatus (controller/door_status.h)
```cpp
#pragma once
#include <string>

struct DoorStatus {
    bool upperLimitHit;      // Upper limit switch status
    bool lowerLimitHit;      // Lower limit switch status
    bool isMoving;           // Whether door is in motion
    int32_t currentPosition; // Current encoder position
    bool isCalibrated;       // System calibration status
    bool isError;            // Error state
    std::string errorMessage;// Error description if any
};
```

### DoorController (controller/door_controller.h)
```cpp
#pragma once
#include "../hardware/limit_switch.h"
#include "../hardware/rotary_encoder.h"
#include "../hardware/stepper_motor.h"
#include "door_status.h"
#include <memory>

class DoorController {
public:
    DoorController(
        std::unique_ptr<LimitSwitch> upperLimit,
        std::unique_ptr<LimitSwitch> lowerLimit,
        std::unique_ptr<RotaryEncoder> encoder,
        std::unique_ptr<StepperMotor> motor
    );

    // Get current door status
    DoorStatus getDoorStatus();

    // Control commands
    void startOpening();
    void startClosing();
    void stopMovement();
    void setCalibrated(bool calibrated);

private:
    std::unique_ptr<LimitSwitch> upperLimit;
    std::unique_ptr<LimitSwitch> lowerLimit;
    std::unique_ptr<RotaryEncoder> encoder;
    std::unique_ptr<StepperMotor> motor;
    bool isCalibrated;
    
    void checkMovement();
    void updatePosition();
};
```

## 3. Command System

### Command Interface (commands/command.h)
```cpp
#pragma once
#include "../controller/door_controller.h"
#include <string>

class DoorCommand {
public:
    virtual ~DoorCommand() = default;
    virtual void execute(DoorController& controller) = 0;
    virtual std::string getType() const = 0;
};
```

### Concrete Commands (commands/open_command.h, etc.)
```cpp
#pragma once
#include "command.h"

class OpenCommand : public DoorCommand {
public:
    void execute(DoorController& controller) override {
        controller.startOpening();
    }
    
    std::string getType() const override { return "OPEN"; }
};

// Similar implementations for Close, Stop, and Calibrate commands
```

## 4. State Machine

### States (state_machine/states.h)
```cpp
#pragma once
#include <string>
#include <map>
#include <vector>

enum class SystemState {
    UNCALIBRATED,
    IDLE,
    OPENING,
    CLOSING,
    STOPPED,
    ERROR,
    CALIBRATING
};

// Convert state to string representation
std::string stateToString(SystemState state);

// Check if state transition is valid
bool isValidTransition(SystemState from, SystemState to);
```

### State Observer (state_machine/state_observer.h)
```cpp
#pragma once
#include "states.h"
#include <string>

class IStateObserver {
public:
    virtual ~IStateObserver() = default;
    virtual void onStateChange(SystemState state) = 0;
    virtual void onError(const std::string& error) = 0;
    virtual void onCalibrationChange(bool isCalibrated) = 0;
    virtual void onPositionUpdate(uint32_t position) = 0;
};
```

### State Machine (state_machine/state_machine.h)
```cpp
#pragma once
#include "states.h"
#include "state_observer.h"
#include "../controller/door_controller.h"
#include "../commands/command.h"
#include <memory>
#include <vector>

class StateMachine {
public:
    explicit StateMachine(DoorController& controller);

    void update();
    void handleCommand(std::unique_ptr<DoorCommand> command);
    void addObserver(IStateObserver* observer);
    SystemState getCurrentState() const;

private:
    DoorController& controller;
    SystemState currentState;
    std::vector<IStateObserver*> observers;

    void handleState(const DoorStatus& status);
    bool isCommandValidForState(const std::string& commandType) const;
    void transitionTo(SystemState newState);
    void notifyObservers();
};
```

## 5. Input Handlers

### Input Handler Interface (input/input_handler.h)
```cpp
#pragma once

class IInputHandler {
public:
    virtual ~IInputHandler() = default;
    virtual void update() = 0;
    virtual void start() = 0;
    virtual void stop() = 0;
};
```

### Button Handler (input/button_handler.h)
```cpp
#pragma once
#include "input_handler.h"
#include "../state_machine/state_machine.h"

class ButtonHandler : public IInputHandler {
public:
    explicit ButtonHandler(StateMachine& stateMachine);
    
    void update() override;
    void start() override;
    void stop() override;

private:
    struct Button {
        uint8_t pin;
        bool lastState;
        uint32_t lastDebounceTime;
        Button(uint8_t p) : pin(p), lastState(false), lastDebounceTime(0) {}
    };

    StateMachine& stateMachine;
    Button sw0{0}, sw1{1}, sw2{2};
    static constexpr uint32_t DEBOUNCE_DELAY = 50;
    
    void handleButtons();
    void createCommandFromButton();
};
```

### MQTT Handler (input/mqtt_handler.h)
```cpp
#pragma once
#include "input_handler.h"
#include "../state_machine/state_observer.h"
#include "../state_machine/state_machine.h"
#include <mosquitto.h>

class MQTTHandler : public IInputHandler, public IStateObserver {
public:
    explicit MQTTHandler(StateMachine& stateMachine);
    ~MQTTHandler();

    // IInputHandler interface
    void update() override;
    void start() override;
    void stop() override;

    // IStateObserver interface
    void onStateChange(SystemState state) override;
    void onError(const std::string& error) override;
    void onCalibrationChange(bool isCalibrated) override;
    void onPositionUpdate(uint32_t position) override;

private:
    StateMachine& stateMachine;
    struct mosquitto* mosq;
    bool connected{false};
    
    static constexpr char COMMAND_TOPIC[] = "garage/door/command";
    static constexpr char STATUS_TOPIC[] = "garage/door/status";
    static constexpr char ERROR_TOPIC[] = "garage/door/error";
    
    void handleMessage(const mosquitto_message* message);
    void publishStatus(const std::string& status);
    void connectMQTT();
};
```

## 6. Main System

### System Class (system.h)
```cpp
#pragma once
#include "controller/door_controller.h"
#include "state_machine/state_machine.h"
#include "input/input_handler.h"
#include <vector>
#include <memory>

class GarageSystem {
public:
    GarageSystem();
    void run();

private:
    // Components
    std::unique_ptr<LimitSwitch> upperLimit;
    std::unique_ptr<LimitSwitch> lowerLimit;
    std::unique_ptr<RotaryEncoder> encoder;
    std::unique_ptr<StepperMotor> motor;
    
    // System layers
    std::unique_ptr<DoorController> controller;
    std::unique_ptr<StateMachine> stateMachine;
    
    // Input handlers
    std::vector<std::unique_ptr<IInputHandler>> inputHandlers;

    void initializeComponents();
    void initializeHandlers();
    void updateHandlers();
    void updateStateMachine();
};
```

### Main (main.cpp)
```cpp
#include "system.h"
#include <iostream>

int main() {
    try {
        GarageSystem system;
        system.run();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
```

