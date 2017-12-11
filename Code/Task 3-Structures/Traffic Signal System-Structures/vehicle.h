typedef struct vehicle
{
    char name[20];//name of the vehicle
    char *id;//unique id for vehicle
    int speed;//speed of vehicle
    char *type;//type of vehicle car,truck or  bike
    struct map_extn *source;//starting map extension node
    struct map_extn *dest;//ending map extension node
    struct route directions; // array of all the route to be followed
    char vehice_no[8];//license plate
    int priority_status;//priority  vehicles such as vip vehicles
    struct coordinate coord;//Stores the coordinates (position) of the Vehicle on the screen
    struct style sty;//Stores the styling properties of the Vehicle
}vehicle_t;
