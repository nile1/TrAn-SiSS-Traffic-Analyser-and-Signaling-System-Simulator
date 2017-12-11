/*Graph Functions*/
void graph_init(graph_tsj_t *g);
{
	g->num_tsj		= 0;
	g->num_street	= 0;
}

/*Add TSJ to Graph*/
int graph_add_tsj (graph_tsj_t *g,traffic_sig_jn_t *tsj)
{

	if(g->num_tsj == MAX_TSJ)
	{
		return 0;
	}

    g->num_tsj++;

    tsj->id=set_id("",t->type);
	initialize_tsj_adj_list_node(&g->tsj_adj_list[num_tsj-1],tsj);

    return 1;
}

/*Initialize Adjacency List*/
void initialize_tsj_adj_list_node(traffic_sig_jn_t *tsj)
{
    a_p->src_tsj         = tsj;
    a_p->head            = NULL;
    a_p->num_of_adj_tsj  = 0;
    a_p->next            = tsj;
}


/*Delete TSJ from Graph*/
int graph_delete_tsj(graph_tsj_t *g,tsj_t *t)
{
	int i,id;

	if(g->num_tsj==0)
	{
		return 0;
	}

	else
	{
		tsj_t *curr,*temp;
		id=get_id(t->id);

		for(int i=(t->id)-1;i<MAX_TSJ;i++)
		{

		    g->tsj_adj_list[i]=g->tsj_adj_list[i+1];

		}

		curr=g->tsj_adj_list[g->id-1]->head;

		while(curr!=NULL)
		{

			temp=curr;
			graph_delete_street(g , id , get_id(curr->dest_tsj_p) );
			curr=curr->next;
			free(temp);
		}
	}

	return 1;
}

/*Add Street to Graph*/ /*TBD : EDIT Street Type Datatype*/
int graph_add_street(struct graph *g,int tsj1_id,int tsj2_id , char st_name , int type)
{
    int index;
	/*If number of streets exceed the maximum (i.e. the graph is full)*/
	if(g->num_street == MAX_STREET)
	{
		return 0;		/*FAILURE*/
	}
    int newlane_id,max_vehicle;
	traffic_sig_jn_t *t1,*t2;

    /*Search for Empty Slot*/
    for(i=0;i<MAX_STREET;++i)
    {
        if(g->street_list[i]==NULL)
        {
            /*Set Index*/
            index = i;
            /*Increment Number of Steets in Graph*/
            g->num_street++;
            break;
        }
    }


	/*Create pointers to the given IDs of the tsj1 and tsj2*/

	t1=get_tsj_ptr(g,tsj1_id);
	t2=get_tsj_ptr(g,tsj2_id);

	/*STYLE*/
    /*Create the dynamically allocated style object*/
    style_t *s=(style_t *)malloc(sizeof(style_t *));

   	/*Set ID*/
	st->id = set_id_by_index("","STR",index);

	/*Set the style properties*/
	set_style(s,LIGHTGREY,LIGHTGREY,LIGHTGREY,5);// TBD: CHECK Properties BY NILADRI

	/*MAX VEHICLE CALULATION => Distance Formula*/
    max_vehicle = get_max_vehicle(t1,t2);


    /*NEW STREET*/
    /*Create dynamically allocated street_t object*/
    street_t *new_street=create_street(st_name,type,max_vehicles,&t1->coord,&t2->coord,s) // timer to be taken from stimulation function//


	/*Allocate Memory to the Edge*/
    g->street_list[g->num_street-1]=new_street;

	/*Add Destination Vertex to the tsj1 and tsj2*/
    add_vertex_dest(g->adj_list[tsj1_id] , t2 , new_street);
    add_vertex_dest(g->adj_list[tsj2_id] , t1 , new_street);

    return 1;
}

/*Create Vertex Node*/
vertex_node_t *create_vertex_node(struct traffic_sig_jn *dest_tsj,struct street *connecting_street)
{
    struct vertex *new_v = (struct vertex *)malloc(sizeof(struct vertex_node));

    new_v->dest_jn_p    = dest_tsj;
    new_v->street_p     = connecting_street;
    new_v->next         = NULL;

    return new_v;
}

/*Add Vertex Destination*/
void add_vertex_dest(struct adj_node *a,struct traffic_sig_jn *dest_tsj_p,struct street *connecting_street_p)
{
    /*Increment Number of Adjacent TSJ*/
    a->num_of_adj_tsj++;

    /*Create Vertex Node with the destination TSJ*/
    vertex_node_t *new_vnode = create_vertex_node(dest_tsj_p,connecting_street_p);
}

