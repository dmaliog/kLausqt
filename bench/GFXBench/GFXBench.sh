#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"
helper="$2"

if ! $helper -Q gfxbench &>/dev/null; then

    # Предлагаем установить gfxbench
    read -p "${pkg} gfxbench ${no_found}. ${instq} gfxbench? (y/n): " answer
    if [[ "$answer" == [yY] ]]; then
        if [ "$helper" = "yay" ]; then
            $helper -S gfxbench
        else
            $helper -S gfxbench --skipreview
        fi
    else
        notify-send "${install}" "${noinstall}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
        exit 1
    fi
fi

# Запускаем gfxbench
gfxbench
