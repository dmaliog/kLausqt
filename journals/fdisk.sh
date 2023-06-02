#name_ru_RU Информация о дисках и разделах (fdisk)
#msg_ru_RU 'fdisk -l' используется для показа информации о всех доступных жестких дисках и их разделах на компьютере. Она позволяет узнать размеры и типы разделов, а также информацию о файловых системах, которые на них установлены.
#name_en_US Disk and Partition Information (fdisk)
#msg_en_US 'fdisk -l' is used to display information about all available hard drives and their partitions on the computer. It allows you to find out the sizes and types of partitions, as well as information about the file systems that are installed on them.
#!/bin/bash
# Определение языка
language="en_US"
if [ -n "$1" ]; then
    language="$1"
fi

# Загрузка файла перевода
translations_file="translations_$language.txt"
source "$HOME/kLaus/other/$translations_file"

sudo fdisk -l
read -p "${enter_ok}"
