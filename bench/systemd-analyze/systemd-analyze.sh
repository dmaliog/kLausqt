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


