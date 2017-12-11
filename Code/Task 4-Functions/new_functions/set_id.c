/* Function:- Generate Number */
char *gen_num(int n)
{
 char *num;                          //a pointer to return the generated number
 num=(char *)malloc(sizeof(char)*5);  //dynamic memory allocation for returning the generated number stored in a character array
 int i;
 for(i=3;i>=0;i--)                   //loop for storing the number in a character array
 {
  *(num+i)=(char)(n%10 + '0');       //storing the number in character format
  n=n/10;
 }
 *(num+4)='/0';
 return num;                         //returning the generated number
}

/* Function:- Set ID */
char *set_id(char *parent_id,char *type_id)
{
 static int ctr_men=0;
 static int ctr_tsj=0;
 static int ctr_lan=0;
 static int ctr_str=0;
 static int ctr_tsl=0;
 static int ctr_veh=0;
 static int ctr_rou=0;
 char *id;
 char *num;
 int id_len=strlen(parent_id)+9;        //storing the length of newly generated id
 id=(char *)malloc(sizeof(char)*id_len);

 strcpy(id,parent_id);                  //copying the parent_id in a pointer named id

 strcat(id,".");                        //adding a dot after id for maintaining the id format

 if(strcmp(type_id,"MEN")==0)          //loop for creating a map extension node type id
 {
     ctr_men++;
     strcat(id,"MEN");
     num=gen_num(ctr_men);
 }
 else if(strcmp(type_id,"TSJ")==0)     //loop for creating a traffic signal junction type id
 {
     ctr_tsj++;
     strcat(id,"TSJ");
     num=gen_num(ctr_tsj);
 }
 else if(strcmp(type_id,"LAN")==0)     //loop for creating a lane type id
 {
     ctr_lan++;
     strcat(id,"LAN");
     num=gen_num(ctr_lan);
 }
 else if(strcmp(type_id,"STR")==0)     //loop for creating a street type id
 {
     ctr_str++;
     strcat(id,"STR");
     num=gen_num(ctr_str);
 }
 else if(strcmp(type_id,"TSL")==0)     //loop for creating a traffic signal light type id
 {
     ctr_tsl++;
     strcat(id,"TSL");
     num=gen_num(ctr_tsl);
 }
 else if(strcmp(type_id,"VEH")==0)     //loop for creating a vehicle type id
 {
     ctr_veh++;
     strcat(id,"VEH");
     num=gen_num(ctr_veh);
 }
 else if(strcmp(type_id,"")==0)     //loop for creating a route type id
 {
     ctr_rou++;
     strcat(id,"ROU");
     num=gen_num(ctr_rou);
 }
 strcat(id,num);

 return id;

}


/* Function:- Generate ID */
int gen_id(char *ptr)
{
 int num=0;                          //a variable to return the generated number
 int i,j,n1;
 int n=str_len(*ptr)-1;
 for(i=n,j=0;i>n-4,j<4;i--,j++)     //loop for storing the number in integer format
 {
  n1=(int)(*(ptr+i) - '0');        //storing the number in integer format
  num=(n1*pow(10,j))+num;
 }
 return num;                       //returning the generated number
}

/* Functions for getting specific ids' */
int get_id(traffic_sig_jn_t *ptr)  //getting traffic signal junction id
{
    return gen_id(ptr->id);
}
int get_id(street_t *ptr)          //getting street id
{
    return gen_id(ptr->id);
}
int get_id(lane_t *ptr)            //getting lane id
{
    return gen_id(ptr->id);
}
int get_id(traffic_light_t *ptr)   //getting traffic light id
{
    return gen_id(ptr->id);
}
int get_id(vehicle_t *ptr)         //getting vehicle id
{
    return gen_id(ptr->id);
}
int get_id(route_t *ptr)           //getting route id
{
    return gen_id(ptr->id);
}
int get_id(map_extn_t *ptr)        //generating map extension node id
{
    return gen_id(ptr->id);
}

