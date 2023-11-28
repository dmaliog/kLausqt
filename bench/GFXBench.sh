#name_ru_RU GFXBench - измеряет производительность графики
#msg_ru_RU <b>GFXBench</b> — Популярный бенчмарк, разработанный для оценки производительности графических и вычислительных возможностей различных устройств, таких как мобильные устройства, компьютеры и игровые консоли. Он предоставляет разнообразные тесты и сценарии, которые позволяют оценить производительность графического процессора (GPU) и центрального процессора (CPU) в различных задачах, включая 3D-рендеринг, игровые эффекты, физические симуляции и другие графические операции.

#name_en_US GFXBench - measures graphics performance
#msg_en_US <b>GFXBench</b> is a popular benchmark designed to evaluate the performance of graphics and computing capabilities of various devices, such as mobile devices, computers and game consoles. It provides a variety of tests and scenarios that allow you to evaluate the performance of the graphics processor (GPU) and the central processing unit (CPU) in various tasks, including 3D rendering, game effects, physical simulations and other graphical operations.

#icon 21

#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"
helper="$2"

if ! $helper -Q gfxbench &>/dev/null; then

    # Предлагаем установить gfxbench
    read -p "${pkg} gfxbench ${no_found}. ${instq} gfxbench? (y/n): " answer
    if [[ "$answer" == [yY] ]]; then
        if [ "$helper" = "yay" ]; then
            $helper -S gfxbench
        else
            $helper -S gfxbench --skipreview
        fi
    else
        notify-send "${error}" "${noinstall}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
        exit 1
    fi
fi

# Запускаем gfxbench
gfxbench