/*Get Maximum Vehicle Capacity of a Steet*/
int get_max_vehicle(traffic_sig_jn *t1,traffic_sig_jn *t2)
{
    int dist;
    int n=2;

    /*Distance Fourmula*/
    dist = abs((int)sqrt(pow(t2->coord->x,2) - pow(t1->coord->y - t2->coord->y)));

    return dist/n;
}

/**********************************************************/
/**********************************************************/
/**********************************************************/
/**********************************************************/
/**********************************************************/
/**********************************************************/

/*Get TSJ Pointer*/
traffic_sig_jn_t *get_tsj_ptr(graph_tsj_t *g,int id)
{
    int i=0;
    for(i=0;i<g->num_tsj;++i)
    {
        if(compare_id(g->tsj_adj_list[i]->src_tsj_p->id id))
            return g->tsj_adj_list[i]->src_tsj_p;
    }
    return NULL; /*FAILURE*/
}


/*Rename Traffic Signal Junction*/
void tsj_rename(graph_tsj_t *g,int id,char *name)
{
    /*Get Pointer to TSJ*/
    traffic_sig_jn_t *t = get_tsj_ptr(g,id);

    strcpy(t->name,name);
}

/*Change Fill Color of TSJ*/
void tsj_change_fill_color(graph_tsj_t *g,char *id,int new_fill_color)
{
    /*Get Pointer to TSJ*/
    traffic_sig_jn_t *t = get_tsj_ptr(g,id);

    t->sty.fill_color = new_fill_color;
}

/*Change TSJ*/
void tsj_change_border_color(graph_tsj_t *g,char *id,int new_border_color)
{
    /*Get Pointer to TSJ*/
    traffic_sig_jn_t *t = get_tsj_ptr(g,id);

    t->sty.border_color = new_border_color;
}

/**************Compare ID********************/
/*Compare String and Integer ID's*/
int compare_id(char *char_id , int int_id)
{
    char id[5];
    strcpy(id,gen_num(int_id));

    int pos = strlen(char_id)-3;

    return strcmp(char_id+pos,id);
}

/*************Get Street Pointer*********/
street_t *get_street_ptr(graph_tsj_t *g,int street_id)
{
    int i=0;

    for(i=0;i<g->num_streets;++i)
    {
        i=0;
        /*Get id of the Street in Character form and Compare*/
        if(compare_id(g->street_list[i] , street_id))
            return g->street_list[i];
    }

    return NULL;
}

/**Slope Of Line**/
float slope_of_line(coordinate_t *c1 , coordiante_t *c2)
{
    return (c1->y - c2->y)/(c1->x - c2->x)
}

/*Set ID by Index*/
char *set_id_by_index(char *parent_id,char *type_id,int id_num)
{
    char *id;
    char *num;
    int id_len=strlen(parent_id)+9;        //storing the length of newly generated id
    id=(char *)malloc(sizeof(char)*id_len);

    strcpy(id,parent_id);                  //copying the parent_id in a pointer named id

    strcat(id,".");                        //adding a dot after id for maintaining the id format

    strcat(id,type_id);
    num=gen_num(id_num);
    strcat(id,num);

    return id;
}


/*Delete Street from Graph*/
int graph_delete_street(struct graph *g, int tsj1_id, int tsj2_id)
{
	street_t *st;
	vertex_node_t ptr,prev;
	int street_id;


	/*Get TSJ id's the street is connecting*/
    t1 = get_tsj_ptr(g,tsj1_id);
	t2 = get_tsj_ptr(g,tsj2_id);

    /*TSJ1*/
	ptr = g->tsj_adj_list[tsj1_id].head;

	while( ptr != NULL && ptr->dest_tsj_p == t2)
    {
        prev = ptr;
        ptr = ptr->next;
    }
	/*Get Street ID*/
    street_id = get_id(ptr->street_p);

    /*Set the connection from TSJ 1 to TSJ 2 and TSJ2 and TSJ 1 to zero(0/NULL)*/
    prev->next = ptr->next;
    free(ptr);


    /*TSJ2*/
	ptr = g->tsj_adj_list[tsj2_id].head;

	while( ptr != NULL && ptr->dest_tsj_p == t1)
    {
        prev = ptr;
        ptr = ptr->next;
    }
	/*Get Street ID*/
    street_id = get_id(ptr->street_p);

    /*Set the connection from TSJ 1 to TSJ 2 and TSJ2 and TSJ 1 to zero(0/NULL)*/
    prev->next = ptr->next;
    free(ptr);


	/*Deallocate the street that is deleted*/
    free(g->street_list[street_id]);
    g->street_list[street_id] = NULL;

    g->num_streets--;
}


/******Add Street by ID**************/
void graph_add_street_by_id(graph_tsj_t *g,int street_id)
{
    street_t *temp_st = get_street_ptr(g,street_id);

    graph_add_street( g , get_id(st->tsj[0]) , get_id(st->tsj[1]) );
}

