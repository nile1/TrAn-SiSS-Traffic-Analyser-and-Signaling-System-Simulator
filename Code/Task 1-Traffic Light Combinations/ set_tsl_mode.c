/*
Function Name :- set_tsl_mode
Created On :- 22/03/15
Function Description :-
This function contains multiple cases of all possible combinations of the operations performed by a single traffic signal light at a junction.
It takes 'mode' as a parameter and each mode corresponds to a particular setting of the traffic signal light for a particular lane coming at a junction.It would return SUCCESS(1) or FAILURE(0) to the caller function.
*/
  
/*
tsl-> It is a pointer to the structure 'traffic_light' which contains all the possible functions depicted by a traffic light such as forward,
right,left,red and yellow.
mode-> It is a variable which stores all the modes of a traffic light.
return type-> (int) i.e. it returns either success(1) or failure(0) set mode function.
 */
 
 #define ON 1
 #define OFF 0
 int set_tsl_mode(struct traffic_light *tsl, int mode)
 {
     int ret=1;
     switch(mode)
     {
         case 1: /* mode1- For stop traffic condition */
         tsl->red=ON;
         tsl->yellow=OFF;
         tsl->forward=OFF;
         tsl->right=OFF;
         tsl->left=OFF;
         break;
         
         case 2: /* mode2- For slow traffic condition */
         tsl->red=OFF;
         tsl->yellow=ON;
         tsl->forward=OFF;
         tsl->right=OFF;
         tsl->left=OFF;
         break;
         
         case 3: /* mode3- 'Forward' traffic flow condition */
         tsl->red=OFF;
         tsl->yellow=OFF;
         tsl->forward=ON;
         tsl->right=OFF;
         tsl->left=OFF;
         break;
         case 4: /* mode4- 'Left' traffic flow condition */
         tsl->red=OFF;
         tsl->yellow=OFF;
         tsl->forward=OFF;
         tsl->right=OFF;
         tsl->left=ON;
         break;
         case 5=: /* mode5- 'Right' traffic flow condition */
         tsl->red=OFF;
         tsl->yellow=OFF;
         tsl->forward=OFF;
         tsl->right=ON;
         tsl->left=OFF;
         break;
         case 6: /* mode6- 'Forward and Left' traffic flow condition */
         tsl->red=OFF;
         tsl->yellow=OFF;
         tsl->forward=ON;
         tsl->right=OFF;
         tsl->left=ON;
         break;
         case 7: /* mode7- 'Forward and Right' traffic flow condition */
         tsl->red=OFF;
         tsl->yellow=OFF;
         tsl->forward=ON;
         tsl->right=ON;
         tsl->left=OFF;
         break;
         case 8: /* mode8- 'Forward, Left and Right' traffic flow condition */
         tsl->red=OFF;
         tsl->yellow=OFF;
         tsl->forward=ON;
         tsl->right=ON;
         tsl->left=ON;
         break;
         case 9: /* mode9- 'Right and Left' traffic flow condition */
         tsl->red=OFF;
         tsl->yellow=OFF;
         tsl->forward=OFF;
         tsl->right=ON;
         tsl->left=ON;
         break;
         default:
         ret=0;
         break;
     }
            
  }
     
 
 
 