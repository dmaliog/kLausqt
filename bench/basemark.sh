#name_ru_RU Basemark - измеряет производительность графики
#msg_ru_RU <b>Basemark</b> — Популярный набор тестов для оценки графической производительности компьютерных систем, особенно графических процессоров (GPU). Он предоставляет различные тесты и бенчмарки, которые позволяют измерять и сравнивать производительность графической подсистемы в различных сценариях, таких как 3D-графика, виртуальная реальность (VR), компьютерные игры и другие графически интенсивные приложения.

#name_en_US Basemark - measures graphics performance
#msg_en_US <b>Basemark</b> is a popular set of tests for evaluating the graphics performance of computer systems, especially graphics processing units (GPUs). It provides various tests and benchmarks that allow you to measure and compare the performance of the graphics subsystem in various scenarios, such as 3D graphics, virtual reality (VR), computer games and other graphically intensive applications.

#icon multimedia-photo-viewer

#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"
helper="$2"

if ! $helper -Q basemark &>/dev/null; then

    # Предлагаем установить basemark
    read -p "${pkg} basemark ${no_found}. ${instq} basemark? (y/n): " answer
    if [[ "$answer" == [yY] ]]; then
        if [ "$helper" = "yay" ]; then
            $helper -S basemark
        else
            $helper -S basemark --skipreview
        fi
    else
        notify-send "${error}" "${noinstall}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
        exit 1
    fi
fi

# Запускаем basemark
basemark
