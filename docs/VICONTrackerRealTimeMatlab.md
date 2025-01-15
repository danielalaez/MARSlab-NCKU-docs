# Real-Time VICON Tracker: Matlab

There are three separate projects contained within this Matlab Simulink project:

1. udp_tracker.slx: basic implementation of a UDP receiver with a *decode_vicon_data* function to output real-time data.
2. gamepadcontroller.slx: UDP receiver of VICON real-time data + interface for connecting a USB game controller, mapping the channels, scaling them to the desired ranges and output an array to the COM3 port (USB for Arduino)
3. testcontroller.slx: UDP receiver of VICON real-time data + very basic PID controller for stabilizing the flapping wing in front of the FAWT.

## 1. UDP Tracker
The UDP tracker is intended for gathering the **most recent** package sent by the VICON Tracker app, despite the server speed and any delay introduced by the code.

### decode_vicon_data function
The matlab block function (as well as the Python equivalent) decodes inconming VICON data and converts it into usable variables: frame number; and for each object tracked: item id, item data size, item name, trans_x, trans_y, trans_z, rot_x, rot_y, rot_z.

### Instructions for setup and running

To set it up:

1. Launch VICON Tracker 3.10 app and wait for the cameras to be ready. Perform calibration if necessary.
2. Under the objects tab, select whichever object(s) you wish to share on the UDP stream. You can also create new custom objects for specific applications. 
3. Under the camera system, show advanced features. Navigate till UDP. Make sure that the UDP server is enabled (checkbox), the IP address (typically *localhost*), and the port match those set in the Simulink UDP receive block.
4. Load the Simulink udp_tracker.slx file. There should be a scope connected to visualize positions in real-time. Open that scope and press play to watch it in action. You can modify the simulation time to increase or decrease it.

![matlabudpblocks](https://github.com/danielalaez/MARSlab-NCKU-docs/blob/main/docs/img/matlab-udp-blocks.png?raw=true)

## Connect the Arduino interface with the NX10 radio transmitter
**Please: read this section carefully before running any tests!
For any communication to be established between the PC running Simulink and the Flapping wing, we have created an Arduino interface that connects with the NX10 radio to send the commands via radio to the flapping wing onboard receiver. 

The Arduino must first be connected via USB to the PC running simulink, and the **Ornithopter > ppm_matlab_interface.ino program must be uploaded**. Also, the script is configured to **output ppm channels on pin 3**. However, Arduino by default outputs 5V on digital outputs, so **a voltage divider must be included!**. For simplicity, we are running two 1K ohm resistors to the GND pin, and one 1K ohm resistor to pin 3. The signal wire can then be picked between both resistors, and the GND wire can be connected to the same GND terminal before the resistors. Please watch the photos below carefully for an adequate connection.

![connection1](https://github.com/danielalaez/MARSlab-NCKU-docs/blob/main/docs/img/connections_1.jpg)

![connection2](https://github.com/danielalaez/MARSlab-NCKU-docs/blob/main/docs/img/connections_2.jpg)

As you can see from the photo, signal and GND wires are connected to the back serial port of the NX10 radio transmitter, on pins 1 and 2 starting from the bottom left. Oscilloscope probes can be connected (as shown in the photograph) to test the voltage output. Please carefully measure the signal voltage to test the voltage divider before connecting the radio transmitter.

## 2. Gamepad controller
This file is currently setup for use with the USB 0908A 8 ch. RC Simulator as a gamepad, and the Spektrum NX10 as a radio. This requires an Arduino UNO (or equivalent) to interface between the PC and the NX10 transmitter. 

![gamepadcontroller](https://raw.githubusercontent.com/danielalaez/MARSlab-NCKU-docs/refs/heads/main/docs/img/gamepadcontroller.png)




## 3. testcontroller (Flapping wing FAWT PID control setup)

![testcontroller](https://github.com/danielalaez/MARSlab-NCKU-docs/blob/main/docs/img/testcontroller.png?raw=true)