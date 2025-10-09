#ifndef RENDER_H
#define RENDER_H

// Declara as funções disponíveis para outros arquivos
void test(int angle, float x, float y, float z);
//void grass(int angle, float x, float y, float z);
//void stone(int angle, float x, float y, float z);
void forma(int angle, float x, float y, float z);
void renderScene(int v, int h, int startX, int startZ);
void renderCloud(float x, float y, float z);

#endif
