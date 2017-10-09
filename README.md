# Synchronize camera files by using PTP-IP from libgphoto
There was no automatic tool to synchronize my Sony I7RII camera using WiFi. This tool runs in background and continuously pings the cameras ip. As soon as the camera is available the camera starts the gphoto_if (simplifies the interface with libgphoto2). The following happens then:
1. Connects to requested camera ip and model
2. Retrieves all files and folder from the camera
3. Loads all files which are not allready registered in the `DATAFILE`
4. Writes the files to the `INCOMMING_FOLDER` and register them in the `DATAFILE`
5. Sends the notification (Pushjet if selected)


## Installation
The synchronization tool works on Linux (tested on Ubuntu 16.04 and 17.04). The following need to be installed:
```bash
# to compile the library
sudo apt-get install -y libgphoto2-dev libboost-python-dev cmake

# compile the pyhton - libgphoto interface library
cd build
cmake ..
make

# required Python packages and tools
sudo apt-get install -y gphoto2 python-requests
```

## Details
### Setup
First prepare your camera. This meens that you need to create a GUID with your camera. Here [https://github.com/falk0069/sony-pm-alt] is a repostiory explaining all that.
After that you need to verify that `gphoto2 --port ptpip:<cameras ip address> --summary` is working.
Now you are ready to use the SyncCamera.py!

### SyncCamera.py --help
```bash
usage: SyncCamera.py [-h] [-i INCOMMING_FOLDER] [--datafile DATAFILE] [-c]
                     [--ip IP] [--model MODEL]

Waits till defined camera is available and copy all new images to defined
foler.

optional arguments:
  -h, --help            show this help message and exit
  -i INCOMMING_FOLDER, --incomming_folder INCOMMING_FOLDER
                        directory where received images are placed (default:
			incomming_folder)
  --datafile DATAFILE   file containing all allready copied images (default:
			copied_images.txt)
  -c, --create_datafile
			Creates a new empty datafile (default: False)

CameraConfiguration:
  Arguments affecting camera.

  --ip IP               Cameras IP address (default: 192.168.1.120)
  --model MODEL         Camera model (default: Sony PTP)
```

### Register service as a service
This allows the SyncCamera.py to run on system startup and automatically syncing when the camera is connected.
1. Copy the systemd service configuration (helper_scrips/syncPTPCamera.service) to /etc/systemd/system/.
2. Reload systemd to read the new configuration file `sudo systemctl daemon-reload`. 
3. Configure the ip, model and output path in the `runSyncCamera.sh` script.
4. Configure the path to `runSyncCamera.sh` script in the `/etc/systemd/system/syncPTPCamera.service`.
5. Enable service `sudo systemctl enable syncPTPCamera.service`


### Debuging
There are two different parts which can be debugged. The SyncCamera (python) and the gphoto_if (c++) together with libgphoto2 (c). Both can be activated in SyncCamera.py
```python
GPhotoIf_debug = True
SyncCamera_debug = logging.DEBUG
```

### Notification on Smartphone
You can get notification on Smartphone by using Pushjet App.
1. Go to http://pushjet.io and register a new Service.
2. Add the Service Secret to the modules/pushjet_notification.py file.
3. Replace `self._notification = NotificationInterface()` with `self._notification = PushjetNotification()`
4. Install python-requests: `sudo apt-get install -y python-requests`
5. Use the Public id to subscribe to the service on smartphone.

