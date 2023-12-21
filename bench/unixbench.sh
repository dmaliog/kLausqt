#name_ru_RU UnixBench - измеряет производительность компьютера
#msg_ru_RU <b>UnixBench</b> — Популярный бенчмарк для оценки производительности Unix-подобных систем. Он измеряет производительность компьютера, выполняя ряд тестовых нагрузок на различные компоненты системы, такие как процессор, память, диск, сеть и другие.

#name_en_US UnixBench - measures computer performance
#msg_en_US <b>UnixBench</b> is a popular benchmark for evaluating the performance of Unix-like systems. It measures computer performance by performing a series of test loads on various system components, such as processor, memory, disk, network, and others.

#icon org.kde.plasma.systemmonitor

#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"
helper="$2"

# Проверяем наличие установленного пакета unixbench
if ! $helper -Q unixbench &>/dev/null; then

    # Предлагаем установить unixbench
    read -p "${pkg} unixbench ${no_found}. ${instq} unixbench? (y/n): " answer
    if [[ "$answer" == [yY] ]]; then
        if [ "$helper" = "yay" ]; then
            $helper -S unixbench
        else
            $helper -S unixbench --skipreview
        fi
    else
        notify-send "${error}" "${noinstall}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
        exit 1
    fi
fi

# Запускаем unixbench
bash -c "ubench; bash"
