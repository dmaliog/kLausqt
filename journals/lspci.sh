#name_ru_RU Информация о подключенных PCI устройствах (lspci)
#msg_ru_RU 'lspci -vnn' показывает информацию о устройствах, подключенных к шине PCI. О производителе, модели и ревизии устройства, а также о том, какой драйвер используется для этого устройства.
#name_en_US Information about Connected PCI Devices (lspci)
#msg_en_US 'lspci -vnn' shows information about devices connected to the PCI bus. About the manufacturer, model and revision of the device, as well as which driver is used for this device.
#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"

output=$(lspci -vnn)
lspci -vnn
send_to_server "$output"

read -p "${enter_ok}"
