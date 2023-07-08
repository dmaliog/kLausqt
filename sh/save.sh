#name_ru_RU Создать локальный пакет
#msg_ru_RU Вы действительно хотите создать локальный пакет?

#name_en_US Create a local package
#msg_en_US Do you really want to create a local package?

#icon 5

#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"

# Запрос названия пакета
read -p "${pkgsave_name}" package_name

# Диалоговое окно выбора каталога для сохранения архива
save_dir=$(zenity --file-selection --directory --title="${path_pkgsave}")

# Проверка, был ли выбран каталог для сохранения
if [ -n "$save_dir" ]; then
    # Скачивание пакета и его зависимостей с помощью yay
    yay -S --noconfirm --answerclean=all --nocleanmenu --nodiffmenu --downloadonly "$package_name"


    # Проверка, был ли пакет успешно скачан
    if [ $? -eq 0 ]; then
        cd "$HOME/.cache/yay/$package_name" && makepkg -f


        # Создание архива скачанных пакетов
        tar czvf "$save_dir/$package_name".tar.gz -C ~/.cache/yay/ "$package_name"
        notify-send "${pkgsave}" "${pkgsave_ok}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
    else
        notify-send "${pkgsave}" "${pkgsave_error}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
    fi
else
    notify-send "${pkgsave}" "${no_direct}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
fi
