#name_ru_RU blender-benchmark - измеряет производительность графики
#msg_ru_RU blender-benchmark — это популярный набор тестов для оценки графической производительности компьютерных систем, особенно графических процессоров (GPU). Он предоставляет различные тесты и бенчмарки, которые позволяют измерять и сравнивать производительность графической подсистемы в различных сценариях, таких как 3D-графика, виртуальная реальность (VR), компьютерные игры и другие графически интенсивные приложения.

#name_en_US blender-benchmark - measures graphics performance
#msg_en_US blender-benchmark is a popular set of tests for evaluating the graphics performance of computer systems, especially graphics processing units (GPUs). It provides various tests and benchmarks that allow you to measure and compare the performance of the graphics subsystem in various scenarios, such as 3D graphics, virtual reality (VR), computer games and other graphically intensive applications.

#icon 21

#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"

if ! yay -Qs blender-benchmark &>/dev/null; then

    # Предлагаем установить blender-benchmark
    read -p "${pkg} blender-benchmark ${no_found}. ${instq} blender-benchmark? (y/n): " choice
    if [[ $choice == "y" || $choice == "Y" ]]; then
        yay -S blender-benchmark
    else
        notify-send "${error}" "${noinstall}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
        exit 1
    fi
fi

# Запускаем blender-benchmark
blender-benchmark