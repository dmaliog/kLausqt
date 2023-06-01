#name Install 1C Enterprise [Training Version]
#msg Do you really want to install 1C Enterprise? To install, you will need to download the package from the 1C website (https://online.1c.ru/catalog/free/28766016 /) for Linux.

# Checking for
packages=("enchant1.6" "webkitgtk2-bin" "pkgextract" "imagemagick")
missing_packages=()

for package in "${packages[@]}"; do
if ! pacman -Qs "$package" > /dev/null; then
missing_packages+=("$package")
fi
done

# Installing missing packages via yay
if [ ${#missing_packages[@]} -gt 0 ]; then
yay -S "${missing_packages[@]}"
fi

# Checking for the presence of the folder /opt/1cv8t/
if [ -d "/opt/1cv8t/" ]; then
# The folder exists, ask the user about deleting 1C Enterprise
read -p "1C is already installed. Do you want to delete 1C Enterprise? (d/n):" answer
if [ "$answer" = "d" ]; then
# Removing the previous version of 1C Enterprise
version=$(ls /opt/1cv8t/x86_64/)
sudo /opt/1cv8t/x86_64/$version/uninstaller-training
fi
fi

# Archive selection using zenity
archive=$(zenity --file-selection --title="Select archive" --file-filter='*.tar.gz ' --file-filter='*.tar.gz ')

# Checking the archive availability
if [ -z "$archive" ]; then
notify-send "1C Installer" "Archive not selected" -i $HOME/kLaus/other/notify.png -a "kLaus" -t 10000
exit 0
fi

# Unpacking the archive
tar -xf "$archive" -C /tmp

# Extracting the .run file name from the archive
run_file=$(find /tmp -name "*.run" -type f -print -quit)

# Checking for a .run file
if [ -z "$run_file" ]; then
notify-send "1C Installer" "File .run not found" -i $HOME/kLaus/other/notify.png -a "kLaus" -t 10000
exit 0
fi

# Extract the version from the .run file name
version=$(echo "$run_file" | sed -E 's/.*-([0-9]+\.[0-9]+\.[0-9]+\.[0-9]+)-x86_64\.run/\1/')

# Setting the rights to execute the file .run
chmod +x "$run_file"

# Running the file .run
run_command="sudo $run_file"
$run_command


# Copy libstdc++.so.6 depending on the version
of sudo cp /usr/lib/libstdc++.so.6 /opt/1cv8t/x86_64/$version/libstdc++.so.6
sudo cp /usr/lib/libstdc++.so.6 /opt/1cv8t/common/libstdc++.so.6

echo "Script executed successfully."
