/***************************
Version: 7.0

This the first run which compiled.... though there are some -Wall ERRORS
******************************/
/*functions started after  dotted line*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<time.h>
#include<math.h>
#include<string.h>
#include<stdlib.h>

#include "user.h"

//#include "graph.c"
//#include "graph_graphics.c"


#define MAX_VERTICES     10
#define MAX_EDGES    	MAX_VERTICES*(MAX_VERTICES-1)/2
#define MAX_TSJ 	10
#define MAX_STREET 	MAX_TSJ*(MAX_TSJ-1)/2

#define MAX_STR_SIZE 15
#define DEFAULT_BORDER_COLOR 11
#define TSJ_RADIUS 4
#define LIGHTGREY 10



/******* STRUCTURES Definition List ********/

typedef struct coordinate
{
  int   x; //integral numeral for x-coordinate of the object
  int   y; //integral numeral for y-coordinate of the object
  float rotation; //floating point value for Angle of Rotation of the object
}coordinate_t;

typedef struct style
{
 int fill_color; //contains the color code of the graph
 int border_style; //contains the border style of the graph
 int border_color; //contains the color code of the graph
 int width; //contains the width of a lane/street or the radius of a circle
}style_t;


/* Nodes that are an extension to the map boundaries. This object will be static in the program.*/
typedef struct map_extn
{

    char 			        name[20];              // name of the extension node
    char 			        *id;                    // id
    time_t 			        start_time;
    float 			        time_of_generation;     // Number of vehicles incoming from the extension.
    struct traffic_sig_jn 	*adj_signal; // Pointing the immediate traffic signal.
    struct street 		    *connecting_street;
    struct map_extn 		*next;
    struct coordinate 		coord;

}map_extn_t;

// Structure to represent each node which is used to extend the graph of TSJs.
typedef struct map_extn_list
{
    int num_map_extn;
    struct map_extn *head;
} map_extn_list_t;




/*Traffic Signal Light*/
typedef struct traffic_light
{
    char    *id; //unique id of a traffic light
    int     red;//status of red light(on/off)
    int     flag;//storing status
    int     yellow;//status of yellow light(on/off)
    int     forward;//status of green light(on/off) for forward movement of vehicle from a junction
    int     left;//status of green light(on/off) for left  movement of vehicle from a junction
    int     right;////status of green light(on/off) for right movement of vehicle from a junction
    float   traffic_status;//Percentage of the traffic present on the lane.
}traffic_light_t;

typedef struct vehicle
{
    //name of the vehicle removed
    char            *id;//unique id for vehicle
    int             speed;//speed of vehicle
    char            type[10];//type of vehicle car,truck or  bike
    struct map_extn *source;//starting map extension node
    struct map_extn *dest;//ending map extension node
    struct route    *directions; // array of all the route to be followed
    char            vehicle_no[8];//license plate
    int             priority_status;//priority  vehicles such as vip vehicles
}vehicle_t;

typedef struct lane
{
    char            	*id;//Unique ID code for the lane
    struct vehicle  	**queue;//Queue of pointers of vehicle
   // int             front;//The position just 1 before the position of the first vehicle
    //int             rear;//The position of the last vehicle
    int             	max_vehicles;//Maximum number of vehicles that can be on the lane.
    int             	curr_vehicles;//Number of vehicles currently present on the lane
    struct coordinate   c1;
    struct coordinate   c2;
    struct style        sty;

 }lane_t;



/*Street*/
typedef struct street
{
    char                    *name;      //Name of the street
    char                    *id;        //Unique ID code for the street
    char                    type[10];   //Street type description
    int                     nol[2];     //Number of Lanes leading
    struct lane             **lanes[2];  /* Lanes leading from lanes[0] : TSJ 1 -to-> TSJ 2 lanes[1] : TSJ 2 -to-> TSJ 1 */
    struct traffic_sig_jn   *tsj[2];    /*Pointer to TSJ's the street is joined to. tsj[0] : ->TSJ 1 tsj[1] : ->TSJ 2  */
    struct traffic_light    *tsl[2];    /* Pointer of TSL's for the corresponding TSJ's the street is joined to. tsl[0] : ->TSL 1 (2->1) tsl[1] : ->TSL 2  (1->2) */

    /*Graphics - Coordinate & Style Variables*/
    struct coordinate       c1;
    struct coordinate       c2;
    struct style            sty;

}street_t;




typedef struct traffic_sig_jn
{

    char        name[20];              // name of the  traffic light
    char        *id;                    // id
    char        type[10];                  // Number of roads connected i.e 1 way, 2 way etc.
    int         flptr;                   //for storing status
    int         mode;
    float       differ;               // stores the difference between current and start
    time_t     start;

    /*Graphics - Coordinate & Style Variables*/
    struct street           **streets;         //road from adjacent traffic signal, although no need for it
    struct coordinate       coord;
    struct style            sty;

} traffic_sig_jn_t;





/* Structure to represent each node of the Adjacency List */
typedef struct vertex_node
{
	struct traffic_sig_jn   *dest_tsj_p; //Pointer to the destination traffic signal
	struct street              *street_p; //Pointer to the connecting street
	struct vertex_node       *next; //Pointer to the next adj_list_node of the linked list
}vertex_node_t;

/* Structure to represent each linked list of the Adjacency List */
typedef struct adj_list
{
 	int                        num_of_adj_tsj; /*Number of Adjacent TSJ(Degree of TSJ).*/
 	struct vertex_node         *head; //Pointer to the starting(head) node of each linked list
 	struct traffic_sig_jn   *src_tsj; //Pointer to the source TSJ node for the edges
}adj_list_t;


typedef struct graph_tsj
{
    int                 num_tsj; /*Number of TSJ’s(vertices).*/
    int                 num_street;  /*Number of Streets(edges).*/
    struct adj_list     tsj_adj_list[MAX_TSJ]; /*Array of pointers of Adjacent list of all vertices to create a linked list of all the vertices a given vertex is connected to. It has size MAX_TSJ.*/
    struct street       *street_list[MAX_STREET]; /*Array of pointers which points to all streets on the graph(map). It has size MAX_STREET.*/
}graph_tsj_t;


typedef struct route
{
    int     no_of_steps; // No. of nodes to cross to take to reach dest.
    struct traffic_sig_jn    *steps[MAX_TSJ]; //dynamic array of steps containg by all the junction(traffic signals) by which vehicle pass.
    int     front; // array of first index
    int     rear; // array last index

}route_t;

	

/**********PROTOTYPES*************/

traffic_light_t *create_tsl(char *parent_id);
lane_t *create_lane(char *parent_id,int max_vehicles, struct coordinate *c1,struct coordinate *c2,struct style *sty);
lane_t **create_n_lane(char *parent_id,int max_vehicles,char *type,struct coordinate *c1,struct coordinate *c2,struct style *sty);
street_t *create_street(char *name, char *type, int max_vehicles ,struct coordinate *c1,struct coordinate *c2,struct style *sty ); // timer to be taken from stimulation function
vehicle_t **create_queue( int max_vehicles );
void queue_add_vehicle( lane_t *l,vehicle_t *v );
//vehicle_t *queue_delete(lane_t *l);
void queue_delete(lane_t *l);
street_t *create_street(char *name, char *type, int max_vehicles ,struct coordinate *c1,struct coordinate *c2,struct style *sty ); // timer to be taken from stimulation function                                                                                                // add graphics function.
char *gen_num(int n);
char *set_id(char *parent_id,char *type_id);
traffic_sig_jn_t *create_tsj(char *name ,coordinate_t *c,style_t *s);  // check this adjacency list.
map_extn_t *create_map_extn(graph_tsj_t *g,char *name, int time_of_generation , traffic_sig_jn_t *adj_tsj);
map_extn_list_t *create_map_extension_list(void);
int add_map_extn(graph_tsj_t *g, map_extn_list_t *map_extn_p , char *name, int time_of_generation, traffic_sig_jn_t *adj_tsj);
int delete_map_extn(map_extn_list_t *map_extn_head, int id);
map_extn_t *get_map_extn_node(map_extn_list_t *map_extn_p, char *id);
int menu_login();
int menu_admin();
int menu_user();
int submenu_admin();
int submenu_user();
int menu_tsj();
int menu_street();

