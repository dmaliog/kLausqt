#name Information about Connected USB Devices (lsusb)
#msg 'lsusb' shows a list of all connected USB devices and detailed information about each of them, such as manufacturer ID, device model, serial number, data transfer rate and other technical information.
#!/bin/bash
lsusb
read -p "Press Enter to continue..."
