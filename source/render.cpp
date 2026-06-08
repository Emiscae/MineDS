#include <nds.h>
#include <stdlib.h>

#include "render.h"

#define CHUNK_SIZE 16
#define CHUNK_HEIGHT 128

// vértices do cubo
v16 CubeVectors[] = {
    floattov16(0.0), floattov16(0.0), floattov16(1.0),
    floattov16(1.0),  floattov16(0.0), floattov16(1.0),
    floattov16(1.0),  floattov16(0.0), floattov16(0.0),
    floattov16(0.0), floattov16(0.0), floattov16(0.0),

    floattov16(0.0), floattov16(1.0),  floattov16(1.0),
    floattov16(1.0),  floattov16(1.0),  floattov16(1.0),
    floattov16(1.0),  floattov16(1.0),  floattov16(0.0),
    floattov16(0.0), floattov16(1.0),  floattov16(0.0)
};

// faces
u8 CubeFaces[] = {
    3,2,1,0,
    0,1,5,4,
    1,2,6,5,
    2,3,7,6,
    3,0,4,7,
    5,6,7,4
};

u32 uv[4];

void setTextureTile(uint8_t tileX, uint8_t tileY, uint8_t i)
{
    int u = tileX * 16;
    int v = tileY * 16;

    switch(i){
        case 0:
            uv[0] = TEXTURE_PACK(inttot16(u + 16), inttot16(v));
            uv[1] = TEXTURE_PACK(inttot16(u + 16), inttot16(v + 16));
            uv[2] = TEXTURE_PACK(inttot16(u), inttot16(v + 16));
            uv[3] = TEXTURE_PACK(inttot16(u), inttot16(v));
            break;
        case 5:
            uv[0] = TEXTURE_PACK(inttot16(u + 16), inttot16(v));
            uv[1] = TEXTURE_PACK(inttot16(u + 16), inttot16(v + 16));
            uv[2] = TEXTURE_PACK(inttot16(u), inttot16(v + 16));
            uv[3] = TEXTURE_PACK(inttot16(u), inttot16(v));
            break;
        default:
            uv[0] = TEXTURE_PACK(inttot16(u), inttot16(v + 16));
            uv[1] = TEXTURE_PACK(inttot16(u + 16), inttot16(v + 16));
            uv[2] = TEXTURE_PACK(inttot16(u + 16), inttot16(v));
            uv[3] = TEXTURE_PACK(inttot16(u), inttot16(v));
            break;
        }
}

u32 normals[] = {
    NORMAL_PACK(0, floattov10(-.97), 0),
    NORMAL_PACK(0, 0, floattov10(.97)),
    NORMAL_PACK(floattov10(.97), 0, 0),
    NORMAL_PACK(0, 0, floattov10(-.97)),
    NORMAL_PACK(floattov10(-.97), 0, 0),
    NORMAL_PACK(0, floattov10(.97), 0)
};

void drawQuad(int poly)
{ 
    u32 f1 = CubeFaces[poly * 4];
    u32 f2 = CubeFaces[poly * 4 + 1];
    u32 f3 = CubeFaces[poly * 4 + 2];
    u32 f4 = CubeFaces[poly * 4 + 3];

    glNormal(normals[poly]);

    GFX_TEX_COORD = uv[0];
    glVertex3v16(
        CubeVectors[f1 * 3],
        CubeVectors[f1 * 3 + 1],
        CubeVectors[f1 * 3 + 2]
    );

    GFX_TEX_COORD = uv[1];
    glVertex3v16(
        CubeVectors[f2 * 3],
        CubeVectors[f2 * 3 + 1],
        CubeVectors[f2 * 3 + 2]
    );

    GFX_TEX_COORD = uv[2];
    glVertex3v16(
        CubeVectors[f3 * 3],
        CubeVectors[f3 * 3 + 1],
        CubeVectors[f3 * 3 + 2]
    );

    GFX_TEX_COORD = uv[3];
    glVertex3v16(
        CubeVectors[f4 * 3],
        CubeVectors[f4 * 3 + 1],
        CubeVectors[f4 * 3 + 2]
    );
}

void renderBlock(int x, int y, int z, uint8_t Chunk[CHUNK_SIZE][CHUNK_HEIGHT][CHUNK_SIZE]){
    glPushMatrix();

    glTranslatef32(inttof32(x), inttof32(y), inttof32(z));
    
    glBegin(GL_QUAD);
    if(Chunk[x][y][z] != 0){
        for (int i = 0; i < 6; i++){         
            switch(Chunk[x][y][z]){
                case 0: // Air
                    break;
                case 1: // Stone
                    setTextureTile(1, 0, i);
                    break;  
                case 2: // Grass
                    if(i == 0) setTextureTile(2, 0, i);
                    else if(i == 5){
                        setTextureTile(0, 0, i);
                    }
                    else{
                        setTextureTile(3, 0, i);
                    }
                    break;
                case 3: // Dirt
                    setTextureTile(2, 0, i);
                    break;
                case 9: // Water
                    setTextureTile(15, 13, i);
                    break;
                case 12: // Sand
                    setTextureTile(2, 1, i);
                    break;
                case 17: // Log
                    if(i == 5 || i == 0){
                        setTextureTile(5, 1, i);
                    }
                    else{
                        setTextureTile(4, 1, i);
                    }
                    break;
                case 18: // Leaves
                    setTextureTile(3, 1, i);
                    break;
                case 7: // TNT
                    if(i == 0) setTextureTile(10, 0, i);
                    else if(i == 5){
                        setTextureTile(9, 0, i);
                    }
                    else{
                        setTextureTile(8, 0, i);
                    }
                    break;
            }
        
            // Down 
            if (i == 0 && (y == 0 || Chunk[x][y - 1][z] == 0)) drawQuad(i);

            // Front
            if (i == 1 && (z == CHUNK_SIZE - 1 || Chunk[x][y][z + 1] == 0)) drawQuad(i);

            // Right
            if (i == 2 && (x == CHUNK_SIZE - 1 || Chunk[x + 1][y][z] == 0)) drawQuad(i);     

            // Back
            if (i == 3 && (z == 0 || Chunk[x][y][z - 1] == 0)) drawQuad(i);       

            // Left
            if (i == 4 && (x == 0 || Chunk[x - 1][y][z] == 0)) drawQuad(i);

            // Up
            if (i == 5 && (y == CHUNK_HEIGHT - 1 || Chunk[x][y + 1][z] == 0)) drawQuad(i); 
            
        }
    }
    glEnd();

    glPopMatrix(1);
}
