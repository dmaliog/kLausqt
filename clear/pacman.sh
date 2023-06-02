#name_ru_RU Загруженные пакеты Pacman
#msg_ru_RU /var/cache/pacman/pkg: В этой папке хранятся загруженные пакеты Pacman. Если вы не планируете откатываться к предыдущим версиям пакетов, то вы можете удалить старые пакеты, чтобы освободить место. Однако, имейте в виду, что если вы захотите удалить пакеты, то вам будет необходимо загрузить их повторно при необходимости.
#name_en_US Downloaded Pacman Packages
#msg_en_US /var/cache/pacman/pkg: Downloaded Pacman packages are stored in this folder. If you do not plan to roll back to previous versions of packages, then you can delete the old packages to free up space. However, keep in mind that if you want to delete packages, you will need to download them again if necessary.
#!/bin/bash
# Определение языка
language="en_US"
if [ -n "$1" ]; then
    language="$1"
fi

# Загрузка файла перевода
translations_file="translations_$language.txt"
source "$HOME/kLaus/other/$translations_file"

# Подсчет размера папки /var/cache/pacman/pkg/
folder_size=$(du -sh /var/cache/pacman/pkg/ | awk '{print $1}')

# Вывод размера папки
notify-send "${pacman}" "${size_clear}" -i $HOME/kLaus/other/notify.png -a "kLaus" -t 10000

# Очистка папки /var/cache/pacman/pkg/
sudo rm -rf /var/cache/pacman/pkg/*
notify-send "${pacman}" "${pacman_clear}" -i $HOME/kLaus/other/notify.png -a "kLaus" -t 10000

