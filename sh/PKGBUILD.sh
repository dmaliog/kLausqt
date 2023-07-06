#name_ru_RU Изменить PKGBUILD пакета перед установкой
#msg_ru_RU Вы действительно хотите изменить PKGBUILD пакета перед установкой?
#name_en_US Change PKGBUILD package before installation
#msg_en_US Do you really want to change the PKGBUILD package before installing?
#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"

if [ -n "$2" ]; then
    program_name="$2"
else
    read -p "Введите название пакета: " program_name
fi

if [ ! -d "$HOME/.config/kLaus/pkg/" ]; then
    mkdir "$HOME/.config/kLaus/pkg/"
fi
cd "$HOME/.config/kLaus/pkg/"

yay --getpkgbuild "$program_name"

# Путь к PKGBUILD файлу
pkgbuild_path="$HOME/.config/kLaus/pkg/$program_name/PKGBUILD"

# Проверка наличия PKGBUILD файла
if [ -f "$pkgbuild_path" ]; then
    notify-send "${pkgbuild}" "${pkgbuild_ok}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000

    # Открытие редактора для редактирования PKGBUILD
    read -p "${pkgbuild_edit}"
    $EDITOR "$pkgbuild_path"

    # Компиляция пакета
    read -p "${pkgbuild_make}"
    cd "$HOME/.config/kLaus/pkg/$program_name" && makepkg -f

    # Установка нового пакета
    read -p "${pkgbuild_newpkg}"
    yay -U "$HOME/.config/kLaus/pkg/$program_name"/*.pkg.tar.zst

else
    notify-send "${pkgbuild}" "${pkgbuild_no}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
fi
