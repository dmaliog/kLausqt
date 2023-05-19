#name Установить локальный пакет
#msg Вы действительно хотите установить локальный пакет? Внимание: Устанавливайте только те пакеты, который создали с помощью kLaus.
#!/bin/bash

# Запрос пути к архиву программы
archive_path=$(zenity --file-selection --title="Выберите архив программы")

# Проверка, был ли выбран архив
if [ -n "$archive_path" ]; then
    # Распаковка архива в каталог кэша yay
    tar xvf "$archive_path" -C "$HOME/.cache/yay/"

    # Получение имени пакета из имени архива
    package_name=$(basename "$archive_path" .tar.gz)

    yay -U "$HOME/.cache/yay/$package_name"/*.pkg.tar.zst

else
    notify-send "Установщик локальных пакетов" "Архив для установки пакета не выбран" -i $HOME/kLaus/other/notify.png -a "kLaus" -t 10000
fi
