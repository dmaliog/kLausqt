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
