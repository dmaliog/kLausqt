#name Убрать пароль SUDO (осторожно!)
#msg Вы действительно хотите убрать пароль sudo для этого пользователя?
#!/bin/bash

# получаем имя текущего пользователя
username=$(whoami)

# проверяем, существует ли файл
if sudo [ -f /etc/sudoers.d/$username ]; then
  # если файл уже существует, то выводим сообщение с опцией "вернуть все"
  read -p "Вы уже убрали пароль. Вы хотите вернуть все? (д/н): " answer
  if [ "$answer" == "д" ]; then
    sudo rm /etc/sudoers.d/$username
    notify-send "Пароль SUDO" "Файл успешно удален" -i $HOME/kLaus/other/notify.png -a "kLaus" -t 10000
  else
    exit 0
  fi
else
    echo "$username ALL=(ALL) NOPASSWD:ALL" | sudo tee /etc/sudoers.d/$username
    notify-send "Пароль SUDO" "Файл успешно создан" -i $HOME/kLaus/other/notify.png -a "kLaus" -t 10000
fi
