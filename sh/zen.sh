#name_ru_RU Установить Zen-ядро
#msg_ru_RU <b>Zen Kernel</b> — Ядро для систем общего пользования или говоря проще, для десктопа, куда интегрированы дополнительные возможности, не включенные в состав основного ядра (например, поддержка нового оборудования и ускоряющие работу изменения).<br><br>Скрипт помогает в легкой установке Zen-ядра с его заголовками и обновляет GRUB после его установки.

#name_en_US Install Zen Kernel
#msg_en_US <b>Zen Kernel</b> is a kernel for public use systems or, more simply, for a desktop, where additional features are integrated that are not included in the main core (for example, support for new hardware and changes that speed up work).<br><br>The script helps in easy installation of the Zen kernel with its headers and updates GRUB after its installation.

#icon 10

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
