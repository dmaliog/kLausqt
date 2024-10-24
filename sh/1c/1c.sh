#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"
helper="$2"

# Проверка наличия пакетов
packages=("enchant1.6" "webkitgtk2" "pkgextract" "imagemagick")
missing_packages=()

for package in "${packages[@]}"; do
    if ! pacman -Q "$package" > /dev/null; then
        missing_packages+=("$package")
    fi
done

# Установка недостающих пакетов через $helper
if [ ${#missing_packages[@]} -gt 0 ]; then
    if [ "$helper" = "yay" ]; then
        $helper -S "${missing_packages[@]}"
    else
        $helper -S "${missing_packages[@]}" --skipreview
    fi
fi

# Проверка наличия папки /opt/1cv8t/
if [ -d "/opt/1cv8t/" ]; then
    # Папка существует, спросить пользователя о удалении 1С Предприятие
    read -p "${pkg} 1C ${found}. ${delq} 1C? (y/n): " answer
    if [[ "$answer" == [yY] ]]; then
        # Удаление предыдущей версии 1С Предприятие
        version=$(ls /opt/1cv8t/x86_64/)
        sudo /opt/1cv8t/x86_64/$version/uninstaller-training
    fi
fi

# Выбор архива с помощью zenity
archive=$(zenity --file-selection --title="${path}" --file-filter='*.tar.gz' --file-filter='*.tar.gz')

# Проверка наличия архива
if [ -z "$archive" ]; then
    notify-send "${c_notify}" "${no_ark}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
    exit 0
fi

# Распаковка архива
tar -xf "$archive" -C /tmp

# Извлечение имени .run файла из архива
run_file=$(find /tmp -name "*.run" -type f -print -quit)

# Проверка наличия .run файла
if [ -z "$run_file" ]; then
    notify-send "${c_notify}" "${c_norun}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
    exit 0
fi

# Извлечение версии из имени .run файла
version=$(echo "$run_file" | sed -E 's/.*-([0-9]+\.[0-9]+\.[0-9]+\.[0-9]+)-x86_64\.run/\1/')

# Установка прав на выполнение файла .run
chmod +x "$run_file"

# Запуск файла .run
run_command="sudo $run_file"
$run_command


# Копирование libstdc++.so.6 в зависимости от версии
sudo cp /usr/lib/libstdc++.so.6 /opt/1cv8t/x86_64/$version/libstdc++.so.6
sudo cp /usr/lib/libstdc++.so.6 /opt/1cv8t/common/libstdc++.so.6
