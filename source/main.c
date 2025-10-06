#include <nds.h>
#include <stdio.h>
#include <math.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

int forceX = 0;
int forceZ = 0;
int moveSpeed = 1;

void calculateMovementVectors(int angle);

void renderCube(int angle, float x, float y, float z) {
    //-------------------------------------------------------

    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef32i(degreesToAngle(angle), inttof32(1), inttof32(1), inttof32(1));

    glBegin(GL_QUADS);
    glColor3b(100, 65, 35);		glVertex3f(-1.0f, 1.0f, 1.0f);
    glColor3b(100, 65, 35);		glVertex3f(1.0f, 1.0f, 1.0f);
    glColor3b(100, 65, 35);		glVertex3f(1.0f, -1.0f, 1.0f);
    glColor3b(100, 65, 35);	    glVertex3f(-1.0f, -1.0f, 1.0f);

    glColor3b(100, 65, 35);		glVertex3f(-1.0f, 1.0f, -1.0f);
    glColor3b(100, 65, 35);		glVertex3f(1.0f, 1.0f, -1.0f);
    glColor3b(100, 65, 35);		glVertex3f(1.0f, -1.0f, -1.0f);
    glColor3b(100, 65, 35);	    glVertex3f(-1.0f, -1.0f, -1.0f);

    glColor3b(0, 200, 0);		glVertex3f(-1.0f, 1.0f, 1.0f);
    glColor3b(0, 200, 0);		glVertex3f(1.0f, 1.0f, 1.0f);
    glColor3b(0, 200, 0);		glVertex3f(1.0f, 1.0f, -1.0f);
    glColor3b(0, 200, 0);	    glVertex3f(-1.0f, 1.0f, -1.0f);

    glColor3b(100, 65, 35);		glVertex3f(-1.0f, -1.0f, 1.0f);
    glColor3b(100, 65, 35);		glVertex3f(1.0f, -1.0f, 1.0f);
    glColor3b(100, 65, 35);		glVertex3f(1.0f, -1.0f, -1.0f);
    glColor3b(100, 65, 35);	    glVertex3f(-1.0f, -1.0f, -1.0f);

    glColor3b(100, 65, 35);		glVertex3f(1.0f, 1.0f, -1.0f);
    glColor3b(100, 65, 35);		glVertex3f(1.0f, 1.0f, 1.0f);
    glColor3b(100, 65, 35);		glVertex3f(1.0f, -1.0f, 1.0f);
    glColor3b(100, 65, 35);	    glVertex3f(1.0f, -1.0f, -1.0f);

    glColor3b(100, 65, 35);		glVertex3f(-1.0f, 1.0f, -1.0f);
    glColor3b(100, 65, 35);		glVertex3f(-1.0f, 1.0f, 1.0f);
    glColor3b(100, 65, 35);		glVertex3f(-1.0f, -1.0f, 1.0f);
    glColor3b(100, 65, 35);	    glVertex3f(-1.0f, -1.0f, -1.0f);

    glEnd();

    glPopMatrix(1);
}

void forma(int angle, float x, float y, float z) {
    renderCube(angle, x, y, z);
    renderCube(angle, x - 2, y, z);
    renderCube(angle, x + 2, y, z);
    renderCube(angle, x - 4, y + 2, z);
    renderCube(angle, x + 4, y + 2, z);
    renderCube(angle, x - 2, y + 6, z);
    renderCube(angle, x + 2, y + 6, z);

}

//-------------------------------------------------------
/*void renderScene() {
    int startX = -1;
    int startZ = -6;
    int spacing = 1;

    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            int x = startX + col * spacing;
            int z = startZ + row * spacing;
            renderCube(0, 0, 0, -4);
            drawTexturedCubeAt(x, -1, z);
        }
    }
}*/

