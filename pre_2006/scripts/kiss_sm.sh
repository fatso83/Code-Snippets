    #!/bin/sh
    eval `ssh-agent`
    gnome-settings-daemon &
    gnome-panel &
    nautilus -n &
    beagled --bg &
    gnome-screensaver &
    exec metacity
