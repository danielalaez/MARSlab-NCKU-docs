# FAWT web interface

The FAWT web interface repository can be found (here)[https://github.com/danielalaez/webFAWT/]. You can simply download it as a .zip file and extract it in your preferred directory, or clone it.

## Installation and running steps
Although an *instructions.txt* file is provided with the same guidelines, here is a markdown version.

On an Anaconda Prompt terminal:

```
pip install flask
pip install pyserial
```

Check the COM port (if you are using windows) where the Arduino is connected (for example, COM8), and modify app.py accordingly: 

```
SERIAL_PORT = 'COM8'  # Windows - Replace with the actual port for your Arduino
```

Then navigate to the folder containing app.py:

```
python app.py
```

If everything works well, the program should output the IP address on the terminal:

```
* Running on http://127.0.0.1:5000/
```

If you get some errors of missing packages, look for the name of the package and run the appropriate "pip install ...".

Open a new browser tab and paste the ip and port: http://127.0.0.1:5000/ (or the one printed in your terminal)

Set the fan array size (6x6) and run. In the terminal you should see the messages sent to the Arduino board.

## Known issues & WIP
There is one known issue: after multiple successive commands sent to the FAWT, the terminal is known to freeze, causing the fans to not accept commands anymore.

### WIP 
An alternative control strategy is proposed: instead of sending individual control commands from the python server to the Arduino interface, and waiting for the velocity to be applied to the correct I2C servo controller, a single message containing the velocity array for all the fans may be sent from the Python server to the Arduino interface. The Arduino interface code should be modified to read the entire array and redirect the speed values to each fan. One known downside of this method is that updating a single fan velocity requires updating the entire array. This could be managed by recording the last commanded speed of each fan and only updating modified speeds, slightly increasing the code complexity.