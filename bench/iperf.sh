#name_ru_RU iperf - измеряет пропускную способность сетевого соединения
#msg_ru_RU <b>iperf</b> — Инструмент для измерения пропускной способности сети. Он позволяет тестировать скорость передачи данных между двумя узлами в сети, чтобы оценить производительность сетевого соединения. iperf может быть использован для проведения тестов на локальной сети или через интернет между удаленными узлами.

#name_en_US iperf - measures the bandwidth of a network connection
#msg_en_US <b>iperf</b> is a tool for measuring network bandwidth. It allows you to test the data transfer rate between two nodes in a network to evaluate the performance of a network connection. iperf can be used to conduct tests on a local network or over the Internet between remote nodes.

#icon 53

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
        notify-send "${error}" "${noinstall}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
        exit 1
    fi
fi

# Запускаем iperf
bash -c "iperf; bash"
