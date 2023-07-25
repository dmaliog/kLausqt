#name_ru_RU Добавить поддержку Snap
#msg_ru_RU Snap (также известный как Snappy) - это формат пакетирования и система доставки приложений, разработанные Canonical, компанией, стоящей за операционной системой Ubuntu. Он предоставляет способ упаковки приложений и их зависимостей в изолированный контейнер, который можно запускать на разных дистрибутивах Linux, обеспечивая большую стабильность и безопасность. Установить поддержку Snap пакетов?

#name_en_US Add Snap support
#msg_en_US Snap (also known as Snappy) is an application bundling format and delivery system developed by Canonical, the company behind the Ubuntu operating system. It provides a way to package applications and their dependencies into an isolated container that can be run on different Linux distributions, providing greater stability and security. Install support for Snap packages?

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
