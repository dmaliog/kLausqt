#name_ru_RU Загруженные пакеты Yay
#msg_ru_RU /.cache/yay/: В этой папке хранятся загруженные пакеты Yay. Если вы не планируете откатываться к предыдущим версиям пакетов, то вы можете удалить старые пакеты, чтобы освободить место. Однако, имейте в виду, что если вы захотите удалить пакеты, то вам будет необходимо загрузить их повторно при необходимости.
#name_en_US Downloaded packages Yay
#msg_en_US /.cache/yay/: Downloaded Yay packages are stored in this folder. If you do not plan to roll back to previous versions of packages, then you can delete the old packages to free up space. However, keep in mind that if you want to delete packages, you will need to download them again if necessary.
#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"

# Подсчет размера папки /var/cache/pacman/pkg/
folder_size=$(du -sh $HOME/.cache/yay/ | awk '{print $1}')

# Вывод размера папки
notify-send "${yay}" "${size_clear}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
# Очистка папки $HOME/.cache/yay/
sudo rm -rf $HOME/.cache/yay/*

notify-send "${yay}" "${yay_clear}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
