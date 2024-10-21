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
