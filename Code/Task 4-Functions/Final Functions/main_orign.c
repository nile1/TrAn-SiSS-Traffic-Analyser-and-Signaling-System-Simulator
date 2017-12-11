/*
Project Name    : TrAn-SiSS
Date Created    : *********
Development Team:
*/

/*
Function Name   : main
*/

/*
Include all the header files of the "user defined" header files.
*/
#include<stdio.h>
#include<stdlib.h>
#define MAX_STR_SIZE 15
#define DEFAULT_BORDER_COLOR 11
#define TSJ_RADIUS 4
#define LIGHTGREY 10

/**************************************/
/********PROTOTYES*********************/
/**************************************/

int menu_login()
{
    int ch;

    printf("\n\tMENU\n\n");

    printf("\n1.\tLogin");
    printf("\n2.\tUser");
    printf("\n3.\tExit");

    printf("\n\nEnter Choice : ");
    scanf("%d",&ch);

    return ch;
}

int menu_admin()
{
    int ch;

    printf("\n\tMENU\n\n");

    printf("\n1.\tNew");
    printf("\n2.\tLoad");
    printf("\n3.\tLogout");

    printf("\n\nEnter Choice : ");
    scanf("%d",&ch);

    return ch;
}

int menu_user()
{
    int ch;

    printf("\n\tMENU\n\n");

    printf("\n1.\tLoad");
    printf("\n2.\tLogout");

    printf("\n\nEnter Choice : ");
    scanf("%d",&ch);

    return ch;
}

int submenu_admin()
{
    int ch;

    printf("\n\tMENU\n\n");

    printf("\n1.\tTSJ");
    printf("\n2.\tStreet");
    printf("\n3.\tMap Extension Node");
    printf("\n4.\tSimulate");
    printf("\n5.\tBack");

    printf("\n\nEnter Choice : ");
    scanf("%d",&ch);

    return ch;
}

int submenu_user()
{
    int ch;

    printf("\n\tMENU\n\n");

    printf("\n1.\tSimulate");
    printf("\n2.\tBack");

    printf("\n\nEnter Choice : ");
    scanf("%d",&ch);

    return ch;
}

int menu_tsj()
{
    int ch;

    printf("\n\tMENU\n\n");

    printf("\n1.\tAdd");
    printf("\n2.\tEdit");
    printf("\n3.\tDelete");
    printf("\n4.\tBack");
    printf("\n\nEnter Choice : ");
    scanf("%d",&ch);

    return ch;
}

int menu_street()
{
    int ch;

    printf("\n\tMENU\n\n");

    printf("\n1.\tAdd");
    printf("\n2.\tDelete");
    printf("\n3.\tBack");

    printf("\n\nEnter Choice : ");
    scanf("%d",&ch);

    return ch;
}

int menu_map_extn_node()
{
    int ch;

    printf("\n\tMENU\n\n");

    printf("\n1.\tAdd");
    printf("\n2.\tDelete");
    printf("\n3.\tBack");

    printf("\n\nEnter Choice : ");
    scanf("%d",&ch);

    return ch;
}

int menu_edit_tsj()
{
    int ch;

    printf("\n\tMENU\n\n");

    printf("\n1.\tEdit Name");
    printf("\n2.\tEdit Fill Color");
    printf("\n3.\tEdit Border Color");
    printf("\n4.\tBack to previous menu");

    printf("\n\nEnter Choice : ");
    scanf("%d",&ch);

    return ch;
}



