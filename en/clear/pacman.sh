#name Downloaded Pacman Packages
#msg/var/cache/pacman/pkg: Downloaded Pacman packages are stored in this folder. If you do not plan to roll back to previous versions of packages, then you can delete the old packages to free up space. However, keep in mind that if you want to delete packages, you will need to download them again if necessary.
#!/bin/bash

# Подсчет размера папки /var/cache/pacman/pkg/
folder_size=$(du -sh /var/cache/pacman/pkg/ | awk '{print $1}')

# Вывод размера папки
notify-send "Pacman Packages" "Cleaning Size: $folder_size" -i $HOME/kLaus/other/notify.png -a "kLaus" -t 10000

# Очистка папки /var/cache/pacman/pkg/
sudo rm -rf /var/cache/pacman/pkg/*
notify-send "Pacman Packages" "Catalog /var/cache/pacman/pkg/ cleared" -i $HOME/kLaus/other/notify.png -a "kLaus" -t 10000

