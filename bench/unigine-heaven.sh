#name_ru_RU Unigine Heaven - измеряет производительность графики
#msg_ru_RU Unigine Heaven (2009) - Этот бенчмарк является одним из наиболее популярных тестов для оценки производительности графических карт. Он предлагает впечатляющую 3D-среду с динамическим освещением, частицами, туманом, водными эффектами и другими деталями.

#name_en_US Unigine Heaven - measures graphics performance
#msg_en_US Unigine Heaven (2009) - This benchmark is one of the most popular tests for evaluating the performance of graphics cards. It offers an impressive 3D environment with dynamic lighting, particles, fog, water effects and other details.

#icon 21

#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"

if ! yay -Q unigine-heaven &>/dev/null; then

    # Предлагаем установить unigine-heaven
    read -p "${pkg} unigine-heaven ${no_found}. ${instq} unigine-heaven? (y/n): " choice
    if [[ $choice == "y" || $choice == "Y" ]]; then
        yay -S unigine-heaven
    else
        notify-send "${error}" "${noinstall}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
        exit 1
    fi
fi

# Запускаем unigine-heaven
unigine-heaven
