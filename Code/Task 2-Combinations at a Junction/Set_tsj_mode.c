/*  Function:- Set Traffic Signal Junction Mode 
    The function takes pointers to traffic lights at a junction and sets traffic light mode for the respective traffic light */

void set_tsj_mode(traffic_light_t *A,traffic_light_t *B,traffic_light_t *C,traffic_light_t *D,int mode) 
{
    switch(mode)                    /* Cases to pass the modes of traffic lights */
    {
        case 1:
            set_tsl_mode(A,8);
            set_tsl_mode(B,1);
            set_tsl_mode(C,1);
            set_tsl_mode(D,5);
            break;
        case 2:
            set_tsl_mode(A,7);
            set_tsl_mode(B,1);
            set_tsl_mode(C,7);
            set_tsl_mode(D,1);
            break;    
        case 3:
            set_tsl_mode(A,7);
            set_tsl_mode(B,1);
            set_tsl_mode(C,5);
            set_tsl_mode(D,5);
            break;
        case 4:
            set_tsl_mode(A,5);
            set_tsl_mode(B,8);
            set_tsl_mode(C,1);
            set_tsl_mode(D,1);
            break;
        case 5:
            set_tsl_mode(A,1);
            set_tsl_mode(B,1);
            set_tsl_mode(C,1);
            set_tsl_mode(D,7);
            break;
        case 6:
            set_tsl_mode(A,5);
            set_tsl_mode(B,1);
            set_tsl_mode(C,1);
            set_tsl_mode(D,5);
            break;
        case 7:
            set_tsl_mode(A,1);
            set_tsl_mode(B,5);
            set_tsl_mode(C,8);
            set_tsl_mode(D,1);
            break;
        case 8:
            set_tsl_mode(A,7);
            set_tsl_mode(B,1);
            set_tsl_mode(C,7);
            set_tsl_mode(D,1);
            break;
        case 9:
            set_tsl_mode(A,5);
            set_tsl_mode(B,5);
            set_tsl_mode(C,7);
            set_tsl_mode(D,1);
            break;
        case 10:
            set_tsl_mode(A,1);
            set_tsl_mode(B,5);
            set_tsl_mode(C,1);
            set_tsl_mode(D,8);
            break;
        case 11:
            set_tsl_mode(A,1);
            set_tsl_mode(B,7);
            set_tsl_mode(C,1);
            set_tsl_mode(D,7);
            break;
        case 12:
            set_tsl_mode(A,1);
            set_tsl_mode(B,5);
            set_tsl_mode(C,5);
            set_tsl_mode(D,7);
            break;
        case 13:
            set_tsl_mode(A,4);
            set_tsl_mode(B,8);
            set_tsl_mode(C,4);
            set_tsl_mode(D,4);
            break;
        case 14:
            set_tsl_mode(A,8);
            set_tsl_mode(B,4);
            set_tsl_mode(C,4);
            set_tsl_mode(D,4);
            break;
        case 15:
            set_tsl_mode(A,4);
            set_tsl_mode(B,4);
            set_tsl_mode(C,4);
            set_tsl_mode(D,8);
            break;
        case 16:
            set_tsl_mode(A,4);
            set_tsl_mode(B,4);
            set_tsl_mode(C,8);
            set_tsl_mode(D,4);
            break;
        case 17:
            set_tsl_mode(A,4);
            set_tsl_mode(B,6);
            set_tsl_mode(C,4);
            set_tsl_mode(D,6);
            break;
        case 18:
            set_tsl_mode(A,6);
            set_tsl_mode(B,4);
            set_tsl_mode(C,6);
            set_tsl_mode(D,4);
            break;
        
        
    }
    
}