void main()
{
    /*Initialize file pointer and choice*/
    int c_login,c_admin,c_user,c_subuser,c_subadmin,c_street,c_tsj,c_edit_tsj,c_map_extn_node;
    //int gd = DETECT, gm;
    user_t *u;
    graph_t g1;
    map_extn_list_t *men_list;
    traffic_signal_jn_t *new_tsj,*tsj_rename,*temp_tsj,*adj_tsj;
    street_t *st;
    coordinate_t c;
    style_t s;
    char name[MAX_STR_SIZE];
    int id;
    int id_1;
    int id_2;
    char ch;
    int t;

    men_list=create_map_extension_list();

    do
    {
            /*
            Display Login Menu:
            MENU (Login)
                1) Admin
                2) User
                3) Exit
            */
        c_login=menu_login();

        switch(c_login)
        {
            case 1:
            if(login(&u)==0||strcmp(u->type,'standard')==0)
              break;
            do
            {
                c_admin=menu_admin();
                /*
                Admin Menu:
                MENU (Admin)
                    1) New
                    2) Load
                    3) Logout (Back to Menu 1)
                */

                switch(c_admin)
                {
                    case 1:
                    /* Function:- Create Graph */
                    graph_init(&g1);

                    case 2:
                    if(c_admin==2)
                    {
                      /* Function:- Load Graph from file */
                      load_graph(&g);//To be done
                    }

                    do
                    {
                        c_subadmin=submenu_admin();
                        /*
                        Proceed Admin Menu:
                        MENU (Proceed Admin)
                            1) TSJ
                            2) Street
                            3) Map Extension Node
                            4) Simulate
                            5) Logout (Back to Menu 2)
                        */

                        switch(c_subadmin)
                        {
                            case 1:
                            do
                            {
                               c_tsj=menu_tsj();
                                /*
                                Proceed Admin Menu:
                                MENU (Proceed Admin)
                                    1) Add
                                    2) Edit
                                    3) Delete
                                    4) Back to Menu 4
                                */
                                switch(c_tsj)
                                {
                                    case 1:
                                    /* Add TSJ */
                                        printf("\nEnter name of traffic signal junction");
                                        gets(name);
                                        printf("\nEnter coordinates");
                                        printf("\nx:");
                                        scanf("%d",&(c.x));
                                        printf("\ny:");
                                        scanf("%d",&(c.y));
                                        printf("\nEnter fill color(1 to 15):");
                                        scanf("&d",&(s.fill_color));
                                        s.border_style=2;
                                        s.border_color=DEFAULT_BORDER_COLOR;
                                        s.width=TSJ_RADIUS;
                                        new_tsj=create_tsj(name,&c,&s);
                                        graph_add_tsj(&g1,new_tsj);
                                        break;
                                    case 2:
                                    /* Edit TSJ */
                                    do
                                    {
                                      c_edit_tsj=menu_edit_tsj();
                                      /*
                                      Edit Traffic Signal Junction Menu:
                                      MENU (Edit TSJ)
                                          1) Edit Name
                                          2) Edit Fill Color
                                          3) Edit Border Color
                                          4) Back to previous menu
                                      */
                                      if(c_edit_tsj != 4)
                                      {
                                          printf("\nEnter the traffic signal junction to edit");
                                          gets(id);
                                      }
                                      switch(c_edit_tsj)
                                      {
                                          case 1:

                                              printf("\nEnter the new name");
                                              gets(name);

                                              tsj_rename(&g1,id,name);
                                              break;
                                          case 2:
                                              printf("\nEnter the color to be filled");
                                              scanf("&d",&(s.fill_color));
                                              tsj_change_fill_color(&g1,id,s.fill_color);
                                              break;
                                          case 3:
                                              printf("\nEnter the color to be filled");
                                              scanf("&d",&(s.border_color));
                                              tsj_change_border_color(&g1,id,s.border_color);
                                              break;
                                          case 4:
                                              break;
                                          default:
                                              printf("\nInvalid Choice");
                                      }
                                    }while(c_edit_tsj != 4);


                                    case 3:
                                        /* Delete TSJ */
                                        printf("\nEnter the traffic signal junction to be deleted");
                                        scan("%d",&id);
                                        temp_tsj=get_tsj_ptr(&g1,id);
                                        graph_delete_tsj(&g1,temp_tsj);
                                        break;
                                    case 4:
                                          break;
                                    default:
                                          printf("\nInvalid Choice");
                                }

                            }while(c_tsj != 4);
                            break;

                            case 2:
                            do
                            {
                               c_street=menu_street();
                                /*
                                Street Menu:
                                MENU (Street)
                                    1) Add Street
                                    2) Delete Street
                                    3) Logout (Back to Menu 4)
                                */
                                switch(c_street)
                                {
                                    case 1:
                                    /* Add Street */
                                        printf("\nEnter name of the street");
                                        gets(name);
                                        printf("\nEnter the id's of traffic signal junctions");
                                        printf("\nJunction 1:");
                                        scanf("%d",&id_1);
                                        printf("\nJunction 2:");
                                        scanf("%d",&id_2);
                                        s.fill_color=LIGHTGREY;
                                        s.border_style=2;
                                        s.border_color=DEFAULT_BORDER_COLOR;
                                        s.width=7;

                                        graph_add_street(&g1,id_1,id_2,name,4);
                                        break;

                                    case 2:
                                    /* Delete Street */
                                        printf("\nEnter the street id to be deleted");
                                        scanf("%d",&id);
                                        graph_add_street_by_id(&g1,id);
                                        break;
                                    case 3:
                                        break;
                                    default:
                                        printf("\nInvalid Choice");

                                }
                            }while(c_street != 3);
                            break;

                            case 3:
                            do
                            {
                               c_map_extn_node=menu_map_extn_node();
                                /*
                                Map Extension Node Menu:
                                MENU (Map Extn Node)
                                    1) Add
                                    2) Delete
                                    3) Logout (Back to Menu 4)
                                */
                                switch(c_map_extn_node)
                                {
                                    case 1:
                                    /* Add map_extn_node */
                                        printf("\nEnter name of  Map Extension Node");
                                        gets(name);
                                        printf("\nEnter coordinates");
                                        printf("\nx:");
                                        scanf("%d",&(c.x));
                                        printf("\ny:");
                                        scanf("%d",&(c.y));

                                        printf("\nEnter time of generation");
                                        scanf("%d",&t);

                                        printf("\nEnter ID of the Adjacent Traffic Signal : ");
                                        scanf("%d",id);

                                        adj_tsj = get_tsj_ptr(id);

                                        st = add_street_with_map_extn();

                                        add_map_extdn(name,  t, adj_tsj , s , *map_extn_p );

                                        break;
                                    case 2:
                                        /* Delete map_extn_node */
                                        printf("\nEnter thr Map Extension Node ID to Delete : ");
                                        scanf("%d",id);

                                        delete_map_extn(men_list, id);
                                        break;
                                    case 3:
                                        break;

                                    default:
                                        printf("\nInvalid Choice");
                                }
                            }while(c_map_extn_node != 3);
                            break;

                            case 4:
                              /* simulate */
                              printf("\nDo you want to simulate?(y/n)");
                              scanf("%c",&ch);
                              if(ch=='y'||ch=='Y')
                              {
                                simulate(&g1,men_list);
                              }
                              break;

                            case 5:
                                break;
                            default:
                                printf("\nInvalid Choice");

                        }

                    }while(c_subadmin != 5);
                    break;

                    default: printf("Invalid Choice\n");
                        /*Invalid choice*/
                }
            }while(c_login != 3);
            break;


            case 2:
            if(login(&u)==0||strcmp(u->type,'admin')==0)
              break;

            do
            {
                c_user=menu_user();
                /*
                Switch-Case:
                    MENU 3 (User)
                        1) Load
                        2) Logout (Back to Menu 1)
                */
                switch(c_user)
                {
                    case 1:
                    do
                    {
                        c_subuser=submenu_user();
                        /*
                        Proceed user menu
                        Menu (Proceed user)
                            1) Simulate
                            2) Back to previous menu
                        */
                        switch(c_subuser)
                        do
                        {
                            case 1:
                              /* Simulate */
                              printf("\nDo you want to simulate?(y/n)");
                              scanf("%c",&ch);
                              if(ch=='y'||ch=='Y')
                              {
                                simulate(&g1,men_list);
                              }
                              break;

                            case 2:
                              /*Back to Previous Menu*/
                              break;

                            default: printf("Invalid choice");
                        }while(c_subuser != 2);
                        break;

                    case 2:
                      /*Back to Previous Menu*/
                      break;

                    }while(c_subuser != 2);
                    break;
                }
            }while(c_user != 2);
            break;

            case 3:
              /*Back to Previous Menu*/
              break;

            default:
                printf("Invalid Choice\n");
                /*Invalid choice*/
                break;
        }
    }while(c_login != 3);



    return 0;
}
