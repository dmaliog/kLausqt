#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"
helper="$2"

if ! $helper -Q glmark2 &>/dev/null; then

    # Предлагаем установить glmark2
    read -p "${pkg} glmark2 ${no_found}. ${instq} glmark2? (y/n): " answer
    if [[ "$answer" == [yY] ]]; then
        if [ "$helper" = "yay" ]; then
            $helper -S glmark2
        else
            $helper -S glmark2 --skipreview
        fi
    else
        notify-send "${install}" "${noinstall}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
        exit 1
    fi
fi

# Запускаем glmark2
glmark2
