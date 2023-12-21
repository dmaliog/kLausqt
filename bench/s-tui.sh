#name_ru_RU s-tui - измеряет производительность CPU
#msg_ru_RU <b>s-tui</b> — Инструмент командной строки для мониторинга и тестирования производительности процессора. Он предоставляет интерактивный интерфейс для отображения различных параметров процессора, таких как загрузка ЦП, температура, частота и другие показатели.

#name_en_US s-tui - measures CPU performance
#msg_en_US <b>s-tui</b> is a command line tool for monitoring and testing CPU performance. It provides an interactive interface for displaying various processor parameters, such as CPU utilization, temperature, frequency, and other indicators.

#icon io.elementary.monitor

#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"
helper="$2"

if ! $helper -Q s-tui &>/dev/null; then

    # Предлагаем установить s-tui
    read -p "${pkg} s-tui ${no_found}. ${instq} s-tui? (y/n): " answer
    if [[ "$answer" == [yY] ]]; then
        if [ "$helper" = "yay" ]; then
            $helper -S s-tui
        else
            $helper -S s-tui --skipreview
        fi
    else
        notify-send "${error}" "${noinstall}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
        exit 1
    fi
fi

# Запускаем s-tui
bash -c "s-tui; bash"
