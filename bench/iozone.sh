#name_ru_RU IOzone - измеряет производительность I/O
#msg_ru_RU <b>IOzone (Input/Output Zone)</b> — Программный инструмент для измерения производительности ввода-вывода (I/O) на файловых системах и устройствах хранения данных. Он позволяет тестировать различные аспекты производительности, такие как скорость чтения/записи файлов, размер блока данных, параллельность операций и другие параметры.

#name_en_US IOzone - measures I/O performance
#msg_en_US <b>IOzone (Input/Output Zone)</b> is a software tool for measuring input/output (I/O) performance on file systems and storage devices. It allows you to test various aspects of performance, such as file read/write speed, data block size, parallelism of operations and other parameters.

#icon io.elementary.monitor

#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"
helper="$2"

if ! $helper -Q iozone &>/dev/null; then

    # Предлагаем установить iozone
    read -p "${pkg} iozone ${no_found}. ${instq} iozone? (y/n): " answer
    if [[ "$answer" == [yY] ]]; then
        if [ "$helper" = "yay" ]; then
            $helper -S iozone
        else
            $helper -S iozone --skipreview
        fi
    else
        notify-send "${error}" "${noinstall}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
        exit 1
    fi
fi

# Запускаем iozone
bash -c "iozone; bash"
