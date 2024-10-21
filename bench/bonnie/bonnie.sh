#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"
helper="$2"

if ! $helper -Q bonnie++ &>/dev/null; then

    # Предлагаем установить bonnie++
    read -p "${pkg} bonnie++ ${no_found}. ${instq} bonnie++? (y/n): " answer
    if [[ "$answer" == [yY] ]]; then
        if [ "$helper" = "yay" ]; then
            $helper -S bonnie++
        else
            $helper -S bonnie++ --skipreview
        fi
    else
        notify-send "${install}" "${noinstall}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
        exit 1
    fi
fi

# Запускаем bonnie++
bash -c "bonnie++; bash"
