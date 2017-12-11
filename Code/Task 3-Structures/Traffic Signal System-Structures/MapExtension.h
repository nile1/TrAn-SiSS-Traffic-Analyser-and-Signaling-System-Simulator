
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