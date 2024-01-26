#name_ru_RU Разблокировать экземпляр Pacman
#msg_ru_RU <b>Pacman</b> — Пакетный менеджер который находит гармонию между двоичным форматом пакетов и простой в использовании системой сборки. Основная цель Pacman - обеспечить удобное управление пакетами, будь то из официальных репозиториев или созданных вручную.<br><br>Pacman поддерживает вспомогательные инструменты, такие как Paru или Yay, расширяя его функциональность. Благодаря этим инструментам Pacman становится еще более гибким.<br><br>Скрипт разблокирует ваш экземпляр Pacman в случае ошибки (/var/lib/pacman/db.lck уже существует. Возможно, запущен другой экземпляр Pacman. Ожидание...).


#name_en_US Unlock the Pacman instance
#msg_en_US <b>Pacman</b> is a package manager that finds harmony between the binary package format and an easy—to-use build system. Pacman's main goal is to provide convenient package management, whether from official repositories or manually created ones.<br><br>Pacman supports auxiliary tools such as Paru or Yay, extending its functionality. Thanks to these tools, Pacman becomes even more flexible.<br><br>The script unlocks your Pacman instance in case of an error (/var/lib/pacman/db.lck already exists. Another Pacman instance may be running. Waiting...).

#icon pacman

#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"
helper="$2"
file_path="/var/lib/pacman/db.lck"

if [ -e "$file_path" ]; then
    sudo rm "$file_path"
else
    notify-send "${pacman}" "${pacman_nodb}" -i "$HOME/.config/kLaus/other/notify.png" -a "kLaus" -t 10000
fi
