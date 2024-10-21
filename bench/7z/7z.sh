#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"
helper="$2"

if ! $helper -Q p7zip &>/dev/null; then

    # Предлагаем установить p7zip
    read -p "${pkg} p7zip ${no_found}. ${instq} p7zip? (y/n): " answer
    if [[ "$answer" == [yY] ]]; then
        if [ "$helper" = "yay" ]; then
            $helper -S p7zip
        else
            $helper -S p7zip --skipreview
        fi
    else
        notify-send "${install}" "${noinstall}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
        exit 1
    fi
fi

# Запускаем p7zip
bash -c "7z b; bash"
