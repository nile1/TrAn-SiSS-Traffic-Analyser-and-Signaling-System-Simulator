#include<stdio.h>
#include<graphics.h>
#include<math.h>
#include<stdlib.h>

#define MAX_VERTICES 10
#define MAX_EDGES    MAX_VERTICES*(MAX_VERTICES-1)/2

struct coordinate
{
  int    x;		//integral numeral for x-coordinate of the object
  int	y;		//integral numeral for y-coordinate of the object
  float	rotation;	//floating point value for Angle of Rotation of the object
};

struct style
{
 int	fill_color;	//contains the color code of the graph
 int	border_style;	//contains the border style of the graph
 int	border_color;	//contains the color code of the graph
 int	width;		//contains the width of a lane/street or the radius of a circle
};

struct tsj //TBD
{
 int            id;
 struct coordinate   coord;
 struct style        sty;
};

struct men //Map Extension Node
{
 int            id;
 struct coordinate   coord;
 struct style        sty;
 struct men     *next;
};

struct lane //Map Extension Node
{
 int            id;
 struct coordinate   c1;
 struct coordinate   c2;
 struct style   sty;
};

struct vertex
{
    struct tsj           *dest;
    struct lane          *l;         //Connecting Lane
    struct vertex   *next;
};

struct adj_node
{
    struct tsj       *src;
    struct vertex    *list;
    int         degree;
};

struct edge_node
{
    struct lane      *l;     /*Pointer to the Edge(lane)*/
};

struct graph
{
    struct adj_node  *adj_list[MAX_VERTICES];          /*Array of pointers of the vertices(TSJs)*/
    struct edge_node *edge_list[MAX_EDGES];             /*Array of pointers of the edges(lanes)*/
    int	        num_of_tsj;               /*Number of Lanes*/
    int	        num_of_edges;               /*Number of Edges*/
};

void set_style(struct style *s,int fill_color,int border_style,int border_color,int width);

/*COPY Functions*/
int copy_coordinate(struct coordinate *dest,struct coordinate *src)
{
        dest->x = src->x;
        dest->y = src->y;
        dest->rotation = src->rotation; 
        
        return 1;
}

int copy_style(struct style *dest,struct style *src)
{
        dest->fill_color = src->fill_color;
        dest->border_style = src->border_style;
        dest->border_color = src->border_color;
        dest->width = src->width; 
        
        return 1;
}

/*Create Functions*/

/*Create TSJ*/
struct tsj *create_tsj(struct coordinate *c,struct style *sty)
{
    static int ctr = 0;
    struct tsj *new_tsj = (struct tsj *)malloc(sizeof(struct tsj));

    ctr++;
    copy_coordinate(&(new_tsj->coord) , c);
    copy_style(&(new_tsj->sty) , sty);
    new_tsj->id         = ctr;
}

/*Create Lane*/
struct lane *create_lane( struct coordinate *c1,struct coordinate *c2,struct style *sty)
{
    struct lane *new_lane = (struct lane *)malloc(sizeof(struct lane));

    copy_coordinate( &new_lane->c1 , c1 );
    copy_coordinate( &new_lane->c2 , c2 );
    copy_style( &new_lane->sty , sty );
    new_lane->id  = 0;
}

/*Create Vertex*/
struct vertex *create_vertex(struct tsj *dest,struct lane *l)
{
    struct vertex *new_v = (struct vertex *)malloc(sizeof(struct vertex));

    new_v->dest = dest;
    new_v->l    = l;
    new_v->next = NULL;

    return new_v;
}

/*Graph Functions*/
/*Add Vertex*/
void add_vertex_dest(struct adj_node *a,struct tsj *t,struct lane *l)
{
    a->degree++;
    struct vertex *new_vnode = create_vertex(t,l);
}

/*Graoh Functions*/
/*Initialize the Graph*/
int graph_initialize(struct graph *g)
{
    g->num_of_edges = 0;
    g->num_of_tsj = 0;
}
/*Function : ADD TSJ(Vertex) to the Graph*/
int graph_add_tsj(struct graph *g,struct tsj *t)
{
    /*If number of vertices exceed the maximum (i.e. the graph is full)*/
    if(g->num_of_tsj == MAX_VERTICES)
        return 0;   /*FAILURE*/

    /*ELSE*/
    /*Increment Number of Lanes*/
    g->num_of_tsj++;
    /*Create new adjacency list node */
    struct adj_node *newnode = (struct adj_node *)malloc(sizeof(struct adj_node));

    /*Set(initialize) the values of the adjacency list node*/
    newnode->src = t;
    newnode->degree = 0;
    newnode->list = NULL;

    g->adj_list[g->num_of_tsj-1] =  newnode;

    return 1;   /*SUCCESS*/
}


