#include <nds.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#include "texture_bin.h"
#include "render.h"
#include "chunk.h"

#define CHUNK_SIZE 16
#define CHUNK_HEIGHT 128
#define EYE_HEIGHT 1.62f
#define GRAVITY   0.020f
#define JUMP_FORCE 0.22f

///////
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

float moveSpeed = 0.072f;
///////

uint8_t Chunk[CHUNK_SIZE][CHUNK_HEIGHT][CHUNK_SIZE] = {0};

// Função auxiliar para verificar se há bloco sólido
bool isSolid(int x, int y, int z) {
    if (x < 0 || x >= CHUNK_SIZE || y < 0 || y >= CHUNK_HEIGHT || z < 0 || z >= CHUNK_SIZE)
        return true; // fora do mundo = sólido
    return Chunk[x][y][z] != 0;
}
bool collides(float newX, float newY, float newZ) {
    int minX = (int)floor(newX - 0.3f);
    int maxX = (int)floor(newX + 0.3f);
    int minZ = (int)floor(newZ - 0.3f);
    int maxZ = (int)floor(newZ + 0.3f);
    int minY = (int)floor(newY);
    int maxY = (int)floor(newY + 1.8f);

    for (int xi = minX; xi <= maxX; xi++) {
        for (int zi = minZ; zi <= maxZ; zi++) {
            for (int yi = minY; yi <= maxY; yi++) {
                if (xi < 0 || xi >= CHUNK_SIZE || yi < 0 || yi >= CHUNK_HEIGHT || zi < 0 || zi >= CHUNK_SIZE)
                    return true; // fora do mundo = sólido
                if (Chunk[xi][yi][zi] != 0) return true;
            }
        }
    }
    return false;
}
bool onGround(float x, float y, float z) {
    int xi = (int)floor(x);
    int zi = (int)floor(z);
    int yi = (int)floor(y - 0.1f); // bloco logo abaixo dos pés

    if (xi < 0 || xi >= CHUNK_SIZE || yi < 0 || yi >= CHUNK_HEIGHT || zi < 0 || zi >= CHUNK_SIZE)
        return true; // fora do mundo = sólido

    return Chunk[xi][yi][zi] != 0;
}

bool hasFloor(float x, float y, float z) {
    float halfWidth = 0.3f; // metade da largura do jogador (~0.6 blocos)
    int minX = (int)floor(x - halfWidth);
    int maxX = (int)floor(x + halfWidth);
    int minZ = (int)floor(z - halfWidth);
    int maxZ = (int)floor(z + halfWidth);
    int yi   = (int)floor(y - 0.1f); // bloco logo abaixo dos pés

    for (int xi = minX; xi <= maxX; xi++) {
        for (int zi = minZ; zi <= maxZ; zi++) {
            if (xi < 0 || xi >= CHUNK_SIZE || yi < 0 || yi >= CHUNK_HEIGHT || zi < 0 || zi >= CHUNK_SIZE)
                return true; // fora do mundo = sólido
            if (Chunk[xi][yi][zi] != 0) return true; // encontrou chão
        }
    }
    return false;
}
// Verifica se há bloco sólido logo acima da cabeça
bool hasCeiling(float x, float y, float z) {
    int xi = (int)floor(x);
    int zi = (int)floor(z);
    int yi = (int)floor(y + 1.8f); // topo da cabeça

    if (xi < 0 || xi >= CHUNK_SIZE || yi < 0 || yi >= CHUNK_HEIGHT || zi < 0 || zi >= CHUNK_SIZE)
        return true;

    return Chunk[xi][yi][zi] != 0;
}

// Calcula o vetor unitário para onde o jogador está olhando
void getLookDirection(float camRotX, float camRotY, float* dirX, float* dirY, float* dirZ) {
    float radX = camRotX * (M_PI / 180.0f);
    float radY = camRotY * (M_PI / 180.0f);

    // No DS, dependendo de como as matrizes foram invertidas:
    *dirX = sinf(radY) * cosf(radX);
    *dirY = -sinf(radX); // Negativo porque olhar para cima diminui o ângulo no passo anterior
    *dirZ = -cosf(radY) * cosf(radX);
}

