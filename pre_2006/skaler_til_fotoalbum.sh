#!/bin/sh
SZ="640x480"
#SZ="1024x768"
KVALITET=85
TILLATT="jpg png gif tiff bmp"
DEBUG=TRUE


#Er snill mot andre prosesser
renice 10 $$ 1>/dev/null

skaler(){
	if [ x$DEBUG = xTRUE ];then
		echo "Konverterer: $ORIGINALBILDE"
	fi


	#Skaler ned til riktig størrelse
	convert	-quality $KVALITET \
		-geometry $SZ\
		 "$ORIGINALBILDE" "$NYTTBILDE"
}

if [ "x$1" = "x" ]; then
	echo Bruk: $0 bilde1 [bilde2]
fi

#Start loop
for ORIGINALBILDE in "$@"; do
	#ORIGINALBILDE="$1"
	#Skaff endelse
	EXT=$(echo $ORIGINALBILDE | sed 's/.*\.\(..*$\)/\1/')
	#Skap et navn for det nye bildet fra det gamle
	NYTTBILDE=$(basename "$ORIGINALBILDE" $EXT)skalert.jpg

	if [ -e $NYTTBILDE ];then
		#Bildet $NYTTBILDE finnes allerede
		continue;
	elif (echo $ORIGINALBILDE | grep skalert);then
		#Tidligere skalert
		continue;
	fi

	for endelse in $TILLATT;do
		if [ $(echo "$EXT"|tr '[:upper:]' '[:lower:]') = "$endelse" ];then
			skaler
		fi
	done
	
#Ferdig
done

