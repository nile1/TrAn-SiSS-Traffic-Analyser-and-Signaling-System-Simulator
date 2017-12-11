/*Function to get the index of the TSJ inside the street corresponding to the passesTSJ*/
int get_tsj_index(street_t *s,traffic_signal_jn *tsj)
{
    /*
    Comparing each of the two TSJ's
    */
    for(i=0 ; i<2 ;++i)
    {
        if(tsj == s->tsj[i])        
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
    else if(strcmp(s->type,"Bus") == 0)
    {
        return 1.75;
    }
    
    else if(strcmp(s->type,"Truck") == 0)
    {
        return 2.25;
    }
    
    
    /*FAILURE*/
    return -1;
    
}
















