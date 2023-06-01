#name List of installed packages (from official repositories)
#msg 'yay -Qn' shows a list of installed packages that are not dependencies of other packages (i.e. that were installed explicitly, and not as a result of installing other packages).
#!/bin/bash
yay -Qn
read -p "Press Enter to continue..."
