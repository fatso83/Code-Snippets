#!/bin/sh

#INCOMING=/mnt/gigant/upload
INCOMING=/mnt/FATstore/upload
TORRENTDIR="/mnt/gigant/Utvidet hjemmemappe/torrents"
#TORRENTDIR="/mnt/gigant/Utvidet hjemmemappe/torrents/upload"

echo "Starter torrentflyttingsprogram..."
echo IN: "$INCOMING" 
echo OUT:"$TORRENTDIR"

cd "$INCOMING"
while [ TRUE ]; do
	if ls *.torrent 2>/dev/null;then
		mv *.torrent "$TORRENTDIR"
	fi
	
	/bin/echo -n .
	sleep 30
done
