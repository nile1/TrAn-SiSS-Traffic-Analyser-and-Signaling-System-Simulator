/* Function:- Setup */
#include <stdio.h>
#include <stdlib.h>
#include "user.h"

void setup()
{
    char ch;
    printf("_______________HELLO____________\n");
    do
    {
        printf("\nEnter user details:\n");
        add_new_user();
        printf("\nDo you want to add another user?(y/n)\n");
        scanf("%c",&ch);
    }while(ch != 'n'|| ch != 'N' );

}

int main()
{
    printf("_______________HELLO____________\n");
    setup();

    return 0;
}
