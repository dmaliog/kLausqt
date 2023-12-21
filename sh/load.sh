#name_ru_RU Установить локальный пакет
#msg_ru_RU <b>Pacman</b> — Пакетный менеджер который находит гармонию между двоичным форматом пакетов и простой в использовании системой сборки. Основная цель Pacman - обеспечить удобное управление пакетами, будь то из официальных репозиториев или созданных вручную.<br><br>Pacman поддерживает вспомогательные инструменты, такие как Paru или Yay, расширяя его функциональность. Благодаря этим инструментам Pacman становится еще более гибким.<br><br>Скрипт устанавливает локальный пакет с использованием вспомогательных инструментов Pacman.<br><br><b>Внимание!</b> Устанавливайте только локальный пакет который создавался в этом приложении (kLaus). Локальный пакет из других источников не сможет быть установлен из-за несовместимости.

#name_en_US Install Local Package
#msg_en_US <b>Pacman</b> is a package manager that finds harmony between the binary format of packages and an easy—to-use build system. Pacman's main goal is to provide convenient package management, whether from official repositories or created manually.<br><br>Pacman supports auxiliary tools such as Paru or Yay, extending its functionality. Thanks to these tools, Pacman becomes even more flexible.<br><br>The script installs a local package using Pacman auxiliary tools.<br><br><b>Attention!</b> Install only the local package that was created in this application (kLaus). A local package from other sources will not be able to be installed due to incompatibility.

#icon system-software-install

#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"
helper="$2"

# Запрос пути к архиву программы
archive_path=$(zenity --file-selection --title="${path}")

# Проверка, был ли выбран архив
if [ -n "$archive_path" ]; then
    # Распаковка архива в каталог кэша
    tar xvf "$archive_path" -C "$HOME/.cache/$helper/"

    # Получение имени пакета из имени архива
    package_name=$(basename "$archive_path" .tar.gz)

    $helper -U "$HOME/.cache/$helper/$package_name"/*.pkg.tar.zst

else
    notify-send "${local_setup}" "${no_ark}" -i "$HOME/.config/kLaus/other/notify.png" -a "kLaus" -t 10000
fi
