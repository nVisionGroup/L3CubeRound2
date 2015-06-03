#include <stdio.h>
#include <pcap.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>
#include <time.h>

 int i=0; 
 char errbuf[PCAP_ERRBUF_SIZE];    /* Error buffer                           */ 
 const unsigned char *packet=NULL; /* Received raw data                      */ 
 time_t rawtime;
 struct tm *info;
 static int count=0,arpcount=0,tcpcount=0;
 char filename[100];
 char* a;
 pcap_t *handle;
 u_char *ptr;


/* ARP Header, (assuming Ethernet+IPv4)            */ 
#define ARP_REQUEST 1   /* ARP Request             */ 
#define ARP_REPLY 2     /* ARP Reply               */ 
typedef struct arphdr1 { 
    u_int16_t htype;    /* Hardware Type           */ 
    u_int16_t ptype;    /* Protocol Type           */ 
    u_char hlen;        /* Hardware Address Length */ 
    u_char plen;        /* Protocol Address Length */ 
    u_int16_t oper;     /* Operation Code          */ 
    u_char sha[6];      /* Sender hardware address */ 
    u_char spa[4];      /* Sender IP address       */ 
    u_char tha[6];      /* Target hardware address */ 
    u_char tpa[4];      /* Target IP address       */ 
}arphdr_t; 

typedef struct {
} Configuration;

struct sniff_ethernet {
	u_char ether_dhost[ETHER_ADDR_LEN]; /* Destination host address */
	u_char ether_shost[ETHER_ADDR_LEN]; /* Source host address */
	u_short ether_type; /* IP? ARP? RARP? etc */
};

#define MAXBYTES2CAPTURE 2048 
arphdr_t *arpheader = NULL;       /* Pointer to the ARP header              */ 

/* IP header */
struct sniff_ip {
        u_char  ip_vhl;                 /* version << 4 | header length >> 2 */
        u_short ip_len;                 /* total length */
        u_char  ip_p;                   /* protocol */
};
#define IP_HL(ip)               (((ip)->ip_vhl) & 0x0f)
#define IP_V(ip)                (((ip)->ip_vhl) >> 4)


/* TCP header */
typedef u_int tcp_seq;

struct sniff_tcp {
        u_short th_sport;               /* source port */
        u_short th_dport;               /* destination port */
        tcp_seq th_seq;                 /* sequence number */
        tcp_seq th_ack;                 /* acknowledgement number */
        u_char  th_offx2;               /* data offset, rsvd */
        #define TH_OFF(th)      (((th)->th_offx2 & 0xf0) >> 4)
        u_char  th_flags;               
        #define TH_FIN  0x01            /* All flags */
        #define TH_SYN  0x02
        #define TH_RST  0x04
        #define TH_PUSH 0x08
        #define TH_ACK  0x10
        #define TH_URG  0x20
        #define TH_ECE  0x40
        #define TH_CWR  0x80
        #define TH_FLAGS        (TH_FIN|TH_SYN|TH_RST|TH_ACK|TH_URG|TH_ECE|TH_CWR)
        u_short th_win;                 /* window */
        u_short th_sum;                 /* checksum */
        u_short th_urp;                 /* urgent pointer */
};



void print_hex_ascii_line(const u_char *payload, int len, int offset)
{

	int i;
	int gap;
	const u_char *ch;

	/* offset */
	printf("%05d   ", offset);
	
	/* hex */
	ch = payload;
	for(i = 0; i < len; i++) {
		printf("%02x ", *ch);
		ch++;
		/* print extra space after 8th byte for visual aid */
		if (i == 7)
			printf(" ");
	}
	/* print space to handle line less than 8 bytes */
	if (len < 8)
		printf(" ");
	
	/* fill hex gap with spaces if not full line */
	if (len < 16) {
		gap = 16 - len;
		for (i = 0; i < gap; i++) {
			printf("   ");
		}
	}
	printf("   ");
	
	/* ascii (if printable) */
	ch = payload;
	for(i = 0; i < len; i++) {
		if (isprint(*ch))
			printf("%c", *ch);
		else
			printf(".");
		ch++;
	}

	printf("\n");

return;
}

/*
 * print packet payload data (avoid printing binary data)
 */
void print_payload(const u_char *payload, int len)
{

	int len_rem = len;
	int line_width = 16;			        /* number of bytes per line */
	int line_len;
	int offset = 0;					/* zero-based offset counter */
	const u_char *ch = payload;

	if (len <= 0)
		return;

	/* data fits on one line */
	if (len <= line_width) {
		print_hex_ascii_line(ch, len, offset);
		return;
	}

	/* data spans multiple lines */
	for ( ;; ) {
		/* compute current line length */
		line_len = line_width % len_rem;
		/* print line */
		print_hex_ascii_line(ch, line_len, offset);
		/* compute total remaining */
		len_rem = len_rem - line_len;
		/* shift pointer to remaining bytes to print */
		ch = ch + line_len;
		/* add offset */
		offset = offset + line_width;
		/* check if we have line width chars or less */
		if (len_rem <= line_width) {
			/* print last line and get out */
			print_hex_ascii_line(ch, len_rem, offset);
			break;
		}
	}

return;
}


