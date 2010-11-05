/*
 * Author:    Sean Copenhaver
 * Created:   2008/05/07 
 * Modified:  2008/05/07
 * Description:
 *  This file describes the structure that represents a state of the regular
 *  expression engine. Either the current state or the saved states.
 */

#ifndef _COPENHAS_RE_STATE_H_ 
#define _COPENHAS_RE_STATE_H_

#include "re_com.h"

typedef struct re_state{ 
    int loc; //current location in the text 
    char *text; //text being match against 
    struct re_com *com; //the current componet 
} re_state;

#endif
