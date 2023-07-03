#name_ru_RU Информация о подключенных USB устройствах (lsusb)
#msg_ru_RU 'lsusb' показывает список всех подключенных USB-устройств и подробную информацию о каждом из них, такую как идентификатор производителя, модель устройства, серийный номер, скорость передачи данных и другую техническую информацию.
#name_en_US Information about Connected USB Devices (lsusb)
#msg_en_US 'lsusb' shows a list of all connected USB devices and detailed information about each of them, such as manufacturer ID, device model, serial number, data transfer rate and other technical information.
#!/bin/bash
# Определение языка
language="en_US"
if [ -n "$1" ]; then
    language="$1"
fi

# Загрузка файла перевода
translations_file="translations_$language.txt"
source "$HOME/.config/kLaus/other/$translations_file"
lsusb
read -p "${enter_ok}"
