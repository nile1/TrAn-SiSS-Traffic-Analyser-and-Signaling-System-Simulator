set_extnode(vehicle_t *ptr,graph_t *di ,map_extn_list *ar)
{
    
    
    }
    char *rand_vehicle()
    {
       int x;char *arr;
        *(arr+0)='Bus';
        *(arr+1)='Car';
        *(arr+2)='Bike';
        *(arr+3)='Truck';
        x=rand()%4;
        return *(arr+x);
    }
        
    
street_t get_street_pointer(traffic_sig_tsj *tsj, street_t *s, int k) // function to return required street pointer either adjacent or opposite
{
    int i,j;
    street_t *temp[4];

    for(i=0;i<4;i++)
    {
        temp[i] = tsj->street[i];
    }
    i=0;
    while(temp[i]!=s)
    {
        i++;

    }

    for(j=0;j<=k;j++)

    {
        i = (i+1)%4;

    }

    return temp[i];

}




int vehicle_direction(vehicle_t *veh, traffic_sig_t *tsj, graph_tsj *g, street_t *street)
{
    int i, j=0, k, count=0;
    traffic_sig_t *temp1, *temp2;
    traffic_sig_t *prev;
    street_t arr[4];

    for(i=0;i<4;i++)
    {
        arr[i] = tsj->street[i];
    }

    temp1 = veh->directions->steps[veh->directions->front]; // next tsl of the vehicle to be followed

        if(temp1 == NULL)
        {
            return -99;
        }


    for(i=0;i<4;i++)// index of present street
    {
        if(arr[i]==street)
            break;
    }

    k=0;
    for(j=(i+1)%4;j<4;j++)
    {
        if(arr[j]->tsj[0]!=tsj)
        {
            temp2=arr[j]->tsl[0];
        }

        else
        {
            temp2=arr[j]->tsj[1];
        }


        if(temp2==temp1)
        {
            break;
        }



    if(j==3)
    {
        j=0;
    }

    k++;
    }


    return k;
}



void set_vehicle(vehicle_t *vehicle, graph_tsj_t *g, map_extn_list_t *extn_list, map_extn_t *ptr )
{
    int i, j=1, k, num_extn;
    map_extn_list_t *temp;

    vehicle->id = set_id("","VEH");
    vehicle->speed = rand()%100;
    strcpy(vehicle->type,rand_vehicle());
    vehicle->source = ptr;

    do
    {
        num_extn = rand()%extn_list->num_map_extn;
        temp = head;
        for(i=0;i<num_extn;i++)
        {
            temp = temp->next;

        }
        if(temp!=ptr)
            {
                j=0;
            }

    }while(j);

    vehicle->dest = temp;
    strcpy(vehicle->vehicle_no, vehicle->id);

    vehicle->directions = (route_t *)malloc(sizeof(route_t));
    get_direction(vehicle->source, vehicle->dest, vehicle->directions, g);

}



