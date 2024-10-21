#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"
helper="$2"

if ! $helper -Q intel-gpu-tools &>/dev/null; then

    # Предлагаем установить intel-gpu-tools
    read -p "${pkg} intel-gpu-tools ${no_found}. ${instq} intel-gpu-tools? (y/n): " answer
    if [[ "$answer" == [yY] ]]; then
        if [ "$helper" = "yay" ]; then
            $helper -S intel-gpu-tools
        else
            $helper -S intel-gpu-tools --skipreview
        fi
    else
        notify-send "${install}" "${noinstall}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
        exit 1
    fi
fi

# Запускаем intel-gpu-tools
intel-gpu-tools
