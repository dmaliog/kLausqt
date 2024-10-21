#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"
helper="$2"

# Проверяем наличие установленного пакета iperf
if ! $helper -Q iperf &>/dev/null; then

    # Предлагаем установить iperf
    read -p "${pkg} iperf ${no_found}. ${instq} iperf? (y/n): " answer
    if [[ "$answer" == [yY] ]]; then
        if [ "$helper" = "yay" ]; then
            $helper -S iperf
        else
            $helper -S iperf --skipreview
        fi
    else
        notify-send "${install}" "${noinstall}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
        exit 1
    fi
fi

# Запускаем iperf
bash -c "iperf; bash"
