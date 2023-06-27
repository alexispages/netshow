#include <stdio.h>
#include <stdlib.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <string.h>

// Count the number of bits value to 1
int bit_count(uint32_t i){
	int count_one = 0;
	unsigned int see_one = 0;

	while (i > 0) {
		if (i & 1) {
			see_one = 1;
			count_one++;
		}
    else {
			if (see_one) {
				return -1;
			}
		}
		i >>= 1;
	}
	return count_one;
}

// Return prefix form (/x) with a given X.X.X.X netmask
int mask2prefix_ipv4(struct in_addr * mask){
	return bit_count(ntohl(mask->s_addr)); // ntohl convert from network byte order to host byte order
}

// Return prefix form (/x) with a given X:X:X:X:X:X:X:X netmask
int mask2prefix_ipv6(unsigned char * c){
  int i = 0;
  int j = 0;
  unsigned char n = 0;
  while (i < 16) {
    n = c[i];
    while (n > 0) {
      if (n & 1) j++;
      n = n/2;
    }
    i++;
  }
  return j;
}

// Display of an error message with the correct formatting
void error(char * msg){
  printf("\033[1;31mError : %s \033[0m\n\n", msg);
}

// Prints the list of system interfaces
void list_interfaces(struct ifaddrs * interfaces){
    if(interfaces == NULL){
      error("No available interface on your system");
      exit(1);
    }
    printf("System interfaces :\n\n");
    int id = 1;
    while (interfaces->ifa_next != NULL) {
      if(interfaces->ifa_addr->sa_family == AF_PACKET){
        printf("%d: %s\n", id, interfaces->ifa_name); // format "id: iname" 
        id++;
      }
      interfaces = interfaces->ifa_next;
    }
}

// Return the address and netmask of an ifname passed into parameter
void show_interface(struct ifaddrs * interfaces, char * ifname){
  int i = 0;
  char addr_str[40]; // Max IPv6 address length
  int netmask;
  printf("%s :\n\n",ifname);
  while (interfaces->ifa_next != NULL) {
    void *tempAddrPtr = NULL;
      if(strcmp(interfaces->ifa_name,ifname) == 0){
        if (interfaces->ifa_addr->sa_family == AF_INET) { // is ifname an IPv4 address
            tempAddrPtr = &((struct sockaddr_in *)interfaces->ifa_addr)->sin_addr; // get ifname address
            inet_ntop(interfaces->ifa_addr->sa_family,tempAddrPtr, addr_str,INET_ADDRSTRLEN); // convert address from binary to text
            tempAddrPtr = &((struct sockaddr_in *)interfaces->ifa_netmask)->sin_addr; // get ifname netmask
            int prefix = mask2prefix_ipv4((struct in_addr*)tempAddrPtr); // convert netmask into prefix form
            printf("inet %s/%d\n",addr_str,prefix); // format : inet addr/prefix
        }
        
        // Same steps for IPv6 addresses
        else if(interfaces->ifa_addr->sa_family == AF_INET6){
          tempAddrPtr = &((struct sockaddr_in6 *)interfaces->ifa_addr)->sin6_addr;
          inet_ntop(interfaces->ifa_addr->sa_family,tempAddrPtr, addr_str,INET6_ADDRSTRLEN);
          tempAddrPtr = &((struct sockaddr_in6 *)interfaces->ifa_netmask)->sin6_addr;
          unsigned char *netmask_char = ((struct sockaddr_in6 *)interfaces->ifa_netmask)->sin6_addr.s6_addr; // convert netmask into char
          netmask = mask2prefix_ipv6(netmask_char); // convert netmask into prefix form
          printf("inet6 %s/%d\n",addr_str,netmask); // format : inet6 addr/prefix
        }
        i++;
      }
      interfaces = interfaces->ifa_next;
  }
  if(i <= 0){
    error("This interface doesn't exist");
    exit(2);
  }
}

int main(int argc, char *argv[]) {

  struct ifaddrs * interfaces;
  getifaddrs(&interfaces);

  if(argc <= 1) {
    error("Missing an option");
    printf("Usage : netshow [ OPTIONS ] OBJECT\n\n");
    printf("OPTIONS :\n");
    printf("-a [list of system interfaces]\n");
    printf("-i OBJECT [address and net mask of a given interface]\n\n");
    printf("OBJECT :\n");
    printf("ifname [interface name]\n\n");
  }

  else if (strcmp(argv[1],"-a") == 0) {
  /* Give a linked list of system available interfaces
  part of "ifaddrs.h" library */
  list_interfaces(interfaces);
  }
  
  else if (strcmp(argv[1],"-i") == 0) {
  // Show IPv4 and IPv6 adresses of a given interface in argument
  show_interface(interfaces,argv[2]);
  }
  else {
    error("Unknown argument");
    printf("Usage : netshow [ OPTIONS ] OBJECT\n\n");
    printf("OPTIONS :\n");
    printf("-a [list of system interfaces]\n");
    printf("-i OBJECT [address and net mask of a given interface]\n\n");
    printf("OBJECT :\n");
    printf("ifname [interface name]\n\n");
  }

  freeifaddrs(interfaces);
  return 0;
}
