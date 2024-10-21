#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"
helper="$2"

# Проверка наличия установленных Zen пакетов
if [ -n "$($helper -Q linux-zen)" ] || [ -n "$($helper -Q linux-zen-headers)" ]; then
    read -p "${pkg} linux-zen ${found}. ${delq} linux-zen & linux-zen-headers? (y/n): " answer
    if [[ "$answer" == [yY] ]]; then
        $helper -R --noconfirm linux-zen linux-zen-headers

        # Проверка наличия Grub
        if [ ! -f "/boot/grub/grub.cfg" ]; then
            notify-send "${zen}" "${grub_no}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
        else
            # Обновление Grub
            sudo grub-mkconfig -o /boot/grub/grub.cfg
            notify-send "${zen}" "${delete_ok}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
        fi
    fi
else
    if [ "$helper" = "yay" ]; then
        $helper -S --noconfirm linux-zen linux-zen-headers
    else
        $helper -S --noconfirm linux-zen linux-zen-headers --skipreview
    fi

    # Проверка наличия Grub
    if [ ! -f "/boot/grub/grub.cfg" ]; then
        notify-send "${zen}" "${grub_no}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
    else
        # Обновление Grub
        sudo grub-mkconfig -o /boot/grub/grub.cfg
        notify-send "${zen}" "${install_ok}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
    fi
fi
