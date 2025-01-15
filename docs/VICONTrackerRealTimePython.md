# Real-Time VICON Tracker: Python

## UDP Tracker
The UDP tracker is intended for gathering the **most recent** package sent by the VICON Tracker app, despite the server speed and any delay introduced by the code. This Python implementation is intended for any Python implementation without requiring Matlab to be installed.

Before running this script, you must have Python installed and configured in your computer (for simplicity, the computer running VICON Tracker). If you haven't installed it yet, I suggest using (Anaconda Distribution)[https://www.anaconda.com/download]. It includes Python, basic package installation tools, and even some software development utilities. Once you have downloaded and installed it, you can test the installation by running Anaconda Prompt, and executing:

```
python

```

If everything works well, you should see a Python terminal. To exit, simply type:

```
exit()
```

The PC running VICON tracker at the lab currently has Python installed in Windows, so you should be able to execute the commands above without having to install Anaconda, just by opening the normal command line in Windows.


### decode_vicon_data function
The python function (as well as the Matlab equivalent) decodes inconming VICON data and converts it into usable variables: frame number; and for each object tracked: item id, item data size, item name, trans_x, trans_y, trans_z, rot_x, rot_y, rot_z. For debugging purposes, it currently prints the variables to screen. Once this is tested, I suggest not printing out every message, as this will noticeably slow down the code.

### Instructions for setup and running

To set it up:

1. Launch VICON Tracker 3.10 app and wait for the cameras to be ready. Perform calibration if necessary.
2. Under the objects tab, select whichever object(s) you wish to share on the UDP stream. You can also create new custom objects for specific applications. 
3. Under the camera system, show advanced features. Navigate till UDP. Make sure that the UDP server is enabled (checkbox), the IP address (typically *localhost*), and the port match those set in the Simulink UDP receive block.
4. Open a windows command line, and a file explorer to browse for the folder containing *test-udp-tracker.py*. Copy the containing folder path from the file explorer and type in the terminal:

```
cd [PASTE_PATH_HERE]
```

5. Once you are in the correct working directory of the Python file, run:

```
python test-udp-tracker.py
```

The script should automatically start pulling out object detections from the UDP socket. Please note that if the selected object is currently not in sight by any of the cameras, there will be no output!.