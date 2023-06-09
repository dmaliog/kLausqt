#name_ru_RU VkMark - измеряет производительность графики Vulkan
#msg_ru_RU VkMark - это бенчмарк для тестирования производительности графических карт и систем с поддержкой Vulkan API. Он разработан специально для оценки и сравнения производительности в графических приложениях, использующих Vulkan, таких как игры и приложения виртуальной реальности.

#name_en_US VkMark - measures the performance of Vulkan graphics
#msg_en_US VkMark is a benchmark for testing the performance of graphics cards and systems with Vulkan API support. It is designed specifically to evaluate and compare performance in graphics applications using Vulkan, such as games and virtual reality applications.

#icon 21

#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"

if ! yay -Q vkmark &>/dev/null; then

    # Предлагаем установить vkmark
    read -p "${pkg} vkmark ${no_found}. ${instq} vkmark? (y/n): " choice
    if [[ $choice == "y" || $choice == "Y" ]]; then
        yay -S vkmark
    else
        notify-send "${error}" "${noinstall}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
        exit 1
    fi
fi

# Запускаем vkmark
vkmark
