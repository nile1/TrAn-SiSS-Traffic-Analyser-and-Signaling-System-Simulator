typedef struct graph_tsj
{
 int num_vertices; //Contains no. of vertices of TSJ
 struct adj_list *tsj_adj_list; /*Dynamic array of pointers of adjacent list of all vertices to create a linked list of all the vertices a given  
                                  vertex is connected to*/
 struct adj_list_node **tsj_list; //Dynamic array of pointers which points to all TSJ on the graph(map)    
 struct street **street_list; //Dynamic array of pointers which points to all streets on the graph(map)
}graph_tsj_t;