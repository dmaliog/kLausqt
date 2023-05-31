#name Добавить поддержку Wayland+Nvidia в KDE
#msg Вы действительно хотите подключить Wayland для Nvidia?
#!/bin/bash

if [[ $(sudo cat /sys/module/nvidia_drm/parameters/modeset) == "N" ]]; then
    if pacman -Qs xorg-xwayland >/dev/null && pacman -Qs libxcb >/dev/null && pacman -Qs egl-wayland >/dev/null && pacman -Qs qt5-wayland >/dev/null && pacman -Qs qt6-wayland >/dev/null; then
        notify-send "Поддержка Wayland+Nvidia" "Все пакеты уже установлены" -i $HOME/kLaus/other/notify.png -a "kLaus" -t 10000
    else
        yay -S --noconfirm xorg-xwayland libxcb egl-wayland qt5-wayland qt6-wayland
    fi
    if ! grep -q "nvidia-drm.modeset=1" /etc/default/grub; then
        sudo sed -i 's/GRUB_CMDLINE_LINUX_DEFAULT="/GRUB_CMDLINE_LINUX_DEFAULT="nvidia-drm.modeset=1 /g' /etc/default/grub
        sudo grub-mkconfig -o /boot/grub/grub.cfg
    else
        notify-send "Поддержка Wayland+Nvidia" "У вас уже добавлены модули в GRUB" -i $HOME/kLaus/other/notify.png -a "kLaus" -t 10000
    fi
else
    notify-send "Поддержка Wayland+Nvidia" "У вас уже установлены модули nvidia_drm.modeset" -i $HOME/kLaus/other/notify.png -a "kLaus" -t 10000
fi
