#!/bin/bash
#vpn tilkoblingen blir brutt hvert 30 minutt
#sjekkintervall=900 #900=15min
sjekkintervall=20

function vis_tid
{ 
	date +"%F %R";
}

echo "Sjekker med" $sjekkintervall "sekunders mellomrom";

while true; do
	if  ifconfig vpn0 > /dev/null 2>&1 ;then 
		#start inner
		if ping -w1 -c1 www.hw.no > /dev/null;then
			echo -e $(vis_tid)":\tVPN er oppe" >/dev/null; 
		
		else 
			vis_tid;
			echo "VPN-tilkoblingen har falt ut";
			echo "Prøver å gjenopprette den...";

			if ~carl-erik/scripts/fix_vpn.sh; then	
				echo "Oppkobling gjenopprettet";
			else
				echo "Feilet";
			
				#echo "Viser routinginformasjon";
				#route -n
				#echo -e "\n"
			fi;
		fi;#end inner 
	else
		echo "Ingen VPN er konfigurert";
	fi;
	
		sleep $sjekkintervall;
done
