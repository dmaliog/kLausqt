#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"
helper="$2"

if ! $helper -Q blender-benchmark &>/dev/null; then

    # Предлагаем установить blender-benchmark
    read -p "${pkg} blender-benchmark ${no_found}. ${instq} blender-benchmark? (y/n): " answer
    if [[ "$answer" == [yY] ]]; then
        if [ "$helper" = "yay" ]; then
            $helper -S blender-benchmark
        else
            $helper -S blender-benchmark --skipreview
        fi
    else
        notify-send "${install}" "${noinstall}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
        exit 1
    fi
fi

# Запускаем blender-benchmark
blender-benchmark
