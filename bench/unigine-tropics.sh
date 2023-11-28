#name_ru_RU Unigine Tropics - измеряет производительность графики
#msg_ru_RU <b>Unigine Tropics (2008)</b> — Бенчмарк предлагает тропическую 3D-среду с живописными пейзажами, растительностью, водными эффектами и динамическим освещением. Он позволяет тестировать производительность графических систем в реалистичной тропической среде.

#name_en_US Unigine Tropics - measures graphics performance
#msg_en_US <b>Unigine Tropics (2008)</b> — This benchmark offers a tropical 3D environment with picturesque landscapes, vegetation, water effects and dynamic lighting. It allows you to test the performance of graphics systems in a realistic tropical environment.

#icon 21

#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"
helper="$2"

if ! $helper -Q unigine-tropics &>/dev/null; then

    # Предлагаем установить unigine-tropics
    read -p "${pkg} unigine-tropics ${no_found}. ${instq} unigine-tropics? (y/n): " answer
    if [[ "$answer" == [yY] ]]; then
        if [ "$helper" = "yay" ]; then
            $helper -S unigine-tropics
        else
            $helper -S unigine-tropics --skipreview
        fi
    else
        notify-send "${error}" "${noinstall}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
        exit 1
    fi
fi

# Запускаем unigine-tropics
unigine-tropics
