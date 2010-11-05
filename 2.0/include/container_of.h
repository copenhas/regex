/*
 * Author(of File):    Sean Copenhaver
 * Created:   2007/12/18
 * Modified:  2007/12/18
 * Description:
 *  This header only defines the container_of macro.
 *  Credit of macro goes to an unknown Linux kernal developer.
 */

#ifndef _CONTAINER_OF_H_
#define _CONTAINER_OF_H_

/*
 * Name: container_of
 * Parameters: ptr - Pointer to the inner structure that you have.
 * Parameters: type - The type of outer structure you want to have.
 * Parameters: member - The member in the outer structure for the inner.
 * Returns: type * - Pointer to the outer structure you wanted.
 * Description:
 *  This is a macro used to get to a child structure when you have a pointer
 *  to the root structure. Notice that this makes no checks or validation.
 *  Use when you know what you have and what you want.
 */
#define container_of(ptr, type, member)({ \
    const typeof( ((type *)0)->member ) *__mptr = (ptr); \
    (type *)( (char *)__mptr - offsetof(type, member) );\
})

#endif

