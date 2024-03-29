#name_ru_RU TTCP - измеряет пропускную способность сетевого соединения
#msg_ru_RU <b>TTCP (Test TCP)</b> — Утилита для измерения пропускной способности сетевого соединения по протоколу TCP. Она предназначена для тестирования и оценки производительности сетевых соединений путем передачи данных между двумя узлами. Утилита должна быть установлена на обоих узлах, между которыми необходимо определить пропускную способность.

#name_en_US TTCP - measures the bandwidth of a network connection
#msg_en_US <b>TTCP (Test TCP)</b> is a solution for measuring the bandwidth of a network connection over the TCP protocol. It is designed to test and evaluate the performance of network connections by transferring data between two nodes. The utility must be installed on both nodes, between which the bandwidth must be determined.

#icon org.kde.plasma.systemmonitor

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
        notify-send "${error}" "${noinstall}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
        exit 1
    fi
fi

# Запускаем ttcp
bash -c "ttcp; bash"