void analyze_tsj(graph_tsj_t *g,int tsj_id);
int get_tsj_index(street_t *s,traffic_sig_jn_t *tsj);
int get_street_type(street_t *s);
float get_vehicle_type(vehicle_t *v);
int gen_id(char *ptr);
int get_tsj_id(traffic_sig_jn_t *ptr);
float set_traffic_status(street_t *s, traffic_sig_jn_t *t);
int timer (float array[]);

/*Graph Function Prototypes*/
void graph_init(graph_tsj_t *g);
int graph_add_tsj (graph_tsj_t *g,traffic_sig_jn_t *t);
int graph_delete_tsj(graph_tsj_t *g,traffic_sig_jn_t *t);
int graph_add_street(graph_tsj_t *g,int tsj1_id,int tsj2_id , char *st_name , char *type);
int graph_delete_street(graph_tsj_t *g, int tsj1_id, int tsj2_id);
void simulate(graph_tsj_t *g,map_extn_list_t *m);
void update_vehicles();
void display_graph(graph_tsj_t *g);
traffic_sig_jn_t *graph_get_pointer(int id);
void diplay_map();
void set_tsj_mode(traffic_light_t *A,traffic_light_t *B,traffic_light_t *C,traffic_light_t *D,int mode);
vertex_node_t *create_vertex_node(struct traffic_sig_jn *dest_tsj,struct street *connecting_street);
vehicle_t *create_vehicle();
int menu_edit_tsj();
void initialize_tsj_adj_list_node(adj_list_t *a_p , traffic_sig_jn_t *tsj);
void add_vertex_dest(struct adj_list *a,struct traffic_sig_jn *dest_tsj_p,struct street *connecting_street_p);
int get_max_vehicle(traffic_sig_jn_t *t1,traffic_sig_jn_t *t2);
void sort_streets_into_tsj(traffic_sig_jn_t *tsj_p);
traffic_sig_jn_t *get_tsj_ptr(graph_tsj_t *g,int id);
void tsj_rename(graph_tsj_t *g,int id,char *name);
void tsj_change_fill_color(graph_tsj_t *g,int id,int new_fill_color);
void tsj_change_border_color(graph_tsj_t *g,char *id,int new_fill_color);
void set_style(struct style *s,int fill_color,int border_style,int border_color,int width);
int copy_coordinate(struct coordinate *dest,struct coordinate *src);
int copy_style(struct style *dest,struct style *src);
int graphics_add_edge(struct graph_tsj *g,int tsj1,int tsj2);
void set_coordinate(struct coordinate *c,int x,int y,float rotation);
void draw_tsj(struct traffic_sig_jn *t);
int draw_street(struct street *st);
int draw_lane(struct lane *l);//(struct coordinate *c1,struct coordinate *c2,struct style *sty)
int display_map(graph_tsj_t *g);
void graph_print(graph_tsj_t *g);
void get_direction(map_extn_t *p_src , map_extn_t *p_dest , route_t *directions, graph_tsj_t *g);
/*Graph Prototypes*/
char *set_id_by_index(char *parent_id,char *type_id,int id_num);

int get_tsj_id(traffic_sig_jn_t *ptr);
int get_street_id(street_t *ptr);
int get_lane_id(lane_t *ptr);
int get_tsl_id(traffic_light_t *ptr);
int get_vehicle_id(vehicle_t *ptr);
int get_men_id(map_extn_t *ptr);

int compare_id(char *char_id , int int_id);

street_t *add_street_to_map_extn_node(struct graph_tsj *g,map_extn_t *m , char *st_name ,int tsj1_id, int type);

/******GRAPH PROTOTYPES*******/
void draw_vehicles(lane_t *l);
void set_style(struct style *s,int fill_color,int border_style,int border_color,int width);
int copy_coordinate(struct coordinate *dest,struct coordinate *src);
int copy_style(struct style *dest,struct style *src);
void set_coordinate(struct coordinate *c,int x,int y,float rotation);
void set_style(struct style *s,int fill_color,int border_style,int border_color,int width);
void draw_tsl();
void draw_tsj(struct traffic_sig_jn *t);
int draw_street(struct street *st);
int draw_lane(struct lane *l);
int display_map(struct graph_tsj *g);
void graph_print(struct graph_tsj *g);
void graphic_add_tsj();

/********************************/

/************************************* GRAPH ************************************************/





/*Updated COPY Functions*/
int copy_coordinate(struct coordinate *dest,struct coordinate *src)
{

    dest->x         = src->x;
    dest->y         = src->y;
    dest->rotation  = src->rotation;

    return 1;
}

int copy_style(struct style *dest,struct style *src)
{
    dest->fill_color    = src->fill_color;
    dest->border_style  = src->border_style;
    dest->border_color  = src->border_color;
    dest->width         = src->width;

    return 1;
}




/*Updated Set Functions*/
/*Updated Set Coordinates*/
void set_coordinate(struct coordinate *c,int x,int y,float rotation)
{
	c->x 		= x;
	c->y		= y;
	c->rotation = rotation;
}
/*Updated Set Style*/
void set_style(struct style *s,int fill_color,int border_style,int border_color,int width)
{
	s->fill_color	= fill_color;
	s->border_style	= border_style;
	s->border_color	= border_color;
	s->width        = width;
}

/*DRAW Functions*/
/*TBD : Draw Traffic Signal Light*/
void draw_tsl()
{
    ;
}

/*Updated Draw Traffic Signal Junction*/
void draw_tsj(struct traffic_sig_jn *t)
{
    circle((t->coord).x,(t->coord).y,(t->sty).width);
    floodfill((t->coord).x,(t->coord).y,(t->sty).fill_color);
}

/*Updated Draw Street*/
int draw_street(struct street *st)
{
    int i;
    
    for(i=0;i<st->nol[0];++i)
    {
        draw_lane(st->lanes[0][i]);
        draw_lane(st->lanes[1][i]);
    }

    /*TBD: Draw TSL*/

    return 1;
}

/*Updated Draw Lane*/
int draw_lane(struct lane *l)/*(struct coordinate *c1,struct coordinate *c2,struct style *sty)*/
{
	double ang,ang2;
    struct coordinate   *c1 = &l->c1;
    struct coordinate   *c2 = &l->c2;
    struct style        *sty= &l->sty;

    /*Set angle of rotation*/
	ang = atan( (c2->y-c1->y)/(c2->x-c1->x) );	//Slope of line joining (c1->x,c1->y) and (c2->x,c2->y).
	ang2 = ang + M_PI/2;
	/*
	c1->xnew = c1->x + width*cos(ang2);
	c1->ynew = c1->y + width*sin(ang2);
	*/
	int p[10];

	p[0] = c1->x + sty->width*cos(ang2);
	p[1] = c1->y + sty->width*sin(ang2);
	p[2] = c2->x + sty->width*cos(ang2);
	p[3] = c2->y + sty->width*sin(ang2);

	p[6] = c1->x - sty->width*cos(ang2);
	p[7] = c1->y - sty->width*sin(ang2);
	p[4] = c2->x - sty->width*cos(ang2);
	p[5] = c2->y - sty->width*sin(ang2);

	p[8] = c1->x + sty->width*cos(ang2);
	p[9] = c1->y + sty->width*sin(ang2);
	setcolor(sty->border_color);

	drawpoly(5,p);

	floodfill((c1->x+c2->x)/2,(c1->y+c2->y)/2,sty->fill_color);
	//printf("%d",getx);

	draw_vehicles(l);
}


int display_map(struct graph_tsj *g)
{
    int i;

    /*Draw Street(Edges)*/
    for(i=0;i<g->num_street;++i)
    {
        draw_street(g->street_list[i]);
    }

    /*Draw TSJ's(Vertices)*/
    for(i=0;i<g->num_tsj;++i)
    {
        draw_tsj(g->tsj_adj_list[i].src_tsj);
    }
}

