#name List of installed packages (from third-party repositories)
##msg 'yay -Qm' shows a list of all packages that were installed into the system manually, that is, not from official repositories, but from third-party sources, such as AUR (Arch User Repository).
#!/bin/bash
yay -Qm
read -p "Press Enter to continue..."
