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
int request_range(int sockfd, int *lower_i, int *higher_i);
int sum_factors(long number);
int perfects(int sockfd,long lower, long higher);
int write_perfects(int sockfd, long number);
int create_socket();
int connect_socket(int sockfd);
int read_socket(int sockfd, char message[]);
int write_socket(int sockfd,char message[]);
int iops();
int close_socket(int sockfd);
int close_server(int sockfd);

static long iops_i;
static long d_count;

int main(int argc, char const *argv[]){
	iops();
	int sockfd = create_socket();
	connect_socket(sockfd);
	int lower_i;
	int higher_i;
	for(int i = 0; i < 10; i++){
		std::cout << "request" << std::endl;
		request_range(sockfd,&lower_i,&higher_i);
		//close(sockfd);
		perfects(sockfd,lower_i,higher_i);
	}
	//close_server(sockfd);
	close_socket(sockfd);
}

int close_server(int sockfd){
	char message[256];
	sprintf(message, "EXT");
	std::cout << message << std::endl;
	write_socket(sockfd,message);
	bzero(message,256);
	read_socket(sockfd, message);
	std::string str_message = std::string(message);
	std::cout << str_message << std::endl;
	std::istringstream iss(str_message);
	close(sockfd);
}

int close_socket(int sockfd){
	char message[256];
	sprintf(message, "CLS");
	std::cout << message << std::endl;
	write_socket(sockfd,message);
	bzero(message,256);
	read_socket(sockfd, message);
	std::string str_message = std::string(message);
	std::cout << str_message << std::endl;
	std::istringstream iss(str_message);
	close(sockfd);
}

int request_range(int sockfd, int *lower_i, int *higher_i){
	
	char message[256];
	sprintf(message, "REQ %ld", iops_i);
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
		iss >> sub;
		if(count == 1){
			*lower_i = atoi(sub.c_str());
		}else if(count == 2){
			*higher_i = atoi(sub.c_str());
		}
		count++;
	}while(iss);
	//std::cout << *lower_i << " to " << *higher_i<< std::endl;	
}

int iops(){
	clock_t begin_t, end_t;
	int j;
	begin_t = clock();
	for(long i = 1; i <= 1000000000; ++i){
		j	= 10000/i;
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

int perfects(int sockfd, long lower, long higher){
	clock_t begin_t, end_t;
	d_count = 0;
	begin_t = clock();
	for(long i = lower; i <= higher ; ++i){
		if(is_perfect(i)){
			std::cout << i << std::endl;
			write_perfects(sockfd, i);
		}
	}
	end_t = clock();
	iops_i = (end_t - begin_t)*1000000000/d_count;
	
}

int write_perfects(int sockfd, long number){
	//int sockfd2 = create_socket();
	//connect_socket(sockfd);	
	char message[256];
	sprintf(message, "PFN %ld", number);
	std::cout << message << std::endl;
	write_socket(sockfd,message);
	bzero(message,256);
	read_socket(sockfd, message);
	std::string str_message = std::string(message);
	std::cout << str_message << std::endl;
	//close(sockfd2);
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
