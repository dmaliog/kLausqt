#name Information about Connected PCI Devices (lspci)
#msg 'lspci -vnn' shows information about devices connected to the PCI bus. About the manufacturer, model and revision of the device, as well as which driver is used for this device.
#!/bin/bash
lspci -vnn
read -p "Press Enter to continue..."
