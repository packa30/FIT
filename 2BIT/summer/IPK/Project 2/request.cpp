#include "stdio.h"
#include "unistd.h"
#include "string.h"
#include "net/if.h"
#include "stdlib.h"
#include "arpa/inet.h"
#include "sys/ioctl.h"
#include "netpacket/packet.h"
#include "net/ethernet.h"
#include "net/if_arp.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

void errs(int i)
{
	switch(i){
		case 1:
			printf("\n%s\n\n","Format spustenia: ipk-scanner -i <interface> -f <file>" );
			break;
		case 2:
			printf("\n%s\n\n","IPV4 error" );
			break;
		case 3:
			printf("\n%s\n\n","Wrong interface" );
			break;
		case 4:
			printf("\n%s\n\n","Error creating socket for ARP request");
			break;
		default:
			printf("%s\n","Neznamy error\n" );
	}
	exit(i);
}

struct arp
{
	struct ether_header ether;
	struct arphdr arp;

	uint8_t  send_mac[ETH_ALEN];
	uint32_t send_ip;
	uint8_t  dest_mac[ETH_ALEN];
	uint32_t dest_ip;

	uint8_t  free[18];
} __attribute__ ((__packed__));

struct arp frame(char *argv[],int *myindex,uint8_t mask[4],uint8_t broadcast[4])
{
	int my_int,ret;
	uint32_t my_ip;	
	struct ifreq mefr;

	my_int = socket(AF_INET, SOCK_STREAM, 0);
	if( my_int < 0 ) {
		errs(2);
	}
	memcpy(mefr.ifr_name, argv[2], IF_NAMESIZE);

	if(ioctl(my_int, SIOCGIFNETMASK, &mefr) < 0){errs(2);}
	memcpy(mask,&mefr.ifr_hwaddr.sa_data[2], 4);

	if(ioctl(my_int, SIOCGIFBRDADDR, &mefr) < 0){errs(2);}
	memcpy(broadcast,&mefr.ifr_hwaddr.sa_data[2], 4);

	/*struct in_addr ip_addr;
    ip_addr.s_addr = broadcast;
    printf("The IP address is %s\n", inet_ntoa(ip_addr));*/
	

	ret = ioctl(my_int, SIOCGIFADDR, &mefr, sizeof(mefr));
	if( ret < 0 ){errs(2);}
	struct sockaddr_in *sin = (struct sockaddr_in *)&mefr.ifr_addr;
	my_ip = ntohl(sin->sin_addr.s_addr);

	ret = ioctl(my_int, SIOCGIFINDEX, &mefr, sizeof(mefr));
	if( ret < 0 ){errs(3);}
	*myindex = mefr.ifr_ifindex;

	ret = ioctl(my_int, SIOCGIFHWADDR, &mefr, sizeof(mefr));
	if( ret < 0 ){errs(2);}

	close(my_int);

	struct arp packet;
	memset(packet.ether.ether_dhost, 0xFF, sizeof(packet.ether.ether_dhost));
	memcpy(packet.ether.ether_shost, mefr.ifr_hwaddr.sa_data, sizeof(packet.ether.ether_dhost));
	packet.ether.ether_type = htons(ETHERTYPE_ARP);

	packet.arp.ar_hrd = htons(ARPHRD_ETHER);
	packet.arp.ar_pro = htons(ETHERTYPE_IP);
	packet.arp.ar_hln = ETHER_ADDR_LEN;
	packet.arp.ar_pln = sizeof(packet.send_ip);
	packet.arp.ar_op = htons(ARPOP_REQUEST);
	

	memcpy(packet.send_mac, mefr.ifr_hwaddr.sa_data, sizeof(packet.send_mac));
	packet.send_ip = htonl(my_ip);
	memset(packet.dest_mac, 0 , sizeof(packet.dest_mac));
	packet.dest_ip = inet_addr(argv[4]);
	memset(packet.free, 0 , sizeof(packet.free));
	
	return packet;

}