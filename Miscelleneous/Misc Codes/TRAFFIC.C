#include <stdio.h>
#include <time.h>

void tenseconds()
{
time_t start;
time_t current;
time(&start);
do
{
time(&current);
}
while(difftime(current,start) < 10.0);
}

void twoseconds()
{
time_t start;
time_t current;
time(&start);
do
{
time(&current);
}
while(difftime(current,start) < 2.0);
}

void redlight()
{
printf("X Ave. - RED; Y Blvd. - GREEN\n");
tenseconds();
printf("X Ave. - RED; Y Blvd. - YELLOW\n");
twoseconds();
printf("X Ave. - RED; Y Blvd. - RED\n");
twoseconds();
printf("X Ave. - GREEN; Y Blvd. - RED\n");
tenseconds();
printf("X Ave. - YELLOW; Y Blvd. - RED\n");
twoseconds();
printf("X Ave. - RED; Y Blvd. - RED\n");
twoseconds();
}

int main()
{
printf("Stoplight 1.0\n\n");
while(1)
redlight();
return 0;
}
