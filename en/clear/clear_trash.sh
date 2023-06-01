#name Trash (forced)
#msg Do you really want to delete all files and folders from the trash?

#!/bin/bash
  TRASH_DIR=$(find ~/ -name Trash)
  rm -rf "$TRASH_DIR"/*
  notify-send "Trash" "The trash has been successfully emptied" -i $HOME/kLaus/other/notify.png -a "kLaus" -t 10000
