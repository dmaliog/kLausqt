#name_ru_RU Добавить поддержку Wayland+Nvidia в KDE
#msg_ru_RU <b>Wayland</b> — Протокол для организации графического сервера. В отличие от X Window System, Wayland не имеет API отрисовки и соответственно ей не занимается.<br><br>Скрипт устанавливает необходимые пакеты и добавляет необходимые модули в загрузчике GRUB.

#name_en_US Add Wayland+Nvidia support to KDE
#msg_en_US <b>Wayland</b> is a protocol for organizing a graphical server. Unlike the X Window System, Wayland does not have a rendering API and, accordingly, does not deal with it.<br><br>The script installs the necessary packages and adds the necessary modules in the GRUB loader.

#icon wayland

#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"
helper="$2"

if [[ $(sudo cat /sys/module/nvidia_drm/parameters/modeset) == "N" ]]; then
    if $helper -Q xorg-xwayland >/dev/null && $helper -Q libxcb >/dev/null && $helper -Q egl-wayland >/dev/null && $helper -Q qt5-wayland >/dev/null && $helper -Q qt6-wayland >/dev/null; then
        notify-send "${wayland_nvidia}" "${pkginstall_ok}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
    else
        if [ "$helper" = "yay" ]; then
            $helper -S --noconfirm xorg-xwayland libxcb egl-wayland qt5-wayland qt6-wayland
        else
            $helper -S --noconfirm xorg-xwayland libxcb egl-wayland qt5-wayland qt6-wayland --skipreview
        fi
    fi
    # Проверка и изменение файла kdeglobals
    kdeglobals_file="$HOME/.config/kdeglobals"
    if grep -q "XwaylandClientsScale=false" "$kdeglobals_file"; then
        sed -i 's/XwaylandClientsScale=false/XwaylandClientsScale=true/g' "$kdeglobals_file"
    fi

    if ! grep -q "nvidia-drm.modeset=1" /etc/default/grub; then
        sudo sed -i 's/GRUB_CMDLINE_LINUX_DEFAULT="/GRUB_CMDLINE_LINUX_DEFAULT="nvidia-drm.modeset=1 /g' /etc/default/grub
        sudo grub-mkconfig -o /boot/grub/grub.cfg
    else
        notify-send "${wayland_nvidia}" "${grubmoduleadd_ok}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
    fi
else
    notify-send "${wayland_nvidia}" "${grubmodule_ok}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
fi
