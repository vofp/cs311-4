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
#include <pthread.h>
#include <csignal>

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
void close_socket(int sockfd);
int close_server(int sockfd);
int finish_range(int sockfd, int lower_i, int higher_i);
int create_listener();
void *listener_code(void *data);

static long iops_i;
static long d_count;
static int sockfd;

int main(int argc, char const *argv[]){
	
	// Setting first number for iops_i
	iops();

	// Opens and connects to server
	sockfd = create_socket();
	connect_socket(sockfd);
	
	// creats thread that listen for a kill message
	create_listener();
	
	// Handles kill and interrupt signals
	signal(SIGTERM,close_socket);
	signal(SIGINT,close_socket);

	int lower_i;
	int higher_i;
	
	// calculate perfects
	for(int i = 0; ; i++){
		request_range(sockfd,&lower_i,&higher_i);
		perfects(sockfd,lower_i,higher_i);
		finish_range(sockfd,lower_i,higher_i);
	}
}

/* iops() calculates out many division in 15 secs
 */
int iops(){
	clock_t begin_t, end_t;
	//int j;
	begin_t = clock();
	for(long i = 1; i <= 1000000000; ++i){
		int j	= 10000/i;
	}
	end_t = clock();
	iops_i = 15/(((double)(end_t - begin_t))/CLOCKS_PER_SEC)*1000000000;
	return iops_i;
}

/* This creates a socket to os-class.engr,oregonstate.edu
 */
int create_socket(){
	int sockfd;
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		//error
	}	
	return sockfd;
}


/* This creates to the sockets
 */
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

/* This creates another socket to the same server.
 * Then opens a thread that will wait for a reply
 * if there is a reply, then it will close program
 */
int create_listener(){
	pthread_t listener;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	int i = 0;
	pthread_create(&listener,&attr, listener_code, (void *)i);
}

/* This is the thread code waiting for a reply to close the program
 */
void *listener_code(void *data){
	int sockfd = create_socket();
	connect_socket(sockfd);
	
	char message[256];
	sprintf(message, "SIG");
	write_socket(sockfd,message);
	bzero(message,256);
	read_socket(sockfd, message);
	std::cout << "end message: "<< message << std::endl;
	
	close(sockfd);
	exit(0);
}

/* This will ask the server a range to check
 */
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

/* This searchs a range and when it finds a perfect number
 * it will send it to the server
 */
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
	std::cout << d_count << " in " << ((double)end_t - begin_t)/CLOCKS_PER_SEC << std::endl;
	iops_i = (15/(((double)end_t - begin_t)/CLOCKS_PER_SEC)) * iops_i;	
}
/* sends a perfect to server
 */
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
/* checks if the sum of the factors is equal to the number
 */
bool is_perfect(long number){
	return sum_factors(number) == number;
}
/* Finds the sum of the factors
 */
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
/* finds if there is a factor_pair
 * it checks if the factor is a factor of number
 * returns 0 if factor is not a factor
 * returns pos-int that is the factor pair of factor
 */
long factor_pair(long number, long factor){
	if(number % factor == 0){
		return number / factor;	
	}
	d_count +=2;
	return 0;
}
/* Sends message to server that we finished this range
 */
int finish_range(int sockfd, int lower_i, int higher_i){
	
	char message[256];
	sprintf(message, "DON %i %i", lower_i, higher_i);
	std::cout << message << std::endl;
	write_socket(sockfd,message);
	bzero(message,256);
	read_socket(sockfd, message);
}
/* Simplify read and write functions to server
 */
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

/* Sends a message to server to close
 */
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

/* Sends a message to server that this is closing
 */
void close_socket(int sockfd22){
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
	exit(0);
}