void got_packet( Configuration args[], const struct pcap_pkthdr *header, const u_char *packet){
  printf("\n");
  count=count+1;
  printf("--------------------------------------- PACKET #%d ------------------------------ \n",count);
  struct sniff_ethernet *eptr;
  eptr      = (struct sniff_ethernet *) packet;   /* Point to Ethernet header */   
  arpheader = (struct arphdr1 *)(packet+14);    /* Point to the ARP header */ 
  const struct sniff_ip *ip;                    /* The IP header */  
  const struct sniff_tcp *tcp;                  /* The TCP header */
  const char *payload;                          /* Packet payload */
  int size_ip;
  int size_tcp;
  int size_payload;

if (ntohs (eptr->ether_type) == ETHERTYPE_ARP)
{
   arpcount++;
   printf("Ethernet type hex:%x dec:%d is an ARP packet\n",
           ntohs(eptr->ether_type),
           ntohs(eptr->ether_type));


  printf("Hardware type      : %s\n", (ntohs(arpheader->htype) == 1) ? "Ethernet" : "Unknown");     /* hardware type */
  printf("Protocol type      : %s\n", (ntohs(arpheader->ptype) == 0x0800) ? "IPv4" : "Unknown");    /* protocol type */
  printf("Operation          : %s\n", (ntohs(arpheader->oper) == ARP_REQUEST)? "ARP Request" : "ARP Reply");   /* request/reply */
  if (ntohs(arpheader->htype) == 1 && ntohs(arpheader->ptype) == 0x0800){ 
    printf("Sender MAC         : "); 								    /* sender's MAC */

    for(i=0; i<6;i++)
        printf("%02X:", arpheader->sha[i]); 

    printf("\nSender IP          : "); 								   /* sender's IP */

    for(i=0; i<4;i++)
        printf("%d.", arpheader->spa[i]); 

    printf("\nTarget MAC         : "); 								  /* Target MAC  */

    for(i=0; i<6;i++)
        printf("%02X:", arpheader->tha[i]); 

    printf("\nTarget IP          : "); 								/* Target IP */

    for(i=0; i<4; i++)
        printf("%d.", arpheader->tpa[i]); 
    
    printf("\n"); }
}

    ptr = eptr->ether_dhost;
    i = ETHER_ADDR_LEN;
    printf("Destination Address: ");							      /* Destination address */
    do{
        printf("%s%x",(i == ETHER_ADDR_LEN) ? " " : ":",*ptr++);
    }while(--i>0);
    printf("\n");

    ptr = eptr->ether_shost;
    i = ETHER_ADDR_LEN;
    printf("Source Address     : ");							    /* Source address */
    do{  
        printf("%s%x",(i == ETHER_ADDR_LEN) ? " " : ":",*ptr++);
    }while(--i>0);
    printf("\n");
    rawtime=header->ts.tv_sec;
    info = localtime( &rawtime );
    printf("Timestamp          : %s", asctime(info));					   /* Timestamp */
    printf("Length of packet   : [%d]\n", header->len);					   /* length of packet(offwire) */
    printf("Length of packet present \n                   : [%d]\n", header->caplen);      /*length of portion present */
    
   



if (ntohs (eptr->ether_type) == ETHERTYPE_IP)
{
   ip = (struct sniff_ip*)(packet + 14);
   size_ip = IP_HL(ip)*4;
   if (size_ip < 20) {
	printf("   * Invalid IP header length: %u bytes\n", size_ip);
	return;}
   

   tcp = (struct sniff_tcp*)(packet + 14 + size_ip);					/* Point to TCP packet */
   size_tcp = TH_OFF(tcp)*4;
   tcpcount++;
   if (size_tcp < 20) {
	printf("   * Invalid TCP header length: %u bytes\n", size_tcp);
	return;
}
   	
   printf("Src port           : %d\n", ntohs(tcp->th_sport));				/* Source port */
   printf("Dst port           : %d\n", ntohs(tcp->th_dport));				/* Destination port */
   printf("Sequence No        : %d\n", ntohs(tcp->th_seq));				/* Sequence number */
   printf("Acknowledge        : %d\n", ntohs(tcp->th_ack));				/* Acknowledgement number */
   printf("Window             : %d\n", ntohs(tcp->th_win));				/* Window */
   printf("checksum           : %d\n", ntohs(tcp->th_sum));				/* Checksum number */
   printf("FLAGS which are set: \n");							/* Flags which are set */ 

   if (tcp->th_flags & TH_ECE){
   printf(" TH_ECE");
   }
   if (tcp->th_flags & TH_RST){
   printf(" TH_RST");
   }
   if (tcp->th_flags & TH_FIN){
   printf(" TH_FIN");
   }
   if (tcp->th_flags & TH_SYN){
   printf(" TH_SYN");
   }
   if (tcp->th_flags & TH_PUSH){
   printf(" TH_PUSH");
   }
   if (tcp->th_flags & TH_ACK){
   printf(" TH_ACK");
   }
   if (tcp->th_flags & TH_URG){
   printf(" TH_URG");
   }
   if (tcp->th_flags & TH_CWR){
   printf(" TH_CWR");
   }
   printf("\nEthernet type hex:%x dec:%d is an TCP/IP packet\n",
   ntohs(eptr->ether_type),
   ntohs(eptr->ether_type));

   /* define/compute tcp payload (segment) offset */ 
   payload = (u_char *)(packet + 14 + size_ip + size_tcp);
	
  /* computing tcp payload (segment) size */ 
  size_payload = ntohs(ip->ip_len) - (size_ip + size_tcp);
	
  /*
  * Print payload data; */
  if (size_payload > 0) {
	printf("   Payload (%d bytes):\n", size_payload);
	print_payload(payload, size_payload);
   }
	
 }

}


int main()
{
    printf("Enter the file name: \n"); 
    scanf("%s", filename);
    FILE *fp = fopen( filename, "r" );
    if(fp == NULL){
    printf("Cannot find/open file...try again ");
    }
    handle = pcap_fopen_offline(fp,a);      /* session handler */
    printf("pcap open");

    pcap_loop(handle, -1, (pcap_handler)got_packet, NULL); /* function to process packets till end  by calling callback function(got_packet) */    
    
    pcap_close(handle);				/* session handler close */
    return 0; 

}
