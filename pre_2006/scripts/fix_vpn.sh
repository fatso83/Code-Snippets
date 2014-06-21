#!/bin/bash
rootuser="0"

if ifconfig vpn0 &> /dev/null; then
	if [ $(id -u) = $rootuser ];then
		vpnc-disconnect
		sleep 2; #otherwise the connectin may not be set up
		vpnc-connect ntnu
	else
		#Cannot continue or get anything done 	
		exit 1
	fi

	#Test if the resetting was succesful
	ifconfig vpn0 &> /dev/null &&\
		ping -w1 -c1 www.hw.no > /dev/null; #is in /etc/hosts
	if [ $? != 0 ]; then
		exit 1;
	else
		exit 0;
	fi
fi	

#failed first. no vpn
exit 1;

#end-of-script
