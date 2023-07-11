#name_ru_RU peakperf - измеряет производительность процессора
#msg_ru_RU peakperf — это микробенчмарк, который пытается достичь пиковой производительности на процессорах x86_64. Некоторые проблемы могут снизить производительность, обеспечиваемую вашим процессором, например, охлаждение процессора. С помощью peakperf вы можете проверить, обеспечивает ли ваш процессор полную мощность, на которую он способен.

#name_en_US peakperfo - measures CPU performance
#msg_en_US peakperf is a microbenchmark that tries to achieve peak performance on x86_64 processors. Some issues may reduce the performance provided by your processor, such as CPU cooling. With peakperf, you can check whether your processor is delivering the full power it is capable of.

#icon 53

#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"

if ! yay -Q peakperf-git &>/dev/null; then

    # Предлагаем установить peakperf-git
    read -p "${pkg} peakperf-git ${no_found}. ${instq} peakperf-git? (y/n): " choice
    if [[ $choice == "y" || $choice == "Y" ]]; then
        yay -S peakperf-git
    else
        notify-send "${error}" "${noinstall}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
        exit 1
    fi
fi

# Запускаем peakperf-git
bash -c "peakperf; bash"
