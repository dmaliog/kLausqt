#name_ru_RU peakperf - измеряет производительность процессора
#msg_ru_RU <b>peakperf</b> — Микробенчмарк, который пытается достичь пиковой производительности на процессорах x86_64. Некоторые проблемы могут снизить производительность, обеспечиваемую вашим процессором, например, охлаждение процессора. С помощью peakperf вы можете проверить, обеспечивает ли ваш процессор полную мощность, на которую он способен.

#name_en_US peakperfo - measures CPU performance
#msg_en_US <b>peakperf</b> is a microbenchmark that tries to achieve peak performance on x86_64 processors. Some issues may reduce the performance provided by your processor, such as CPU cooling. With peakperf, you can check whether your processor is delivering the full power it is capable of.

#icon org.kde.plasma.systemmonitor

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
        notify-send "${error}" "${noinstall}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
        exit 1
    fi
fi

# Запускаем peakperf-git
bash -c "peakperf; bash"
