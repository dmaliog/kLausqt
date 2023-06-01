#name Create a local package
#msg Do you really want to create a local package?
#!/bin/bash

# Request the name of the package
read -p "Enter the name of the package to download: " package_name

# Dialog box for selecting a directory to save the archive
save_dir=$(zenity --file-selection --directory --title="Select a directory to save the archive")

# Checking whether a directory has been selected to save
if [ -n "$save_dir" ]; then
# Downloading a package and its dependencies using yay
yay -S --noconfirm --answerclean=all --nocleanmenu --nodiffmenu --downloadonly "$package_name"


# Checking if the package was downloaded successfully
if [ $? -eq 0 ]; then
cd "$HOME/.cache/yay/$package_name" && makepkg -f


# Creating an archive of downloaded packages
tar czvf "$save_dir/$package_name".tar.gz -C ~/.cache/yay/ "$package_name"
notify-send "Creating local packages" "Packages $package_name and its dependencies have been successfully archived" -i $HOME/kLaus/other/notify.png -a "kLaus" -t 10000
else
notify-send "Creating local packages" "Error downloading package $package_name" -i $HOME/kLaus/other/notify.png -a "kLaus" -t 10000
fi
else
notify-send "Creating local packages" "No save directory selected" -i $HOME/kLaus/other/notify.png -a "kLaus" -t 10000
fi
