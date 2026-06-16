# PremaController ESP32 Framework

Welcome to the **PremaController ESP32 Framework**! This repository provides an end-to-end environment for real-time robotic control, gamepad integration, and dynamic web telemetry. 

This guide is written specifically for absolute beginners. If you have never worked with an ESP32, microcontrollers, or PlatformIO before, this document will take you from zero to a fully working setup.

---

## Table of Contents
1. [Prerequisites & Software Installation](#1-prerequisites--software-installation)
2. [Setting Up Your PlatformIO Project](#2-setting-up-your-platformio-project)
3. [Understanding and Configuring `platformio.ini`](#3-understanding-and-configuring-platformioini)
4. [Project Structure & Code Deployment](#4-project-structure--code-deployment)
5. [Connecting Your Bluetooth Gamepad](#5-connecting-your-bluetooth-gamepad)
6. [Launching the Web Dashboard](#6-launching-the-web-dashboard)
7. [Troubleshooting Common Issues](#7-troubleshooting-common-issues)

---

## 1. Prerequisites & Software Installation

Instead of using the basic Arduino IDE, this project utilizes **PlatformIO**, a professional, extension-based development ecosystem running inside **Visual Studio Code (VS Code)**.

### Step-by-Step Installation:
1. **Download and Install VS Code:** * Go to the official [Visual Studio Code Website](https://code.visualstudio.com/) and download the installer for your operating system (Windows, macOS, or Linux). Run the installer with default settings.
2. **Install Python:**
   * PlatformIO requires Python to compile code. Download and install Python from the [Python Official Site](https://www.python.org/). Ensure you check the box that says **"Add Python to PATH"** during installation.
3. **Install the PlatformIO Extension:**
   * Open VS Code.
   * Click on the **Extensions** icon on the left-hand sidebar (the 4 squares icon, or press `Ctrl+Shift+X` / `Cmd+Shift+X`).
   * In the search bar, type `PlatformIO IDE`.
   * Click the green **Install** button on the extension offered by *PlatformIO*.
   * **Important:** Wait for the installation to finish. You will see a message in the bottom right corner. Once finished, restart VS Code when prompted. You will now see an **Ant Head Icon** on your left sidebar.

---

## 2. Setting Up Your PlatformIO Project

To get this repository's code running inside your newly installed environment:

1. **Open the Project:**
   * Open VS Code.
   * Go to **File > Open Folder...** and select the root directory where you cloned or downloaded this repository.
2. **Let PlatformIO Initialize:**
   * Once the folder is open, PlatformIO will automatically recognize the `platformio.ini` file and begin indexing your project workspace. This initial index might take a minute or two.

---

## 3. Understanding and Configuring `platformio.ini`

The `platformio.ini` file is the brain of your project configuration. It tells PlatformIO which chip you are using, what custom frameworks are required, and what external libraries to download automatically.

Open the `platformio.ini` file in the root directory. It contains critical configurations:

```ini
[env:esp32doit-devkit-v1]
platform = espressif32@6.10.0
board = esp32doit-devkit-v1
framework = arduino
monitor_speed = 115200

; Allocates a huge 3MB application partition block to safely host complex logic
board_build.partitions = huge_app.csv

; CRITICAL: Replaces the stock ESP32 framework core with a custom Bluepad32-compatible core
platform_packages = 
    framework-arduinoespressif32@[https://github.com/maxgerhardt/pio-framework-bluepad32/archive/refs/heads/main.zip](https://github.com/maxgerhardt/pio-framework-bluepad32/archive/refs/heads/main.zip)

; Automated dependency manager: PlatformIO downloads these on compilation
lib_deps = 
    bblanchon/ArduinoJson@^7.0.4
    links2004/WebSockets
    madhephaestus/ESP32Servo
```

### Why this configuration is unique:
* **The Custom Core (`platform_packages`):** Standard ESP32 boards cannot handle modern Bluetooth gamepads (Xbox, PS4, Switch Pro) natively out of the box alongside heavy framework code. We override the standard core with a custom version compiled with **Bluepad32** engine optimizations natively baked in.
* **`lib_deps`:** You never need to go hunt down `.zip` files for your libraries. PlatformIO reads this list and dynamically sandboxes the exact dependencies into a hidden `.pio` workspace folder when you first hit compile.

---

## 4. Project Structure & Code Deployment

Your project workspace must strictly follow this file hierarchy:

```text
├── .pio/                 # Auto-generated build caches and dependencies (Do not touch)
├── include/              # Header files (.h)
├── lib/                  # Private custom local libraries 
├── src/
│   └── main.cpp          # Your primary C++ firmware file
├── index.html            # The custom Glassmorphism UI dashboard
└── platformio.ini        # The project configuration file
```

### How to Compile and Upload to your ESP32:

1. **Prepare your network credentials:**
   * Open `src/main.cpp`.
   * Find the Wi-Fi constants at the top of the file and insert your exact smartphone hotspot or local router credentials:
     ```cpp
     const char* ssid = "YOUR_WIFI_SSID"; 
     const char* password = "YOUR_WIFI_PASSWORD";
     ```
2. **Connect your Hardware:**
   * Plug your ESP32 into your computer using a high-quality data-capable Micro-USB or USB-C cable. *(Note: Cheap charging-only cables will not expose the board's COM serial port).*
3. **Use the PlatformIO Task Toolbar:**
   Look at the very bottom status bar of VS Code. You will see a row of symbols:
   * **Checkmark Icon (✓):** **Build.** Compiles your C++ code to verify there are no syntax errors.
   * **Right Arrow Icon (→):** **Upload.** Compiles the code and flashes it onto your connected physical ESP32 board.
   * **Trash Can Icon:** **Clean.** Cleans old cache files if you encounter strange compile issues.
   * **Plug Icon:** **Serial Monitor.** Opens the terminal stream coming from your ESP32 so you can read `Serial.printf` logs.

4. **Flash the Code:**
   * Click the **Upload** (→) button. 
   * Once completed successfully, click the **Serial Monitor** (plug icon).
   * Set your monitor speed to `115200` baud if prompted.
   * Look closely at the terminal. It will output `Connecting to WiFi...` and finally display your **ESP32 IP Address** (e.g., `192.168.1.45`). Write this number down!

---

## 5. Connecting Your Bluetooth Gamepad

Because our custom core integrates Bluepad32, your ESP32 acts as a dedicated gamepad receiver host.

1. Turn on your Bluetooth controller (DualShock 4, Xbox One S/X, Nintendo Switch Pro, etc.).
2. Put the controller into **Pairing Mode** (e.g., hold the Share + PS buttons on a PlayStation controller until light flashes rapidly).
3. The ESP32 will automatically scan, authenticate, and connect to the controller. The controller will rumble or show a solid connection indicator when paired.
4. Check your VS Code Serial Monitor to verify that the controller state changes to connected.

---

## 6. Launching the Web Dashboard

The `index.html` file acts as a universal, high-performance UI interface. It communicates bidirectionally over high-speed raw TCP WebSockets.

### Running the Dashboard:
1. Open the `index.html` file using any web browser (Chrome, Edge, or Firefox are highly recommended).
2. Look at the **Connection Panel** at the top of the browser dashboard page.
3. In the input text field, type your ESP32's unique IP Address that you noted earlier from the Serial Monitor (e.g., `192.168.1.x`). Ensure the port suffix remains set to `:8080`.
4. Click **Connect**.

### How the Interaction Works:
* **The JSON Handshake:** As soon as the browser establishes a WebSocket connection, the ESP32 sends a pre-formatted JSON block (`CONFIG:...`). This block explicitly instructs the browser UI on how many sliders to draw, what their min/max mathematical scale limits are, and how many lines to draw on the rolling data chart.
* **Real-time Adjustments:** Moving any slider on the web browser automatically packages a `SET:<parameter_id>:<value>` string message and shoots it back to the ESP32 instantaneously over the socket. This immediately updates the global kinematics variables controlling the mechanical locomotion cycle.
* **CSV Logging:** Clicking **Start Recording** dynamically parses and saves the timestamped data stream to a local browser matrix array. Clicking **Stop Recording** instantly creates and downloads a cleanly formatted `.csv` file directly onto your hard drive for easy plotting in Python, MATLAB, or Excel.

---

## 7. Troubleshooting Common Issues

> **Error: `Serial Port not found` or `Failed to connect to ESP32` during upload.**

* Your USB cable might be power-only. Swap it for a certified data cable.
* You may need to download the USB-to-UART bridge drivers for your specific ESP32 development board variant. Look up either the **CP2102** or **CH340** driver depending on the stamp logo on your board's physical USB controller chip.
* On some ESP32 development boards, you must physically hold down the physical **BOOT** button on the PCB while PlatformIO attempts to connect during the upload process.

> **Error: Code compiles, but Web Dashboard won't connect.**

* Ensure your computer/laptop and your ESP32 are connected to the exact same Wi-Fi network or smartphone hotspot. Devices on separate subnets or guest networks cannot route WebSocket packets to each other.
* Ensure you typed the exact correct IP address visible on your VS Code Serial Monitor output.

> **Error: My Bluetooth Gamepad isn't registering inputs.**

* Press the **EN / RST** button on the side of your ESP32 chip to reboot the processor cleanly. This triggers a fresh hardware device scan cycle.
