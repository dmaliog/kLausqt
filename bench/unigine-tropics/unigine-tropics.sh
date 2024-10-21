#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"
helper="$2"

if ! $helper -Q unigine-tropics &>/dev/null; then

    # Предлагаем установить unigine-tropics
    read -p "${pkg} unigine-tropics ${no_found}. ${instq} unigine-tropics? (y/n): " answer
    if [[ "$answer" == [yY] ]]; then
        if [ "$helper" = "yay" ]; then
            $helper -S unigine-tropics
        else
            $helper -S unigine-tropics --skipreview
        fi
    else
        notify-send "${install}" "${noinstall}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
        exit 1
    fi
fi

# Запускаем unigine-tropics
unigine-tropics
