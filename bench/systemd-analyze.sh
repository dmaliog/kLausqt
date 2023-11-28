#name_ru_RU systemd-analyze - анализирует время загрузки компонентов и служб
#msg_ru_RU <b>systemd-analyze</b> — Анализирует время загрузки различных компонентов и служб во время запуска и создает графическое представление, которое показывает зависимости и последовательность выполнения этих компонентов.

#name_en_US systemd-analyze - analyzes the loading time of components and services
#msg_en_US <b>systemd-analyze</b> — analyzes the loading time of various components and services during startup and creates a graphical representation that shows the dependencies and the sequence of execution of these components.

#icon 53

#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"

# Выполняем команду systemd-analyze plot и сохраняем результат в файл boot.svg
systemd-analyze plot > boot.svg

# Загружаем файл boot.svg на Transfer.sh и подавляем вывод о прогрессе загрузки
upload_result=$(curl -sS --upload-file boot.svg https://transfer.sh/boot.svg)

# Извлекаем ссылку из результата загрузки
image_url=$(echo "$upload_result" | awk '{print $NF}')

# Получаем реальное местоположение картинки
real_location=$(realpath boot.svg)

# Выводим ссылку на загруженное изображение и реальное местоположение картинки
echo -e "\n$dir $real_location"
echo -e "\n$link $image_url"

echo -e "\n${enter_ok}"
read


