#name_ru_RU Установить драйвера AMD для видеокарты
#msg_ru_RU Скрипт устанавливает драйвера <b>AMD</b>:<br><b>mesa</b>, <b>lib32-mesa</b>, <b>vulkan-radeon</b>, <b>lib32-vulkan-radeon</b>, <b>vulkan-icd-loader</b>, <b>lib32-vulkan-icd-loader</b>, <b>mesa-vdpau</b>, <b>lib32-mesa-vdpau</b>, <b>libva-mesa-driver</b>, <b>lib32-libva-mesa-driver</b>, <b>vulkan-mesa-layers</b><br><br><b>libva-mesa-driver</b>, <b>mesa-vdpau</b> и <b>lib32-mesa-vdpau</b>, а также <b>lib32-libva-mesa-driver</b> нужны для работы аппаратного видео ускорения. <b>vulkan-mesa-layers</b> - это дополнительные уровни совместимости с вулканом

#name_en_US Install AMD drivers for the graphics card
#msg_en_US The script installs the drivers <b>AMD</b>:<br><b>mesa</b>, <b>lib32-mesa</b>, <b>vulkan-radeon</b>, <b>lib32-vulkan-radeon</b>, <b>vulkan-icd-loader</b>, <b>lib32-vulkan-icd-loader</b>, <b>mesa-vdpau</b>, <b>lib32-mesa-vdpau</b>, <b>libva-mesa-driver</b>, <b>lib32-libva-mesa-driver</b>, <b>vulkan-mesa-layers</b><br><br><b>libva-mesa-driver</b>, <b>mesa-vdpau</b> and <b>lib32-mesa-vdpau</b>, as well as <b>lib32-libva-mesa-driver</b> are needed for hardware video acceleration. <b>vulkan-mesa-layers</b> are additional levels of compatibility with vulkan

#icon amd

#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"
helper="$2"

if [ "$helper" = "yay" ]; then
    $helper -S --noconfirm mesa lib32-mesa vulkan-radeon lib32-vulkan-radeon vulkan-icd-loader lib32-vulkan-icd-loader mesa-vdpau lib32-mesa-vdpau libva-mesa-driver lib32-libva-mesa-driver vulkan-mesa-layers
else
    $helper -S --noconfirm mesa lib32-mesa vulkan-radeon lib32-vulkan-radeon vulkan-icd-loader lib32-vulkan-icd-loader mesa-vdpau lib32-mesa-vdpau libva-mesa-driver lib32-libva-mesa-driver vulkan-mesa-layers --skipreview
fi
