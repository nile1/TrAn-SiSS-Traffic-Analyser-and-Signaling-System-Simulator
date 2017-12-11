#include<stdio.h>
#include<string.h>
#include "user.h"


/* Function:- Set user entry fields */
void set_user(user_t *u,char name[],char username[],char password[],char type[])
{
    strcpy(u->name,name);
    strcpy(u->password,password);
    strcpy(u->username,username);
    strcpy(u->type,type);
}

/* Function:- Copy user details in another variable */
void copy_user(user_t *u,user_t *u2)
{
    strcpy(u->name,u2->name);
    strcpy(u->password,u2->password);
    strcpy(u->username,u2->username);
    strcpy(u->type,u2->type);
}


/* Function:- Check user credentials */
int check_credentials(user_t *u)
{
    FILE *fp;                                   //File pointer
    user_t temp;
    char filename[]="user_data.dat";
    fp=fopen(filename,"rb");
    while(!feof(fp))
    {
        fread(&temp,sizeof(user_t),1,fp);        //Reading a file in binary format
        if((strcmp(u->username,temp.username)==0)&&(strcmp(u->password,temp.password)==0))
        {
            copy_user(u,&temp);
            return 1;
        }
    }
    return 0;

}

/* Function:- Save user credentials */
void save_user(user_t *u)
{
    FILE *fp;
    char filename[]="user_data.dat";
    fp=fopen(filename,"ab");
    fwrite(u,sizeof(user_t),1,fp);
    fclose(fp);
}
/* Function:- Login for user */
int login(user_t *u)
{
    int ret,temp;
    printf("\nEnter username");
    scanf("%s",u->username);
    printf("\nEnter password");
    scanf("%s",u->password);
    temp = check_credentials(u);
    if(temp==0)
        return 0;
    else if(temp==1)
        return 1;
}

/*Function:- User */
void add_new_user()
{
    user_t u;
    printf("\nEnter name");
    gets(u.name);
    printf("\nEnter password");
    scanf("%s",u.password);
    printf("\nEnter username");
    scanf("%s",u.username);
    printf("\nEnter type");
    scanf("%s",u.type);
    save_user(&u);
}