//-------------------------------------------------------
int main() {
    float y, z, a;
    float x = y = z = a = 0;

    int camRotX = 0;
    int camRotY = 0;

    
    float angleRad = camRotY * (M_PI / 32768.0); // converte de f32 para radianossad

    /*s16 dx = cosf(camRotY);
    s16 dz = sinf(camRotY);*/


    videoSetMode(MODE_0_3D);
    videoSetModeSub(MODE_0_2D);

    consoleDemoInit();

    glInit();
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_ANTIALIAS);

    glClearColor(0, 0, 0, 31);
    glClearPolyID(63);
    glClearDepth(0x7FFF);

    glViewport(0, 0, 255, 191);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70, 256.0 / 192.0, 0.1, 100);

    glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE);

    //glTexPalette(0, 256, (u16*)blocoPal);


    while (1) {
        swiWaitForVBlank();
        


        float radY = camRotY * (M_PI / 32768.0); // converte f32 para rad
        float radX = camRotX * (M_PI / 32768.0);

        float forwardX = sinf(radY);             // eixo X
        float forwardZ = cosf(radY);             // eixo Z

        


        scanKeys();
        int keys = keysHeld();
        if (keys & KEY_START) break;

        if (keys & KEY_Y)   camRotY -= 1; // 1 grau
        if (keys & KEY_A)   camRotY += 1;
        if (keys & KEY_X)   camRotX -= 1;
        if (keys & KEY_B)   camRotX += 1;

        if (keys & KEY_UP) {
            x += forwardX * moveSpeed;
            z += forwardZ * moveSpeed;
        }
        if (keys & KEY_DOWN) {
            x -= forwardX * moveSpeed;
            z -= forwardZ * moveSpeed;
        }

        float rightX = cosf(radY);   // perpendicular a forward
        float rightZ = -sinf(radY);

        if (keys & KEY_LEFT) {
            x += rightX * moveSpeed;
            z += rightZ * moveSpeed;
        }
        if (keys & KEY_RIGHT) {
            x -= rightX * moveSpeed;
            z -= rightZ * moveSpeed;
        }


        if (keys & KEY_L) {
            z -= 0.2;
        }
        if (keys & KEY_R) {
            z += 0.2;
        }
        /*if (keys & KEY_A) {
            a += 2;
        }
        if (keys & KEY_B) {
            a -= 2;
        }*/

        

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glRotateX(camRotX);
        glRotateY(camRotY);
        glRotatef32i(camRotX, inttof32(1), 0, 0); // rotação em X
        glRotatef32i(camRotY, 0, inttof32(1), 0); // rotação em Y


        forma(0, x, -2, z);

        //renderScene();

        //renderCube(0, x, y, z);


        


        consoleClear();
        iprintf("X: %d\n", (int)x);
        iprintf("Y: %d\n", (int)y);
        iprintf("Z: %d\n", (int)z);
        iprintf("camY: %d\n", (int)camRotY);

        glFlush(0);
    }

    return 0;
}

void calculateMovementVectors(int angle) {
    // Obter o seno e cosseno do ângulo.
        // O libnds usa sinLerp/cosLerp para ponto fixo, que retornam valores Q15 (entre -32768 e 32767).
    int sin_val = sinLerp(angle);
    int cos_val = cosLerp(angle);

    // --- CÁLCULO DAS FORÇAS (X e Z) ---

    // Força Z: moveSpeed * cos(angle)
    // O cálculo de ponto fixo precisa ser ajustado:
    // (moveSpeed * cos_val) >> 12 é um bom ponto de partida se moveSpeed for 'int' e cos_val for Q15.
    // Dependendo do seu sistema de coordenadas, você pode precisar de um ajuste de escala diferente.

    // Usaremos um shift de 12 para um bom equilíbrio (moveSpeed é 'int' e sin/cos é Q15)
    // Isso é equivalente a: (moveSpeed * cos_val) / 4096.0
    // O 4096 (2^12) é um valor de escala escolhido empiricamente para um bom movimento.

    // Cálculo da Força Z (Movimento Frente/Trás)
    // Sinal Negativo/Positivo no Z:
    // Se a sua câmera usa +Z como "para frente" (padrão em muitos motores), você pode usar o sinal direto.
    // Se a sua câmera usa -Z como "para frente" (comum no DevKitPro para 'olhar para -Z'), você
    // pode precisar inverter o sinal do cos_val ou do resultado.

    // Vamos assumir o padrão: +Z é para a frente.
    forceZ = (moveSpeed * cos_val) >> 12;

    // Cálculo da Força X (Movimento Esquerda/Direita)
    forceX = (moveSpeed * sin_val) >> 12;

    // --- Aplicação ao movimento (exemplo) ---
    // Você aplicaria estes valores às coordenadas da sua entidade (playerX, playerZ).
    // playerX += forceX;
    // playerZ += forceZ;

    // Exemplo de exibição (apenas para debug)
    // iprintf("\nRot Y: %d", angle);
    // iprintf("\nForca X: %d", forceX);
    // iprintf("\nForca Z: %d", forceZ);
}