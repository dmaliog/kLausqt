#name_ru_RU Конфигурация репозиториев Pacman (/etc/pacman.conf)
#msg_ru_RU '/etc/pacman.conf' - это основной файл конфигурации для менеджера пакетов Pacman. В этом файле определяются настройки pacman, такие как репозитории пакетов, зеркала, параметры поиска, обновления и многое другое.
#name_en_US Pacman Repository Configuration (/etc/pacman.conf)
#msg_en_US '/etc/pacman.conf' is the main configuration file for the Pacman package manager. This file defines pacman settings, such as package repositories, mirrors, search parameters, updates, and more.
#!/bin/bash
# Определение языка
language="en_US"
if [ -n "$1" ]; then
    language="$1"
fi

# Загрузка файла перевода
translations_file="translations_$language.txt"
source "$HOME/.config/kLaus/other/$translations_file"
sudo nano /etc/pacman.conf
read -p "${enter_ok}"
