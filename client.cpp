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
#include <sstream>

#define PORT 7331

long factor_pair(long number, long factor);
bool is_perfect(long number);
int sum_factors(long number);
int perfects(long lower, long higher);
int create_socket();
int connect_socket(int sockfd);
int read_socket(int sockfd, char message[]);
int write_socket(int sockfd,char message[]);
int iops();

static int iops_i;
static int d_count;

int main(int argc, char const *argv[]){
	int sockfd = create_socket();
	connect_socket(sockfd);
	char message[256];
	sprintf(message, "IOPS %i", iops());
	std::cout << message << std::endl;
	write_socket(sockfd,message);
	bzero(message,256);
	read_socket(sockfd, message);
	std::string str_message = std::string(message);
	std::cout << str_message << std::endl;
	std::istringstream iss(str_message);
	int count = 0;
	std::string lower_s;
	std::string higher_s; 
	do{
		std::string sub;
		if(count == 2){
			iss >> lower_s;
		}else if(count == 3){
			iss >> higher_s;
		}else{
			iss >> sub;
		}
		count++;
	}while(iss);
	int lower_i = atoi(lower_s.c_str());
	int higher_i = atoi(higher_s.c_str());
	std::cout << atoi(lower_s.c_str()) << " to " << atoi(higher_s.c_str())<< std::endl;
	perfects(lower_i,higher_i);
	std::cout << "New IOPS " << iops_i << std::endl;
}

int iops(){
	clock_t begin_t, end_t;
	begin_t = clock();
	for(int i = 1; i <= 1000000000; ++i){
		int j = 10000/i;
	}
	end_t = clock();
	iops_i = 3*(end_t - begin_t);
	//std::cout << ((double)(end_t - begin_t))/CLOCKS_PER_SEC << std::endl;
	return iops_i;
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

int read_socket(int sockfd, char message[]){
	if(read(sockfd,message, 255)< 0){
		//error
	}
}

int write_socket(int sockfd,char message[]){
	if (write(sockfd, message, strlen(message))< 0){
		//error
	}
	
}

int perfects(long lower, long higher){
	clock_t begin_t, end_t;
	begin_t = clock();
	for(int i = lower; i <= higher ; ++i){
		if(is_perfect(i)){
			std::cout << i << std::endl;
		}
	}
	end_t = clock();
	iops_i = (end_t - begin_t)*1000000000/d_count;
	
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
	d_count +=2;
	return 0;
}
