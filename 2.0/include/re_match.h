/*
 * Author:    Sean Copenhaver
 * Created:   2008/05/04
 * Modified:  2008/05/04
 * Description:
 *	This header describes the match structure.
 */

#ifndef _COPENHAS_MATCH_H
#define _COPENHAS_MATCH_H

/*
 * This is to represent a successful match by a regex being applied to
 * text. This is used to provide the sub-string matched plus in the future
 * perhaps other features.
 */
typedef struct re_match{
	char *text; //the full text being matched against
	char *substr; //the text actually matched
} re_match;

#endif
