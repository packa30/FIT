#include "request.cpp"
#include <inttypes.h>
#include <string.h>

volatile sig_atomic_t interrupted=false; 
void sigintHandler(int sig_num)
{
	interrupted=true;
}
int main(int argc, char *argv[])
{
	int ret,arp;
	struct arp packet;
	int myindex;
	uint32_t store_ip;
	uint8_t mask[4];
	uint8_t broadcast[4];
	uint8_t net[4];
	FILE *file;

	signal(SIGINT, sigintHandler);

	char uv='"';
	if(argc != 5){errs(1);}
	if(strcmp(argv[3],"-f")!=0 || strcmp(argv[1],"-i")!=0){errs(1);}
	if((file=fopen(argv[4],"w"))==NULL)
		errs(1);
	else
	{	
		fprintf(file, "<?xml version=\"1.0\"?>\n<devices>\n");
		fflush(file);
	}
	packet=frame(argv,&myindex,mask,broadcast);

	memcpy(net, &packet.send_ip, sizeof(packet.send_ip));

	for(int i=0;i<4;i++)
	{
		net[i] = mask[i] & net[i];
	}
	net[3]+=1;
	//printf("%u.%u.%u.%u\n",net[0],net[1],net[2],net[3]);

	
	arp = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
	if( arp == -1 ){errs(4);}

	struct sockaddr_ll sa;
	sa.sll_family   = AF_PACKET;
	sa.sll_protocol = htons(ETH_P_ARP);
	sa.sll_ifindex  = myindex;
	sa.sll_hatype   = ARPHRD_ETHER;
	sa.sll_pkttype  = PACKET_BROADCAST;
	sa.sll_halen    = 0;

	//memcpy(sa.sll_addr, ADDR_MAC_BROADCAST, LEN_ADDR_MAC);
	
	struct in_addr ip_addr;

	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 10000;
	if (setsockopt(arp, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {errs(2);}
	struct arp packet2;
	pid_t pid;

	while((net[0] <= broadcast[0]) && (net[1] <= broadcast[1]) && (net[2] <= broadcast[2]) && (net[3] < broadcast[3]))
	{
		while( net[3] <= 255 )
		{
			packet.dest_ip=net[0] | (net[1] << 8) | (net[2] << 16) | (net[3] << 24);
			//printf("%u.%u.%u.%u\n",net[0],net[1],net[2],net[3]);
			if( ret = sendto(arp, &packet, sizeof(packet), 0,(struct sockaddr *)&sa, sizeof(sa)) < 0 ){errs(4);}

			if(interrupted==true)
    		{
    			while (wait(NULL) > 0);
    			fprintf(file, "</devices>");
    			fflush(file);
    			fclose(file);
    			close(arp);
    			exit(1);
    		}

			pid=fork();
			while (wait(NULL) > 0);
			//usleep(10000);
			if( pid == 0)
			{
				break;
			}

			if(net[3] < 255)
				net[3]+=1;
			else
				break;
			
		}
		if(pid == 0)
			break;
		if((net[0] <= broadcast[0]) && (net[1] <= broadcast[1]) && (net[2] <= broadcast[2]))
		{
			net[3]=0;
			if(net[2] == 255)
			{
				net[2]=0;
				if(net[1] == 255)
				{
					net[1]=0;
					if(net[0] == 255)
						errs(2);
					else
						net[0]+=1;
				}
				else
					net[1]+=1;
			}
			else
				net[2]+=1;
		}

	}

	if(pid==0)
	{
		ret=recv(arp,&packet2,sizeof(packet2),0);
		if ( ret >= 0 )
		{
			ip_addr.s_addr = packet2.send_ip;
			/*if((file=fopen(argv[4],"w"))==NULL)
				errs(1);
			else
			{*/
			fprintf(file, "\t<host mac=\"%02x:%02x:%02x:%02x:%02x:%02x\">\n\t\t<ipv4>%s</ipv4>\n\t</host>\n",
			packet2.send_mac[0] & 0xff, packet2.send_mac[1] & 0xff, packet2.send_mac[2] & 0xff,
			packet2.send_mac[3] & 0xff, packet2.send_mac[4] & 0xff, packet2.send_mac[5] & 0xff,inet_ntoa(ip_addr));
    		fflush(file);
    		//}
    		//fclose(file);

    		/*printf("The IP address is %s\n", inet_ntoa(ip_addr));

			printf("MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
   	   		packet2.send_mac[0] & 0xff, packet2.send_mac[1] & 0xff, packet2.send_mac[2] & 0xff,
   	  	 	packet2.send_mac[3] & 0xff, packet2.send_mac[4] & 0xff, packet2.send_mac[5] & 0xff);*/
		}
	}
	if(pid!=0)
	{
		int j;
		while (wait(NULL) > 0);//{printf("%d child completed\n", j++);}
		fprintf(file,"</devices>");
		close(arp);
		return 0;
	}
	//exit(0);
}
