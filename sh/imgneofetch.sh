#name_ru_RU Добавить картинку в neofetch
#msg_ru_RU <b>Neofetch</b> — Очень простая в использовании и кроссплатформенная утилита которая позволяет отображать информацию о системе в терминале, рядом с красивой картинкой. В качестве картинки можно использовать логотип вашего дистрибутива или любую другую картинку.<br><br>Эта утилита очень похожа на ScreenFetch или LinuxLogo, но она более настраиваемая и гибкая, а также содержит некоторые дополнительные функции.<br><br>Скрипт устанавливает вашу картинку в Neofetch.

#name_en_US Add a picture to neofetch
#msg_en_US <b>Neofetch</b> is a very easy—to-use and cross-platform utility that allows you to display information about the system in the terminal, next to a beautiful picture. As an image, you can use the logo of your distribution or any other image.<br><br>This utility is very similar to ScreenFetch or LinuxLogo, but it is more customizable and flexible, and also contains some additional features.<br><br>The script installs your picture in Neofetch.

#icon kipi-dngconverter

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
      sed -i 's|image_source=.*/|image_source="'"$image_path"'" # Расположение картинки|g' ~/.config/neofetch/config.conf
      notify-send "${neofetch_img}" "${settings_ok}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
    fi
  fi
fi
