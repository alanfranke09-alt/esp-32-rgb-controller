# ESP32 Wi-Fi RGB LED Controller

## 1. Project Overview

This project involves designing, building, and programming a Wi-Fi-controlled RGB LED system using an ESP32 development board. The system allows a user to control the color of an RGB LED remotely through a simple web interface hosted by the ESP32.

The ESP32 acts as the main controller, handling the Wi-Fi connection, processing user commands, and controlling the individual red, green, and blue channels of the LED through its General Purpose Input/Output (GPIO) pins.

The aim of this project is to develop practical experience in electronics, embedded systems, microcontroller programming, circuit construction, and wireless communication. It also provides an opportunity to combine hardware and software to create a working interactive system.

Rather than controlling the LED directly from a physical switch, the project uses a web page to provide a more flexible way of interacting with the circuit.

## 2. Project Objectives

The main objectives of this project are:

- Build a functional electronic circuit using an ESP32 development board and an RGB LED.
- Understand how GPIO pins are used to control external electronic components.
- Use current-limiting resistors to protect the LED and control the current flowing through its individual color channels.
- Program the ESP32 using the Arduino IDE and the Arduino programming framework.
- Establish a Wi-Fi connection between the ESP32 and a device connected to the network.
- Create a simple web interface that allows the user to interact with the hardware.
- Process commands received through the web interface and translate them into changes in the LED outputs.
- Test the circuit and software, identify faults, and troubleshoot problems during development.
- Document the design, components, wiring, code, and testing process for future reference.

## 3. Components and Equipment

The following components are used in this project.

| Component | Quantity | Purpose |
|---|---:|---|
| ESP32 development board | 1 | Main microcontroller and Wi-Fi communication |
| RGB LED | 1 | Produces red, green, blue, and mixed colors |
| 220 Ω resistors | 3 | Limit current through the LED channels |
| Breadboard | 1 | Allows the circuit to be assembled without soldering |
| USB Type-C cable | 1 | Provides power and allows code to be uploaded |
| Computer | 1 | Used to write, compile, upload, and debug the code |
| Arduino IDE | Software | Used to program the ESP32 |
| Wi-Fi network | 1 | Allows the ESP32 and user device to communicate |

## 4. Hardware Design

The hardware consists of an ESP32 development board connected to an RGB LED through a breadboard. Each of the three LED color channels is connected through its own 220 Ω current-limiting resistor.

The ESP32 provides the control signals required to switch the red, green, and blue channels independently. By changing which channels are activated, the system can produce different colors.

The current-limiting resistors help prevent excessive current from flowing through the LED channels. This is important because connecting an LED directly to a GPIO output without appropriate current limiting can damage the LED or the microcontroller.

The breadboard is used to create temporary electrical connections between the components. This makes it easier to modify the circuit, test different configurations, and troubleshoot wiring problems during development.

### GPIO Connections

The current circuit uses the following ESP32 GPIO assignments:

- Red channel: GPIO 23
- Green channel: GPIO 22
- Blue channel: GPIO 21
- Common LED connection: GND

Each color channel is connected through its own resistor. The common LED connection is connected to ground.

**Important:** These connections assume a common-cathode RGB LED. If the LED is common-anode, the wiring and control logic will need to be adjusted. The GPIO assignments in the code must also match the physical wiring.

## 5. How the System Works

The project combines hardware control, embedded programming, and wireless communication.

### Step 1: Powering the ESP32

The ESP32 is powered through its USB Type-C connection. Once powered, the microcontroller starts executing the uploaded program.

### Step 2: Connecting to Wi-Fi

The program configures the ESP32's wireless networking functionality. Depending on the configuration, the board can connect to an existing Wi-Fi network or create its own wireless access point.

If connecting to an existing network, the correct network name (SSID) and password must be provided in the program.

### Step 3: Hosting the Web Interface

The ESP32 runs a lightweight web server that serves a web page to a connected device. The page provides controls that allow the user to select or change the LED's color.

The web interface communicates with the ESP32 over the local network. This means that the user can interact with the hardware through a browser instead of needing a separate physical control panel.

### Step 4: Processing User Commands

When the user selects a color on the web page, the browser sends a request to the ESP32.

The program receives the request, identifies the requested color, and updates the relevant GPIO outputs.

### Step 5: Controlling the RGB LED

The ESP32 controls the red, green, and blue channels individually. By combining these channels, the LED can produce a range of colors.

For example:

