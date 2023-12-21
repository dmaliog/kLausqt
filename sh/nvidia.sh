#name_ru_RU Установить драйвера NVIDIA для видеокарты
#msg_ru_RU Скрипт устанавливает самый актуальный драйвер <b>DKMS</b>, который подстроится под нужное ядро и не позволит убить систему при обновлении (не касается свободных драйверов Mesa).<br><br>Перед установкой рекомендуется отключить <b>Secure Boot</b> в UEFI, ибо из-за этого модули драйвера могут не загрузиться.

#name_en_US Install NVIDIA drivers for the graphics card
#msg_en_US The script installs the most up-to-date driver <b>DKMS</b>, which will adjust to the right kernel and will not allow you to kill the system when updating (does not apply to free Mesa drivers).<br><br>Before installation, it is recommended to disable <b>Secure Boot</b> in UEFI, because because of this, the driver modules may not boot.

#icon nvidia

#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"
helper="$2"

if [ "$helper" = "yay" ]; then
    $helper -S --noconfirm nvidia-dkms nvidia-utils lib32-nvidia-utils nvidia-settings vulkan-icd-loader lib32-vulkan-icd-loader lib32-opencl-nvidia opencl-nvidia libxnvctrl
else
    $helper -S --noconfirm nvidia-dkms nvidia-utils lib32-nvidia-utils nvidia-settings vulkan-icd-loader lib32-vulkan-icd-loader lib32-opencl-nvidia opencl-nvidia libxnvctrl --skipreview
fi
