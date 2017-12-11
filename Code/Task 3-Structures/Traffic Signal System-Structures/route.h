typedef struct route
{
    int no_of_steps; // No. of nodes to cross to take to reach dest.
    char *steps; //dynamic array of steps containg by all the junction(traffic signals) by which vehicle pass.
    int front; // array of first index
    int rear; // array last index

}route_t;