/*Function : ADD Lane(Edge) to the Graph*/
int graph_add_edge(struct graph *g,int tsj1,int tsj2)
{
    /*If number of edges exceed the maximum (i.e. the graph is full)*/
    if(g->num_of_edges == MAX_EDGES)
        return 0;   /*FAILURE*/

    /*Increment Number of Edges in Edge*/
    g->num_of_edges++;

    /*Create pointers to the given IDs of the tsj1 and tsj2*/
    struct tsj *t1 = g->adj_list[tsj1]->src;
    struct tsj *t2 = g->adj_list[tsj2]->src;

    /*STYLE*/
    /*Create the dynamically allocated style object*/
    struct style *s = (struct style *)malloc(sizeof(struct style));
    /*Set the style properties*/
    set_style(s,YELLOW,YELLOW,YELLOW,5);

    /*Create dynamically allocated lane object*/
    struct lane *new_lane = create_lane(&(t1->coord), &(t2->coord),s);
    /*Set ID for the edges*/
    new_lane->id = g->num_of_edges;
    
    /*Allocate Memory to the Edge*/
    g->edge_list[g->num_of_edges-1] = (struct edge_node *)malloc(sizeof(struct edge_node));
    
    /*Set the new node to the edge list*/
    g->edge_list[g->num_of_edges-1]->l = new_lane;

    /*Add Destination Vertex to the tsj1 and tsj2*/
    add_vertex_dest(g->adj_list[tsj1] , t2 , new_lane);
    add_vertex_dest(g->adj_list[tsj2] , t1 , new_lane);

    return 1;   /*SUCCESS*/
}


/*Set Functions*/
void set_coordinate(struct coordinate *c,int x,int y,float rotation)
{
	c->x 		= x;
	c->y		= y;
	c->rotation     = rotation;
}

void set_style(struct style *s,int fill_color,int border_style,int border_color,int width)
{
	s->fill_color	= fill_color;
	s->border_style	= border_style;
	s->border_color	= border_color;
	s->width        = width;
}

/*DRAW Functions*/
void draw_tsj(struct tsj *t)
{
    circle((t->coord).x,(t->coord).y,(t->sty).width);
    floodfill((t->coord).x,(t->coord).y,BLUE);
}

int draw_lane(struct coordinate *c1,struct coordinate *c2,struct style *sty)
{
	double ang,ang2;
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
	printf("%d",getx);
}


int graph_display(struct graph *g)
{
    int i;

    /*Draw Lanes(Edges)*/
    for(i=0;i<g->num_of_edges;++i)
    {
        draw_lane(&g->edge_list[i]->l->c1,&g->edge_list[i]->l->c2,&g->edge_list[i]->l->sty);
    }

    /*Draw TSJ's(Vertices)*/
    for(i=0;i<g->num_of_tsj;++i)
    {
        draw_tsj(g->adj_list[i]->src);
    }
}

/*Print Graph Data*/
void graph_print(struct graph *g)
{
    int i;

    printf("\nTSJ ID\tList of Connected TSJs");

    /*Print Linked list of each connected node*/
    for(i=0;i<g->num_of_tsj;++i)
    {
        printf("\n%d :\t",g->adj_list[i]->src->id);

        struct vertex *ptr = g->adj_list[i]->list;
        while(ptr)
        {
            printf("%d ",ptr->dest->id);
            ptr = ptr->next;
        }
    }

    /*Edge List Display*/
    printf("\nEdge ID\tTSJ1\tTSJ2");

    for(i=0;i<g->num_of_edges;++i)
    {
        struct lane *l = g->edge_list[i]->l;
        printf("\n%d",l->id);
    }
}

int main()
{
    int ch;
    int gd = DETECT, gm;
    
    struct coordinate c;
    struct style s;
    int x,y,color;
    int t1,t2;
    
    struct graph *g1 = (struct graph *)malloc(sizeof(struct graph));

    graph_initialize(g1);
    
    //initgraph(&gd,&gm,NULL);

    do{
        printf("\n\tMENU\n\n");

        printf("\n1.\tAdd TSJ");
        printf("\n2.\tAdd Lane");
        printf("\n3.\tDisplay");
        printf("\n0.\tExit");
        
        printf("\nEnter Choice : ");
        scanf("%d",&ch);

        switch(ch){

        case 1:
            printf("Enter x,y,color");
            scanf("%d%d%d",&x,&y,&color);

            /*Set Coordinates and Style*/
            set_coordinate(&c,x,y,0);
            set_style(&s,color,color,color,10);

            /*Create TSJ*/
            struct tsj *t = create_tsj(&c,&s);

            /*Add to Graph*/
            graph_add_tsj(g1,t);
            break;
            
        case 2:
            printf("Enter t1,t2,color");
            scanf("%d%d%d",&t1,&t2,&color);

            /*Set Style*/
            set_style(&s,color,color,color,10);

            /*Add to Graph*/
            graph_add_edge(g1,t1,t2);

            break;
     
        case 3:
            initgraph(&gd,&gm,NULL);
            graph_display(g1);
            getch();
            closegraph();
            break;
     
        default:
            printf("\nINVALID INPUT");
        }

    }while(ch!=0);
    
    //closegraph();
    
    return 0;
}