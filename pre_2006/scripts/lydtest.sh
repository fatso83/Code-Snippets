length=500; i=1000; while true; do let i=i*51/50; let a=i; setterm -bfreq $a -blength $length; /bin/echo -e "\a"; echo "$a";done
