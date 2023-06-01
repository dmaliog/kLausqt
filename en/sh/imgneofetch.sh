#name Add a picture to neofetch
#msg Do you really want to change the picture in neofetch?
#!/bin/bash

# Checking for the installed package neofetch
if ! which neofetch > /dev/null; then
echo "The neofetch package is not installed, the script is terminated"
else
# File selection using zenity
image_path=$(zenity --file-selection --title="Select an image")

# If the file is not selected, we output a message and continue the script
if [ -z "$image_path" ]; then
notify-send "Picture in neofetch" "File not selected" -i $HOME/kLaus/other/notify.png -a "kLaus" -t 10000
else
# Checking for file availability
if [ ! -f "$image_path" ]; then
notify-send "Picture in neofetch" "File $image_path not found" -i $HOME/kLaus/other/notify.png -a "kLaus" -t 10000
else
# Replacing the corresponding lines in the configuration file
sed -i 's/image_backend=.*/image_backend="iterm2" # Image processing tool/g'~/.config/neofetch/config.conf
sed -i 's|image_source=.*/|image_source="'"$image_path"'" # Image location|g' ~/.config/neofetch/config.conf
notify-send "Picture in neofetch" "Setup completed" -i $HOME/kLaus/other/notify.png -a "kLaus" -t 10000
fi
fi
fi
