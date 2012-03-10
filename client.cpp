#include <stdlib.h>
#include <stdio.h>
#include <iostream>


long factor_pair(long number, long factor);
bool is_perfect(long number);

int main(int argc, char const *argv[]){
	long test = strtol( argv[1], NULL, 10 );
	long pair = factor_pair(test, 2);

	std::cout << "one factor pair of " << test << " is 2 and " << pair << std::endl;
}

bool is_perfect(long number){
	return false;
}

long factor_pair(long number, long factor){
	if(number % factor == 0){
		return number / factor;	
	}
	return 0;
}
