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