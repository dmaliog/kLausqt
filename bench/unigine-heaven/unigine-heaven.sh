#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"
helper="$2"

if ! $helper -Q unigine-heaven &>/dev/null; then

    # Предлагаем установить unigine-heaven
    read -p "${pkg} unigine-heaven ${no_found}. ${instq} unigine-heaven? (y/n): " answer
    if [[ "$answer" == [yY] ]]; then
        if [ "$helper" = "yay" ]; then
            $helper -S unigine-heaven
        else
            $helper -S unigine-heaven --skipreview
        fi
    else
        notify-send "${install}" "${noinstall}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
        exit 1
    fi
fi

# Запускаем unigine-heaven
unigine-heaven