void update_vehicles(graph_tsj_t *g)
{
    int i,j, k[2], count, index, l[2], la[2];
    int num_street;
    num_tsj = g->num_tsj;
    street_t *street[4], *required_street[2];
    traffic_signal_jn_t *temp, *next_tsj[2];
    lane_t *lane[4][2];
    int rand;




    for(i=0;i<num_tsj;i++)  // all tsj
    {
        temp = g->tsj_adj_list[i]->src_tsj;


        for(j=0;j<4,j++) // all street
        {
            street[j]   = temp->streets[i];
            index       = get_tsj_index(street[j],temp);

            lane[j][0]  = street[i]->lanes[index][0]; // lanes
            lane[j][1]  =  street[i]->lanes[index][1];

            k[1] = vehicle_direction(lane[j][1]->queue[0],temp,g,street[j]);
            k[0] = vehicle_direction(lane[j][0]->queue[0],temp,g,street[j]);

            required_street[1] = get_street_pointer(temp,street[j],k[1]);
            required_street[0] = get_street_pointer(temp,street[j],k[0]);

            next_tsj[1] = lane[j][1]->queue[0]->directions->steps[(lane[j][1]->queue[0]->directions->front)+1];
            next_tsj[0] = lane[j][0]->queue[0]->directions->steps[(lane[j][0]->queue[0]->directions->front)+1];

            l[0] = vehicle_direction(lane[j][0]->queue[0],next_tsj[0],g,required_street); // vehicle direction after next tsj
            l[1] = vehicle_direction(lane[j][1]->queue[0],next_tsj[1],g,required_street);


            if(l[0]==0) // which lane to follow 0 or 1;
            {
                la[0] = 0;
            }

            else
            {
                la[0] = 1;
            }

            if(l[1]==0)
            {
                la[1] = 0;
            }

            else
            {
                la[1] = 1;
            }

            if(lane[j][1]->queue[0]->directions->steps[(lane[j][1]->queue[0]->directions->front)]!=NULL)
            {


        // lane = = 1
                if(k[1]==1)
                {
                    if(required_street[1]->lanes[get_tsj_index(required_street[1],lane[j][1]->queue[0]->directions->steps[(lane[j][1]->queue[0]->directions->front)])][la[1]]->queue[ required_street[1]->lanes->max_vehicles-1 ]==NULL && street[j]->tsl[get_tsj_index(street[j],temp)]->forward==0 && lane[j][1]->queue[0]!=NULL)
                    {

                        queue_add_vehicle(required_street[1]->lanes[get_tsj_index(required_street[1],lane[j][1]->queue[0]->directions->steps[(lane[j][1]->queue[0]->directions->front)])][la[1]],lane[j][1]->queue[0]);
                        queue_delete(lane[j][1]);
                        required_street[1]->lanes[required_street[1]->lanes[get_tsj_index(required_street[1],lane[j][1]->queue[0]->directions->steps[(lane[j][1]->queue[0]->directions->front)])][la[1]]->current_vehicles++;
                        lane[j][1]->current_vehicles--;
                        lane[j][1]->queue[0]->directions->front++;

                    }
                }

                else if(k[1]==2)
                {
                    if(required_street[1]->lanes[get_tsj_index(required_street[1],lane[j][1]->queue[0]->directions->steps[(lane[j][1]->queue[0]->directions->front)])][la[1]]->queue[ required_street[1]->lanes->max_vehicles-1 ]==NULL && street[j]->tsl[get_tsj_index(street[j],temp)]->right==0 && lane[j][1]->queue[0]!=NULL)
                    {

                        queue_add_vehicle(required_street[1]->lanes[get_tsj_index(required_street[1],lane[j][1]->queue[0]->directions->steps[(lane[j][1]->queue[0]->directions->front)])][la[1]],lane[j][1]->queue[0]);
                        queue_delete(lane[j][1]);
                        required_street[1]->lanes[required_street[1]->lanes[get_tsj_index(required_street[1],lane[j][1]->queue[0]->directions->steps[(lane[j][1]->queue[0]->directions->front)])][la[1]]->current_vehicles++;
                        lane[j][1]->current_vehicles--;
                        lane[j][1]->queue[0]->directions->front++;

                    }
                }
            }

            else // if next tsj is ext_node
            {
                rand = rand()%2;
                if(k[1]==1)
                {
                    if(required_street[1]->lanes[get_tsj_index(required_street[1],lane[j][1]->queue[0]->directions->steps[(lane[j][1]->queue[0]->directions->front)])][rand]->queue[ required_street[1]->lanes->max_vehicles-1 ]==NULL && street[j]->tsl[get_tsj_index(street[j],temp)]->forward==0 && lane[j][1]->queue[0]!=NULL)
                    {

                        queue_add_vehicle(required_street[1]->lanes[get_tsj_index(required_street[1],lane[j][1]->queue[0]->directions->steps[(lane[j][1]->queue[0]->directions->front)])][rand],lane[j][1]->queue[0]);
                        queue_delete(lane[j][1]);
                        required_street[1]->lanes[required_street[1]->lanes[get_tsj_index(required_street[1],lane[j][1]->queue[0]->directions->steps[(lane[j][1]->queue[0]->directions->front)])][rand]->current_vehicles++;
                        lane[j][1]->current_vehicles--;
                        lane[j][1]->queue[0]->directions->front++;

                    }
                }

                else if(k[1]==2)
                {
                    if(required_street[1]->lanes[get_tsj_index(required_street[1],lane[j][1]->queue[0]->directions->steps[(lane[j][1]->queue[0]->directions->front)])][rand]->queue[ required_street[1]->lanes->max_vehicles-1 ]==NULL && street[j]->tsl[get_tsj_index(street[j],temp)]->right==0 && lane[j][1]->queue[0]!=NULL)
                    {

                        queue_add_vehicle(required_street[1]->lanes[get_tsj_index(required_street[1],lane[j][1]->queue[0]->directions->steps[(lane[j][1]->queue[0]->directions->front)])][rand],lane[j][1]->queue[0]);
                        queue_delete(lane[j][1]);
                        required_street[1]->lanes[required_street[1]->lanes[get_tsj_index(required_street[1],lane[j][1]->queue[0]->directions->steps[(lane[j][1]->queue[0]->directions->front)])][rand]->current_vehicles++;
                        lane[j][1]->current_vehicles--;
                        lane[j][1]->queue[0]->directions->front++;

                    }
                }

            }


            if(required_street[0]->lanes[get_tsj_index(required_street[0],lane[j][0]->queue[0]->directions->steps[(lane[j][0]->queue[0]->directions->front)])][lane[0]]->queue[ required_street[0]->lanes->max_vehicles-1 ]==NULL && lane[j][0]->queue[0]!=NULL)
            {
                queue_add_vehicle(required_street[0]->lanes[get_tsj_index(required_street[0],lane[j][0]->queue[0]->directions->steps[(lane[j][0]->queue[0]->directions->front)])][lane[0]],lane[j][0]->queue[0]);
                queue_delete(lane[j][0]);
                required_street[0]->lanes[required_street[0]->lanes[get_tsj_index(required_street[0],lane[j][0]->queue[0]->directions->steps[(lane[j][0]->queue[0]->directions->front)])]lane[j][0]->current_vehicles++;
                lane[j][0]->current_vehicles--;
                lane[j][0]->queue[0]->directions->front++;
            }


// find opposite tsl
// light on && rear empty
//add in lane acc to another function and increse current_vehicles
// delete in 1st lane - current_vehicles and route

        vehicle_move(lane[j][0]->queue, lane[j][0]->max_vehicles);// both lanes;
        vehicle_move(lane[j][1]->queues, lane[j][1]->max_vehicles);
        }


    }

}

update_map_extn(map_extn_list_t *map_extn, graph_t *g)
{
    int num_extn_list, i, j, k, index;

    map_extn_list_t *head;
    head = map_extn->head;
    time_t current;
    vehicle_t *ptr;

    num_extn_list = map_extn->num_map_extn;


    for(i=0;i<num_extn_list;i++)  // travelling every extn_node
    {
        time(&current);
        if(difftime(current,head->start_time) > head->time_of_generation)
        {
            index = get_tsj_index(head->connecting_street, head->adj_signal);
            if(head->connecting_street->lanes[!index][0]!= NULL)
            {
                queue_delete(head->connecting_street->lanes[!index][0]);
            }

            if(head->connecting_street->lanes[!index][1]!= NULL)
            {
                queue_delete(head->connecting_street->lanes[!index][1]);
            }

            ptr = create_vehicle();
            set_vehicle(ptr, g, map_extn, head );
            k = vehicle_direction(ptr, head->adj_signal, g, head->connecting_street)



            if(k==0)
            {

                if(head->connecting_street->lanes[index][0]== NULL)
                {
                    queue_add_vehicle(head->connecting_street->lanes[index][0],ptr);
                }


            }

            else
            {
                if(head->connecting_street->lanes[index][1]== NULL)
                {
                    queue_add_vehicle(head->connecting_street->lanes[index][1],ptr);
                }
            }

        }

        head->start_time = current;
        head = head->next;
    }


}
















