# Crazyflie setup for Bolt boards
In this guide, you'll learn how to install the Crazyflie client, the crazyflie firmware for development, compile and flash to a vehicle.

## Prerequisites
For better compatibility and simplified compilation process, we strongly advise using Ubuntu instead of Windows. To install Ubuntu on a new machine using a USB bootable, [follow this guide](https://ubuntu.com/tutorials/install-ubuntu-desktop#1-overview).

Our setup has successfully been tested with Ubuntu 24 LTS.

Once Ubuntu is installed, Python should be available right away without any additional installation. We also recommend running a Virtual Environment for package version control. Below is a guide on how to do it.

### Install and setup a virtual environment
First, open a terminal and update the system package list:

```bash
sudo apt update
```

Then, make sure Python 3 and *pip* are installed:

```bash
sudo apt install python3 python3-pip
```

To check if Python 3 is installed:

```bash
python3 --version
```

To check if *pip* is installed:

```bash
pip3 --version
```

After that, you can install *venv*:

```bash
sudo apt install python3-venv
```

Create a new working directory in the location you wish:

```bash
mkdir name_of_your_folder && cd name_of_your_folder
```

Create the virtual environment inside (it can be accessed later from any other directory):

```bash
python3 -m venv myvenv_name
```

Activate it:

```bash
source myenv_name/bin/activate
```

After activation, you should see that the shell prompt is now preceeded by your *venv* name: *(myenv_name) user@hostname:*. Now you can activate it whenever you need by running the previous command from the folder where it is installed.

## Python client installation (GUI)
Link to the [Official bitcraze guide](https://www.bitcraze.io/documentation/repository/crazyflie-clients-python/master/installation/install/#debianubuntu).

Before you start, you need packages for *git*, *pip* and for the *Qt GUI*:

```bash
sudo apt install git python3-pip libxcb-xinerama0 libxcb-cursor0
pip3 install --upgrade pip
```

Afterwards, from the same terminal, run the commands listed in [the following guide](https://www.bitcraze.io/documentation/repository/crazyflie-lib-python/master/installation/usb_permissions/).

Now, having *pip3* installed, things become very easy. Just run:

```bash
pip3 install cfclient
```

With that, you can finally type on the terminal (remember that the terminal must have the virtual environment active):

```bash
cfclient
```

For details on how to configure the client, refer to [this guide](https://www.bitcraze.io/documentation/tutorials/getting-started-with-crazyflie-2-x/#config-client).

## Crazyflie-firmware
Link to the official [bitcraze building and flashing documentation](https://github.com/bitcraze/crazyflie-firmware/blob/master/docs/building-and-flashing/build.md).

Since we are using Ubuntu, you don't need to setup any virtual machines or development IDEs other than the packages listed below. Again, we recommend having the virtual environment active. In your terminal:

```bash
sudo apt-get install make gcc-arm-none-eabi
```

Then, you must clone the *crazyflie-firmware* repository from Github (important to add the --recursive option):

```bash
git clone --recursive https://github.com/bitcraze/crazyflie-firmware.git
```

Once you have cloned the repository, you can compile default configurations (included in *crazyflie-firmware/configs*):

```bash
make bolt_defconfig
```

After, you have to build the firmware with:

```bash
make -j$(nproc)
```

### Flashing
If the compilation and build succeed, it's time for flashing. There are multiple options, but the easiest one we have found is to automatically enter bootloader mode and flash. For that, you first need to connect the Crazyflie Bolt and the USB radio dongle. You can check the radio dongle URI from the Crazyflie Client (after running scan, it should be visible). Once you know the URI of your platform:

```bash
cfloader flash build/[BUILD_NAME].bin stm32-fw -w [CRAZYFLIE_URI]
```
This will automatically change the state of your crazyflie and begin the upload. For reference, the command for my platform build and radio URI is as follows:

```bash
cfloader flash build/bolt.bin stm32-fw -w radio://0/80/2M/E7E7E7E7E7
```

The radio module URI can be obtained from the cfclient GUI:

![cfclient radio](https://github.com/danielalaez/MARSlab-NCKU-docs/blob/main/docs/img/radio-settings.png?raw=true)


## Customizing the Crazyflie-firmware
There are multiple ways you can modify the firmware. One of the simplest, is to modify one of the existing default configs to add your desired configuration flags. The crazyflie firmware is currently structured in a way that *CONFIG_* boolean definitions define whether some parts of the code will be executed or not.

For example, if we want the Crazyflie not to require pressing the arm button every time we want to fly, we can go into *configs/bolt_defconfig* and edit it. By default, it should contain:

```
CONFIG_PLATFORM_BOLT=y

CONFIG_ESTIMATOR_AUTO_SELECT=y
CONFIG_CONTROLLER_AUTO_SELECT=y

CONFIG_MOTORS_REQUIRE_ARMING=y
```

If we comment out the last line of the file (*# CONFIG_MOTORS_REQUIRE_ARMING=y*) and press save, after repeating the compilation process the arming button should be disabled by default in the *cfclient*. Note that when compiling the code, we should see something like:

```
Build for the bolt platform!
Build 22:59fa2f6c6837 (2024.10.2 +22) MODIFIED
```

### Change the motor output protocol
A slightly more complex example is modifying the motor output protocol. The motor output is managed by what is called a driver, so the path to the motor setup is:

```
src/drivers/interface/motors.h
```

Inside that file, you will fine multiple *CONFIG_* variables that define the default behaviour of the motor driver. Currently, there are definitions for:

- OneShot125: *CONFIG_MOTORS_ESC_PROTOCOL_ONESHOT125*
- OneShot42: *CONFIG_MOTORS_ESC_PROTOCOL_ONESHOT42*
- DShot: *CONFIG_MOTORS_ESC_PROTOCOL_DSHOT*
- Standard PWM (400 Hz): *CONFIG_MOTORS_ESC_PROTOCOL_STANDARD_PWM*

Including any of the definitions above in the default config file for the Bolt platform will enable the desired signal output for all the motors.

Next, let's suppose your ESCs only support standard PWM input, but with a lower frequency than 400 Hz. Besides adding:

```
CONFIG_MOTORS_ESC_PROTOCOL_STANDARD_PWM=y
```

to the default configuration, you need to update the BLMC_PERIOD constant under the *#else* statement, which by default is set to 0.0025 s (2.5 ms = 400 Hz). Most standard PWM controllers should support 50 Hz, so you can increase the period up to:

```
#define BLMC_PERIOD 0.02   // 20 ms = 50 Hz
```

The compilation process remains the same. After applying the changes, compiling and flashing, you can measure the output signal with the oscilloscope to verify that it works as expected.

Here is our preferred default config. You can replace bolt_defconfig file content with this:

```
CONFIG_PLATFORM_BOLT=y

CONFIG_ESTIMATOR_AUTO_SELECT=y
CONFIG_CONTROLLER_AUTO_SELECT=y
#Modified for standard PWM instead of ONESHOT125
CONFIG_MOTORS_ESC_PROTOCOL_STANDARD_PWM=y
#CONFIG_MOTORS_ESC_PROTOCOL_ONESHOT42=y
CONFIG_MOTORS_REQUIRE_ARMING=y
CONFIG_MOTORS_DEFAULT_IDLE_THRUST=3500
```

## ESC Firmware changes
Since the ESCs were extracted from a different quadcopter build, an unexpected behaviour was observed during initial testing. The Tarot 6A ESCs run BLHeli (not S!), which is an outdated firmware version. Currently crazyflie-firmware does not allow BLHeli setup through the Bolt board. Therefore, an external programmer must be used. The best solution we have found is following [this tutorial](https://www.locarbftw.com/programming-a-blheli-esc-with-blheli-suite-for-use-with-autonomous-boats/). By using an Arduino UNO and reprogramming it as a 4-way interface, it can be connected through the GND and Signal wires of the ESC to digital pin 11. Note that these ESCs only run at 2S, while Arduino outputs 5V. Therefore, you'll need to connect the positive and GND cables of the ESC to the crazyflie board, and the GND (yes, you need a splitter here) and signal wires to the Arduino.

The current ESCs have already been flashed to the latest available BLHeli firmware (14.9), but if you tweak the parameters they might need reflashing. As for the parameters that have worked best for us, check the screenshot:

![BLHeli ESC setup](https://github.com/danielalaez/MARSlab-NCKU-docs/blob/main/docs/img/BLHeli.png?raw=true)

Once all the ESCs have been calibrated, you can connect a potentiometer to A3 input on the Arduino UNO, reflash it with the *arduino/PWMpotentiometer* code, open up the terminal, and check the maximum and minimum PWM values. You may use the potentiometer as a PWM calibration utility. Please be careful when spinning the motors for long periods without propellers, they may burn!

## Setup for initial flight
*Most of the settings can now be modified through the cfclient. A great explanation of the GUI can be found in [this tutorial](https://www.bitcraze.io/documentation/repository/crazyflie-clients-python/master/userguides/userguide_client/flightcontrol_tab/).*

Since the Bolt build will be noticeably heavier than a standard crazyflie, the default PID values are way too high. For reference, a similar build with their adjusted PID parameters can be found in: [https://www.bitcraze.io/2020/10/testing-crazyflie-bolt-and-1-cell-li-ion/](https://www.bitcraze.io/2020/10/testing-crazyflie-bolt-and-1-cell-li-ion/). To modify the PID values, go to the parameters tab (activate it from the top bar if it's not visible), and look for the rate and attitude PID values. As a rule of thumb, you should start lowering the values until the quad is almost non-responsive, and start increasing the values until you are satisfied. Once you are happy, reducing the values a little bit is on the safe side. Rembember that **restarting the crazyflie will cause the parameters to reset to default**. If you want to keep the PID values, **make sure to write them to the persistent memory one by one**.

Here is an example setup of the last tuned PID parameters. Note that these values are still not perfect and can cause some heavy oscillations or even crashes:
![PID setup](https://github.com/danielalaez/MARSlab-NCKU-docs/blob/main/docs/img/current_PID.png?raw=true)

Another thing we have found is that the **Normal flight mode made the motors impossible to control** from take-off. That's because the minimum throttle is set to 25% by default, which is sufficient to violently flip the drone from the ground if there is a slight imbalance. To avoid that, you should fly in advanced flight mode, and lower the maximum and minimum values. Check the attached screenshot for reference.

![cfclient setup](https://github.com/danielalaez/MARSlab-NCKU-docs/blob/main/docs/img/advanced_setup.png?raw=true)

Before your maiden flight, plug in the USB RC gamepad and create a new configuration to map all the channels. You can save that setup for future flights.

With that set, you should be ready to go! Be careful with the throttle mapping, since USB RC simulator gamepads only recognize throttle from mid-stick > up.

## Python autonomous control
The last setup on setting up the Crazyflie bolt board is to allow for autonomous control. This can be achieved simply by using Crazyflie's Python API, which can be installed on your virtual environment by running:

```
pip3 install cflib
```

After successfully executing that command, you can create any Python file to control the Crazyflie. An important note is that **brushless boards such as bolt require an idle thrust when armed** so that all the motors are spinning before any motion command is sent. **If not, motors will start spinning during take-off at different speeds causing it to crash**. To set it up, into the parameters tab of *cfclient*, look for *idleThrust* and set a minimum value that sets all the motors to spin but not too high for the drone to take-off. For reference, **a value of 3000** has worked well for us. Remember to save the parameter as a default.

An example Python code for testing is as follows:

```python
import logging
import time
import cflib.crtp
from cflib.crazyflie.syncCrazyflie import SyncCrazyflie
from cflib.positioning.motion_commander import MotionCommander
from cflib.crazyflie import Crazyflie
from cflib.crazyflie.commander import Commander

# URI for the Crazyflie
URI = 'radio://0/80/2M'

logging.basicConfig(level=logging.ERROR)

def main():
    cflib.crtp.init_drivers(enable_debug_driver=False)

    with SyncCrazyflie(URI) as scf:
        cf = scf.cf
        
        print("Starting slow motor rotation...")
        cf.platform.send_arming_request(True)
        time.sleep(2.0)

        with MotionCommander(scf) as mc:
            
            print('Taking off!')
            mc.up(0.5)
            time.sleep(3)
            mc.land()

            # Now land the drone
            print('Landing...')
        
        print("finished, disarming")
        time.sleep(1.0)
        cf.platform.send_arming_request(False)
if __name__ == '__main__':
    main()
```

You can simply create a new Python file, paste this, modify the radio URI to your current setup, and save it. To run it, in your virtual environment type:

```
python name_of_file.py
```

Calling *with MotionCommander(scf) as mc:* will automatically make the vehicle take off when entering this part of the code, and landing when exiting this part of the code. If something goes wrong (such as the drone flipping down), the supervisor state machine should stop spinning the motors.

