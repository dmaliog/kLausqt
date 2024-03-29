#name_ru_RU Информация о дисках и разделах (lsblk)
#msg_ru_RU <b>lsblk -f -o+SIZE</b> — Отображает информацию о блочных устройствах, таких как жесткие диски, флешки, CD/DVD-приводы и т.д. Она позволяет узнать список устройств, их файловые системы и размеры.

#name_en_US Disk and Partition Information (lsblk)
#msg_en_US <b>lsblk -f -o+SIZE</b> shows information about block devices such as hard drives, flash drives, CD/DVD drives, etc. It allows you to find out the list of devices, their file systems and sizes.

#icon disk-usage-analyzer

#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"

output=$(lsblk -f -o+SIZE)
lsblk -f -o+SIZE
send_to_server "$output"

echo -e "\n${enter_ok}"
read
