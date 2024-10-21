#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"
helper="$2"

# Проверяем наличие установленного пакета ttcp
if ! $helper -Q ttcp &>/dev/null; then

# Предлагаем установить ttcp
    read -p "${pkg} ttcp ${no_found}. ${instq} ttcp? (y/n): " answer
    if [[ "$answer" == [yY] ]]; then
        if [ "$helper" = "yay" ]; then
            $helper -S ttcp
        else
            $helper -S ttcp --skipreview
        fi
    else
        notify-send "${install}" "${noinstall}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
        exit 1
    fi
fi

# Запускаем ttcp
bash -c "ttcp; bash"
