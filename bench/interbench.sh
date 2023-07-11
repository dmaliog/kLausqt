#name_ru_RU interbench - оценивает интерактивность производительности системы
#msg_ru_RU interbench - это инструмент для измерения и оценки интерактивной производительности системы. Он разработан для тестирования и сравнения производительности систем в контексте работы с интерактивными приложениями, такими как загрузка веб-страниц, отзывчивость пользовательского интерфейса и другие сценарии, в которых важна непосредственная отзывчивость системы.

#name_en_US interbench - evaluates interactivity of system performance
#msg_en_US interbench is a tool for measuring and evaluating interactive system performance. It is designed to test and compare the performance of systems in the context of working with interactive applications, such as loading web pages, responsiveness of the user interface and other scenarios in which the immediate responsiveness of the system is important.

#icon 53

#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"

# Проверяем наличие установленного пакета interbench
if ! yay -Q interbench &>/dev/null; then

    # Предлагаем установить interbench
    read -p "${pkg} interbench ${no_found}. ${instq} interbench? (y/n): " choice
    if [[ $choice == "y" || $choice == "Y" ]]; then
        yay -S interbench
    else
        notify-send "${error}" "${noinstall}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
        exit 1
    fi
fi

# Запускаем interbench
bash -c "interbench; bash"
