#name_ru_RU gputest - измеряет производительность графики
#msg_ru_RU <b>gputest</b> — Программный инструмент, который используется для тестирования и оценки производительности графического процессора (GPU). Он предоставляет различные тесты и нагрузки, которые могут быть использованы для проверки графической мощности и стабильности GPU.

#name_en_US gputest - measures graphics performance
#msg_en_US <b>gputest</b> is a software tool that is used to test and evaluate the performance of a graphics processor (GPU). It provides various tests and loads that can be used to test GPU graphics power and stability.

#icon 21

#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"
helper="$2"

if ! $helper -Q gputest &>/dev/null; then

    # Предлагаем установить gputest
    read -p "${pkg} gputest ${no_found}. ${instq} gputest? (y/n): " answer
    if [[ "$answer" == [yY] ]]; then
        if [ "$helper" = "yay" ]; then
            $helper -S gputest
        else
            $helper -S gputest --skipreview
        fi
    else
        notify-send "${error}" "${noinstall}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
        exit 1
    fi
fi

# Запускаем gputest
gputest
