#name Создать локальный пакет
#msg Вы действительно хотите создать локальный пакет?
#!/bin/bash

# Запрос названия пакета
read -p "Введите название пакета для скачивания: " package_name

# Диалоговое окно выбора каталога для сохранения архива
save_dir=$(zenity --file-selection --directory --title="Выберите каталог для сохранения архива")

# Проверка, был ли выбран каталог для сохранения
if [ -n "$save_dir" ]; then
    # Скачивание пакета и его зависимостей с помощью yay
    yay -S --noconfirm --answerclean=all --nocleanmenu --nodiffmenu --downloadonly "$package_name"


    # Проверка, был ли пакет успешно скачан
    if [ $? -eq 0 ]; then
        cd "$HOME/.cache/yay/$package_name" && makepkg -f


        # Создание архива скачанных пакетов
        tar czvf "$save_dir/$package_name".tar.gz -C ~/.cache/yay/ "$package_name"
        notify-send "Создание локальных пакетов" "Пакеты $package_name и его зависимости успешно сохранены в архиве" -i $HOME/kLaus/other/notify.png -a "kLaus" -t 10000
    else
        notify-send "Создание локальных пакетов" "Ошибка при скачивании пакета $package_name" -i $HOME/kLaus/other/notify.png -a "kLaus" -t 10000
    fi
else
    notify-send "Создание локальных пакетов" "Каталог для сохранения не выбран" -i $HOME/kLaus/other/notify.png -a "kLaus" -t 10000
fi
