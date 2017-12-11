
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