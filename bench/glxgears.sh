#name_ru_RU glxgears - измеряет производительность графики
#msg_ru_RU <b>glxgears</b> — Простая 3D графическая демонстрация, которая часто используется в качестве теста производительности для графических систем, работающих на основе X Window System.

#name_en_US glxgears - measures graphics performance
#msg_en_US <b>glxgears</b> is a simple 3D graphics demo that is often used as a performance test for graphics systems running on the X Window System.

#icon multimedia-photo-viewer

#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"
helper="$2"

if ! $helper -Q mesa-utils &>/dev/null; then

    # Предлагаем установить mesa-utils
    read -p "${pkg} mesa-utils ${no_found}. ${instq} mesa-utils? (y/n): " answer
    if [[ "$answer" == [yY] ]]; then
        if [ "$helper" = "yay" ]; then
            $helper -S mesa-utils
        else
            $helper -S mesa-utils --skipreview
        fi
    else
        notify-send "${error}" "${noinstall}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
        exit 1
    fi
fi

# Запускаем glxgears
glxgears
