#name Загруженные пакеты Pacman
#msg /var/cache/pacman/pkg: В этой папке хранятся загруженные пакеты Pacman. Если вы не планируете откатываться к предыдущим версиям пакетов, то вы можете удалить старые пакеты, чтобы освободить место. Однако, имейте в виду, что если вы захотите удалить пакеты, то вам будет необходимо загрузить их повторно при необходимости.
#!/bin/bash

# Подсчет размера папки /var/cache/pacman/pkg/
folder_size=$(du -sh /var/cache/pacman/pkg/ | awk '{print $1}')

# Вывод размера папки
notify-send "Пакеты Pacman" "Размер очистки: $folder_size" -i $HOME/kLaus/other/notify.png -a "kLaus" -t 10000

# Очистка папки /var/cache/pacman/pkg/
sudo rm -rf /var/cache/pacman/pkg/*
notify-send "Пакеты Pacman" "Каталог /var/cache/pacman/pkg/ очищен" -i $HOME/kLaus/other/notify.png -a "kLaus" -t 10000

