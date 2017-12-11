int timer (float array[])
{
    
    int i, j, k, total_time, main_time;
    
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

