/* Structure to represent each linked list of the Adjacency List */
typedef struct struct adj_list
{
 struct adj_list_node *head; //Pointer to the starting(head) node of each linked list
 struct traffic_signal_jn *src; //Pointer to the source TSJ node for the edges
 struct adj_list *next_list; //Pointer to the next adj_list of the graph
}adj_list_t;
