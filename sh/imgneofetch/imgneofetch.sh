#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"


# Проверка наличия установленного пакета neofetch
if ! which neofetch > /dev/null; then
  echo "${no_neofetch}"
else
  # Выбор файла с помощью zenity
  image_path=$(zenity --file-selection --title="${path_img}")

  # Если файл не выбран, выводим сообщение и продолжаем работу скрипта
  if [ -z "$image_path" ]; then
    notify-send "${neofetch_img}" "${no_file}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
  else
    # Проверка наличия файла
    if [ ! -f "$image_path" ]; then
      notify-send "${neofetch_img}" "${noselect_file}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
    else
      # Замена соответствующих строк в конфигурационном файле
      sed -i 's/image_backend=.*/image_backend="iterm2" # Инструмент для обработки изображений/g' ~/.config/neofetch/config.conf
      sed -i 's|image_source=.*|image_source="'"$image_path"'" # Расположение картинки|g' ~/.config/neofetch/config.conf
      notify-send "${neofetch_img}" "${settings_ok}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
    fi
  fi
fi
