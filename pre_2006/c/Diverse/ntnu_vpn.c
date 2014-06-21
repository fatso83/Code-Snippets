#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

int
main()
{
	int status;

	printf("Setter opp VPN tilkoblingen mot NTNU");
 	status = system("/usr/sbin/vpnc ntnu");
	if( status == -1 ){
		perror("Det har skjedd en feil i VPN-oppkoblingen!");
		exit(1);
	}

	system("echo 0 > /proc/sys/net/ipv4/conf/ath0/rp_filter");
	system("echo 0 > /proc/sys/net/ipv4/conf/vpn0/rp_filter");

	return 0;
}
