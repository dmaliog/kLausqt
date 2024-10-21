#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"
helper="$2"

# Проверяем наличие установленного пакета unixbench
if ! $helper -Q unixbench &>/dev/null; then

    # Предлагаем установить unixbench
    read -p "${pkg} unixbench ${no_found}. ${instq} unixbench? (y/n): " answer
    if [[ "$answer" == [yY] ]]; then
        if [ "$helper" = "yay" ]; then
            $helper -S unixbench
        else
            $helper -S unixbench --skipreview
        fi
    else
        notify-send "${install}" "${noinstall}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
        exit 1
    fi
fi

# Запускаем unixbench
bash -c "ubench; bash"
