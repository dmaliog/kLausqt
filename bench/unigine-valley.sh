#name_ru_RU Unigine Valley - измеряет производительность графики
#msg_ru_RU Unigine Valley (2013) - Этот бенчмарк представляет собой детализированную 3D-среду с горными пейзажами, реками, лесами и динамическим освещением. Он использует передовые технологии для тестирования производительности графических систем.

#name_en_US Unigine Valley - measures graphics performance
#msg_en_US Unigine Valley (2013) - This benchmark is a detailed 3D environment with mountain landscapes, rivers, forests and dynamic lighting. It uses advanced technology to test the performance of graphics systems.

#icon 21

#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"

if ! yay -Qs unigine-valley &>/dev/null; then

    # Предлагаем установить unigine-valley
    read -p "${pkg} unigine-valley ${no_found}. ${instq} unigine-valley? (y/n): " choice
    if [[ $choice == "y" || $choice == "Y" ]]; then
        yay -S unigine-valley
    else
        notify-send "${error}" "${noinstall}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
        exit 1
    fi
fi

# Запускаем unigine-valley
unigine-valley
