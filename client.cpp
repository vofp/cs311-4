#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <math.h>

long factor_pair(long number, long factor);
bool is_perfect(long number);
int sum_factors(long number);

int main(int argc, char const *argv[]){
	long test = strtol( argv[1], NULL, 10 );
	for(int i = 2; i <= test ; ++i){
		is_perfect(i);
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
