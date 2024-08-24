#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"
helper="$2"

if [ -n "$3" ]; then
    program_name="$3"
else
    read -p "${name_pkg}" program_name
fi

if [ ! -d "$HOME/.config/kLaus/pkg/" ]; then
    mkdir -p "$HOME/.config/kLaus/pkg/"
fi
cd "$HOME/.config/kLaus/pkg/"

# Скачиваем PKGBUILD
$helper --getpkgbuild "$program_name"

# Извлекаем только имя пакета, игнорируя префикс репозитория
pkg_name=$(echo "$program_name" | cut -d'/' -f2)

# Путь к PKGBUILD файлу
pkgbuild_path="$HOME/.config/kLaus/pkg/$pkg_name/PKGBUILD"

# Проверка наличия PKGBUILD файла
if [ -f "$pkgbuild_path" ]; then
    notify-send "${pkgbuild}" "${pkgbuild_ok}" -i "$HOME/.config/kLaus/other/notify.png" -a "kLaus" -t 10000

    # Открытие редактора для редактирования PKGBUILD
    read -p "${pkgbuild_edit}"
    $EDITOR "$pkgbuild_path"

    # Компиляция пакета
    read -p "${pkgbuild_make}"
    cd "$HOME/.config/kLaus/pkg/$pkg_name" && makepkg -f

    # Установка нового пакета
    read -p "${pkgbuild_newpkg}"
    $helper -U "$HOME/.config/kLaus/pkg/$pkg_name"/*.pkg.tar.zst

else
    notify-send "${pkgbuild}" "${pkgbuild_no}" -i "$HOME/.config/kLaus/other/notify.png" -a "kLaus" -t 10000
fi
