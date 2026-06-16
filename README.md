# PremaController ESP32 Framework

Welcome to the **PremaController ESP32 Framework**! This repository provides an end-to-end environment for real-time robotic control, gamepad integration, and dynamic web telemetry. 

This guide is written specifically for absolute beginners. If you have never worked with an ESP32, microcontrollers, or PlatformIO before, this document will take you from zero to a fully working setup.

---

## Table of Contents
1. [Why Use PremaController?](#1-why-use-premacontroller)
2. [Prerequisites & Software Installation](#2-prerequisites--software-installation)
3. [Setting Up Your PlatformIO Project](#3-setting-up-your-platformio-project)
4. [Understanding `platformio.ini`](#4-understanding-platformioini)
5. [Project Structure & Code Deployment](#5-project-structure--code-deployment)
6. [Accelerated Setup: Using the LLM Template](#6-accelerated-setup-using-the-llm-template)
7. [Connecting Your Bluetooth Gamepad](#7-connecting-your-bluetooth-gamepad)
8. [The Universal Web Dashboard](#8-the-universal-web-dashboard)
9. [Troubleshooting Common Issues](#9-troubleshooting-common-issues)

---

## 1. Why Use PremaController?

In robotics research and development, hardware iterations happen incredibly fast. Rewriting a frontend web dashboard, reconfiguring communication protocols, and formatting data logs for every new robot iteration or kinematic model is a massive time sink.

**PremaController eliminates this overhead by completely decoupling the user interface from the hardware.** You only ever write the ESP32 code. When the ESP32 connects to the universal dashboard, it sends a "Handshake" describing what it is—declaring exactly what telemetry it wants to plot and what physical parameters it needs sliders for. The web dashboard dynamically morphs to fit the robot. 

Whether you are building a 4-DOF mass-shifting worm, a tracked vehicle, or a simple sensor rig, the framework instantly provides:
* **Zero-config Wi-Fi telemetry.**
* **Native PlayStation/Xbox Gamepad support.**
* **Automatic CSV data logging for external analysis.**

---

## 2. Prerequisites & Software Installation

Instead of using the basic Arduino IDE, this project utilizes **PlatformIO**, a professional, extension-based development ecosystem running inside **Visual Studio Code (VS Code)**.

### Step-by-Step Installation:
1. **Download and Install VS Code:** Go to the [Visual Studio Code Website](https://code.visualstudio.com/) and download the installer.
2. **Install Python:** PlatformIO requires Python to compile code. Install it from the [Python Official Site](https://www.python.org/) and ensure you check **"Add Python to PATH"** during installation.
3. **Install the PlatformIO Extension:**
   * Open VS Code.
   * Click on the **Extensions** icon on the left-hand sidebar (`Ctrl+Shift+X` / `Cmd+Shift+X`).
   * Search for `PlatformIO IDE`.
   * Click **Install**. Wait for the installation to finish in the bottom right corner, then restart VS Code.

---

## 3. Setting Up Your PlatformIO Project

1. **Open the Project:** Go to **File > Open Folder...** and select the root directory where you cloned this repository.
2. **Let PlatformIO Initialize:** PlatformIO will automatically recognize the project and begin indexing. This might take a minute or two on the first run.

---

## 4. Understanding `platformio.ini`

The `platformio.ini` file is the brain of your project. It automatically downloads external libraries so you never have to hunt down `.zip` files manually.

```ini
[env:esp32doit-devkit-v1]
platform = espressif32@6.10.0
board = esp32doit-devkit-v1
framework = arduino
monitor_speed = 115200
board_build.partitions = huge_app.csv

; CRITICAL: Replaces the stock ESP32 core with a custom Bluepad32-compatible core
platform_packages = 
    framework-arduinoespressif32@https://github.com/maxgerhardt/pio-framework-bluepad32/archive/refs/heads/main.zip

lib_deps = 
    bblanchon/ArduinoJson@^7.0.4
    links2004/WebSockets
    madhephaestus/ESP32Servo
```
*Note: We override the standard ESP32 core because standard boards cannot handle modern Bluetooth gamepads natively. The custom package has Bluepad32 engine optimizations natively baked in.*

---

## 5. Project Structure & Code Deployment

Your workspace follows this file hierarchy:
```text
├── include/              # Header files (.h)
├── src/
│   ├── main.cpp          # Your primary C++ firmware file
│   └── main_template.cpp # The blank framework template
├── index.html            # The raw UI file (hosted online)
├── llm_prompt_template.txt # AI code generation tool
└── platformio.ini        # Project configuration
```

### Compiling and Uploading:
1. Open `src/main.cpp`. Update the Wi-Fi credentials to match your hotspot or router.
2. Connect your ESP32 via a data-capable USB cable.
3. Use the PlatformIO Task Toolbar at the bottom of VS Code:
   * **Build (✓):** Checks for syntax errors.
   * **Upload (→):** Compiles and flashes the code to the ESP32.
   * **Serial Monitor (Plug):** Opens the terminal to read outputs. Look for `Connecting to WiFi...` and write down the **ESP32 IP Address** it displays!

---

## 6. Accelerated Setup: Using the LLM Template

To make iterating even faster, this repository includes an `llm_prompt_template.txt` file. You do not need to write the dense WebSocket JSON formatting yourself when making a new robot.

**How to use it:**
1. Open `llm_prompt_template.txt`.
2. Fill in the brackets `[ ]` with your specific hardware pins, logic requirements, desired sliders, and telemetry parameters.
3. Copy the entire filled text, along with the contents of `main_template.cpp`.
4. Paste it into an LLM (like ChatGPT, Claude, or Gemini).
5. The LLM will populate the "Injection Zones" perfectly and return a fully compilable, ready-to-flash `main.cpp` customized for your exact robot.

---

## 7. Connecting Your Gamepad

1. Turn on your Bluetooth controller (DualShock 4, Xbox One, Switch Pro).
2. Put the controller into **Pairing Mode** (e.g., hold Share + PS on PlayStation).
3. The ESP32 will automatically scan, authenticate, and connect. It will rumble or show a solid light when paired.

---

## 8. The Universal Web Dashboard

You do not need to host or modify any HTML/JS files. We have deployed the universal client directly via GitHub Pages.

**Access the dashboard here:** [[https://meneralllka.github.io/WorMa_Controller/](https://meneralllka.github.io/WorMa_Controller/)](https://prema-lab.github.io/PremaController_Web/)

### A. The Connection Handshake
1. Ensure your computer/phone is on the **same Wi-Fi network** as the ESP32.
2. Enter your ESP32's IP address into the dashboard and click **Connect**.
3. **What happens under the hood?** The ESP32 instantly sends a JSON string `CONFIG:...`. The website reads this, determines exactly what your robot is, and spawns the exact sliders and chart legends required.

### B. How the Dynamic Sliders Work
When the dashboard builds a slider (e.g., "Frequency" from 0 to 100), moving that slider executes a real-time command back to the robot.
* The website sends a lightweight packet: `SET:frequency:45.0`.
* The ESP32 intercepts this in the `webSocketEvent` function and instantly updates the internal variables controlling your kinematics or logic.

### C. How Telemetry and Logging Works
The ESP32 pushes a continuous stream of sensor data over the socket using the format `DATA:<timestamp>,<val1>,<val2>`. 
* **Live Plotting:** The website parses this and smoothly rolls the data across a multi-line chart for visual feedback.
* **Recording:** Click **Start Recording** to begin buffering this stream into local memory.
* **Exporting:** Click **Stop Recording**, and the dashboard will automatically compile the buffered matrix, generate correct headers based on the initial handshake, and download a cleanly formatted `.csv` file directly to your machine. This file is immediately ready for Python/Matplotlib or Excel analysis.

---

## 9. Troubleshooting Common Issues

* **`Serial Port not found` during upload:** Your USB cable might be power-only. Swap it for a certified data cable, or install CP2102/CH340 drivers.
* **Web Dashboard won't connect:** Ensure your device and the ESP32 are on the exact same Wi-Fi subnet. Double-check the IP address in the Serial Monitor.
* **Gamepad isn't registering inputs:** Press the **EN / RST** button on the ESP32 chip to trigger a fresh hardware scan cycle.
* > **Error: Dashboard won't connect due to a "Mixed Content" or security block.**
* Because the dashboard is hosted securely on GitHub Pages (`https://`) but connects to your local ESP32 over an unencrypted socket (`ws://`), modern browsers will block the connection by default. 
* Look for a **shield or lock icon** in your browser's URL address bar indicating that insecure content was blocked. Click the icon, go to Site Settings, and select **"Allow Insecure Content"** (or click "Load Unsafe Scripts") to permit the local connection to your robot.
