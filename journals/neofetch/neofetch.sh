#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"
helper="$2"

# Проверка наличия установленного neofetch
if ! $helper -Q neofetch &>/dev/null; then
    read -p "${pkg} neofetch ${no_found}. ${instq} neofetch? (y/n): " answer
    if [[ "$answer" == [yY] ]]; then
        if [ "$helper" = "yay" ]; then
            $helper -S neofetch
        else
            $helper -S neofetch --skipreview
        fi
    else
        notify-send "${install} neofetch" "${noinstall}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
        exit 0
    fi
fi

neofetch

echo -e "\n${enter_ok}"
read