/*Print Graph Data*
void graph_print(struct graph_tsj *g)
{
    int i;

    printf("\nTSJ ID\tList of Connected TSJs");

    /*Print Linked list of each connected node*
    for(i=0 ; i<(g->num_of_tsj) ; ++i)
    {
        printf("\n%s :\t",g->adj_list[i]->src_tsj_p->name);

        struct vertex *ptr = g->adj_list[i]->list;
        while(ptr)
        {
            printf("%d ",ptr->dest->id);
            ptr = ptr->next;
        }
    }

    /*Edge List Display*
    printf("\nEdge ID\tTSJ1\tTSJ2");

    for(i=0;i<g->num_of_edges;++i)
    {
        struct lane *l = g->street_list[i]->l;
        printf("\n%d",*l->id);
    }*
}
*/
/*Draw Vehicles*/
void draw_vehicles(lane_t *l)
{
    int i;
	
    int x1 = l->c1.x;
    int x2 = l->c2.x;

    int y1 = l->c1.y;
    int y2 = l->c2.y;

    //float slope = slope_of_line(l->c1,l->c2);

    int x,y;
    int vehicle_sz=2;

    int div         = l->max_vehicles + 1 ;
    int x_increment   = ( x2-x1 )/div;
    int y_increment   = ( y2-y1 )/div;

    for(i=1 ; i <= l->max_vehicles ; ++i)
    {
        if((l->queue+i) != NULL)
        {
            /*Formulation of x and y*/
            x = x1 + i*x_increment;
            y = y1 + i*y_increment;
    
            /*Display Vehicle*/
            fillellipse(x,y,vehicle_sz,vehicle_sz);
        }
    }
}





/********************************************************************************************/

/************************************* GRAPH ************************************************/



/*Graph Functions*/
void graph_init(graph_tsj_t *g)
{
	g->num_tsj		= 0;
	g->num_street	= 0;
}

/*Add TSJ to Graph*/
int graph_add_tsj (graph_tsj_t *g,traffic_sig_jn_t *src_tsj)
{
	if(g->num_tsj == MAX_TSJ)
	{
		return 0;
	}

    g->num_tsj++;

    src_tsj->id=set_id("",src_tsj->type);
	initialize_tsj_adj_list_node(&g->tsj_adj_list[g->num_tsj-1],src_tsj);

    return 1;
}

/*Initialize Adjacency List*/
void initialize_tsj_adj_list_node(adj_list_t *a_p , traffic_sig_jn_t *tsj)
{
    a_p->src_tsj         = tsj;
    a_p->head            = NULL;
    a_p->num_of_adj_tsj  = 0;
}


/*Delete TSJ from Graph*/
int graph_delete_tsj(graph_tsj_t *g,traffic_sig_jn_t *t)
{
	int i,id;

	if(g->num_tsj==0)
	{
		return 0;
	}

	else
	{
		vertex_node_t *curr,*temp;
		id=get_tsj_id(t);

		for(i=get_tsj_id(t)-1;i<MAX_TSJ;i++)
		{

		    g->tsj_adj_list[i]=g->tsj_adj_list[i+1];

		}

		curr=g->tsj_adj_list[gen_id(t->id)-1].head;

		while(curr!=NULL)
		{

			temp=curr;
			graph_delete_street(g , get_tsj_id(t) , get_tsj_id(curr->dest_tsj_p) );
			curr=curr->next;
			free(temp);
		}
	}

	return 1;
}

/*Add Street to Map Extension Node*/
street_t *add_street_to_map_extn_node(struct graph_tsj *g,map_extn_t *m , char *st_name ,int tsj1_id, int type)
{
	int index,i;
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
	t2=(traffic_sig_jn_t *)malloc(sizeof(traffic_sig_jn_t));
	
	copy_coordinate(&t2->coord,&m->coord);
	
	/*STYLE*/
    /*Create the dynamically allocated style object*/
    style_t *s=(style_t *)malloc(sizeof(style_t *));


	/*Set the style properties*/
	set_style(s,LIGHTGREY,LIGHTGREY,LIGHTGREY,5);// TBD: CHECK Properties BY NILADRI

	/*MAX VEHICLE CALULATION => Distance Formula*/
    max_vehicle = get_max_vehicle(t1,t2);


    /*NEW STREET*/
    /*Create dynamically allocated street_t object*/
    street_t *new_street=create_street(st_name,"four_way",max_vehicle,&t1->coord,&t2->coord,s); // timer to be taken from stimulation function//

   	/*Set STREET ID*/
	new_street->id = set_id_by_index("","STR",index);

	/*Allocate Memory to the Edge*/
    g->street_list[g->num_street-1]=new_street;

	/*Add Destination Vertex to the tsj1 and tsj2*/
    add_vertex_dest(&g->tsj_adj_list[tsj1_id] , t2 , new_street);

   return new_street;


}
/*Create Vertex Node*/
vertex_node_t *create_vertex_node(struct traffic_sig_jn *dest_tsj,struct street *connecting_street)
{
    struct vertex_node *new_v = (struct vertex_node *)malloc(sizeof(struct vertex_node));

    new_v->dest_tsj_p    = dest_tsj;
    new_v->street_p     = connecting_street;
    new_v->next         = NULL;

    return new_v;
}

/*Add Vertex Destination*/
void add_vertex_dest(struct adj_list *a,struct traffic_sig_jn *dest_tsj_p,struct street *connecting_street_p)
{
    /*Increment Number of Adjacent TSJ*/
    a->num_of_adj_tsj++;

    /*Create Vertex Node with the destination TSJ*/
    vertex_node_t *new_vnode = create_vertex_node(dest_tsj_p,connecting_street_p);
}

