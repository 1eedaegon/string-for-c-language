#include <stdio.h>

int factorial(int n);

int main() {
	int arg=5;
	int result=0;
	int result2=0;
	result = factorial(arg);
	result2 = factorial(3);
	printf("%d를 대입한 결과는 :%d",arg,result);
	return 0;
}

int factorial(int n) {
	if (n==0 || n==1) {
		return 1;
	}
	return n * factorial(n-1);
}
