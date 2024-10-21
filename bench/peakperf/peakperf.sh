#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"
helper="$2"

if ! $helper -Q peakperf-git &>/dev/null; then

    # Предлагаем установить peakperf-git
    read -p "${pkg} peakperf-git ${no_found}. ${instq} peakperf-git? (y/n): " answer
    if [[ "$answer" == [yY] ]]; then
        if [ "$helper" = "yay" ]; then
            $helper -S peakperf-git
        else
            $helper -S peakperf-git --skipreview
        fi
    else
        notify-send "${install}" "${noinstall}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
        exit 1
    fi
fi

# Запускаем peakperf-git
bash -c "peakperf; bash"
