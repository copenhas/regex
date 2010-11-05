/*
 * Author:    Sean Copenhaver
 * Created:   2007/12/18
 * Modified:  2007/12/18
 * Description:
 *  This header defines the base structure to use when building
 *  child structures to represent the regular expression components
 *  to be support. Examples: Star (*), Character Classes ([a-z]), etc.
 */

#ifndef _RE_COM_H_
#define _RE_COM_H_

#include "container_of.h"

/*
 * It is suggested to have a separate file to contain the list of component
 * constants to represent the individual types that are supported in the engine.
 *
 * When creating a child structure to represent a new supported feature the
 * child structure must contain a function pointer to a destroy function that
 * takes in a pointer to the re_com structure. This destroy function is
 * responcible for freeing all the memory used by the child and the root re_com
 * structures.
 *
 * void (*destroy)(struct re_com *);
 *
 * You will also need to take the next constant in line and fill in a section 
 * in the re_com_create() and re_com_destroy() implementations so that they 
 * will know what to do when a component of that type is asked for or given.
 *
 * A section in regex_create() will need to be filled in to test for the new
 * component.
 */
typedef struct re_com{
    int type; //Type of component this structure is
    int (*match)(struct re_com *, char *, int); //The function to match with
    struct regex *re; //The regular expression this component belongs to.
    struct re_com *next; //The component next in the pattern expression
    struct re_com *prev; //The component previous to this one
} re_com;

/*
 * Name: exp_create
 * Parameters: ptrn - The pattern represented as a string to compile.
 * Returns: struct re_com * - Success
 * Returns: NULL - Failure
 * Description:
 *  This function is used to create the list of components that represent
 *  the pattern given. This function actually does all the parsing and 
 *  calling of re_com_create(). re_com_create() can be called directly to
 *  create a new single component that is not a part of an expression.
 */
struct re_com *exp_create(char *ptrn);

/*
 * Name: re_com_create
 * Parameters: type - The type of component to be created.
 * Returns: struct re_com * - Success
 * Returns: NULL - Failure
 * Description:
 *  This function acts as a structure factory allowing the needed
 *  structures to be created and returned as the abstract type that
 *  is used through the regular expression engine.
 */
struct re_com *re_com_create(int type);

/*
 * Name: re_com_destroy
 * Parameters: com - The componet to free the memory of.
 * Returns: void
 * Description:
 *  This function exists to allow easier destruction of the various
 *  components. It takes the component passed in casts it to its
 *  child type and calls the destroy function on the child. That
 *  function is responcible to free all the memory used include that
 *  used by the root re_com structure.
 */
void re_com_destroy(struct re_com *com);

#endif

