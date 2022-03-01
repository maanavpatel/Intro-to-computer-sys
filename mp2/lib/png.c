/**
 * [PROVIDED CODE]: waf's solution
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#include "crc32.h"
#include "png.h"

const int ERROR_INVALID_PARAMS = 1;
const int ERROR_INVALID_FILE = 2;
const int ERROR_INVALID_CHUNK_DATA = 3;
const int ERROR_NO_UIUC_CHUNK = 4;

const unsigned char PNG_FILE_FINGERPRINT[8] = { 0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a };

/**
 * Opens a PNG file for reading (mode == "r" or mode == "r+") or writing (mode == "w").
 * 
 * (Note: The function follows the same function prototype as `fopen`.)
 * 
 * When the file is opened for reading this function must verify the PNG signature.  When opened for
 * writing, the file should write the PNG signature.
 * 
 * This function must return NULL on any errors; otherwise, return a new PNG struct for use
 * with further fuctions in this library.
 */
PNG * PNG_open(const char *filename, const char *mode) {
  FILE *f = fopen(filename, mode);
  if (!f) {
    perror("Failed to open file.");
    return NULL;
  }

  if (strcmp(mode, "r") == 0 || strcmp(mode, "r+") == 0) {
    // Read the first 8-bytes of the file:
    char buffer[8];
    size_t len = fread(buffer, sizeof(char), 8, f);

    // Ensure the first eight chars are a valid PNG file:
    if (len != 8 || memcmp(buffer, PNG_FILE_FINGERPRINT, len) != 0) {
      fclose(f);
      return NULL;
    }
  } else if (strcmp(mode, "w") == 0) {
    // Write the PNG signature:
    fwrite(PNG_FILE_FINGERPRINT, sizeof(char), sizeof(PNG_FILE_FINGERPRINT), f);
  } else {
    perror("Unknown mode.");
    return NULL;
  }

  PNG *png = malloc(sizeof(PNG));
  png->file = f;
  return png;
}


/**
 * Reads the next PNG chunk from `png`.
 * 
 * If a chunk exists, a the data in the chunk is populated in `chunk` and the
 * number of bytes read (the length of the chunk in the file) is returned.
 * Otherwise, a zero value is returned.
 * 
 * Any memory allocated within `chunk` must be freed in `PNG_free_chunk`.
 * Users of the library must call `PNG_free_chunk` on all returned chunks.
 */
size_t PNG_read(PNG *png, PNG_Chunk *chunk) {
  FILE *f = png->file;
  size_t size = 0;

  // Read the 4-byte length of the chunk:
  // - The value is always in "network byte order", which may differ from the "host byte order".
  // - `ntohl` converts "network byte order" to "host byte order" (if they differ).
  size += fread(&chunk->len, 1, sizeof(uint32_t), f);
  if (size != 4) { perror("Failed to read 4-byte header."); return 0; }
  chunk->len = ntohl(chunk->len);

  // Read the 4-byte chunk type:
  // - The chunk type will always be ASCII and may be printed as a string.
  // - Therefore, a NULL byte is added at chunk->type[4] to terminate the string.
  size += fread(chunk->type, sizeof(char), 4, f);
  if (size != 8) { perror("Failed to read 4-byte chunk type."); return 0; }
  chunk->type[4] = 0;

  // Allocate and read the `chunk->len` bytes of data (or set the pointer to NULL if length is 0):
  if (chunk->len > 0) {
    chunk->data = malloc(chunk->len);
    size += fread(chunk->data, sizeof(char), chunk->len, f);
    if (size != 8 + chunk->len) { perror("Failed to read contents."); return 0; }
  } else {
    chunk->data = NULL;
  }

  // Finally, read the 4-byte CRC (using ntohl again to convert from network byte order to host byte order):
  size += fread(&chunk->crc, 1, sizeof(uint32_t), f);
  if (size != 12 + chunk->len) { perror("Failed to read CRC."); return 0; }
  chunk->crc = ntohl(chunk->crc);

  return size;
}


/**
 * Writes a PNG chunk to `png`.
 * 
 * Returns the number of bytes written. 
 */
size_t PNG_write(PNG *png, PNG_Chunk *chunk) {
  FILE *f = png->file;
  size_t bytesWritten = 0;

  // Write the length in network byte order:
  uint32_t networkOrder_len = htonl(chunk->len);
  bytesWritten += fwrite(&networkOrder_len, sizeof(uint32_t), 1, f) * sizeof(uint32_t);

  // Write the byte type and data:
  bytesWritten += fwrite(chunk->type, sizeof(char), 4, f);
  bytesWritten += fwrite(chunk->data, sizeof(char), chunk->len, f);

  // Calculate the CRC:
  unsigned char *crcBuffer = malloc(4 + chunk->len);
  memcpy(crcBuffer, chunk->type, 4);
  memcpy(crcBuffer + 4, chunk->data, chunk->len);
  uint32_t crc = 0;
  crc32(crcBuffer, 4 + chunk->len, &crc);
  free(crcBuffer);

  // Write the CRC in network byte order:
  uint32_t networkOrder_crc = htonl(crc);
  bytesWritten += fwrite(&networkOrder_crc, sizeof(uint32_t), 1, f) * sizeof(uint32_t);

  return bytesWritten;
}


/**
 * Frees all memory allocated by this library related to `chunk`.
 */
void PNG_free_chunk(PNG_Chunk *chunk) {
  if (chunk) {
    free(chunk->data);
    chunk->data = NULL;
  }
}


/**
 * Closes the PNG file and frees all memory related to `png`.
 */
void PNG_close(PNG *png) {
  fclose(png->file);
}
