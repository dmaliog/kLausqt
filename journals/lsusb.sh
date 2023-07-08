#name_ru_RU Информация о подключенных USB устройствах (lsusb)
#msg_ru_RU 'lsusb' показывает список всех подключенных USB-устройств и подробную информацию о каждом из них, такую как идентификатор производителя, модель устройства, серийный номер, скорость передачи данных и другую техническую информацию.
#name_en_US Information about Connected USB Devices (lsusb)
#msg_en_US 'lsusb' shows a list of all connected USB devices and detailed information about each of them, such as manufacturer ID, device model, serial number, data transfer rate and other technical information.
#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"

output=$(lsusb)
lsusb
send_to_server "$output"

echo -e "\n${enter_ok}"
read