// Retorna true se encontrou e quebrou um bloco
bool breakBlockLookedAt(float playerX, float playerY, float playerZ, float camRotX, float camRotY) {
    float dirX, dirY, dirZ;
    getLookDirection(camRotX, camRotY, &dirX, &dirY, &dirZ);

    // Ponto de partida do raio: os olhos do jogador
    float rayX = playerX;
    float rayY = playerY + EYE_HEIGHT;
    float rayZ = playerZ;

    float maxDistance = 5.0f; // Alcance do clique (em blocos)
    float step = 0.05f;       // Precisão do raio (passos de 0.05 blocos)
    
    for (float dist = 0; dist < maxDistance; dist += step) {
        rayX += dirX * step;
        rayY += dirY * step;
        rayZ += dirZ * step;

        // Converte a posição atual do raio para coordenadas inteiras do bloco
        int blockX = (int)floor(rayX);
        int blockY = (int)floor(rayY);
        int blockZ = (int)floor(rayZ);

        // Garante que o raio não saiu dos limites do Chunk
        if (blockX >= 0 && blockX < CHUNK_SIZE &&
            blockY >= 0 && blockY < CHUNK_HEIGHT &&
            blockZ >= 0 && blockZ < CHUNK_SIZE) {
            
            // Se encontrar qualquer bloco diferente de 0 (Ar)
            if (Chunk[blockX][blockY][blockZ] != 0) {
                Chunk[blockX][blockY][blockZ] = 0; // Quebra o bloco!
                return true; // Sucesso
            }
        }
    }
    return false; // Nenhum bloco no alcance
}

