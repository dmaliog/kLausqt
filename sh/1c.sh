#name_ru_RU Установить 1С Предприятие [Учебная версия]
#msg_ru_RU Вы действительно хотите установить 1С Предприятие? Для установки вам понадобится скачать пакет с сайта 1С (https://online.1c.ru/catalog/free/28766016/) для Linux.
#name_en_US Install 1C Enterprise [Training Version]
#msg_en_US Do you really want to install 1C Enterprise? To install, you will need to download the package from the 1C website (https://online.1c.ru/catalog/free/28766016 /) for Linux.
#!/bin/bash
# Определение языка
language="en_US"
if [ -n "$1" ]; then
    language="$1"
fi

# Загрузка файла перевода
translations_file="translations_$language.txt"
source "$HOME/kLaus/other/$translations_file"

# Проверка наличия пакетов
packages=("enchant1.6" "webkitgtk2-bin" "pkgextract" "imagemagick")
missing_packages=()

for package in "${packages[@]}"; do
    if ! pacman -Qs "$package" > /dev/null; then
        missing_packages+=("$package")
    fi
done

# Установка недостающих пакетов через yay
if [ ${#missing_packages[@]} -gt 0 ]; then
    yay -S "${missing_packages[@]}"
fi

# Проверка наличия папки /opt/1cv8t/
if [ -d "/opt/1cv8t/" ]; then
    # Папка существует, спросить пользователя о удалении 1С Предприятие
    read -p "${c_ok}" answer
    if [ "$answer" = "y" ]; then
        # Удаление предыдущей версии 1С Предприятие
        version=$(ls /opt/1cv8t/x86_64/)
        sudo /opt/1cv8t/x86_64/$version/uninstaller-training
    fi
fi

# Выбор архива с помощью zenity
archive=$(zenity --file-selection --title="${path}" --file-filter='*.tar.gz' --file-filter='*.tar.gz')

# Проверка наличия архива
if [ -z "$archive" ]; then
    notify-send "${c_notify}" "${no_ark}" -i $HOME/kLaus/other/notify.png -a "kLaus" -t 10000
    exit 0
fi

# Распаковка архива
tar -xf "$archive" -C /tmp

# Извлечение имени .run файла из архива
run_file=$(find /tmp -name "*.run" -type f -print -quit)

# Проверка наличия .run файла
if [ -z "$run_file" ]; then
    notify-send "${c_notify}" "${c_norun}" -i $HOME/kLaus/other/notify.png -a "kLaus" -t 10000
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
