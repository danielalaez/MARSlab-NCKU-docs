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
**Please: read this section carefully before running any tests!**

For any communication to be established between the PC running Simulink and the Flapping wing, we have created an Arduino interface that connects with the NX10 radio to send the commands via radio to the flapping wing onboard receiver. 

The Arduino must first be connected via USB to the PC running simulink, and the **Ornithopter > ppm_matlab_interface.ino program must be uploaded**. Also, the script is configured to **output ppm channels on pin 3**. However, Arduino by default outputs 5V on digital outputs, so **a voltage divider must be included!**. The transmitter serial port requires 3.3V input, maximum. For simplicity, we are running two 1K ohm resistors to the GND pin, and one 1K ohm resistor to pin 3. The signal wire can then be picked between both resistors, and the GND wire can be connected to the same GND terminal before the resistors. Please watch the photos below carefully for an adequate connection.

![connection1](https://github.com/danielalaez/MARSlab-NCKU-docs/blob/main/docs/img/connections_1.jpg)

![connection2](https://github.com/danielalaez/MARSlab-NCKU-docs/blob/main/docs/img/connections_2.jpg)

As you can see from the photo, signal and GND wires are connected to the back serial port of the NX10 radio transmitter, on pins 1 and 2 starting from the bottom left. Oscilloscope probes can be connected (as shown in the photograph) to test the voltage output. Please carefully measure the signal voltage to test the voltage divider before connecting the radio transmitter.

Lastly, you need to setup the radio transmitter as a **Pilot link instructor** for the serial port ppm signal to be forwarded to the radio receiver onboard the aircraft. For that, go into settings, set up the pilot link instructor, and then go back and activate the trainer mode as an instructor. For further instructions, refer to the [official Spektrum NX10 manual](https://spektrumrc.com/ProdInfo/Files/SPMR10100-Manual-EN.pdf).

## 2. Gamepad controller
*Note: This requires an Arduino UNO (or equivalent) to interface between the PC and the NX10 transmitter. Refer to the previous section for detailed instructions.*

This file is currently setup for use with the USB 0908A 8 ch. RC Simulator as a gamepad, and the Spektrum NX10 as a radio. The USB gamepad controller must be connected to the PC for external radio input. If a different radio transmitter is used, a new channel mapping must be performed in the Simulink project. *USB-gamepad-mapping-matlab.txt* contains an overview of the mapping obtained for the selected gamepad controller. You can modify the channel numbers by altering the order in the signal multiplexer block after the joystick. The maximum and minimum values can be edited from the scale_[yaw, pitch, roll, throttle] functions. The output values are arranged in a single array and sent to the selected COM port. Please make sure to update the COM port of the connected Arduino to the port detected by your computer (Device manager).

![gamepadcontroller](https://raw.githubusercontent.com/danielalaez/MARSlab-NCKU-docs/refs/heads/main/docs/img/gamepadcontroller.png)

You can adjust the simulation time to run longer or shorter tests.

Why bother having two separate controllers: one USB gamepad and one radio transmitter forwarding the channels? This has proven to be the simplest solution to be able to record real-time VICON Tracker data along with user inputs, for further data analysis or even dynamic model estimation.


## 3. testcontroller (Flapping wing FAWT PID control setup)
*Note: This requires an Arduino UNO (or equivalent) to interface between the PC and the NX10 transmitter. Refer to the previous section for detailed instructions.*

The idea of this project is to replace the gamepad controller (or in other words, the pilot) by a very basic PID controller. This is designed to place the flapping wing in front of a FAWT, so that the incoming airspeed allows the flapping wing to fly stationary. For that, a outer-inner loop position and atittude basic flight controller has been implemented in Simulink, following the philosophy detailed [in this video](https://youtu.be/GK1t8YIvGM8?si=XXKP-D1GhhyG8yT8).

![testcontroller](https://github.com/danielalaez/MARSlab-NCKU-docs/blob/main/docs/img/testcontroller.png?raw=true)

**Important note:** this PID setup remains experimental and has not yet been tested nor adjusted to the actual flapping wing. 