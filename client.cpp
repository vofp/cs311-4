#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <math.h>

long factor_pair(long number, long factor);
bool is_perfect(long number);
int sum_factors(long number);
int perfects(long lower, long higher);

int main(int argc, char const *argv[]){
	long lower = strtol( argv[1], NULL, 10 );
	long higher = strtol( argv[2], NULL, 10 );
	perfects(lower,higher);
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
