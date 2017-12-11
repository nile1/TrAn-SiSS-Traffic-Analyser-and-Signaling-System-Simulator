/*Sample Simulate Function*/
void simulate(graph_t *g,map_extn_list *mel)
{
/*Loop to analize and update the TSJ Mode*/
for(i=1;i<=g->num_tsj;++i)
{
/*Call to Analize function*/
analize_tsj(i);
}

/*Update vehicle position*/
update_vehicles();

/*Display Graphics*/
diplay_map();
}

/*Analize Function*/
void analize_tsj(int tsj_id)
{
/*
Identify and assign the pointer to the TSJ corresponding to the 'tsj_id' which is passed as a parameter.
Use function :
'traffic_sig_jn *graph_get_pointer(int id);' 
which will return the pointer to the requied tsj_id.
*/

/*
First assign the street pointers in the order of their 'traffic_status'(which will be in %)
with initial status as A>B>C>D
*/

street_t *A,*B,*C,*D;

/*
Analize and assign the TSL mode to the requied TSL according to the current order of traffic_status.
*/
}