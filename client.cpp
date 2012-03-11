#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h> 
#include <string.h>

#define PORT 7331

long factor_pair(long number, long factor);
bool is_perfect(long number);
int sum_factors(long number);
int perfects(long lower, long higher);
int create_socket();
int connect_socket(int sockfd);
int read_socket(int sockfd);
int write_socket(int sockfd,char message[]);
int iops();


int main(int argc, char const *argv[]){
	int sockfd = create_socket();
	connect_socket(sockfd);
	char message[256];
	sprintf(message, "IOPS %i", iops());
	std::cout << message << std::endl;
	write_socket(sockfd,message);
	read_socket(sockfd);
}

int iops(){
	clock_t begin_t, end_t;
	begin_t = clock();
	for(int i = 1; i <= 1000000000; ++i){
		int j = 10000/i;
	}
	end_t = clock();
	return end_t - begin_t;
}

int create_socket(){
	int sockfd;
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		//error
	}	
	return sockfd;
}

int connect_socket(int sockfd){
	struct sockaddr_in serveraddr;	
	serveraddr.sin_family = AF_INET;
	struct hostent *server;
	server = gethostbyname("os-class.engr.oregonstate.edu");
	bcopy((char *)server->h_addr, 
         (char *)&serveraddr.sin_addr.s_addr,
         server->h_length);
	serveraddr.sin_port = htons(PORT);
	memset(&(serveraddr.sin_zero), '\0', 8);
	if(connect(sockfd,(struct sockaddr *) &serveraddr, sizeof(serveraddr))< 0){
		//error
	}
}

int read_socket(int sockfd){
	char message[256];
	if(read(sockfd,message, 255)< 0){
		//error
	}
	std::cout << message << std::endl;
}

int write_socket(int sockfd,char message[]){
	if (write(sockfd, message, strlen(message))< 0){
		//error
	}
	
}

int perfects(long lower, long higher){

	for(int i = lower; i <= higher ; ++i){
		if(is_perfect(i)){
			std::cout << i << std::endl;
		}
	}
	
}

bool is_perfect(long number){
	return sum_factors(number) == number;
}

int sum_factors(long number){
	long sum = 0;
	for(long factor = 2; factor <= sqrt(number); ++factor){
		long pair;
		if((pair = factor_pair(number, factor)) != 0){
			if(number != factor){
				sum = sum + factor + pair;
				if(factor == pair){
					sum -= factor;
				}
			}
		}
	}
	return sum + 1;
}

long factor_pair(long number, long factor){
	if(number % factor == 0){
		return number / factor;	
	}
	return 0;
}
