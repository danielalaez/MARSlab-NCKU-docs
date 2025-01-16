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