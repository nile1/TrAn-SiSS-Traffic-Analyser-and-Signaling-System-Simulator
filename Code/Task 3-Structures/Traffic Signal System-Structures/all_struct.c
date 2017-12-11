
#include<stdio.h>
#include<time.h>
#include<string.h>
#include<stdlib.h>

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
typedef struct street
{
    char name;//Name of the street
    char *id;//Unique ID code for the street
    char *type;//Street type description
    int no;//Number of Lanes leading
    struct lane **lanes[2];/* Lanes leading from lanes[0] : TSJ 1 -to-> TSJ 2 lanes[1] : TSJ 2 -to-> TSJ 1 */
    struct traffic_signal_jn *tsj[2];/*Pointer to TSJ�s the street is joined to. tsj[0] : ->TSJ 1 tsj[1] : ->TSJ 2  */ 
    struct traffic_light *tsl[2];/* Pointer of TSL�s for the corresponding TSJ�s the street is joined to. tsl[0] : ->TSL 1 (2->1) tsl[1] : ->TSL 2  (1->2) */
    float traffic_status;//Percentage of the traffic present on the lane.
    struct coordinate coord;//Stores the coordinates (position) of the street on the screen
    struct style sty;//Stores the styling properties of the street

}street_t;

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

typedef struct traffic_sig_jn
{

    char name[20];              // name of the  traffic light
    char *id;                    // id
    char *type;                  // Number of roads connected i.e 1 way, 2 way etc.
    int no_of_adj_sig;          // Number of adjacent signals
    struct traffic_sig_jn **adj_signal; // double pointers pointing all immediate traffic signal.
    struct street **streets;//Pointers to all streets leading into the junction from adjacent TSJ nodes
    struct coordinate coord;//Stores the coordinates (position) of the TSJ on the screen
    struct style sty;//Stores the styling properties of the TSJ
} traffic_sig_jn_t;

typedef struct map_extn
{

    char name[20];              // name of the extension node
    char *id;                    // id
    int rate_of_generation;     // Number of vehicles incoming from the extension.
    struct traffic_sig_jn *adj_signal; // Pointing the immediate traffic signal.
    struct lane *lane_out;           // Pointer to node toward adjacent traffic light
    struct lane *lane_in;         //lane from adjacent traffic signal//
    struct coordinate coord;//Stores the coordinates (position) of the map extension on the screen
    struct style sty;//Stores the styling properties of the map extension

}map_extn_t;

typedef struct route
{
    int no_of_steps; // No. of nodes to cross to take to reach dest.
    char *steps; //dynamic array of steps containg by all the junction(traffic signals) by which vehicle pass.
    int front; // array of first index
    int rear; // array last index

}route_t;


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





