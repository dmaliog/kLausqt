#name Загруженные пакеты Yay
#msg /.cache/yay/: В этой папке хранятся загруженные пакеты Yay. Если вы не планируете откатываться к предыдущим версиям пакетов, то вы можете удалить старые пакеты, чтобы освободить место. Однако, имейте в виду, что если вы захотите удалить пакеты, то вам будет необходимо загрузить их повторно при необходимости.
#!/bin/bash

# Подсчет размера папки /var/cache/pacman/pkg/
folder_size=$(du -sh $HOME/.cache/yay/ | awk '{print $1}')

# Вывод размера папки
notify-send "Пакеты Yay" "Размер очистки: $folder_size" -i $HOME/kLaus/other/notify.png -a "kLaus" -t 10000
# Очистка папки $HOME/.cache/yay/
sudo rm -rf $HOME/.cache/yay/*

notify-send "Пакеты Yay" "Каталог $HOME/.cache/yay/ очищен" -i $HOME/kLaus/other/notify.png -a "kLaus" -t 10000
