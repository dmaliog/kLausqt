#name Установить Zen-ядро
#msg Вы действительно хотите установить Zen ядро?
#!/bin/bash

# Проверка наличия установленных Zen пакетов
if [ -n "$(yay -Qs linux-zen)" ] || [ -n "$(yay -Qs linux-zen-headers)" ]; then
    echo "У вас уже установлено Zen ядро!"
    echo "Хотите удалить его и обновить GRUB? (д/н)"
    read answer
    if [ "$answer" == "д" ]; then
        sudo yay -R --noconfirm linux-zen linux-zen-headers

        # Проверка наличия Grub
        if [ ! -f "/boot/grub/grub.cfg" ]; then
            notify-send "Установка ZEN" "Grub не обнаружен..." -i $HOME/kLaus/other/notify.png -a "kLaus" -t 10000
        else
            # Обновление Grub
            sudo grub-mkconfig -o /boot/grub/grub.cfg
            notify-send "Установка ZEN" "Удаление завершено успешно" -i $HOME/kLaus/other/notify.png -a "kLaus" -t 10000
        fi
    fi
else
    # Установка ядра Linux Zen и заголовков
    yay -S --noconfirm linux-zen linux-zen-headers

    # Проверка наличия Grub
    if [ ! -f "/boot/grub/grub.cfg" ]; then
        notify-send "Установка ZEN" "Grub не обнаружен" -i $HOME/kLaus/other/notify.png -a "kLaus" -t 10000
    else
        # Обновление Grub
        sudo grub-mkconfig -o /boot/grub/grub.cfg
        notify-send "Установка ZEN" "Установка завершена успешно" -i $HOME/kLaus/other/notify.png -a "kLaus" -t 10000
    fi
fi

