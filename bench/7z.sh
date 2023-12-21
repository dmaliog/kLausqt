#name_ru_RU 7z - измеряет скорость сжатия и распаковки
#msg_ru_RU <b>7z benchmark</b> — Используется для выполнения бенчмарка производительности с использованием утилиты 7z. Она выполняет тестирование скорости сжатия и распаковки файлов разных типов и размеров, чтобы оценить производительность системы и оптимальные настройки сжатия.

#name_en_US 7z - measures the speed of compression and placement
#msg_en_US <b>7z benchmark</b> is used to perform a performance benchmark using the 7z utility. It performs compression and decompression speed testing of files of different types and sizes to evaluate system performance and optimal compression settings.

#icon org.kde.plasma.systemmonitor

#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"
helper="$2"

if ! $helper -Q p7zip &>/dev/null; then

    # Предлагаем установить p7zip
    read -p "${pkg} p7zip ${no_found}. ${instq} p7zip? (y/n): " answer
    if [[ "$answer" == [yY] ]]; then
        if [ "$helper" = "yay" ]; then
            $helper -S p7zip
        else
            $helper -S p7zip --skipreview
        fi
    else
        notify-send "${error}" "${noinstall}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
        exit 1
    fi
fi

# Запускаем p7zip
bash -c "7z b; bash"
