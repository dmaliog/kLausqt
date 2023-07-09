#name_ru_RU glxgears - измеряет производительность графики
#msg_ru_RU glxgears - это простая 3D графическая демонстрация, которая часто используется в качестве теста производительности для графических систем, работающих на основе X Window System.

#name_en_US glxgears - measures graphics performance
#msg_en_US glxgears is a simple 3D graphics demo that is often used as a performance test for graphics systems running on the X Window System.

#icon 21

#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"

if ! yay -Qs mesa-utils &>/dev/null; then

    # Предлагаем установить mesa-utils
    read -p "${pkg} mesa-utils ${no_found}. ${instq} mesa-utils? (y/n): " choice
    if [[ $choice == "y" || $choice == "Y" ]]; then
        yay -S mesa-utils
    else
        notify-send "${error}" "${noinstall}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
        exit 1
    fi
fi

# Запускаем glxgears
glxgears
