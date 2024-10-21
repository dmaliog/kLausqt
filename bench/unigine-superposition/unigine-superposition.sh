#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"
helper="$2"

if ! $helper -Q unigine-superposition &>/dev/null; then

    # Предлагаем установить unigine-superposition
    read -p "${pkg} unigine-superposition ${no_found}. ${instq} unigine-superposition? (y/n): " answer
    if [[ "$answer" == [yY] ]]; then
        if [ "$helper" = "yay" ]; then
            $helper -S unigine-superposition
        else
            $helper -S unigine-superposition --skipreview
        fi
    else
        notify-send "${install}" "${noinstall}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
        exit 1
    fi
fi

# Запускаем unigine-superposition
unigine-superposition
