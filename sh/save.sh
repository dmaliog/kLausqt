#name_ru_RU Создать локальный пакет
#msg_ru_RU <b>Pacman</b> — Пакетный менеджер который находит гармонию между двоичным форматом пакетов и простой в использовании системой сборки. Основная цель Pacman - обеспечить удобное управление пакетами, будь то из официальных репозиториев или созданных вручную.<br><br>Pacman поддерживает вспомогательные инструменты, такие как Paru или Yay, расширяя его функциональность. Благодаря этим инструментам Pacman становится еще более гибким.<br><br>Скрипт создает локальный пакет с использованием вспомогательных инструментов Pacman.

#name_en_US Create a local package
#msg_en_US <b>Pacman</b> is a package manager that finds harmony between the binary format of packages and an easy—to-use build system. Pacman's main goal is to provide convenient package management, whether from official repositories or created manually.<br><br>Pacman supports auxiliary tools such as Paru or Yay, extending its functionality. Thanks to these tools, Pacman becomes even more flexible.<br><br>The script creates a local package using Pacman auxiliary tools.

#icon system-software-install

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
