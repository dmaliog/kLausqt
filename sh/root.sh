#name_ru_RU Убрать пароль SUDO (осторожно!)
#msg_ru_RU Вы действительно хотите убрать пароль sudo для этого пользователя?
#name_en_US Remove SUDO password (caution!)
#msg_en_US Do you really want to remove sudo password for this user?
#!/bin/bash
# Определение языка
language="en_US"
if [ -n "$1" ]; then
    language="$1"
fi

# Загрузка файла перевода
translations_file="translations_$language.txt"
source "$HOME/kLaus/other/$translations_file"

# получаем имя текущего пользователя
username=$(whoami)

# проверяем, существует ли файл
if sudo [ -f /etc/sudoers.d/$username ]; then
  # если файл уже существует, то выводим сообщение с опцией "вернуть все"
  read -p "${sudo_ok}" answer
  if [ "$answer" == "y" ]; then
    sudo rm /etc/sudoers.d/$username
    notify-send "${sudo}" "${removefile_ok}" -i $HOME/kLaus/other/notify.png -a "kLaus" -t 10000
  else
    exit 0
  fi
else
    echo "$username ALL=(ALL) NOPASSWD:ALL" | sudo tee /etc/sudoers.d/$username
    notify-send "${sudo}" "${createfile_ok}" -i $HOME/kLaus/other/notify.png -a "kLaus" -t 10000
fi
