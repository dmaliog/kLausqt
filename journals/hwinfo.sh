#name_ru_RU Краткая информация о железе (hwinfo)
#msg_ru_RU <b>hwinfo --short</b> — Отображает краткую информацию о железе (процессор, материнская плата, видеокарта, жесткий диск, звуковая карта и т.д). Содержит информацию о каждом обнаруженном устройстве, включая его модель, производителя, тип, состояние и используемые драйверы.

#name_en_US Brief information about Hardware (hwinfo)
#msg_en_US <b>hwinfo --short</b> shows brief information about hardware (processor, motherboard, video card, hard disk, sound card, etc.). Contains information about each detected device, including its model, manufacturer, type, condition, and drivers used.

#icon gnome-firmware

#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"

output=$(hwinfo --short)
hwinfo --short
send_to_server "$output"

echo -e "\n${enter_ok}"
read
