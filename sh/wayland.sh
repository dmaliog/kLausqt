#name_ru_RU Добавить поддержку Wayland+Nvidia в KDE
#msg_ru_RU Вы действительно хотите подключить Wayland для Nvidia?
#name_en_US Add Wayland+Nvidia support to KDE
#msg_en_US Do you really want to connect Wayland to Nvidia?
#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"

if [[ $(sudo cat /sys/module/nvidia_drm/parameters/modeset) == "N" ]]; then
    if pacman -Qs xorg-xwayland >/dev/null && pacman -Qs libxcb >/dev/null && pacman -Qs egl-wayland >/dev/null && pacman -Qs qt5-wayland >/dev/null && pacman -Qs qt6-wayland >/dev/null; then
        notify-send "${wayland_nvidia}" "${pkginstall_ok}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
    else
        yay -S --noconfirm xorg-xwayland libxcb egl-wayland qt5-wayland qt6-wayland
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
