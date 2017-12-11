void display_tsj(graph_tsj_t *g)
{
    int n, i;
    vertex_node_t *temp;
    n=g->num_tsj;
    printf("\nTsj's and their adjacency list is: \n");
    for(i=0;i<n;i++)
    {
        temp = g->tsj_adj_list[i]->head->dest_tsj_p;
        printf("%s %s : ",g->tsj_adj_list[i]->src_tsj->id,g->tsj_adj_list[i]->src_tsj->name);
        while(temp!=NULL)
        {

            printf("%s ->  ",temp->id);

            temp = temp->next_list;
        }

        printf("\n");
    }

}



void display_street(graph_tsj_t *g)
{
    int n, i;
    n=g->num_street;
    printf("\nStreets and the tsj's - \n");
    for(i=0;i<n;i++)
    {

        printf("%s %s : ",g->street_list[i]->id,g->street_list[i]->name);
        printf("%s & %s\n",g->street_list[i]->tsj[0]->id,g->street_list[i]->tsj[1]->id);



    }
}



void display_extn_node(map_extn_list_t *g)
{
    int n, i;
    map_extn_list_t *temp;
    printf("\nExtension nodes are\n");

    temp = g->head;

    while(temp!=NULL)
    {

            printf("%s %s : ",temp->id,temp->name);
            printf(" Adjacent street - %s  & Adjacent tsj - %s /n ",temp->connecting_street->id,temp->adj_signal->id);

    }

}