/*Get Maximum Vehicle Capacity of a Steet*/
int get_max_vehicle(traffic_sig_jn_t *t1,traffic_sig_jn_t *t2)
{
    int dist;
    int n=2;

    /*Distance Fourmula*/
    dist = abs((int)sqrt(pow(t1->coord.x - t2->coord.x,2) - pow(t1->coord.y - t2->coord.y,2)));

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
        if(compare_id(g->tsj_adj_list[i].src_tsj->id,id))
            return g->tsj_adj_list[i].src_tsj;
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
void tsj_change_fill_color(graph_tsj_t *g,int id,int new_fill_color)
{
    /*Get Pointer to TSJ*/
    traffic_sig_jn_t *t = get_tsj_ptr(g,id);

    t->sty.fill_color = new_fill_color;
}

/*Change TSJ*/
void tsj_change_border_color(graph_tsj_t *g,char *id,int new_border_color)
{
    /*Get Pointer to TSJ*/
    traffic_sig_jn_t *t = get_tsj_ptr(g,gen_id(id));

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

    for(i=0;i<g->num_street;++i)
    {
        i=0;
        /*Get id of the Street in Character form and Compare*/
        if(compare_id(g->street_list[i]->id , street_id))
            return g->street_list[i];
    }

    return NULL;
}

/**Slope Of Line**/
float slope_of_line(coordinate_t *c1 , coordinate_t *c2)
{
    return (c1->y - c2->y)/(c1->x - c2->x);
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
int graph_delete_street(graph_tsj_t *g, int tsj1_id, int tsj2_id)
{
	street_t *st;
	vertex_node_t *ptr, *prev;
	int street_id;

	traffic_sig_jn_t *t1,*t2;


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
    street_id = get_street_id(ptr->street_p);

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
    street_id = get_street_id(ptr->street_p);

    /*Set the connection from TSJ 1 to TSJ 2 and TSJ2 and TSJ 1 to zero(0/NULL)*/
    prev->next = ptr->next;
    free(ptr);


	/*Deallocate the street that is deleted*/
    free(g->street_list[street_id]);
    g->street_list[street_id] = NULL;

    g->num_street--;
}

/********* GRAPH : ADD STREET **************/
/*Add Street to Graph*/ /*TBD : EDIT Street Type Datatype*/
int graph_add_street(graph_tsj_t *g,int tsj1_id,int tsj2_id , char *st_name , char *type)
{
    int index;
    int i;
    
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


	/*Set the style properties*/
	set_style(s,LIGHTGREY,LIGHTGREY,LIGHTGREY,5);// TBD: CHECK Properties BY NILADRI

	/*MAX VEHICLE CALULATION => Distance Formula*/
    max_vehicle = get_max_vehicle(t1,t2);


    /*NEW STREET*/
    /*Create dynamically allocated street_t object*/
    street_t *new_street=create_street(st_name,type,max_vehicle,&t1->coord,&t2->coord,s); // timer to be taken from stimulation function//

   	/*Set ID*/
	new_street->id = set_id_by_index("","STR",index);

	/*Allocate Memory to the Edge*/
    g->street_list[g->num_street-1]=new_street;

	/*Add Destination Vertex to the tsj1 and tsj2*/
    add_vertex_dest(&g->tsj_adj_list[tsj1_id] , t2 , new_street);
    add_vertex_dest(&g->tsj_adj_list[tsj2_id] , t1 , new_street);

    return 1;
}

/******Add Street by ID**************/
void graph_add_street_by_id(graph_tsj_t *g,int street_id , char *st_name , char *type)
{
    street_t *temp_st = get_street_ptr(g,street_id);

    graph_add_street( g , get_tsj_id(temp_st->tsj[0]) , get_tsj_id(temp_st->tsj[1]) , st_name , type);
}

/******Delete Street by ID**************/
void graph_delete_street_by_id(graph_tsj_t *g,int street_id)
{
    street_t *st = get_street_ptr(g,street_id);

    graph_delete_street( g , get_tsj_id(st->tsj[0]) , get_tsj_id(st->tsj[1]) );
}


/*Rename Traffic Signal Junction*/
void rename_tsj(graph_tsj_t *g,int id,char *name)
{
    /*Get Pointer to TSJ*/
    traffic_sig_jn_t *t = get_tsj_ptr(g,id);

    strcpy(t->name,name);
}




/*******************************************************************************************/

/******************************* GENERATE ID ********************************/

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
 *(num+4)='\0';
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
 int n=strlen(ptr)-1;
 for(i=n,j=0;i>n-4,j<4;i--,j++)     //loop for storing the number in integer format
 {
  n1=(int)(*(ptr+i) - '0');        //storing the number in integer format
  num=(n1*pow(10,j))+num;
 }
 return num;                       //returning the generated number
}

/* Functions for getting specific ids'*/
int get_tsj_id(traffic_sig_jn_t *ptr)  //getting traffic signal junction id
{
    return gen_id(ptr->id);
}
int get_street_id(street_t *ptr)          //getting street id
{
    return gen_id(ptr->id);
}
int get_lane_id(lane_t *ptr)            //getting lane id
{
    return gen_id(ptr->id);
}
int get_tsl_id(traffic_light_t *ptr)   //getting traffic light id
{
    return gen_id(ptr->id);
}
int get_vehicle_id(vehicle_t *ptr)         //getting vehicle id
{
    return gen_id(ptr->id);
}
int get_men_id(map_extn_t *ptr)        //generating map extension node id
{
    return gen_id(ptr->id);
}

/****************************************************************************/

/****************************************************************************/

/*------------------------------------------------------------------------------------------------------------------------------------
function definition- in this function traffic light is being created by a street function having argument an id of that street,the id of this traffic light  is obtained by function set_id().the status of all light and directions will be inputted by traffic_light tsl function call by stimulator from the traffic_light pointer ie tsl from the respective street.
-------------------------------------------------------------------------------------------------------------------------------------
*/

vehicle_t **create_queue( int max_vehicles )
{
    int i;
    vehicle_t **ptr;
    ptr=(vehicle_t **)malloc((sizeof(vehicle_t *))*max_vehicles);
    for(i=0;i<max_vehicles;i++){
    	ptr[i] = NULL	 ;
    }
    return ptr;

}


traffic_light_t *create_tsl(char *parent_id)
{
    traffic_light_t *ptr=NULL;

    ptr=(traffic_light_t *)malloc(sizeof(traffic_light_t));
    ptr->id=set_id(parent_id, "TSL");/*id of the street fro it is created being passed into function set_id*/
   /* ptr->timer=0;//int value to be taken by stimulation function/analysis*/
    ptr->red=0;/*all these are initially null and value provided by stimuation function(as it provide the mode) by the pointer*/
    ptr->yellow=0;
    ptr->forward=0;
    ptr->left=0;
    ptr->right=0;
    ptr->flag=0;
    ptr->traffic_status=0;
    //initialize coordinate(ptr,coord) TBD
    return ptr;

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




street_t *create_street(char *name, char *type, int max_vehicles ,struct coordinate *c1,struct coordinate *c2,struct style *sty ) // timer to be taken from stimulation function                                                                                                // add graphics function.
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



traffic_sig_jn_t *create_tsj(char *name ,coordinate_t *c,style_t *s)  // check this adjacency list.
{

    traffic_sig_jn_t *ptr=(traffic_sig_jn_t *)malloc(sizeof(traffic_sig_jn_t)) ;

    int i;

    ptr->differ		= 0;
    ptr->start		= 0;
    ptr->mode		= 0;
    ptr->flptr		= 0;

    strcpy(ptr->name,name);
    ptr->id = set_id("", "TSJ");
    strcpy(ptr->type,"FOUR");

    copy_coordinate(&(ptr->coord) , c);
    copy_style(&(ptr->sty) , s);

   /*  if(strcmp(type,"three_side")==0)
    {
        ptr->no_of_adj_sig = 3;
    }

   else if(strcmp(type,"four_side")==0)
    {
        ptr->no_of_adj_sig = 4;
    }

    else if(strcmp(type,"five_side")==0)
    {
        ptr->no_of_adj_sig = 5;
    }
*/



   /* if(strcmp(type,"three_side")==0)
    {
        i=0;
        ptr->adj_signal = (traffic_sig_jn_t **)malloc((sizeof(traffic_sig_jn_t *))*3);  // to complete this syntax of adjacency list is required.
        while(adjacency_list[id]!= NULL)
            {

            adj_signal[i] = adjacent_l1st[id]->ptr some thing;
            i++;
            }


    }

   else if(strcmp(type,"four_side")==0)
    {
        ptr->adj_signal = (traffic_sig_jn_t **)malloc((sizeof(traffic_sig_jn_t *))*4);
        i=0;
        while(adjacent_l1st[id]->ptr some thing != NULL)
            {

            adj_signal[i] = adjacent_l1st[id]->ptr some thing;
            i++;
            }

    }
*/
   /*else  if(strcmp(type,"five_side")==0)
    {
        ptr->adj_signal = (traffic_sig_jn_t **)malloc((sizeof(traffic_sig_jn_t *))*5);
        i=0;
        while(adjacent_l1st[id]->ptr some thing != NULL)
            {

            adj_signal[i] = adjacent_l1st[id]->ptr some thing;
            i++
            }
    }*/

// will be completed after above one.

   /* struct street **streets_out;           // Pointer to node toward adjacent traffic light,
    struct street **streets_in;         road from adjacent traffic signal, although no need for it*/

	ptr->streets=NULL;


    return ptr;
}

map_extn_t *create_map_extn(graph_tsj_t *g,char *name, int time_of_generation , traffic_sig_jn_t *adj_tsj)
{

    map_extn_t *newnode;
    newnode = (map_extn_t *)malloc(sizeof(map_extn_t));
    strcpy(newnode->name,name);
    newnode->id = set_id("","MEN");
    newnode->time_of_generation = time_of_generation;
    newnode->next = NULL;
    newnode->adj_signal = adj_tsj;

    newnode->connecting_street = add_street_to_map_extn_node(g,newnode,name,get_tsj_id(adj_tsj),2);

    return newnode;
}

map_extn_list_t *create_map_extension_list(void)
{


    map_extn_list_t *newnode;
    newnode = (map_extn_list_t *)malloc(sizeof(map_extn_list_t));
    newnode->num_map_extn = 0;
    newnode->head = NULL;

    return newnode;
}


void queue_add_vehicle( lane_t *l,vehicle_t *v )  // circular queue of array
{
	l->queue[l->max_vehicles-1]=v;

}


void queue_delete(lane_t *l)
{
    l->queue[0]=NULL;
}


/******************************** MAP EXTENSION NODE *************************************************/

int add_map_extn(graph_tsj_t *g, map_extn_list_t *map_extn_p , char *name, int time_of_generation, traffic_sig_jn_t *adj_tsj)
{
    map_extn_t *newnode, *temp,*head;

    newnode = create_map_extn(g, name,time_of_generation ,adj_tsj);
    (map_extn_p->num_map_extn)++;
    if (map_extn_p->head == NULL){
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

/***********/
//get_min_dist_tsj

/************/


int delete_map_extn(map_extn_list_t *map_extn_p, int id)
{
    int i=0;
    map_extn_t *temp, *prev;
    temp = map_extn_p->head;
    prev = map_extn_p->head;

    while(temp->next != NULL)
    {
        if( compare_id(temp->id,id)==0)
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

    else if(compare_id(map_extn_p->head->id,id) == 0)
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


map_extn_t *get_map_extn_node(map_extn_list_t *map_extn_p, char *id)
{

 int i=0;
    map_extn_t *temp;
    temp = map_extn_p->head;

    while(temp->next != NULL)
    {
        if(strcmp(temp->id,id)==0)
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



/*********************************************/


/************************* VEHICLE ******************************/
/* Function:- Initialize vehicle */
vehicle_t *create_vehicle()
{
    vehicle_t *ptr;
    ptr=(vehicle_t *)malloc(sizeof(vehicle_t));
    ptr->id=NULL;
    ptr->speed=0;
    ptr->source=NULL;
    ptr->dest=NULL;
    ptr->directions=NULL;
    ptr->priority_status=0;

    return ptr;
}


/*Function to get the index of the TSJ inside the street corresponding to the passesTSJ*/
int get_tsj_index(street_t *s,traffic_sig_jn_t *tsj)
{

    int i;
    /*
    Comparing each of the two TSJ's
    */
    for(i=0 ; i<2 ;++i)
    {
        if(s->tsj[i] == tsj)
        {
            return i;   /*FOUND: TSJ found at index 'i'*/
        }
    }

    return -1;  /*FAILURE: TSJ not found*/
}

/*Function to return type of the street as an int*/
int get_street_type(street_t *s)
{
    char two_way[]="two_way";
    char four_way[]="four_way";

    if(strcmp(s->type,two_way) == 0)
    {
        return 2;
    }
    else if(strcmp(s->type,four_way) == 0)
    {
        return 4;
    }

    /*FAILURE*/
    return -1;
}

float vehicle_type(vehicle_t *v)
{
     if(strcmp(v->type,"Car") == 0 || strcmp(v->type,"Bike") == 0)
    {
        return 1;
    }
    else if(strcmp(v->type,"Bus") == 0)
    {
        return 1.75;
    }

    else if(strcmp(v->type,"Truck") == 0)
    {
        return 2.25;
    }


    /*FAILURE*/
    return -1;

}









/*
Function Name :- set_tsl_mode
Created On :- 22/03/15
Function Description :-
This function contains multiple cases of all possible combinations of the operations performed by a single traffic signal light at a junction.
It takes 'mode' as a parameter and each mode corresponds to a particular setting of the traffic signal light for a particular lane coming at a junction.It would return SUCCESS(1) or FAILURE(0) to the caller function.
*/

/*
tsl-> It is a pointer to the structure 'traffic_light' which contains all the possible functions depicted by a traffic light such as forward,
right,left,red and yellow.
mode-> It is a variable which stores all the modes of a traffic light.
return type-> (int) i.e. it returns either success(1) or failure(0) set mode function.
 */

 #define ON 1
 #define OFF 0
 int set_tsl_mode(struct traffic_light *tsl, int mode)
 {
     int ret=1;
     switch(mode)
     {
         case 1: /* mode1- For stop traffic condition */
         tsl->red=ON;
         tsl->yellow=OFF;
         tsl->forward=OFF;
         tsl->right=OFF;
         tsl->left=OFF;
         break;

         case 2: /* mode2- For slow traffic condition */
         tsl->red=OFF;
         tsl->yellow=ON;
         tsl->forward=OFF;
         tsl->right=OFF;
         tsl->left=OFF;
         break;

         case 3: /* mode3- 'Forward' traffic flow condition */
         tsl->red=OFF;
         tsl->yellow=OFF;
         tsl->forward=ON;
         tsl->right=OFF;
         tsl->left=OFF;
         break;
         case 4: /* mode4- 'Left' traffic flow condition */
         tsl->red=OFF;
         tsl->yellow=OFF;
         tsl->forward=OFF;
         tsl->right=OFF;
         tsl->left=ON;
         break;
         case 5: /* mode5- 'Right' traffic flow condition */
         tsl->red=OFF;
         tsl->yellow=OFF;
         tsl->forward=OFF;
         tsl->right=ON;
         tsl->left=OFF;
         break;
         case 6: /* mode6- 'Forward and Left' traffic flow condition */
         tsl->red=OFF;
         tsl->yellow=OFF;
         tsl->forward=ON;
         tsl->right=OFF;
         tsl->left=ON;
         break;
         case 7: /* mode7- 'Forward and Right' traffic flow condition */
         tsl->red=OFF;
         tsl->yellow=OFF;
         tsl->forward=ON;
         tsl->right=ON;
         tsl->left=OFF;
         break;
         case 8: /* mode8- 'Forward, Left and Right' traffic flow condition */
         tsl->red=OFF;
         tsl->yellow=OFF;
         tsl->forward=ON;
         tsl->right=ON;
         tsl->left=ON;
         break;
         case 9: /* mode9- 'Right and Left' traffic flow condition */
         tsl->red=OFF;
         tsl->yellow=OFF;
         tsl->forward=OFF;
         tsl->right=ON;
         tsl->left=ON;
         break;
         default:
         ret=0;
         break;
     }

     return 1;
  }





/*  Function:- Set Traffic Signal Junction Mode
    The function takes pointers to traffic lights at a junction and sets traffic light mode for the respective traffic light */

void set_tsj_mode(traffic_light_t *A,traffic_light_t *B,traffic_light_t *C,traffic_light_t *D,int mode)
{
    switch(mode)                    /* Cases to pass the modes of traffic lights */
    {
        case 1:
            set_tsl_mode(A,8);
            set_tsl_mode(B,1);
            set_tsl_mode(C,1);
            set_tsl_mode(D,5);
            break;
        case 2:
            set_tsl_mode(A,7);
            set_tsl_mode(B,1);
            set_tsl_mode(C,7);
            set_tsl_mode(D,1);
            break;
        case 3:
            set_tsl_mode(A,7);
            set_tsl_mode(B,1);
            set_tsl_mode(C,5);
            set_tsl_mode(D,5);
            break;
        case 4:
            set_tsl_mode(A,5);
            set_tsl_mode(B,8);
            set_tsl_mode(C,1);
            set_tsl_mode(D,1);
            break;
        case 5:
            set_tsl_mode(A,1);
            set_tsl_mode(B,1);
            set_tsl_mode(C,1);
            set_tsl_mode(D,7);
            break;
        case 6:
            set_tsl_mode(A,5);
            set_tsl_mode(B,1);
            set_tsl_mode(C,1);
            set_tsl_mode(D,5);
            break;
        case 7:
            set_tsl_mode(A,1);
            set_tsl_mode(B,5);
            set_tsl_mode(C,8);
            set_tsl_mode(D,1);
            break;
        case 8:
            set_tsl_mode(A,7);
            set_tsl_mode(B,1);
            set_tsl_mode(C,7);
            set_tsl_mode(D,1);
            break;
        case 9:
            set_tsl_mode(A,5);
            set_tsl_mode(B,5);
            set_tsl_mode(C,7);
            set_tsl_mode(D,1);
            break;
        case 10:
            set_tsl_mode(A,1);
            set_tsl_mode(B,5);
            set_tsl_mode(C,1);
            set_tsl_mode(D,8);
            break;
        case 11:
            set_tsl_mode(A,1);
            set_tsl_mode(B,7);
            set_tsl_mode(C,1);
            set_tsl_mode(D,7);
            break;
        case 12:
            set_tsl_mode(A,1);
            set_tsl_mode(B,5);
            set_tsl_mode(C,5);
            set_tsl_mode(D,7);
            break;
        case 13:
            set_tsl_mode(A,4);
            set_tsl_mode(B,8);
            set_tsl_mode(C,4);
            set_tsl_mode(D,4);
            break;
        case 14:
            set_tsl_mode(A,8);
            set_tsl_mode(B,4);
            set_tsl_mode(C,4);
            set_tsl_mode(D,4);
            break;
        case 15:
            set_tsl_mode(A,4);
            set_tsl_mode(B,4);
            set_tsl_mode(C,4);
            set_tsl_mode(D,8);
            break;
        case 16:
            set_tsl_mode(A,4);
            set_tsl_mode(B,4);
            set_tsl_mode(C,8);
            set_tsl_mode(D,4);
            break;
        case 17:
            set_tsl_mode(A,4);
            set_tsl_mode(B,6);
            set_tsl_mode(C,4);
            set_tsl_mode(D,6);
            break;
        case 18:
            set_tsl_mode(A,6);
            set_tsl_mode(B,4);
            set_tsl_mode(C,6);
            set_tsl_mode(D,4);
            break;


    }

}

float set_traffic_status(street_t *s, traffic_sig_jn_t *t)
{
    lane_t ***l;
    int i, j, k, type, sum=0, index ;
    float a, *array;
    int streets;
    index = get_tsj_index(s,t);
    streets = get_street_type(s);

    if(index==0)
    {
        *l= s->lanes[index];
    }

    else if(index==1)
    {
        *l= s->lanes[index];
    }

    if(streets==2)
    {
        k=l[index][0]->curr_vehicles;
        array =(float *) malloc(sizeof(float)*k);
        for(i=0 ; i<k ; i++)
        {
            array[i] = vehicle_type(l[index][0]->queue[i]);
        }

    }

    else if(streets==4)
    {
        k= l[index][0]->curr_vehicles + l[index][1]->curr_vehicles;
        array =(float *) malloc(sizeof(float)*k);

        type = 0;
        for(j=0;j<2;j++)
        {
            for(i=0 ; i<l[index][j]->curr_vehicles ; i++)
            {
                array[type+i] = vehicle_type(l[index][j]->queue[i]);
            }
            type = i;
        }
    }

    for(i=0;i<k;i++)
    {
        sum += array[i];
    }


    if (streets==2)
    {
        a= (sum*100)/((l[index][0]->max_vehicles)*2.25);
    }

    else if (streets==4)
    {
        a= (sum*200)/((l[index][0]->max_vehicles)*2.25);

    }


    return a;
}


int timer (float array[])
{
    int total_time, main_time;

    if(array[0]<30.0 && array[0]> 0.00)
    {
        total_time = 120;
    }

    else if(array[0]<50.0 && array[0]> 30.0)
    {
        total_time = 180;
    }

    else if(array[0]> 50.00)
    {
        total_time = 240;
    }

    main_time = (array[0]/(array[0]+array[1]+array[4]+array[3]))*total_time;

    return main_time;
}




int is_adjacent(traffic_light_t *travelled[],traffic_light_t *sorted_array[],traffic_light_t *A,traffic_light_t *B,traffic_light_t *C,traffic_light_t *D) // function to check if tls is adjacent or not
{
    if(sorted_array[0]==A && (travelled[0]==B||travelled[0]==D))
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


void analyze_tsj(graph_tsj_t *g,int tsj_id) // main analyze function
{
    time_t current;
    int street_type,i,j,k, index;
    street_t *arr[4];
    float busy[4];
    traffic_sig_jn_t *point;
    traffic_light_t *sorted_array[4], *travelled[3],*temp;
    point=get_tsj_ptr(g,tsj_id);  // pointer of tsj from tsj id
    street_type=get_street_type(arr[0]); // type of street 2 way or 4 way


    /*storing the tls pointer of a tsj in arr*/
    for(i=0;i<4;i++)
    {
        arr[i]=point->streets[i];
    }


/*naming the pointes A, B, C, D to simplify*/
    traffic_light_t *A=arr[0]->tsl[get_tsj_index(arr[0],point)];
    traffic_light_t *B=arr[1]->tsl[get_tsj_index(arr[1],point)];
    traffic_light_t *C=arr[2]->tsl[get_tsj_index(arr[2],point)];
    traffic_light_t *D=arr[3]->tsl[get_tsj_index(arr[3],point)];


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
            if((sorted_array[j]->traffic_status)<(sorted_array[k]->traffic_status))
            {
                (sorted_array[j]->traffic_status)+=(sorted_array[k]->traffic_status);
                (sorted_array[k]->traffic_status)=(sorted_array[j]->traffic_status)-(sorted_array[k]->traffic_status);
                (sorted_array[j]->traffic_status)-=(sorted_array[k]->traffic_status);
            }
            else
            {
                k++;
            }
        }
    }

for(i=0;i<4;i++)
{
	busy[i]=sorted_array[i]->traffic_status;
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
            if((travelled[i]->flag==1))
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
   if(difftime(current,point->start) >point->differ)
   {
      point->differ=timer(busy);//return difference btw current time and starting time

        /*2. if of street type, in this case 2 way*/
       /* if(street_type==2)// if of type
       {

       }*/

       /*2. if of street type, in this case 4 way*/
       if(street_type==4)// if of type
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
                    if(is_adjacent(travelled,sorted_array,A,B,C,D))//check both are adjacent or not
                    {
                        set_tsj_mode(A,B,C,D,mode_type);//b first_condition in 4way.xlsx
                        point->mode=mode_type;
                    }
                    else
                    {
                        set_tsj_mode(A,B,C,D,mode_type);// c first condition in 4way.xlsx
                        point->mode=mode_type;
                        point->flptr=1;
                    }

               }

                /*Loop to increment the flags of all tsl of a junction after travelling base mode.*/
                for(k=0;k<4;k++)
                {
                    if(sorted_array[k]->flag!=0)
                    {
                        sorted_array[k]->flag=((sorted_array[k]->flag)+1)%4;
                    }
                }
                (sorted_array[0]->flag)++;
           }


            else  // mode for sub mode
            {
                if(point->flptr==1)
                {
                    point->flptr=0;
                    point->mode=0;

                }
                else
                {
                    if(point->mode==14)//A
                    {
                        set_tsj_mode(A,B,C,D,18);
                        point->mode=18;
                    }
                    if(point->mode==13)//B
                    {
                        set_tsj_mode(A,B,C,D,17);
                        point->mode=17;
                    }
                    if(point->mode==16)//C
                    {
                        set_tsj_mode(A,B,C,D,18);
                        point->mode=18;
                    }
                    if(point->mode==15)//D
                    {
                        set_tsj_mode(A,B,C,D,17);
                        point->mode=17;
                    }

                }
                point->differ=point->differ*0.25;

            }

       }
       point->start=current;
     }
}




char *random_vehicle()//function to return random vehicle type
    {
       int x;char *ptr;
       char *arr[4]={"Bus","Car","Bike","Truck"};
        x=rand()%4;
        ptr=(char *)malloc(sizeof(char)*strlen(arr[x]+1));
        strcpy(ptr,arr[x]);
        return ptr;
    }


    void vehicle_move(vehicle_t *arr[],int size)
    {
    int f=0,r,i;
    r=size-1;
   for(i=f;i<=r;i++)//loop from front to rear
   {
        while(arr[i]==NULL)//
        {
            i++;
        }
        arr[i-1]=arr[i];
        arr[i]=NULL;
    }
    }


/********************************UNTILED.TXT********************************/


street_t *get_street_pointer(traffic_sig_jn_t *tsj, street_t *s, int k) // function to return required street pointer either adjacent or opposite
{
    int i,j;
    street_t *temp[4];

    for(i=0;i<4;i++)
    {
        temp[i] = tsj->streets[i];
    }
    i=0;
    while(temp[i]!=s)
    {
        i++;
    }

    for(j=0;j<=k;j++)

    {
        i = (i+1)%4;
    }

    return temp[i];
}




int vehicle_direction(vehicle_t *veh, traffic_sig_jn_t *tsj, graph_tsj_t *g, street_t *street)
{
    int i, j=0, k, count=0;
    traffic_sig_jn_t *temp1, *temp2;
    traffic_sig_jn_t *prev;
    street_t *arr[4];

    for(i=0;i<4;i++)
    {
        arr[i] = tsj->streets[i];
    }

    temp1 = veh->directions->steps[veh->directions->front]; // next tsl of the vehicle to be followed

        if(temp1 == NULL)
        {
            return -99;
        }


    for(i=0;i<4;i++)// index of present street
    {
        if(arr[i]==street)
            break;
    }

    k=0;
    for(j=(i+1)%4;j<4;j++)
    {
        if(arr[j]->tsj[0]!=tsj)
        {
            temp2 = arr[j]->tsj[0];
        }

        else
        {
            temp2=arr[j]->tsj[1];
        }


        if(temp2==temp1)
        {
            break;
        }



        if(j==3)
        {
            j=0;
        }

        k++;
    }


    return k;
}



void set_vehicle(vehicle_t *vehicle, graph_tsj_t *g, map_extn_list_t *extn_list, map_extn_t *ptr )
{
    int i, j=1, k, num_extn;
    map_extn_t *temp;

    vehicle->id = set_id("","VEH");
    vehicle->speed = rand()%100;
    strcpy(vehicle->type,random_vehicle());
    vehicle->source = ptr;

    do
    {
        num_extn = rand()%extn_list->num_map_extn;
        temp = extn_list->head;
        for(i=0;i<num_extn;i++)
        {
            temp = temp->next;

        }
        if(temp!=ptr)
            {
                j=0;
            }

    }while(j);

    vehicle->dest = temp;
    strcpy(vehicle->vehicle_no, vehicle->id);

    vehicle->directions = (route_t *)malloc(sizeof(route_t));
    
    /*****************************************************************************<<<<<<<<<<<<<<<<<<<<<<<<<ADD GET DIRECTIONS ********/
    //get_direction(vehicle->source, vehicle->dest, vehicle->directions, g);
}


void update_vehicles(graph_tsj_t *g)
{
    int i,j, k[2], count, index, l[2], la[2], num_tsj;
    int num_street;
    num_tsj = g->num_tsj;
    street_t *street[4];
    street_t *required_street[2];
    traffic_sig_jn_t *temp, *next_tsj[2];
    lane_t *lane[4][2];
    int random;




    for(i=0;i<num_tsj;i++)  // all tsj
    {
        temp = g->tsj_adj_list[i].src_tsj;


        for(j=0;j<4;j++) // all street
        {
            street[j]   = temp->streets[j];
            index       = get_tsj_index(street[j],temp);

            lane[j][0]  = street[i]->lanes[index][0]; // lanes
            lane[j][1]  = street[i]->lanes[index][1];

            k[1] = vehicle_direction(lane[j][1]->queue[0],temp,g,street[j]);
            k[0] = vehicle_direction(lane[j][0]->queue[0],temp,g,street[j]);

            required_street[1] = (street_t *)get_street_pointer(temp,street[j],k[1]);
            required_street[0] = (street_t *)get_street_pointer(temp,street[j],k[0]);

            next_tsj[1] = lane[j][1]->queue[0]->directions->steps[(lane[j][1]->queue[0]->directions->front)+1];
            next_tsj[0] = lane[j][0]->queue[0]->directions->steps[(lane[j][0]->queue[0]->directions->front)+1];

            l[0] = vehicle_direction(lane[j][0]->queue[0],next_tsj[0],g,required_street[0]); // vehicle direction after next tsj
            l[1] = vehicle_direction(lane[j][1]->queue[0],next_tsj[1],g,required_street[1]);


            if(l[0]==0) // which lane to follow 0 or 1;
            {
                la[0] = 0;
            }

            else
            {
                la[0] = 1;
            }

            if(l[1]==0)
            {
                la[1] = 0;
            }

            else
            {
                la[1] = 1;
            }

            if(lane[j][1]->queue[0]->directions->steps[(lane[j][1]->queue[0]->directions->front+1)]!=NULL)
            {


        // lane = = 1
                if(k[1]==1)
                {
                    if(required_street[1]->lanes[get_tsj_index(required_street[1],lane[j][1]->queue[0]->directions->steps[(lane[j][1]->queue[0]->directions->front+1)])][la[1]]->queue[ required_street[1]->lanes[get_tsj_index(required_street[1],lane[j][1]->queue[0]->directions->steps[(lane[j][1]->queue[0]->directions->front+1)])][la[1]]->max_vehicles-1 ]==NULL && street[j]->tsl[get_tsj_index(street[j],temp)]->forward==1 && lane[j][1]->queue[0]!=NULL)
                    {

                        queue_add_vehicle(required_street[1]->lanes[get_tsj_index(required_street[1],lane[j][1]->queue[0]->directions->steps[(lane[j][1]->queue[0]->directions->front)])][la[1]],lane[j][1]->queue[0]);
                        queue_delete(lane[j][1]);
                        required_street[1]->lanes[get_tsj_index(required_street[1],lane[j][1]->queue[0]->directions->steps[(lane[j][1]->queue[0]->directions->front)])][la[1]]->curr_vehicles++;
                        lane[j][1]->curr_vehicles--;
                        lane[j][1]->queue[0]->directions->front++;

                    }
                }

                else if(k[1]==2)
                {
                    if(required_street[1]->lanes[get_tsj_index(required_street[1],lane[j][1]->queue[0]->directions->steps[(lane[j][1]->queue[0]->directions->front+1)])][la[1]]->queue[ required_street[1]->lanes[get_tsj_index(required_street[1],lane[j][1]->queue[0]->directions->steps[(lane[j][1]->queue[0]->directions->front+1)])][la[1]]->max_vehicles-1 ]==NULL && street[j]->tsl[get_tsj_index(street[j],temp)]->right==1 && lane[j][1]->queue[0]!=NULL)
                    {

                        queue_add_vehicle(required_street[1]->lanes[get_tsj_index(required_street[1],lane[j][1]->queue[0]->directions->steps[(lane[j][1]->queue[0]->directions->front)])][la[1]],lane[j][1]->queue[0]);
                        queue_delete(lane[j][1]);
                        required_street[1]->lanes[get_tsj_index(required_street[1],lane[j][1]->queue[0]->directions->steps[(lane[j][1]->queue[0]->directions->front)])][la[1]]->curr_vehicles++;
                        lane[j][1]->curr_vehicles--;
                        lane[j][1]->queue[0]->directions->front++;

                    }
                }
            }

            else // if next tsj is ext_node
            {
                random = rand()%2;
                if(k[1]==1)
                {
                    if(required_street[1]->lanes[get_tsj_index(required_street[1],lane[j][1]->queue[0]->directions->steps[(lane[j][1]->queue[0]->directions->front+1)])][random]->queue[ required_street[1]->lanes[get_tsj_index(required_street[1],lane[j][1]->queue[0]->directions->steps[(lane[j][1]->queue[0]->directions->front+1)])][random]->max_vehicles-1 ]==NULL && street[j]->tsl[get_tsj_index(street[j],temp)]->forward==1 && lane[j][1]->queue[0]!=NULL)
                    {

                        queue_add_vehicle(required_street[1]->lanes[get_tsj_index(required_street[1],lane[j][1]->queue[0]->directions->steps[(lane[j][1]->queue[0]->directions->front)])][random],lane[j][1]->queue[0]);
                        queue_delete(lane[j][1]);
                       required_street[1]->lanes[get_tsj_index(required_street[1],lane[j][1]->queue[0]->directions->steps[(lane[j][1]->queue[0]->directions->front)])][random]->curr_vehicles++;
                        lane[j][1]->curr_vehicles--;
                        lane[j][1]->queue[0]->directions->front++;

                    }
                }

                else if(k[1]==2)
                {
                    if(required_street[1]->lanes[get_tsj_index(required_street[1],lane[j][1]->queue[0]->directions->steps[(lane[j][1]->queue[0]->directions->front)])][random]->queue[ required_street[1]->lanes[get_tsj_index(required_street[1],lane[j][1]->queue[0]->directions->steps[(lane[j][1]->queue[0]->directions->front+1)])][random]->max_vehicles-1 ]==NULL && street[j]->tsl[get_tsj_index(street[j],temp)]->right==1 && lane[j][1]->queue[0]!=NULL)
                    {

                       queue_add_vehicle(required_street[1]->lanes[get_tsj_index(required_street[1],lane[j][1]->queue[0]->directions->steps[(lane[j][1]->queue[0]->directions->front)])][random],lane[j][1]->queue[0]);
                        queue_delete(lane[j][1]);
                        required_street[1]->lanes[get_tsj_index(required_street[1],lane[j][1]->queue[0]->directions->steps[(lane[j][1]->queue[0]->directions->front)])][random]->curr_vehicles++;
                        lane[j][1]->curr_vehicles--;
                        lane[j][1]->queue[0]->directions->front++;

                    }
                }

            }


            if(required_street[0]->lanes[get_tsj_index(required_street[0],lane[j][0]->queue[0]->directions->steps[(lane[j][0]->queue[0]->directions->front+1)])][la[0]]->queue[ required_street[0]->lanes[get_tsj_index(required_street[0],lane[j][0]->queue[0]->directions->steps[(lane[j][0]->queue[0]->directions->front+1)])][la[0]]->max_vehicles-1 ]==NULL && lane[j][0]->queue[0]!=NULL)
            {
                queue_add_vehicle(required_street[0]->lanes[get_tsj_index(required_street[0],lane[j][0]->queue[0]->directions->steps[(lane[j][0]->queue[0]->directions->front)])][la[0]],lane[j][0]->queue[0]);
                queue_delete(lane[j][0]);
                required_street[0]->lanes[get_tsj_index(required_street[0],lane[j][0]->queue[0]->directions->steps[(lane[j][0]->queue[0]->directions->front)])][la[0]]->curr_vehicles++;
                lane[j][0]->curr_vehicles--;
                lane[j][0]->queue[0]->directions->front++;
            }


// find opposite tsl
// light on && rear empty
//add in lane acc to another function and increse curr_vehicles
// delete in 1st lane - curr_vehicles and route

        vehicle_move(lane[j][0]->queue, lane[j][0]->max_vehicles);// both lanes;
        vehicle_move(lane[j][1]->queue, lane[j][1]->max_vehicles);
        }


    }

}

int update_map_extn(map_extn_list_t *men, graph_tsj_t *g)
{
    int num_extn_list, i, j, k, index;

    map_extn_t *head;
    head = men->head;
    time_t current;
    vehicle_t *ptr;

    num_extn_list = men->num_map_extn;


    for(i=0;i<num_extn_list;i++)  // travelling every extn_node
    {
        time(&current);
        if(difftime(current,head->start_time) > head->time_of_generation)
        {
            index = get_tsj_index(head->connecting_street, head->adj_signal);
            if(head->connecting_street->lanes[!index][0]!= NULL)
            {
                queue_delete(head->connecting_street->lanes[!index][0]);
            }

            if(head->connecting_street->lanes[!index][1]!= NULL)
            {
                queue_delete(head->connecting_street->lanes[!index][1]);
            }

            ptr = create_vehicle();
            set_vehicle(ptr, g, men, head );
            k = vehicle_direction(ptr, head->adj_signal, g, head->connecting_street);



            if(k==0)
            {

                if(head->connecting_street->lanes[index][0]== NULL)
                {
                    queue_add_vehicle(head->connecting_street->lanes[index][0],ptr);
                }


            }

            else
            {
                if(head->connecting_street->lanes[index][1]== NULL)
                {
                    queue_add_vehicle(head->connecting_street->lanes[index][1],ptr);
                }
            }

        }

        head->start_time = current;
        head = head->next;
    }


}


/************************* SIMULATE *********************************/

#define EXIT_KEY 97

void simulate(graph_tsj_t *g,map_extn_list_t *m)
{
    int key;
    int i;
    
    while(1)
    {
        /*analyze each TSJ and update the "TSJ Mode"*/
        for(i=0 ; i < g->num_tsj ; ++i)
        {
            analyze_tsj(g,get_tsj_id(g->tsj_adj_list->src_tsj));
        }
        
        /*Update Vehicle Positions on Each Street*/
        update_vehicles(g);
        
        /*Update Map Extention Nodes*/
        for(i=0 ; i<m->num_map_extn;++i)
        {
            ;
        }
        
        /*Display Map*/
        display_map(g);
        
        key=getchar();
        /*Check user key press for EXIT character*/
        if(key == EXIT_KEY)
        {
            break;
        }
    }
}

/***********************************************/
/**** MAIN ****/



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



int main()
{
    /*Initialize file pointer and choice*/
    int c_login,c_admin,c_user,c_subuser,c_subadmin,c_street,c_tsj,c_edit_tsj,c_map_extn_node;
    //int gd = DETECT, gm;
    user_t u;
    graph_tsj_t g1;
    map_extn_list_t *men_list;
    traffic_sig_jn_t *new_tsj,*tsj_rename,*temp_tsj,*adj_tsj;
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
            if(login(&u)==0||strcmp(u.type,"standard")==0)
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
                      /****************/
                      /*load_graph(&g1);//To be done*/
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
                                        scanf("%s",name);
                                        printf("\nEnter coordinates");
                                        printf("\nx:");
                                        scanf("%d",&(c.x));
                                        printf("\ny:");
                                        scanf("%d",&(c.y));
                                        printf("\nEnter fill color(1 to 15):");
                                        scanf("%d",&(s.fill_color));
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
                                          scanf("%d",&id);
                                      }
                                      switch(c_edit_tsj)
                                      {
                                          case 1:

                                              printf("\nEnter the new name");
                                              scanf("%s",name);

                                              rename_tsj(&g1,id,name);
                                              break;
                                          case 2:
                                              printf("\nEnter the color to be filled");
                                              scanf("%d",&(s.fill_color));
                                              tsj_change_fill_color(&g1,id,s.fill_color);
                                              break;
                                          case 3:
                                              printf("\nEnter the color to be filled");
                                              scanf("%d",&(s.border_color));
                                              tsj_change_border_color(&g1,gen_num(id),s.border_color);
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
                                        scanf("%d",&id);
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
                                        scanf("%s",name);
                                        printf("\nEnter the id's of traffic signal junctions");
                                        printf("\nJunction 1:");
                                        scanf("%d",&id_1);
                                        printf("\nJunction 2:");
                                        scanf("%d",&id_2);
                                        s.fill_color=LIGHTGREY;
                                        s.border_style=2;
                                        s.border_color=DEFAULT_BORDER_COLOR;
                                        s.width=7;

                                        graph_add_street(&g1,id_1,id_2,name,"four lane");
                                        break;

                                    case 2:
                                    /* Delete Street */
                                        printf("\nEnter the street id to be deleted");
                                        scanf("%d",&id);
                                        graph_delete_street_by_id(&g1,id);
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
                                        scanf("%s",name);
                                        printf("\nEnter coordinates");
                                        printf("\nx:");
                                        scanf("%d",&(c.x));
                                        printf("\ny:");
                                        scanf("%d",&(c.y));

                                        printf("\nEnter time of generation");
                                        scanf("%d",&t);

                                        printf("\nEnter ID of the Adjacent Traffic Signal : ");
                                        scanf("%d",&id);

                                        adj_tsj = get_tsj_ptr(&g1,id);

                                        //st = add_street_to_map_extn_node(&g1,id,men_id , char st_name , int type);

                                        add_map_extn(&g1, men_list,name, t, adj_tsj);

                                        break;
                                    case 2:
                                        /* Delete map_extn_node */
                                        printf("\nEnter thr Map Extension Node ID to Delete : ");
                                        scanf("%d",&id);

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
            if(login(&u)==0||strcmp(u.type,"admin")==0)
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
