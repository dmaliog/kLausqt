#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"
helper="$2"

# Проверяем наличие установленного пакета interbench
if ! $helper -Q interbench &>/dev/null; then

    # Предлагаем установить interbench
    read -p "${pkg} interbench ${no_found}. ${instq} interbench? (y/n): " answer
    if [[ "$answer" == [yY] ]]; then
        if [ "$helper" = "yay" ]; then
            $helper -S interbench
        else
            $helper -S interbench --skipreview
        fi
    else
        notify-send "${install}" "${noinstall}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
        exit 1
    fi
fi

# Запускаем interbench
bash -c "interbench; bash"
