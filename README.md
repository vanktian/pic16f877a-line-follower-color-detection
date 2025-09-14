# Line-Following Robot with Color Detection (PIC16F877A)

## Description
This project implements a **line-following robot with color detection** using a **PIC16F877A microcontroller**.  
The robot combines motor control, IR sensors, and a TCS3200 color sensor to navigate along a path and respond to color-based commands.  

Key functionalities include:
- **Line Following:** IR sensors detect black/white line patterns to guide movement.  
- **Color Detection:** Robot responds to detected colors (red, green, blue) using LEDs and motion logic.  
- **Motor Control:** Direction and speed are controlled via PWM.  
- **Decision-Making Logic:** Different behaviors triggered based on sensor input (e.g., stop, reverse, resume).  

---

## Features
- **Line-following capability** using 3 IR sensors (left, center, right).  
- **Color recognition** with TCS3200 color sensor.  
- **Motor control** with PWM-based speed regulation.  
- **Interrupt-based counting** for accurate color sensing.  
- **Behavior rules:**
  - **Red** → Stop robot.  
  - **Green** → Resume movement.  
  - **Blue** → Reverse briefly, then continue.  
  - **Yellow** → Ignored.  

---

## Hardware Requirements
- **PIC16F877A Microcontroller**  
- **L293D or equivalent motor driver**  
- **IR sensors** (for line following)  
- **TCS3200 color sensor**  
- **DC motors + wheels**  
- **RGB LEDs** (status indicators)  
- **Power supply**  

---

## Software Requirements
- MPLAB X IDE  
- XC8 Compiler  

---

## Project Structure
- `config.h` – Microcontroller configuration bits and oscillator setup.  
- `main.c` – Main source code implementing motor control, IR sensors, color detection, and decision logic.  
- `nbproject/` – MPLAB X project configuration files.  
- `Makefile` – Build automation for compilation.  

---

## How It Works
- Robot follows a black line using IR sensors.  
- Encounters are handled as follows:  
  - **Intersection detected** → Stop or continue based on context.  
  - **Color detected via TCS3200** → Execute programmed behavior (stop/resume/reverse).  
- Uses interrupts and timers for precise sensor readings.  

---

## Demo Behavior
- Starts with **kickstart motor pulse** to overcome stall torque.  
- Follows line until object is detected.  
- Reads color → interprets behavior.  
- Uses LEDs for visual feedback (red/green/blue).  
- Stops at terminal point after completing the path.  
