#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"
helper="$2"

# Запрос названия пакета
read -p "${pkgsave_name}" package_name

# Диалоговое окно выбора каталога для сохранения архива
save_dir=$(zenity --file-selection --directory --title="${path_pkgsave}")

# Проверка, был ли выбран каталог для сохранения
if [ -n "$save_dir" ]; then
    # Скачивание пакета и его зависимостей с помощью $helper
    if [ "$helper" = "yay" ]; then
        $helper -S --noconfirm --answerclean=all --nocleanmenu --nodiffmenu --downloadonly "$package_name"
    else
        $helper -S --noconfirm --answerclean=all --nocleanmenu --nodiffmenu --downloadonly "$package_name" --skipreview
    fi

    # Проверка, был ли пакет успешно скачан
    if [ $? -eq 0 ]; then
        cd "$HOME/.cache/$helper/$package_name" && makepkg -f


        # Создание архива скачанных пакетов
        tar czvf "$save_dir/$package_name".tar.gz -C ~/.cache/$helper/ "$package_name"
        notify-send "${pkgsave}" "${pkgsave_ok}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
    else
        notify-send "${pkgsave}" "${pkgsave_error}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
    fi
else
    notify-send "${pkgsave}" "${no_direct}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
fi
