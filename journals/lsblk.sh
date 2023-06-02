#name_ru_RU Информация о дисках и разделах (lsblk)
#msg_ru_RU 'lsblk -f -o+SIZE' показывает информацию о блочных устройствах, таких как жесткие диски, флешки, CD/DVD-приводы и т.д. Она позволяет узнать список устройств, их файловые системы и размеры.
#name_en_US Disk and Partition Information (lsblk)
#msg_en_US 'lsblk -f -o+SIZE' shows information about block devices such as hard drives, flash drives, CD/DVD drives, etc. It allows you to find out the list of devices, their file systems and sizes.
#!/bin/bash
# Определение языка
language="en_US"
if [ -n "$1" ]; then
    language="$1"
fi

# Загрузка файла перевода
translations_file="translations_$language.txt"
source "$HOME/kLaus/other/$translations_file"
lsblk -f -o+SIZE
read -p "${enter_ok}"