int main()
{
    float y = 66.0f, z = 5.0f, x = 4.0f;
    float sensitivity = 3.0f;
    float camRotX = 0, camRotY = 0;
    float velocityY = 0.0f;
    bool isGrounded = false;   

    int textureID;

    // Inicialização do hardware do DS (Mantido igual)
    videoSetMode(MODE_0_3D);
    glInit();
    glEnable(GL_TEXTURE_2D);
    glViewport(0, 0, 255, 191);
    glEnable(GL_ANTIALIAS);
    
    // Configuração de Fog (Mantido igual)
    glEnable(GL_FOG);
    glFogColor(18, 20, 24, 31);
    glFogOffset(0x7000);
    glFogShift(3);
    for(int i = 0; i < 32; i++) glFogDensity(i, i * 4);

    glClearColor(18, 20, 24, 31);
    glClearPolyID(63);
    glClearDepth(0x7FFF);
    
    vramSetBankA(VRAM_A_TEXTURE);
    glGenTextures(1, &textureID);
    glBindTexture(0, textureID);

    glTexImage2D(
        0, 0, GL_RGB,
        TEXTURE_SIZE_256, TEXTURE_SIZE_256,
        0, TEXGEN_TEXCOORD, (u8*)texture_bin
    );

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70, 256.0 / 192.0, 0.1, 40);
    gluLookAt(
        0.0, 0.0, 1.0,
        0.0, 0.0, 0.0,
        0.0, 1.0, 0.0
    );

    consoleDemoInit();
    chunkStart(Chunk);

    while (pmMainLoop())
    {
        // ==========================================
        // 1. INPUTS & VETORES DE DIREÇÃO
        // ==========================================
        scanKeys();
        u16 keys = keysHeld();
        u16 keysD = keysDown();

        // Rotação da Câmera
        if (keys & KEY_Y) camRotY -= sensitivity;
        if (keys & KEY_A) camRotY += sensitivity;
        if (keys & KEY_X && camRotX >= -90) camRotX -= sensitivity;
        if (keys & KEY_B && camRotX <= 90)  camRotX += sensitivity;

        float radY = camRotY * (M_PI / 180.0f);
        float forwardX = sinf(radY);
        float forwardZ = -cosf(radY);
        float rightX = cosf(radY);
        float rightZ = sinf(radY);

        // ==========================================
        // 2. FÍSICA E COLISÃO VERTICAL (EIXO Y)
        // ==========================================
        // Aplica Gravidade
        velocityY -= GRAVITY;
        velocityY *= 0.98f; // Resistência do ar

        // Pulo (só funciona se estiver no chão)
        if ((keys & KEY_START) && isGrounded) {
            velocityY = JUMP_FORCE;
            isGrounded = false;
        }

        // Tenta mover no eixo Y
        float newY = y + velocityY;

        if (velocityY <= 0) {
            // Descendo ou parado: verifica se há chão embaixo do novo Y
            if (hasFloor(x, newY, z)) {
                int yi = (int)floor(newY - 0.1f);
                y = (float)(yi + 1); // Posiciona exatamente no topo do bloco detector
                velocityY = 0;
                isGrounded = true;
            } else {
                y = newY;
                isGrounded = false;
            }
        } 
        else if (velocityY > 0) {
            // Subindo: verifica colisão com o teto (topo da cabeça)
            if (collides(x, newY, z)) {
                velocityY = 0; // Bateu no teto, interrompe a subida
            } else {
                y = newY;
            }
            isGrounded = false;
        }

        if (keysD & KEY_R) breakBlockLookedAt(x, y, z, camRotX, camRotY);

        // ==========================================
        // 3. MOVIMENTAÇÃO LATERAL (EIXOS X / Z)
        // ==========================================
        float moveX = 0.0f;
        float moveZ = 0.0f;

        if (keys & KEY_UP) {    moveX += forwardX; moveZ += forwardZ; }
        if (keys & KEY_DOWN) {  moveX -= forwardX; moveZ -= forwardZ; }
        if (keys & KEY_LEFT) {  moveX -= rightX;   moveZ -= rightZ;   }
        if (keys & KEY_RIGHT) { moveX += rightX;   moveZ += rightZ;   }

        // Aplica movimento com checagem de colisão deslizante (Separada por eixo)
        if (moveX != 0.0f || moveZ != 0.0f) {
            float targetX = x + moveX * moveSpeed;
            if (!collides(targetX, y, z)) {
                x = targetX;
            }
            
            float targetZ = z + moveZ * moveSpeed;
            if (!collides(x, y, targetZ)) {
                z = targetZ;
            }
        }

        // ==========================================
        // 4. MATRIZES E MATERIAIS GL
        // ==========================================
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity(); 

        // Aplica transformações da câmera
        glRotatef32i(degreesToAngle(camRotX), inttof32(1), 0, 0);
        glRotatef32i(degreesToAngle(camRotY), 0, inttof32(1), 0);
        glTranslatef32(floattof32(-x), floattof32(-(y + EYE_HEIGHT)), floattof32(-z));

        glMatrixMode(GL_TEXTURE);
        glLoadIdentity();
        
        glMatrixMode(GL_MODELVIEW);
        glMaterialf(GL_AMBIENT, RGB15(31,31,31));  
        glMaterialf(GL_DIFFUSE, RGB15(31,31,31));  
        glMaterialf(GL_SPECULAR, RGB15(0,0,0));    
        glMaterialf(GL_EMISSION, RGB15(31,31,31)); 
        glMaterialShinyness();

        glPolyFmt(POLY_ALPHA(31) | POLY_CULL_BACK | POLY_FOG);

        // ==========================================
        // 5. RENDER E SINCRONISMO
        // ==========================================
        glBindTexture(0, textureID);
        chunkRender(Chunk, (int)x, (int)y, (int)z, camRotY);

        glFlush(0);
        swiWaitForVBlank();

        // Debug
        consoleClear();
        iprintf("\nX: %d | Y: %d | Z: %d", int(x), int(y), int(z));
        iprintf("\nGround: %d\n", isGrounded);
    }

    return 0;
}
