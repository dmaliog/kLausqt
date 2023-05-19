#name Изменить PKGBUILD пакета перед установкой
#msg Вы действительно хотите изменить PKGBUILD пакета перед установкой?
#!/bin/bash

read -p "Введите название пакета: " program_name

yay --getpkgbuild "$program_name"

# Путь к PKGBUILD файлу
pkgbuild_path="$HOME/.cache/yay/$program_name/PKGBUILD"

# Проверка наличия PKGBUILD файла
if [ -f "$pkgbuild_path" ]; then
    notify-send "Редактор PKGBUILD" "Найден PKGBUILD файл для пакета $program_name" -i $HOME/kLaus/other/notify.png -a "kLaus" -t 10000

    # Открытие редактора для редактирования PKGBUILD
    read -p "Нажмите Enter, чтобы открыть редактор для редактирования PKGBUILD..."
    $EDITOR "$pkgbuild_path"

    # Компиляция пакета
    read -p "Нажмите Enter, чтобы начать компиляцию пакета..."
    cd "$HOME/.cache/yay/$program_name" && makepkg -f

    # Установка нового пакета
    read -p "Нажмите Enter, чтобы установить новый пакет..."
    yay -U "$HOME/.cache/yay/$program_name"/*.pkg.tar.zst

else
    notify-send "Редактор PKGBUILD" "PKGBUILD файл не найден для пакета $program_name" -i $HOME/kLaus/other/notify.png -a "kLaus" -t 10000
fi
