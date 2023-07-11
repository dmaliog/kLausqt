#name_ru_RU TTCP - измеряет пропускную способность сетевого соединения
#msg_ru_RU TTCP (Test TCP) - это утилита для измерения пропускной способности сетевого соединения по протоколу TCP. Она предназначена для тестирования и оценки производительности сетевых соединений путем передачи данных между двумя узлами. Утилита должна быть установлена на обоих узлах, между которыми необходимо определить пропускную способность.

#name_en_US TTCP - measures the bandwidth of a network connection
#msg_en_US TTCP (Test TCP) is a solution for measuring the bandwidth of a network connection over the TCP protocol. It is designed to test and evaluate the performance of network connections by transferring data between two nodes. The utility must be installed on both nodes, between which the bandwidth must be determined.

#icon 53

#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"

# Проверяем наличие установленного пакета ttcp
if ! yay -Q ttcp &>/dev/null; then

# Предлагаем установить ttcp
    read -p "${pkg} ttcp ${no_found}. ${instq} ttcp? (y/n): " choice
    if [[ $choice == "y" || $choice == "Y" ]]; then
        yay -S ttcp
    else
        notify-send "${error}" "${noinstall}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
        exit 1
    fi
fi

# Запускаем ttcp
bash -c "ttcp; bash"
