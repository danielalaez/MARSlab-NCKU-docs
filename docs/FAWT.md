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