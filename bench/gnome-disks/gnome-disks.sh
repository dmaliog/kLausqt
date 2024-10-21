#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"
helper="$2"

# Проверяем наличие установленного пакета gnome-disk-utility
if ! $helper -Q gnome-disk-utility &>/dev/null; then

    # Предлагаем установить gnome-disk-utility
    read -p "${pkg} gnome-disk-utility ${no_found}. ${instq} gnome-disk-utility? (y/n): " answer
    if [[ "$answer" == [yY] ]]; then
        if [ "$helper" = "yay" ]; then
            $helper -S gnome-disk-utility
        else
            $helper -S gnome-disk-utility --skipreview
        fi
    else
        notify-send "${install}" "${noinstall}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
        exit 1
    fi
fi

# Запускаем gnome-disk-utility
gnome-disks
