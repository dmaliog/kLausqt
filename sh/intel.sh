#name_ru_RU Установить драйвера Intel для видеокарты
#msg_ru_RU Скрипт устанавливает драйвера <b>Intel</b>:<br><b>mesa</b>, <b>lib32-mesa</b>, <b>vulkan-intel</b>, <b>lib32-vulkan-intel</b>, <b>vulkan-icd-loader</b>, <b>lib32-vulkan-icd-loader</b>

#name_en_US Install Intel drivers for the graphics card
#msg_en_US The script installs the <b>Intel</b> drivers:<br><b>mesa</b>, <b>lib32-mesa</b>, <b>vulkan-intel</b>, <b>lib32-vulkan-intel</b>, <b>vulkan-icd-loader</b>, <b>lib32-vulkan-icd-loader</b>

#icon intel

#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"
helper="$2"

if [ "$helper" = "yay" ]; then
    $helper -S --noconfirm mesa lib32-mesa vulkan-intel lib32-vulkan-intel vulkan-icd-loader lib32-vulkan-icd-loader
    $helper -R xf86-video-intel
else
    $helper -S --noconfirm mesa lib32-mesa vulkan-intel lib32-vulkan-intel vulkan-icd-loader lib32-vulkan-icd-loader --skipreview
    $helper -R xf86-video-intel
fi
