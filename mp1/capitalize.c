#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Modify the string `s` to capitalize the first letter of the string if the first letter of `s`
// is lower case.  Otherwise, do nothing.
void capitalize(char *s) {
  if (s[0] >= 'a' && s[0] <= 'z'){
    s[0] -= 32;
  }
}

// Modify the string `s` to capitalize the ALL of the lower case letters in `s`.  If the letter
// is not lower case, do nothing.  (You can assume you will be given only ASCII characters.)
void capitalizeAll(char *s) {
  char * t; // first copy the pointer to not change the original
  for (size_t i = 0; i < strlen(s); i++) {   //found out how to do this from stack over flow
      if (s[i] >= 'a' && s[i] <= 'z'){
        s[i] = toupper(s[i]);
      }
  }
}

// Return a new string (allocated in heap memory that you allocate) that transformed `s` in
// the same way as `capitalizeAll`.  (In this function `s` cannot be modified directly and
// must be done in the new memory you will allocate.)
char *capitalizeAll_alloc(const char *s) {
  char *ret = malloc(strlen(s));
  strcpy(ret, s);
  capitalizeAll(ret);
  return ret;
}
