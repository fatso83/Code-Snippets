/* This should be a utility function on the form
**       Node * gettokens(char *separator);
** It will take a seperator string, and then return a singly linked
** list with the string tokens from stdin. 
** It will return on newline or read error.
*/

/* IMPLEMENTATION: 
   This implementation can not read the string into a buffer!
   That is because it is intended to be used with the stdin,
   and we do not know in advance how big the string can be.

   use getc() and read into a temp buffer, one char at a time.
   When encountering a sep-char, null terminate the buffer, and
   copy it into a Node. Read the next token.
   If a newline is read, return.
*/
