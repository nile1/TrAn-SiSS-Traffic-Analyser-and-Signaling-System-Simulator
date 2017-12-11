#include<stdio.h>
#include<graphics.h>
#include<math.h>
#include<stdlib.h>

#define MAX_VERTICES 10
#define MAX_EDGES    MAX_VERTICES*(MAX_VERTICES-1)/2

struct coordinate
{
  int   x;		//integral numeral for x-coordinate of the object
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


void set_style(struct style *s,int fill_color,int border_style,int border_color,int width);

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
void draw_tsj(struct traffic_signal_jn *t)
{
    circle((t->coord).x,(t->coord).y,(t->sty).width);
    floodfill((t->coord).x,(t->coord).y,(t->sty).fill_color);
}

/*Updated Draw Street*/
int draw_street(struct street *st)
{
    for(i=0;i<st->nol;++i)
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
    struct coordinate   *c1 = l->c1;
    struct coordinate   *c2 = l->c2;
    struct style        *sty= l->sty;

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
	printf("%d",getx);

	draw_vehicles(l);
}


int display_map(struct graph *g)
{
    int i;

    /*Draw Street(Edges)*/
    for(i=0;i<g->num_;++i)
    {
        draw_street(g->street_list[i]);
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

    /*Edge List Display*/
    printf("\nEdge ID\tTSJ1\tTSJ2");

    for(i=0;i<g->num_of_edges;++i)
    {
        struct lane *l = g->edge_list[i]->l;
        printf("\n%d",l->id);
    }
}

/*Updated Graphics ADD TSJ*/
void graphic_add_tsj()
{
    printf("Enter x,y,color");
    scanf("%d%d%d",&x,&y,&color);

    /*Set Coordinates and Style*/
    set_coordinate(&c,x,y,0);
    set_style(&s,color,color,color,10);
    /*Create TSJ*/
    struct tsj *t = create_tsj(&c,&s);
    /*Add to Graph*/
    graph_add_tsj(g1,t);
}

/*Draw Vehicles*/
void draw_vehicles(lane_t *l)
{
    int x1 = l->c1->x;
    int x2 = l->c2->x;

    int y1 = l->c1->y;
    int y2 = l->c2->y;

    //float slope = slope_of_line(l->c1,l->c2);

    int x,y;
    int vehicle_sz=2;

    int div         = l->max_vehicle + 1 ;
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

/*
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
        /*
            printf("Enter x,y,color");
            scanf("%d%d%d",&x,&y,&color);

            /*Set Coordinates and Style
            set_coordinate(&c,x,y,0);
            set_style(&s,color,color,color,10);

            /*Create TSJ
            struct tsj *t = create_tsj(&c,&s);

            /*Add to Graph
            graph_add_tsj(g1,t);
            break;
        case 2:
            printf("Enter t1,t2,color");
            scanf("%d%d%d",&t1,&t2,&color);

            /*Set Style
            set_style(&s,color,color,color,10);

            /*Add to Graph*
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
*/
