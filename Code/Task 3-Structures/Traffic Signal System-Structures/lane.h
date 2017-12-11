typedef struct lane
{
    char *id;//Unique ID code for the lane
    struct vehicle **queue;//Queue of pointers of vehicle
    int front;//The position just 1 before the position of the first vehicle
    int rear;//The position of the last vehicle
    int max_vehicles;//Maximum number of vehicles that can be on the lane.
    int curr_vehicles;//Number of vehicles currently present on the lane
    char *type;//Type of lane.(Lane Number/Position)
    struct coordinate coord;//Stores the coordinates (position) of the lane on the screen
    struct style sty;//Stores the styling properties of the lane

 }lane_t;