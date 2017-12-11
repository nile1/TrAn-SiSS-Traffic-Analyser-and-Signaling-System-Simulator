void simulate(graph_t *g,map_extn_t *m)
{
    int key;
    while(1)
    {
        /*Analize each TSJ and update the "TSJ Mode"*/
        for(i=0 ; i < g->num_tsj ; ++i)
        {
            analize(g->tsj_adj_list->id);
        }
        
        /*Update Vehicle Positions on Each Street*/
        update_vehicles();
        
        /*Update Map Extention Nodes*/
        for(i=0 ; i<m->num_men;++i)
        {
            
        }
        
        /*Display Map*/
        display_graph(g);
        
        /*Check user key press for EXIT character*/
        if(key == EXIT_KEY)
        {
            break;
        }
    }
}