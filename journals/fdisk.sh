#name_ru_RU Информация о дисках и разделах (fdisk)
#msg_ru_RU <b>fdisk -l</b> — Отображает информацию о всех доступных жестких дисках и их разделах на компьютере. Она позволяет узнать размеры и типы разделов, а также информацию о файловых системах, которые на них установлены.

#name_en_US Disk and Partition Information (fdisk)
#msg_en_US <b>fdisk -l</b> is used to display information about all available hard drives and their partitions on the computer. It allows you to find out the sizes and types of partitions, as well as information about the file systems that are installed on them.

#icon disk-usage-analyzer

#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"

output=$(sudo fdisk -l)
sudo fdisk -l
send_to_server "$output"

echo -e "\n${enter_ok}"
read
