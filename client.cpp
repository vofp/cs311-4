#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <math.h>

#define PORT 7331

long factor_pair(long number, long factor);
bool is_perfect(long number);
int sum_factors(long number);
int perfects(long lower, long higher);

int main(int argc, char const *argv[]){
	long lower = strtol( argv[1], NULL, 10 );
	long higher = strtol( argv[2], NULL, 10 );
	perfects(lower,higher);
}

int create_socket(){
	int sockfd
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		//error
	}	
	return sockfd;
}

int connect_socket(int sockfd){
	struct sockaddr_in serveraddr;	
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = gethostbyname("os-class.oregonstate.edu");
	serveraddr.sin_port = htons(PORT);
	memset(&(serveraddr.sin_zero), '\0', 8);
	if(connect(sockfd,(struct sockaddr *) &serveraddr, sizeof(serveraddr))< 0){
		//error
	}
}

int read_socket(){
	char message[256];
	if(read(sockfd,message, 255)< 0){
		//error
	}
}

int write socket(int sockfd,message){
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
