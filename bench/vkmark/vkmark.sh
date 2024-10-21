#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"
helper="$2"

if ! $helper -Q vkmark &>/dev/null; then

    # Предлагаем установить vkmark
    read -p "${pkg} vkmark ${no_found}. ${instq} vkmark? (y/n): " answer
    if [[ "$answer" == [yY] ]]; then
        if [ "$helper" = "yay" ]; then
            $helper -S vkmark
        else
            $helper -S vkmark --skipreview
        fi
    else
        notify-send "${install}" "${noinstall}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
        exit 1
    fi
fi

# Запускаем vkmark
vkmark