- Red: red channel activated.
- Green: green channel activated.
- Blue: blue channel activated.
- Yellow: red and green channels activated.
- Cyan: green and blue channels activated.
- Magenta: red and blue channels activated.
- White: all three channels activated, assuming the LED and circuit support the required operation.

The exact output depends on the LED type and whether the program uses digital on/off control or pulse-width modulation (PWM) to adjust brightness.

## 6. Software and Programming

The software is developed using the Arduino IDE, which is used to write and compile the program before uploading it to the ESP32.

The program is responsible for configuring the GPIO pins, establishing wireless communication, running the web server, processing incoming requests, and updating the LED outputs.

The main software functions include:

- Pin configuration: assigns GPIO pins to the red, green, and blue LED channels.
- Wi-Fi setup: configures the ESP32's wireless connection.
- Web server setup: creates an endpoint through which the browser can communicate with the board.
- Request handling: processes commands received from the web interface.
- LED control: changes the GPIO outputs to produce the requested color.
- Error handling: helps identify problems with the Wi-Fi connection, web requests, or hardware.

The program can be expanded in the future to support brightness adjustment, a wider range of colors, preset lighting modes, and more advanced user controls.

## 7. Testing and Troubleshooting

Testing is an important part of developing this project. The hardware and software must work together correctly for the web interface to control the LED.

The following checks can be used during development:

- Confirm that the ESP32 is detected by the computer and the correct serial port is selected in the Arduino IDE.
- Confirm that the correct ESP32 board is selected in the development environment.
- Test whether the program compiles and uploads successfully.
- Check that the GPIO assignments in the code match the actual wiring.
- Verify that each LED channel is connected through a resistor.
- Confirm that the LED's common connection is wired correctly for its type.
- Test the red, green, and blue channels individually before testing mixed colors.
- Confirm that the ESP32 connects to the intended Wi-Fi network or that its access point is available.
- Check that the browser can reach the web server.
- Verify that changing a control on the web page produces the expected change in the LED.

If the LED does not illuminate, possible causes include incorrect wiring, an incorrect GPIO assignment, an incompatible LED configuration, a failed upload, or a problem in the control logic.

If the ESP32 fails to upload a program, the selected board, serial port, USB connection, and boot mode should be checked.

## 8. Skills Developed

This project provides practical experience in several areas of engineering and programming.

**Electronics:** Building circuits on a breadboard, identifying LED connections, understanding current-limiting resistors, and using GPIO pins.

**Embedded systems:** Programming a microcontroller, configuring outputs, and controlling external hardware through software.

**Wireless communication:** Using Wi-Fi to connect a microcontroller to a network and enable remote interaction.

**Web development:** Understanding how a browser-based interface can communicate with an embedded device through HTTP requests.

**Testing and fault-finding:** Diagnosing upload errors, checking circuit connections, verifying pin assignments, and separating hardware problems from software problems.

**Technical documentation:** Recording component choices, explaining system operation, documenting wiring, and maintaining a public repository of the project's development.

## 9. Future Improvements

Possible improvements include:

- Adding a color picker to select a wider range of colors.
- Implementing PWM brightness control.
- Adding preset lighting modes.
- Creating a more polished and responsive web interface.
- Improving error handling and Wi-Fi reconnection.
- Adding a circuit schematic and a clear wiring diagram.
- Recording test results and documenting any changes made during development.
- Expanding the project to control additional LEDs or other low-voltage electronic components.

These improvements would build on the existing system and provide further experience in hardware design, embedded software, and user-interface development.

## 10. Project Status

**Current stage:** Development and testing.

The project is intended to demonstrate a working connection between an ESP32 microcontroller, a physical RGB LED circuit, and a browser-based control interface.

As development continues, this repository will contain the source code, hardware information, wiring documentation, testing notes, and project improvements.

## 11. Repository Contents

The repository can include the following files as the project develops:

- `README.md` — project overview, hardware details, setup instructions, and documentation.
- `esp32_rgb_controller.ino` — Arduino source code for the ESP32.
- `images/` — photographs of the assembled circuit and screenshots of the web interface.
- `docs/` — additional documentation, wiring diagrams, and test results.

## 12. Learning Outcome

The main learning outcome is understanding how hardware and software can be integrated to create a practical electronic system.

By developing this project, I aim to strengthen my understanding of microcontrollers, circuit construction, programming, wireless communication, and systematic troubleshooting. The project also serves as a starting point for more advanced ESP32 projects involving sensors, automation, and connected devices.
