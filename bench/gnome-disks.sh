#name_ru_RU GNOME Disk Utility - измеряет производительность дисков
#msg_ru_RU GNOME Disk Utility предоставляет информацию о производительности дисков. Она позволяет измерять скорость чтения и записи на диске, а также оценивать производительность дисковой системы. Вы можете использовать GNOME Disk Utility для проверки производительности своих дисков или сравнения различных дисковых устройств. диск, сеть и другие.

#name_en_US GNOME Disk Utility - measures disk performance
#msg_en_US GNOME Disk Utility provides information about disk performance. It allows you to measure the speed of reading and writing on the disk, as well as to evaluate the performance of the disk system. You can use GNOME Disk Utility to check the performance of your disks or compare different disk devices. disk, network, and others.

#icon 29

#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"

# Проверяем наличие установленного пакета gnome-disk-utility
if ! yay -Qs gnome-disk-utility &>/dev/null; then

    # Предлагаем установить gnome-disk-utility
    read -p "${pkg} gnome-disk-utility ${no_found}. ${instq} gnome-disk-utility? (y/n): " choice
    if [[ $choice == "y" || $choice == "Y" ]]; then
        yay -S gnome-disk-utility
    else
        notify-send "${error}" "${noinstall}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
        exit 1
    fi
fi

# Запускаем gnome-disk-utility
gnome-disks
