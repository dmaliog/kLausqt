#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"
helper="$2"

if ! $helper -Q unigine-sanctuary &>/dev/null; then

    # Предлагаем установить unigine-sanctuary
    read -p "${pkg} unigine-sanctuary ${no_found}. ${instq} unigine-sanctuary? (y/n): " answer
    if [[ "$answer" == [yY] ]]; then
        if [ "$helper" = "yay" ]; then
            $helper -S unigine-sanctuary
        else
            $helper -S unigine-sanctuary --skipreview
        fi
    else
        notify-send "${install}" "${noinstall}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
        exit 1
    fi
fi

# Запускаем unigine-sanctuary
unigine-sanctuary
