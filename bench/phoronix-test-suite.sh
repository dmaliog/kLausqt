#name_ru_RU Phoronix Test Suite - измеряет производительность системы
#msg_ru_RU Phoronix Test Suite (PTS) - это мощный набор инструментов и бенчмарков для тестирования производительности и сравнительного анализа компьютерных систем. Он предоставляет широкий спектр тестов и бенчмарков, позволяющих измерять производительность процессора, графического ускорителя, памяти, дискового подсистемы, сети и других компонентов системы.

#name_en_US Phoronix Test Suite - measures system performance
#msg_en_US Phoronix Test Suite (PTS) is a powerful set of tools and benchmarks for performance testing and comparative analysis of computer systems. It provides a wide range of tests and benchmarks that allow you to measure the performance of the processor, graphics accelerator, memory, disk subsystem, network and other system components.

#icon 49

#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"

if ! yay -Q phoronix-test-suite &>/dev/null; then

    # Предлагаем установить phoronix-test-suite
    read -p "${pkg} phoronix-test-suite ${no_found}. ${instq} phoronix-test-suite? (y/n): " choice
    if [[ $choice == "y" || $choice == "Y" ]]; then
        yay -S phoronix-test-suite
    else
        notify-send "${error}" "${noinstall}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
        exit 1
    fi
fi

# Запускаем phoronix-test-suite
bash -c "phoronix-test-suite; bash"
