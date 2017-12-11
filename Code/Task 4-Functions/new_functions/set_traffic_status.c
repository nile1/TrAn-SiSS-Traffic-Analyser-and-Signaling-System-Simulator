float set_traffic_status(street_t *s, traffic_sig_jn_t *t)
{
    lane *l;
    int i, j, k, street type, sum=0 ;
    i = get_tsj_index(s,t);
    street = get_street_type(s);
    
    if(i==0)
    {
        l= streets->lanes[i];
    }
    
    else if(i==1)
    {
        l= streets->lanes[i];
    }
    
    if(street==2)
    {
        K=l[0]->current_vehicles;    
        float array[K];
        for(i=0 ; i<K ; i++)
        {
            array[i] = vehicle_type(l[0]->queue[i]);
        }
    
    }
    
    else if(street==4)
    {
        K= l[0]->current_vehicles + l[1]->current_vehicles;
        float array[K];
        
        type = 0;
        for(j=0;j<2;j++)
        {
            for(i=0 ; i<l[j]->current_vehicles ; i++)
            {
                array[type+i] = vehicle_type(l[j]->queue[i]);
            }
            type = i;
        }
    }
    
    for(i=0;i<k;i++)
    {
        sum += array[i];
    }
    
    
    if (street==2)
    {
        (float)i= (sum*100)/(l[0]->max_vehicles*2.25);    
    }
     
    else if (street==4)
    {
        (float)i= (sum*200)/(l[0]->max_vehicles*2.25);    

    } 
        
    
    return i;
}