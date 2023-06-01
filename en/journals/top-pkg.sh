#name Installed Package Statistics (yay)
#msg 'yay -Ps' shows package usage statistics on your system, including the number of installed, updated and deleted packages, as well as how much time has passed since the last system update. It also shows a list of installed packages that are not dependencies of other packages.
#!/bin/bash
yay -Ps
read -p "Press Enter to continue..."
