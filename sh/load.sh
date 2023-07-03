#name_ru_RU Установить локальный пакет
#msg_ru_RU Вы действительно хотите установить локальный пакет? Примечание: Устанавливайте только пакеты, созданные с помощью kLaus
#name_en_US Install Local Package
#msg_en_US Do you really want to install the local package? Note: Install only packages created using kLaus
#!/bin/bash
# Определение языка
language="en_US"
if [ -n "$1" ]; then
    language="$1"
fi

# Загрузка файла перевода
translations_file="translations_$language.txt"
source "$HOME/.config/kLaus/other/$translations_file"

# Запрос пути к архиву программы
archive_path=$(zenity --file-selection --title="${path}")

# Проверка, был ли выбран архив
if [ -n "$archive_path" ]; then
    # Распаковка архива в каталог кэша yay
    tar xvf "$archive_path" -C "$HOME/.cache/yay/"

    # Получение имени пакета из имени архива
    package_name=$(basename "$archive_path" .tar.gz)

    yay -U "$HOME/.cache/yay/$package_name"/*.pkg.tar.zst

else
    notify-send "${local_setup}" "${no_ark}" -i "$HOME/.config/kLaus/other/notify.png" -a "kLaus" -t 10000
fi
