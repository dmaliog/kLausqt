#name_ru_RU bonnie++ - измеряет производительность ФС
#msg_ru_RU Bonnie++ - это программное обеспечение для тестирования производительности файловой системы. Оно предоставляет набор инструментов для измерения скорости чтения и записи на диске, производительности операций с файлами, кеша файловой системы и других параметров, связанных с хранением данных на диске. ВАЖНО: По умолчанию bonnie++ записывает на диск как минимум два объёма оперативной памяти. Если вы не хотите тратить ресурс своего SSD, используйте нестандартные опции.

#name_en_US bonnie++ - measures FS performance
#msg_en_US Bonnie++ is a file system performance testing software. It provides a set of tools for measuring the speed of reading and writing on disk, the performance of file operations, the cache of the file system and other parameters related to data storage on disk. Important: By default, bonnie++ writes at least two volumes of RAM to disk. If you do not want to waste the resource of your SSD, use non-standard options.

#icon 49

#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"
helper="$2"

if ! $helper -Q bonnie++ &>/dev/null; then

    # Предлагаем установить bonnie++
    read -p "${pkg} bonnie++ ${no_found}. ${instq} bonnie++? (y/n): " answer
    if [[ "$answer" == [yY] ]]; then
        if [ "$helper" = "yay" ]; then
            $helper -S bonnie++
        else
            $helper -S bonnie++ --skipreview
        fi
    else
        notify-send "${error}" "${noinstall}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
        exit 1
    fi
fi

# Запускаем bonnie++
bash -c "bonnie++; bash"
