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