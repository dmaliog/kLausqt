#name_ru_RU intel-gpu-tools - диагностирует встроенный GPU Intel
#msg_ru_RU intel-gpu-tools - это набор утилит и инструментов для работы с графическими процессорами Intel. Он предоставляет различные команды и функции, которые позволяют получить информацию о графическом аппарате Intel, мониторить его состояние, отлаживать и настраивать его работу.

#name_en_US intel-gpu-tools - diagnoses Intel's embedded GPU
#msg_en_US intel-gpu-tools is a set of utilities and tools for working with Intel GPUs. It provides various commands and functions that allow you to get information about the Intel graphics hardware, monitor its status, debug and configure its operation.

#icon 21

#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"

if ! yay -Q intel-gpu-tools &>/dev/null; then

    # Предлагаем установить intel-gpu-tools
    read -p "${pkg} intel-gpu-tools ${no_found}. ${instq} intel-gpu-tools? (y/n): " choice
    if [[ $choice == "y" || $choice == "Y" ]]; then
        yay -S intel-gpu-tools
    else
        notify-send "${error}" "${noinstall}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
        exit 1
    fi
fi

# Запускаем intel-gpu-tools
intel-gpu-tools
