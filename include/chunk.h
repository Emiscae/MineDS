#pragma once
#include <stdlib.h>

#define CHUNK_SIZE 16
#define CHUCK_HEIGHT 128

void chunkStart(uint8_t Chunk[CHUNK_SIZE][CHUNK_HEIGHT][CHUNK_SIZE]);
/*void chunkRender(uint8_t Chunk[CHUNK_SIZE][CHUNK_HEIGHT][CHUNK_SIZE], int playerY);*/
void chunkRender(uint8_t Chunk[CHUNK_SIZE][CHUNK_HEIGHT][CHUNK_SIZE], int playerX, int playerY, int playerZ, float camRotY);
