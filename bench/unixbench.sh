#name_ru_RU UnixBench - измеряет производительность компьютера
#msg_ru_RU UnixBench - это популярный бенчмарк для оценки производительности Unix-подобных систем. Он измеряет производительность компьютера, выполняя ряд тестовых нагрузок на различные компоненты системы, такие как процессор, память, диск, сеть и другие.
#name_en_US UnixBench - measures computer performance
#msg_en_US UnixBench is a popular benchmark for evaluating the performance of Unix-like systems. It measures computer performance by performing a series of test loads on various system components, such as processor, memory, disk, network, and others.
#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"

# Проверяем наличие установленного пакета unixbench
if ! yay -Qs unixbench &>/dev/null; then

    # Предлагаем установить unixbench
    read -p "${pkg} unixbench ${no_found}. ${instq} unixbench? (y/n): " choice
    if [[ $choice == "y" || $choice == "Y" ]]; then
        yay -S unixbench
    else
        notify-send "${error}" "${noinstall}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
        exit 1
    fi
fi

# Запускаем unixbench
bash -c "ubench; bash"
