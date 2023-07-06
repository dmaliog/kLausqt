#name_ru_RU Информация о дисках и разделах (lsblk)
#msg_ru_RU 'lsblk -f -o+SIZE' показывает информацию о блочных устройствах, таких как жесткие диски, флешки, CD/DVD-приводы и т.д. Она позволяет узнать список устройств, их файловые системы и размеры.
#name_en_US Disk and Partition Information (lsblk)
#msg_en_US 'lsblk -f -o+SIZE' shows information about block devices such as hard drives, flash drives, CD/DVD drives, etc. It allows you to find out the list of devices, their file systems and sizes.
#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"

output=$(lsblk -f -o+SIZE)
lsblk -f -o+SIZE
send_to_server "$output"

read -p "${enter_ok}"
