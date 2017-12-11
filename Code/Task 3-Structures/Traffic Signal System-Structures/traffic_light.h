
typedef struct traffic_light
{
    char *id; //unique id of a traffic light
    int timer;// TImer set for this tsl for the next analysis
    int red;//status of red light(on/off)
    int yellow;//status of yellow light(on/off)
    int forward;//status of green light(on/off) for forward movement of vehicle from a junction
    int left;//status of green light(on/off) for left  movement of vehicle from a junction
    int right;////status of green light(on/off) for right movement of vehicle from a junction
    struct coordinate coord;//Stores the coordinates (position) of the TSL on the screen
    struct style;//Stores the styling properties of the TSL
}traffic_light_t;