#name_ru_RU interbench - оценивает интерактивность производительности системы
#msg_ru_RU <b>interbench</b> — Инструмент для измерения и оценки интерактивной производительности системы. Он разработан для тестирования и сравнения производительности систем в контексте работы с интерактивными приложениями, такими как загрузка веб-страниц, отзывчивость пользовательского интерфейса и другие сценарии, в которых важна непосредственная отзывчивость системы.

#name_en_US interbench - evaluates interactivity of system performance
#msg_en_US <b>interbench</b> is a tool for measuring and evaluating interactive system performance. It is designed to test and compare the performance of systems in the context of working with interactive applications, such as loading web pages, responsiveness of the user interface and other scenarios in which the immediate responsiveness of the system is important.

#icon org.kde.plasma.systemmonitor

#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"
helper="$2"

# Проверяем наличие установленного пакета interbench
if ! $helper -Q interbench &>/dev/null; then

    # Предлагаем установить interbench
    read -p "${pkg} interbench ${no_found}. ${instq} interbench? (y/n): " answer
    if [[ "$answer" == [yY] ]]; then
        if [ "$helper" = "yay" ]; then
            $helper -S interbench
        else
            $helper -S interbench --skipreview
        fi
    else
        notify-send "${error}" "${noinstall}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
        exit 1
    fi
fi

# Запускаем interbench
bash -c "interbench; bash"
