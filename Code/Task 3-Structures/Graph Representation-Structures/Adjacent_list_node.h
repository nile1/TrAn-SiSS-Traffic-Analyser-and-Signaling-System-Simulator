/* Structure to represent each node of the Adjacency List */
typedef struct adj_list_node
{
 int dest; //Node at the destination
 struct traffic_signal_jn *dest_jn_p; //Pointer to the destination traffic signal
 struct street *street_p; //Pointer to the connecting street
 struct adj_list_node *next; //Pointer to the next adj_list_node of the linked list
}adj_list_name_t;
