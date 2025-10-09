#ifndef CHUNK_H
#define CHUNK_H

#include <stdint.h>

#define CHUNK_SIZE 8
#define MAX_BLOCKS 100

typedef struct {
    uint8_t blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];
} Chunk;

// Inicializa o chunk com blocos aleatórios
void initChunk(Chunk* chunk);

// Pega o bloco de um chunk
uint8_t getBlock(Chunk* chunk, int x, int y, int z);

// Define o bloco de um chunk
void setBlock(Chunk* chunk, int x, int y, int z, uint8_t blockType);

// Renderiza o chunk, usando apenas faces visíveis
void renderChunk(Chunk* chunk, int startX, int startY, int startZ);

void grass(int angle, int x, int y, int z, int px, int py, int pz);
void stone(int angle, int x, int y, int z, int px, int py, int pz);
void chunkGenerator(int startX, int startY, int startZ, int px, int py, int pz);

#endif
