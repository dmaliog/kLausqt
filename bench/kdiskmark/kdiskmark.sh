#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"
helper="$2"

# Проверяем наличие установленного пакета kdiskmark
if ! $helper -Q kdiskmark &>/dev/null; then

    # Предлагаем установить kdiskmark
    read -p "${pkg} kdiskmark ${no_found}. ${instq} kdiskmark? (y/n): " answer
    if [[ "$answer" == [yY] ]]; then
        if [ "$helper" = "yay" ]; then
            $helper -S kdiskmark
        else
            $helper -S kdiskmark --skipreview
        fi
    else
        notify-send "${install}" "${noinstall}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
        exit 1
    fi
fi

# Запускаем kdiskmark
kdiskmark
