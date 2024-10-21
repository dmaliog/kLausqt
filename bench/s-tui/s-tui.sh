#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"
helper="$2"

if ! $helper -Q s-tui &>/dev/null; then

    # Предлагаем установить s-tui
    read -p "${pkg} s-tui ${no_found}. ${instq} s-tui? (y/n): " answer
    if [[ "$answer" == [yY] ]]; then
        if [ "$helper" = "yay" ]; then
            $helper -S s-tui
        else
            $helper -S s-tui --skipreview
        fi
    else
        notify-send "${install}" "${noinstall}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
        exit 1
    fi
fi

# Запускаем s-tui
bash -c "s-tui; bash"
