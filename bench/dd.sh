#name_ru_RU dd - измеряет скорость чтения и записи
#msg_ru_RU dd измеряет скорость чтения и скорости записи. Этот метод зависит от выравнивания разделов! Другими словами, если разделы выровнены неправильно, это скажется на результатах тестирования, поскольку оно выполняется в примонтированной файловой системе.

#name_en_US dd - measures the speed of reading and writing
#msg_en_US dd measures read speed and write speed. This method depends on the alignment of the partitions! In other words, if the partitions are aligned incorrectly, this will affect the results of testing, since it is performed in a mounted file system.

#icon 53

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
