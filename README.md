# Autonomous Line-Following Robot with Color Detection (PIC16F877A)

## Video Showcase

https://github.com/user-attachments/assets/8c2bc2d5-69d1-4212-a03c-4c8ff95afe04

## Description

This project implements a **closed-loop autonomous line-following robot** using a **PIC16F877A microcontroller**. The system integrates low-level firmware for motor control, infrared navigation, and color frequency analysis to navigate paths and respond to real-time environment commands.

Key functionalities include:

* **Autonomous Navigation:** IR sensor array detects path patterns to calculate directional error.
* **Interrupt-Driven Color Detection:** Utilizes the **TCS3200 sensor** to trigger behaviors (stop, reverse, resume) based on frequency capture.
* **Hardware PWM Motor Control:** Precise speed and direction regulation achieved through the microcontroller’s internal **CCP modules**.
* **Embedded Decision Logic:** Firmware-level state management to handle sensor interrupts and environmental triggers.

---

## Features

* **Path-Following Logic:** Implemented using **Bang-Bang control** across a 3-sensor IR array (Left, Center, Right).  
* **Advanced Color Recognition:** Leverages the **CCP1 module in Capture Mode** to process TCS3200 frequency outputs.  
* **Motor Regulation:** Hardware-based **PWM (Pulse Width Modulation)** for consistent speed control via the L293D driver.
* **Interrupt Service Routines (ISR):** Used for non-blocking color frequency counting to ensure navigation stability.
* **Behavioral Logic Matrix:**
  - **Red** → Execute hard-brake (Stop).  
  - **Green** → Re-engage PWM logic (Resume).  
  - **Blue** → Reverse polarity, then resume path tracking.  
  - **Yellow** → Frequency filtered (Ignored).

---

## Hardware Requirements

* **PIC16F877A Microcontroller** (8-bit, 20MHz CMOS)
* **L293D H-Bridge Motor Driver**  
* **IR Reflectance Sensors** (Logic-level output)
* **TCS3200 Color Sensor** (Frequency-based output)
* **DC Motors + Chassis**  
* **RGB LEDs** (Status indicators)
* **Regulated Power Supply**  

---

## Software Requirements

* **MPLAB X IDE** (Development Environment)
* **XC8 Compiler** (Standard C)

---

## Project Structure

* `config.h` – Configuration bits, oscillator frequency setup, and pin definitions.
* `main.c` – Core firmware implementing the main loop, **Interrupt Service Routines (ISR)**, and Bang-Bang navigation logic.
* `nbproject/` – MPLAB X internal project metadata.
* `Makefile` – Automated build instructions.

---

## How It Works

1. **Navigation:** The robot follows a black line using Bang-Bang feedback. If the error  deviates from the center, the firmware adjusts motor duty cycles.
2. **Signal Processing:** The TCS3200 sensor outputs a square wave. The firmware uses **Timer1 and the CCP1 interrupt** to capture the period of the signal, converting it to a color value.
3. **Execution:** Once a color is identified via the ISR, the main loop pauses navigation to execute the corresponding motion command (stop/resume/reverse).

---

## Demo Behavior

* **Stall Torque Compensation:** Initiates with a **high-duty cycle kickstart pulse** to overcome static friction.  
* **Real-Time Pathing:** Continuously tracks the line until a color trigger is detected.  
* **Visual Feedback:** RGB LEDs mirror the detected color for real-time debugging.  
* **Terminal Logic:** Automatically executes a full stop upon reaching the terminal path marker.
