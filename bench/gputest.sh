#name_ru_RU gputest - измеряет производительность графики
#msg_ru_RU gputest - это программный инструмент, который используется для тестирования и оценки производительности графического процессора (GPU). Он предоставляет различные тесты и нагрузки, которые могут быть использованы для проверки графической мощности и стабильности GPU.

#name_en_US gputest - measures graphics performance
#msg_en_US gputest is a software tool that is used to test and evaluate the performance of a graphics processor (GPU). It provides various tests and loads that can be used to test GPU graphics power and stability.

#icon 21

#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"

if ! yay -Q gputest &>/dev/null; then

    # Предлагаем установить gputest
    read -p "${pkg} gputest ${no_found}. ${instq} gputest? (y/n): " choice
    if [[ $choice == "y" || $choice == "Y" ]]; then
        yay -S gputest
    else
        notify-send "${error}" "${noinstall}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
        exit 1
    fi
fi

# Запускаем gputest
gputest
