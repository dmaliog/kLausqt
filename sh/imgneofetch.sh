#name Добавить картинку в neofetch
#msg Вы действительно хотите поменять в neofetch картинку?
#!/bin/bash

# Проверка наличия установленного пакета neofetch
if ! which neofetch > /dev/null; then
  echo "Пакет neofetch не установлен, скрипт завершается"
else
  # Выбор файла с помощью zenity
  image_path=$(zenity --file-selection --title="Выберите картинку")

  # Если файл не выбран, выводим сообщение и продолжаем работу скрипта
  if [ -z "$image_path" ]; then
    notify-send "Картинка в neofetch" "Файл не выбран" -i $HOME/kLaus/other/notify.png -a "kLaus" -t 10000
  else
    # Проверка наличия файла
    if [ ! -f "$image_path" ]; then
      notify-send "Картинка в neofetch" "Файл $image_path не найден" -i $HOME/kLaus/other/notify.png -a "kLaus" -t 10000
    else
      # Замена соответствующих строк в конфигурационном файле
      sed -i 's/image_backend=.*/image_backend="iterm2" # Инструмент для обработки изображений/g' ~/.config/neofetch/config.conf
      sed -i 's|image_source=.*/|image_source="'"$image_path"'" # Расположение картинки|g' ~/.config/neofetch/config.conf
      notify-send "Картинка в neofetch" "Настройка завершена" -i $HOME/kLaus/other/notify.png -a "kLaus" -t 10000
    fi
  fi
fi
