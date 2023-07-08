#name_ru_RU KDiskMark - измеряет производительность дисков
#msg_ru_RU KDiskMark - это утилита для измерения производительности дисков. Она предоставляет простой интерфейс и позволяет проверить скорость чтения и записи на различных дисковых устройствах, таких как жесткие диски (HDD), твердотельные накопители (SSD) и USB-флешки.
#name_en_US KDiskMark - measures disk performance
#msg_en_US KDiskMark is a utility for measuring disk performance. It provides a simple interface and allows you to check the speed of reading and writing on various disk devices, such as hard drives (HDD), solid-state drives (SSD) and USB flash drives.
#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"

# Проверяем наличие установленного пакета kdiskmark
if ! yay -Qs kdiskmark &>/dev/null; then

    # Предлагаем установить kdiskmark
    read -p "${pkg} kdiskmark ${no_found}. ${instq} kdiskmark? (y/n): " choice
    if [[ $choice == "y" || $choice == "Y" ]]; then
        yay -S kdiskmark
    else
        notify-send "${error}" "${noinstall}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
        exit 1
    fi
fi

# Запускаем kdiskmark
kdiskmark
