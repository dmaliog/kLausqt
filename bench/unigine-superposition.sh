#name_ru_RU Unigine Superposition - измеряет производительность графики
#msg_ru_RU Unigine Superposition (2017) - Это один из последних бенчмарков от Unigine. Он предлагает реалистичную 3D-среду с различными эффектами, включая динамическое освещение, частицы, туман, тени и многое другое. Superposition позволяет оценить производительность графических систем в требовательных условиях.

#name_en_US Unigine Superposition - measures graphics performance
#msg_en_US Unigine Superposition (2017) - This one of the latest benchmarks from Unigine. It offers a realistic 3D environment with various effects including dynamic lighting, particles, fog, shadows and more. Superposition allows you to evaluate the performance of graphics systems in demanding conditions.

#icon 21

#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"

if ! yay -Qs unigine-superposition &>/dev/null; then

    # Предлагаем установить unigine-superposition
    read -p "${pkg} unigine-superposition ${no_found}. ${instq} unigine-superposition? (y/n): " choice
    if [[ $choice == "y" || $choice == "Y" ]]; then
        yay -S unigine-superposition
    else
        notify-send "${error}" "${noinstall}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
        exit 1
    fi
fi

# Запускаем unigine-superposition
unigine-superposition
