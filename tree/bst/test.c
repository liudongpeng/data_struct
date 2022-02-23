#include <stdio.h>



int main()
{

	char *str = "123";
	int a = 0;

	sprintf(str, "%d", a);
	printf("%d\n", a);


	return 0;
}
