
    int is_adjacent(travelled[0],sorted_array[0]) // function to check if tls is adjacent or not
{
    if(sorted_array[0]==A&&(travelled[0]==B||travelled[0]==D))
    {
        return 1;
    }
    else if(sorted_array[0]==B&&(travelled[0]==A||travelled[0]==C))
    {
        return 1;
    }
    else if(sorted_array[0]==C&&(travelled[0]==B||travelled[0]==D))
    {
        return 1;
    }
    else if(sorted_array[0]==D&&(travelled[0]==A||travelled[0]==C))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


void analize_tsj(int tsj_id) // main analize function
{
    time_t current;
    int street_type,i,j,k, index;
    street_t *arr[4];
    traffic_sig_jn_t *point;
    tsl_t *sorted_array[4], *travelled[3];
    point=get_pointer(tsj_id);  // pointer of tsj from tsj id
    street_type=get_street_type(arr[0]); // type of street 2 way or 4 way


    /*storing the tls pointer of a tsj in arr*/
    for(i=0;i<4;i++)
    {
        arr[i]=point->streets[i];
    }


/*naming the pointes A, B, C, D to simplify*/
    tsl_t *A=arr[0]->tsl[get_tsj_index(arr[0],point)];
    tsl_t *B=arr[1]->tsl[get_tsj_index(arr[1],point)];
    tsl_t *C=arr[2]->tsl[get_tsj_index(arr[2],point)];
    tsl_t *D=arr[3]->tsl[get_tsj_index(arr[3],point)];


/*loop to store traffic status of each tsl by calling function set_traffic status*/
    for(i=0;i<4;i++)
    {
        index = get_tsj_index(arr[i],point);
        arr[i]->tsl[index]->traffic_status = set_traffic_status(arr[i], point);
    }

/*setting A B C D in sorted array and then sorting them acc to their traffic status in below loop.*/
    sorted_array[0]=A;
    sorted_array[1]=B;
    sorted_array[2]=C;
    sorted_array[3]=D;

    for(j=0;j<4;j++)
    {
        for(k=j+1;k<4;k++)
        {
            if((sorted_array[j]->traffic_status)<(sorted_arr[k]->traffic_status))
            {
                (sorted-array[j]->traffic_status)+=(sorted_array[k]->traffic_status);
                (sorted-array[k]->traffic_status)=(sorted_array[j]->traffic_status)-(sorted_array[k]->traffic_status);
                (sorted_array[j]->traffic_status)-=(sorted_array[k]->traffic_status);
            }
            else
            {
                k++;
            }
        }
    }


/*storing the tsl pointers in travelled array except the most bussiest tsl and then sorting it acc to the flag, whether  visited or not */
    for(i=0;i<3;i++)
    {
        travelled[i]=sorted_array[i+1];
    }


    if(travelled[0]->flag!=0&&travelled[1]->flag!=0&&travelled[2]->flag!=0)
{
    for(i=0;i<3;i++)
        {
            if(travelled[i]->flag==1)
                {
                    temp = travelled[i];
                    travelled[i] = travelled[0];
                    travelled[0] = temp;
                }

        }
}


/*predefining modes of different tsl */

    int mode_type;
     if(sorted_array[0]==A)
    {
        mode_type=14;
    }
    else if(sorted_array[0]==B)
    {
        mode_type=13;
    }
    else if(sorted_array[0]==C)
    {
        mode_type=16;
    }
    else if(sorted_array[0]==D)
    {
        mode_type=15;
    }


/* Stroing current time in current*/
    time(&current);


  /* 1. if of time  */
   if(difftime(current,tsj->start) >tsj->differ)
   {
      point->differ=timer(sorted_array);//return difference btw current time and starting time

        /*2. if of street type, in this case 2 way*/
        if(street_type==2)// if of type
       {

       }

       /*2. if of street type, in this case 4 way*/
       else if(street_type==4)// if of type
       {

           // if of mode for base
           if(point->mode!=14&&point->mode!=13&&point->mode!=15&&point->mode!=16)
           {
               /* this stores the timer of current tsl to be executed*/
               point->differ=point->differ*0.75;


               // conditional  ifs  depending upon flag.
               if(travelled[0]->flag==0)
               {
                   set_tsj_mode(A,B,C,D,mode_type);//base condition in 4way.xlsx
                    point->mode=mode_type;
               }

               else
               {
                    if(is_adjacent(travelled[0],sorted_array[0]))//check both are adjacent or not
                    {
                        set_tsj_mode(A,B,C,D,mode_type);//b first_condition in 4way.xlsx
                        point->mode=mode_type;
                    }
                    else
                    {
                        set_tsj-mode(A,B,C,D,mode_type);// c first condition in 4way.xlsx
                        point->mode=mode_type;
                        point->flptr=1;
                    }

               }

                /*Loop to increment the flags of all tls of a junction after travelling base mode.*/
                for(int k=1;k<sorted_array;k++)
                {
                    if(sorted_array[k]->flag!=0)
                    {
                        sorted_array[k]->flag=((sorted_array[k]->flag)+1)%4;
                    }
                }
                sorted_array[0]->flag++;
           }


            else  // mode for sub mode
            {
                if(point->flag==1)
                {
                    point->flptr=0;
                    point->mode=0;
                    break;

                }
                else
                {
                    if(point->mode==14)//A
                    {
                        set_mode(A,B,C,D,18);
                        point->mode=18;
                    }
                    if(point->mode==13)//B
                    {
                        set_mode(A,B,C,D,17);
                        point->mode=17;
                    }
                    if(point->mode==16)//C
                    {
                        set_mode(A,B,C,D,18);
                        point->mode=18;
                    }
                    if(point->mode==15)//D
                    {
                        set_mode(A,B,C,D,17);
                        point->mode=17;
                    }

                }
                point->differ=point->differ*0.25;

            }

       }
       tsj->start=current;

   }



/*
Identify and assign the pointer to the TSJ corresponding to the 'tsj_id' which is passed as a parameter.
Use function :
'traffic_sig_jn *graph_get_pointer(int id);'
which will return the pointer to the requied tsj_id.
*/

/*
First assign the street pointers in the order of their 'traffic_status'(which will be in %)
with initial status as A>B>C>D
*/
/*
Analize and assign the TSL mode to the requied TSL according to the current order of traffic_status.
*/
}
