#name_ru_RU Установить Zen-ядро
#msg_ru_RU Вы действительно хотите установить Zen ядро?
#name_en_US Install Zen Core
#msg_en_US Do you really want to install Zen kernel?
#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"

# Проверка наличия установленных Zen пакетов
if [ -n "$(yay -Qs linux-zen)" ] || [ -n "$(yay -Qs linux-zen-headers)" ]; then
    echo "${pkg} linux-zen ${found}. ${delq} linux-zen & linux-zen-headers? (y/n): "
    read answer
    if [ "$answer" == "y" ]; then
        yay -R --noconfirm linux-zen linux-zen-headers

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
    # Установка ядра Linux Zen и заголовков
    yay -S --noconfirm linux-zen linux-zen-headers

    # Проверка наличия Grub
    if [ ! -f "/boot/grub/grub.cfg" ]; then
        notify-send "${zen}" "${grub_no}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
    else
        # Обновление Grub
        sudo grub-mkconfig -o /boot/grub/grub.cfg
        notify-send "${zen}" "${install_ok}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
    fi
fi
