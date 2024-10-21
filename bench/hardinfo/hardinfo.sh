#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"
helper="$2"

if ! $helper -Q hardinfo-git &>/dev/null; then

    # Предлагаем установить hardinfo-git
    read -p "${pkg} hardinfo-git ${no_found}. ${instq} hardinfo-git? (y/n): " answer
    if [[ "$answer" == [yY] ]]; then
        if [ "$helper" = "yay" ]; then
            $helper -S hardinfo-git
        else
            $helper -S hardinfo-git --skipreview
        fi
    else
        notify-send "${install}" "${noinstall}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
        exit 1
    fi
fi

# Запускаем hardinfo-git
bash -c "hardinfo; bash"
