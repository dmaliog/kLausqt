#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"

# Ввод размера файла
echo "${file_size}"
read file_size

# Создание временного файла для записи
echo -e "\n${new_tmp}"

temp_file="/tmp/klausdd"
dd if=/dev/zero of="${temp_file}" bs=1M count="${file_size}" conv=fdatasync,notrunc status=progress

# Скорость устройства
echo -e "\n${time_dev}"
echo 3 > sudo tee /proc/sys/vm/drop_caches
dd if="${temp_file}" of=/dev/null bs=1M count=1024 status=progress

# Скорость работы кэша
echo -e "\n${time_cache}"
dd if="${temp_file}" of=/dev/null bs=1M count="$file_size" status=progress

# Удаление временного файла
echo -e "\n${tmp_file}"
rm "${temp_file}"

notify-send "${klaus}" "${test_ok}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000

echo -e "\n${enter_ok}"
read
