#name_ru_RU Конфигурация репозиториев Pacman (/etc/pacman.conf)
#msg_ru_RU <b>/etc/pacman.conf</b> — Основной файл конфигурации для менеджера пакетов Pacman. В этом файле определяются настройки pacman, такие как репозитории пакетов, зеркала, параметры поиска, обновления и многое другое.

#name_en_US Pacman Repository Configuration (/etc/pacman.conf)
#msg_en_US <b>/etc/pacman.conf</b> is the main configuration file for the Pacman package manager. This file defines pacman settings, such as package repositories, mirrors, search parameters, updates, and more.

#icon 27

#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"

sudo nano /etc/pacman.conf

echo -e "\n${enter_ok}"
read
