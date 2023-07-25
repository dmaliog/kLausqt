#name_ru_RU Unigine Sanctuary - измеряет производительность графики
#msg_ru_RU Unigine Sanctuary (2007) - это демонстрационный бенчмарк, созданный для тестирования производительности графических систем. Он предлагает красочную 3D-среду с детализированными ландшафтами, растительностью и эффектами погоды.

#name_en_US Unigine Sanctuary - measures graphics performance
#msg_en_US Unigine Sanctuary (2007) is a demo benchmark created to test the performance of graphics systems. It offers a colorful 3D environment with detailed landscapes, vegetation and weather effects.

#icon 21

#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"
helper="$2"

if ! $helper -Q unigine-sanctuary &>/dev/null; then

    # Предлагаем установить unigine-sanctuary
    read -p "${pkg} unigine-sanctuary ${no_found}. ${instq} unigine-sanctuary? (y/n): " answer
    if [[ "$answer" == [yY] ]]; then
        if [ "$helper" = "yay" ]; then
            $helper -S unigine-sanctuary
        else
            $helper -S unigine-sanctuary --skipreview
        fi
    else
        notify-send "${error}" "${noinstall}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
        exit 1
    fi
fi

# Запускаем unigine-sanctuary
unigine-sanctuary
