#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>       /* time */


// Return your favorite emoji; do not allocate new memory.
// (This should **really** be your favorite emoji, we plan to use this later in the semester. :))
const char *emoji_favorite() {
  return "\xF0\x9F\x99\x83";
}


// Count the number of emoji in the UTF-8 string `utf8str`, returning the count.  You should
// consider everything in the ranges starting from (and including) U+1F000 up to (and including) U+1FAFF.
int emoji_count(char *utf8str) {
  const unsigned char * b = (const unsigned char *)utf8str;

  int count = 0;
  for (size_t i = 0; i < strlen(b); i++) {
    if (b[i] == 0xF0 && (i+3) < strlen(utf8str)) {
      if (b[i+1] == 0x9F) {
        if (b[i+2] >= 0x80 && b[i+2] <= 0xab) {
          count++;
          if ((b[i+2] == 0x80 && b[i+3] <= 0x80) || (b[i+2] == 0xAB && b[i+3] >= 0xBF)) {
            count--;
          } //checks final byte to be in proper range
        }
      }
    }
  }
  return count;
}


// Modify the UTF-8 string `utf8str` to invert ONLY the FIRST UTF-8 character (which may be up to 4 bytes)
// in the string if it the first character is an emoji.  At a minimum:
// - Invert "😊" U+1F60A ("\xF0\x9F\x98\x8A") into a non-simpling face.
// - Choose at least five more emoji to invert.
void emoji_invertChar(char *utf8str) {
  const unsigned char * b = (const unsigned char *)utf8str;
    size_t i = 0;
    if (b[i] == 0xF0 && (i+3) < strlen(utf8str)){
      if (b[i+1] == 0x9F && b[i+2] == 0x98 && b[i+3] == 0x8A) { //😊 U+1F60A ("\xF0\x9F\x98\x8A") 
        //U+1F641 0xF0 0x9F 0x99 0x81 == 🙁 SLIGHTLY FROWNING FACE
        utf8str[i+2] = '\x99';
        utf8str[i+3] = '\x81';
      }
      else if (b[i+1] == 0x9F && b[i+2] == 0xA7 && b[i+3] == 0xA1) { //🧡 U+1F9E1 ("0xF0 0x9F 0xA7 0xA1")
        //U+1F499 0xF0 0x9F 0x92 0x99 == 💙 
        utf8str[i+2] = '\x92';
        utf8str[i+3] = '\x99';
      }
      else if (b[i+1] == 0x9F && b[i+2] == 0x91 && b[i+3] == 0xBC) { //"👼" U+1F47C ("0xF0 0x9F 0x91 0xBC") 
        //U+1F608 0xF0 0x9F 0x98 0x88 == 😈
        utf8str[i+2] = '\x98';
        utf8str[i+3] = '\x88';
      }
      else if (b[i+1] == 0x9F && b[i+2] == 0x8D && b[i+3] == 0x89) { //"🍉" U+1F349 	0xF0 0x9F 0x8D 0x89
      // 💩 U+1F4A9	0xF0 0x9F 0x92 0xA9
        utf8str[i+2] = '\x92';
        utf8str[i+3] = '\xA9';
      }
      else if (b[i+1] == 0x9F && b[i+2] == 0x94 && b[i+3] == 0xA5) { //🔥 U+1F525  0xF0 0x9F 0x94 0xA5
      //💧 U+1F4A7 0xF0 0x9F 0x92 0xA7
        utf8str[i+2] = '\x92';
        utf8str[i+3] = '\xA7';
      }
      else if (b[i+1] == 0x9F && b[i+2] == 0x94 && b[i+3] == 0x88) { //🔈 U+1F508 0xF0 0x9F 0x94 0x88
      //🔇 U+1F507 0xF0 0x9F 0x94 0x87
        utf8str[i+2] = '\x94';
        utf8str[i+3] = '\x87';
      }
    }
 
}


// Modify the UTF-8 string `utf8str` to invert ALL of the character by calling your
// `emoji_invertChar` function on each character.
void emoji_invertAll(char *utf8str) {
  const unsigned char * b = (const unsigned char *)utf8str;
  
  for (size_t i = 0; i < strlen(utf8str); i++) {
    if (b[i] == 0xF0) {
      emoji_invertChar(utf8str + i);
    }
  }
  
}

int isEmoji(const char *s) {
  unsigned int val = 0;
  for(int i=0;i<strlen(s);i++)
  {
    val = (val << 8) | ((unsigned int)(s[i]) & 0xFF);
  }
  return
  (
    (val >= 14844092 /* U+203C */ && val <= 14912153 /* U+3299 */) ||
    (val >= 4036984960 /* U+1F000 */ && val <= 4036995737 /* U+1FA99 */ )
  );
}


// Return a random emoji stored in new heap memory you have allocated.  Make sure what
// you return is a valid C-string that contains only one random emoji.
char *emoji_random_alloc() {
  char * rando = malloc(sizeof(char) * 4);
  // srand (time(NULL));
  rando[0] = 240;
  rando[1] = 159;
  int a = (rand() % 43) + 128;
  int b = (rand() % 63) +128;
  rando[2] = a;
  rando[3] = b;
  while (!isEmoji(rando)){
    a = (rand() % 43) + 128;
    b = (rand() % 63) +128;
    rando[2] = a;
    rando[3] = b;
  }
  return rando;
}
