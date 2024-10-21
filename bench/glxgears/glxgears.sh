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
        notify-send "${install}" "${noinstall}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
        exit 1
    fi
fi

# Запускаем glxgears
glxgears
