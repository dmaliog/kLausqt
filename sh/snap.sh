#name_ru_RU Добавить поддержку Snap
#msg_ru_RU <b>Snap</b> — Система развёртывания программного обеспечения и управления пакетами. Пакеты называются «snaps», а инструмент для их использования — «snapd», который работает в различных дистрибутивах Linux и, следовательно, позволяет развёртывать программное обеспечение, не зависящее от дистрибутива. Изначально система Snap была разработана и создана компанией Canonical.<br><br>Скрипт устанавливает и настраивает Snapd с его службами.

#name_en_US Add Snap support
#msg_en_US <b>Snap</b> is a software deployment and package management system. The packages are called "snaps", and the tool for using them is "snapd", which works on various Linux distributions and, therefore, allows you to deploy software that does not depend on the distribution. Initially, the Snap system was developed and created by Canonical.<br><br>The script installs and configures Snapd with its services.

#icon snap

#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"
helper="$2"

#!/bin/bash

# Проверка наличия установленного пакета Snap
if $helper -Q snapd &>/dev/null; then
    read -p "${pkg} Snapd ${found}. ${delq} Snapd & ${pkgs}? (y/n): " answer

    if [[ "$answer" == [yY] ]]; then
        # Удаление пакета Snap
        $helper -Rs snapd
    fi

else
    # Установка пакета Snap
    if [ "$helper" = "yay" ]; then
        $helper -S snapd
    else
        $helper -S snapd --skipreview
    fi

    # Включение службы Snapd
    sudo systemctl enable snapd --now

    # Включение службы AppArmor
    sudo systemctl enable --now apparmor
    sudo systemctl enable --now snapd.apparmor

    # Создание символической ссылки для поддержки классической привязки
    sudo ln -s /var/lib/snapd/snap /snap

    # Установка основных файлов Snap
    sudo snap install core
fi
