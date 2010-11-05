/*
 * Author:    Sean Copenhaver
 * Created:   2007/12/18
 * Modified:  2007/12/18
 * Description:
 *  Primary header file for using the regex engine. It
 *  provides the basic programmatic interface in order
 *  to match a regular expression against text.
 */

#ifndef _COPENHAS_REGEX_H_
#define _COPENHAS_REGEX_H_

#include "re_com.h"
#include "re_state.h"
#include "re_match.h"

/*
 * The top level structure that is used in major events.
 */
typedef struct regex{
    char *ptrn; //The original pattern in string form
    struct re_com *exp; //The pattern compiled into internal structures
    struct re_state *states; //The stack of states the engine has
} regex;

/*
 * Name: regex_create
 * Parameter: ptrn - A string representing the pattern to use for matching.
 * Returns: struct regex * - Success
 * Returns: NULL - Failure
 * Description:
 *  This actually creates and fills in a regex structure to use for matching.
 *  It in turn calls the exp_create() function to parse the pattern and fill
 *  in the expression.
 */
struct regex *regex_create(char *ptrn);

/*
 * Name: regex_destroy
 * Parameter: re - The regex structure to be freed
 * Returns: void
 * Description:
 *  Since we are using a function to create a complex composite
 *  structure it makes sense to provide a function to deallocate
 *  memory used.
 */
void regex_destroy(struct regex *re);

/*
 * Name: regex_match
 * Parameter: re - The regex structure to try and match.
 * Parameter: text - The text to match against.
 * Returns: struct match * - Success
 * Returns: NULL - Failure
 * Description:
 *  Calling this causes the engine to apply the regular expression
 *  that was created to the text provided. It will return a pointer
 *  to a match structure if there is a match. If no match occurs
 *  then NULL is returned.
 */
struct re_match *regex_match(struct regex *re, char *text);

#endif

