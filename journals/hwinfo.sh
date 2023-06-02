#name_ru_RU Краткая информация о железе (hwinfo)
#msg_ru_RU 'hwinfo --short' показывает краткую информацию о железе (процессор, материнская плата, видеокарта, жесткий диск, звуковая карта и т.д). Содержит информацию о каждом обнаруженном устройстве, включая его модель, производителя, тип, состояние и используемые драйверы.
#name_en_US Brief information about Hardware (hwinfo)
#msg_en_US 'hwinfo --short' shows brief information about hardware (processor, motherboard, video card, hard disk, sound card, etc.). Contains information about each detected device, including its model, manufacturer, type, condition, and drivers used.
#!/bin/bash
# Определение языка
language="en_US"
if [ -n "$1" ]; then
    language="$1"
fi

# Загрузка файла перевода
translations_file="translations_$language.txt"
source "$HOME/kLaus/other/$translations_file"
hwinfo --short
read -p "${enter_ok}"
