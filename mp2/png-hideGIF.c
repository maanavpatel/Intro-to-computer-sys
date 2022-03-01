#include <stdio.h>
#include <stdlib.h>
#include "lib/png.h"
#include <string.h>

int png_hideGIF(const char *png_filename, const char *gif_filename) {
  size_t byte_count;
  PNG_Chunk *c;
  FILE *wfile = fopen(gif_filename, "w");
  FILE *rfile = fopen(gif_filename, "r");
  PNG *png = PNG_open(png_filename, "r");
  uint32_t tot = htonl(c->len);

  byte_count += fwrite(&tot, sizeof(char), 4, png->file) * sizeof(uint32_t);
  byte_count += fwrite(c->type, sizeof(char), 4, png->file);
  byte_count += fwrite(c->data, sizeof(char), c->len, png->file);
  unsigned char *charu = malloc(c->len + 4);
  memcpy(charu, c->type, 4);
  memcpy(charu + 4, c->data, c->len);

  uint32_t crc = 0;
  crc32(charu, 4 + c->len, &crc);

  uint32_t uin = htonl(crc);
  byte_count += fwrite(&uin, sizeof(char), 4, png->file) * sizeof(uint32_t);

  return 0;
}

int main(int argc, char *argv[]) {
  // Ensure the correct number of arguments:
  if (argc != 3) {
    printf("Usage: %s <PNG File> <GIF File>\n", argv[0]);
    
    return ERROR_INVALID_PARAMS;
  }

  return png_hideGIF(argv[1], argv[2]);
}
