#ifndef __ENGINE_H__
#define __ENGINE_H__

#define MATCH 1
#define NO_MATCH 0

/*match: search for regexp anywhere in text */
int match(char *, char *);

/*matchhere: search for regexp at beginning of text */
int matchhere(char *, char *);

/*matchstar: search for c*regexp at beginning of text */
int matchstar(int, char *, char *);

#endif
