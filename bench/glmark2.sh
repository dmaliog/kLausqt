#name_ru_RU GLmark2 - измеряет производительность графики
#msg_ru_RU GLmark2 - это бенчмарк, предназначенный для измерения производительности графической системы, основанной на OpenGL, на различных устройствах. Он использует различные тестовые сценарии и рендеринговые техники для оценки графической производительности, включая обработку текстур, освещение, эффекты частиц, тени и другие графические операции.

#name_en_US GLmark2 - measures graphics performance
#msg_en_US GLmark2 is a benchmark designed to measure the performance of an OpenGL-based graphics system on various devices. It uses various test scenarios and rendering techniques to evaluate graphical performance, including texture processing, lighting, particle effects, shadows, and other graphical operations.

#icon 21

#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"
helper="$2"

if ! $helper -Q glmark2 &>/dev/null; then

    # Предлагаем установить glmark2
    read -p "${pkg} glmark2 ${no_found}. ${instq} glmark2? (y/n): " answer
    if [[ "$answer" == [yY] ]]; then
        if [ "$helper" = "yay" ]; then
            $helper -S glmark2
        else
            $helper -S glmark2 --skipreview
        fi
    else
        notify-send "${error}" "${noinstall}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
        exit 1
    fi
fi

# Запускаем glmark2
glmark2