/***TBD List******/
/*********
 * Get angle between
 * Sort Streets
 *
 * Recheck Add ... Lane,
 *          Street, -> Assign Lane Coordinates
 *          TSJ
 *
 * Add Street to Graph : Lane Coordinate Setup
 * Delete Street from Graph
 * Add Street By ID
 *
 *
 * ****Recheck****
 * Create Map Extension List
 * Add Map Extension Node
 * Delete Map Extension Node
 * Get Map Extension List
 *
 * Get ID Modified ->DONE
 *
*******/



/**TBD & RECHECK: Get Angle Between two Adjacent Streets**/
float get_angle_between(street_t *s1, street_t *s2)
{
    coordinate_t *com , *coord1 , *coord2;

    if(s1->c1 == s2->c1)
    {
        com = &s1->c1;
        coord1 = &s1->c2;
        coord2 = &s2->c2;
    }
    else if(s1->c1 == s2->c2)
    {
        com     = &s1->c1;
        coord1  = &s1->c2;
        coord2  = &s2->c1;
    }
    else if(s1->c2 == s2->c1)
    {
        com     = &s1->c2;
        coord1  = &s1->c1;
        coord2  = &s2->c2;
    }
    else if(s1->c2 == s2->c2)
    {
        com     = &s1->c2;
        coord1  = &s1->c1;
        coord2  = &s2->c1;
    }

    float slope1 = slope_of_line(com , coord1);
    float slope2 = slope_of_line(com , coord2);

    float angle;

    angle = atan((slope1 - slope)/(1-slope1*slope2));

    return angle;
}



/***TBD******************/
/*Sort Streets into TSJ*/
void sort_streets_into_tsj(traffic_signal_j_t *tsj_p)
{
    /************************/
}


/***TBD******************/





/*************************MAP_EXTENSION_NODE*********************/

map_extn_t *create_map_extn(char *name, int time_of_generation , traffic_sig_jn_t *adj_tsj , street_t *connecting_street , int index)
{F
    map_extn_t *newnode;
    newnode = (map_extn_t *)malloc(sizeof(map_extn_t));
    strcpy(newnode->name,name);
    newnode->id                 = set_id_by_index("","MEN",index);
    newnode->time_of_generation = time_of_generation;
    newnode->next               = NULL;
    newnode->adj_signal         = adj_tsj;
    newnode->connecting_street  = connecting_street;

    return newnode;
}

map_extn_list_t *create_map_extension_list(void)
{
    map_extn_list_t *newnode= (map_extn_list_t *)malloc(sizeof(map_extn_list_t));
    newnode->num_map_extn   = 0;
    newnode->head           = NULL;

    return newnode;
}

/*Add Street to Map Extension Node*/
int add_street_to_map_extn_node(struct graph *g,int tsj1_id,int men_id , char st_name , int type)
{

}

/*Add Map Extension Node*/
int add_map_extn( map_extn_list_t *map_extn_p , char *name, int time_of_generation, traffic_sig_jn_t *adj_tsj , coordinate_t *c1 )
{
    map_extn_t *newnode, *temp,*head;

    /******* TBD *******/
    /* COPY from graph_add_street() */

    if(g->num_street == MAX_STREET)
	{
		return 0;		/*FAILURE*/
	}

    /**MODIFY THIS**/
	add_vertex_dest(&g->tsj_adj_list[tsj1_id] , t2 , new_street);


    /**************/

    style_t sty = (style_t *)malloc(sizeof(style_t));
    set_style(sty,LIGHTGREY,LIGHTGREY,LIGHTGREY,2);
    street_t *new_street=create_street(st_name,type,max_vehicles,c1,adj_tsj->coord,&sty ) // timer to be taken from stimulation function//


    newnode = create_map_extn(name,time_of_generation ,adj_tsj ,connecting_street , map_extn_p->num_map_extn+1 );
    (map_extn_p->num_map_extn)++;

    if (map_extn_p->head == NULL)
    {
        map_extn_p->head = newnode;
    }
    else
    {
        temp = head;
        head = newnode;
        newnode->next = temp;
    }

    return 1;
}

/***********
get_min_dist_tsj
***********/


