#name Change PKGBUILD package before installation
#msg Do you really want to change the PKGBUILD package before installing?
#!/bin/bash

read -p "Enter the package name: " program_name

yay --getpkgbuild "$program_name"

# Path to PKGBUILD file
pkgbuild_path="$HOME/.cache/yay/$program_name/PKGBUILD"

# Checking for PKGBUILD file
if [ -f "$pkgbuild_path" ]; then
notify-send "PKGBUILD Editor" "PKGBUILD file found for package $program_name" -i $HOME/kLaus/other/notify.png -a "kLaus" -t 10000

# Opening the editor to edit PKGBUILD
read -p "Press Enter to open the editor to edit PKGBUILD..."
$EDITOR "$pkgbuild_path"

# Package Compilation
read -p "Press Enter to start compiling the package..."
cd "$HOME/.cache/yay/$program_name" && makepkg -f

# Installing a new package
read -p "Press Enter to install a new package..."
yay -U "$HOME/.cache/yay/$program_name"/*.pkg.tar.zst

else
notify-send "PKGBUILD editor" "PKGBUILD file not found for package $program_name" -i $HOME/kLaus/other/notify.png -a "kLaus" -t 10000
fi