int delete_map_extn(map_extn_list_t *map_extn_p, int id)
{
    int i=0;
    map_extn_t *temp, *prev;
    temp = map_extn_p->head;
    prev = map_extn_p->head;

    while(temp->next != NULL)
    {
        if(compare_id(temp->id,id)==0)
        {
            i++;
            break;
        }
        prev = temp;
        temp = temp->next;

    }


    if(i==0)
    {
        return 0;
    }
    else if(compare_id(map_extn_p->head->id==id)
    {

        temp = map_extn_p->head;
        map_extn_p->head = map_extn_p->head->next;
        free(temp);
    }
    else
    {
        prev->next = temp->next;
        free(temp);
    }

    return 1;

}

/*Get Extension Node Pointer with ID*/
map_extn_t *get_map_extn_node(map_extn_list_t *map_extn_p, int id)
{

 int i=0;
    map_extn_t *temp;
    temp = map_extn_p->head;

    while(temp->next != NULL)
    {
        if(compare_id(temp->id,id)==0)
        {
            i++;
            break;
        }
        temp = temp->next;

    }


    if(i==0)
    {
        return NULL;
    }

    return temp;
}



/*********************************************************/
/*********************************************************/
/*********************************************************/
/**************** STREET and LANES ***********************/
/*********************************************************/
/*********************************************************/
/*********************************************************/


/*Create 'n'Lanes*/
lane_t *create_lane(char *parent_id,int max_vehicles, struct coordinate *c1,struct coordinate *c2,struct style *sty)
{


      	lane_t *ptr		= NULL;

      	ptr=(lane_t *)malloc(sizeof(lane_t));
      	ptr->id=set_id(parent_id, "LAN");/*id of the street fro it is created being passed into function set_id*/

      	ptr->max_vehicles	= max_vehicles;
      	ptr->curr_vehicles	= 0;
      	ptr->queue		= create_queue(max_vehicles);

      	copy_coordinate( &(ptr->c1) , c1 );
   	copy_coordinate( &(ptr->c2) , c2 );

   	copy_style( &(ptr->sty) , sty );

      	return ptr;

 }

/*Create 'n'Lanes*/
lane_t **create_n_lane(char *parent_id,int max_vehicles,char *type,struct coordinate *c1,struct coordinate *c2,struct style *sty)
{
       lane_t **ptr;
       if(strcmp(type,"two_way")==0)//whenever type function  would be defined remember to define street type  as "two_way" or "four_way"//
       {
             ptr=(lane_t **)malloc(sizeof(lane_t *)*1);
             ptr[0]=create_lane(parent_id,max_vehicles,c1,c2,sty);//forward one way is created
        }
       else if(strcmp(type,"four_way")==0)
       {
            ptr=(lane_t **)malloc(sizeof(lane_t *)*2);
             ptr[0]=create_lane(parent_id,max_vehicles,c1,c2,sty);//forward one way is created
             ptr[1]=create_lane(parent_id,max_vehicles,c1,c2,sty);

       }
       else if(strcmp(type,"six_way")==0)
       {
             ptr=(lane_t **)malloc(sizeof(lane_t *)*3);
             ptr[0]=create_lane(parent_id,max_vehicles,c1,c2,sty);//forward one way is created
             ptr[1]=create_lane(parent_id,max_vehicles,c1,c2,sty);
             ptr[2]=create_lane(parent_id,max_vehicles,c1,c2,sty);

       }
       return ptr;
}



void queue_add_vehicle( lane_t *l,vehicle_t *v )  // circular queue of array
{
	l->queue[l->max_vehicles-1]=v;

} // add graphic variables.


void *queue_delete(lane_t *l)
{
    l->queue[0]=NULL;
} // add graphics function.




street_t *create_street(char *name, char *type, int max_vehicles ,struct coordinate *c1,struct coordinate *c2,struct style *sty ) // timer to be taken from stimulation function
                                                                                                // add graphics function.
{
    street_t *ptr;
    ptr 		= (street_t *)malloc(sizeof(street_t));

    strcpy(ptr->name, name);

    ptr->id 		= set_id("","STR");

    strcpy(ptr->type, type);

    if(strcmp(type,"two_way")==0)
    {
        ptr->nol[0]=1;
        ptr->nol[1]=1;
    }

    else if(strcmp(type,"four_way")==0)
    {
        ptr->nol[0]=2;
        ptr->nol[1]=2;
    }


    else if(strcmp(type,"six_way")==0)
    {
        ptr->nol[0]=3;
        ptr->nol[1]=3;
    }

    ptr->lanes[0] = create_n_lane(ptr->id ,max_vehicles,ptr->type,c1,c2,sty);
    ptr->lanes[1] = create_n_lane(ptr->id ,max_vehicles,ptr->type,c1,c2,sty);

    ptr->tsl[0] = create_tsl(ptr->id); // timer to be taken from stimulation.
    ptr->tsl[1] = create_tsl(ptr->id); // timer to be taken from stimulation.

    ptr->tsj[0] = NULL;
    ptr->tsj[1] = NULL;    //If and then take in the argument and add other wise delete it from every where in street.
    return ptr;
}


