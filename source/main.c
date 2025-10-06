#include <nds.h>
#include <stdio.h>
#include <math.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif



int textureID;

//-------------------------------------------------------
// Desenha um cubo com textura na posição (x, y, z)
//-------------------------------------------------------
/*void drawTexturedCubeAt(int x, int y, int z) {
    glPushMatrix();
    glTranslatef(x, y, z);

    glBindTexture(GL_TEXTURE_2D, textureID);
    glBegin(GL_QUADS);

    glColor3b(255, 255, 255); // cor neutra para textura

    // Frente
    glTexCoord2f(0.0, 1.0); glVertex3f(-0.5, -0.5, 0.5);
    glTexCoord2f(1.0, 1.0); glVertex3f(0.5, -0.5, 0.5);
    glTexCoord2f(1.0, 0.0); glVertex3f(0.5, 0.5, 0.5);
    glTexCoord2f(0.0, 0.0); glVertex3f(-0.5, 0.5, 0.5);

    // Trás
    glTexCoord2f(0.0, 1.0); glVertex3f(-0.5, -0.5, -0.5);
    glTexCoord2f(1.0, 1.0); glVertex3f(0.5, -0.5, -0.5);
    glTexCoord2f(1.0, 0.0); glVertex3f(0.5, 0.5, -0.5);
    glTexCoord2f(0.0, 0.0); glVertex3f(-0.5, 0.5, -0.5);

    // Esquerda
    glTexCoord2f(0.0, 1.0); glVertex3f(-0.5, -0.5, -0.5);
    glTexCoord2f(1.0, 1.0); glVertex3f(-0.5, -0.5, 0.5);
    glTexCoord2f(1.0, 0.0); glVertex3f(-0.5, 0.5, 0.5);
    glTexCoord2f(0.0, 0.0); glVertex3f(-0.5, 0.5, -0.5);

    // Direita
    glTexCoord2f(0.0, 1.0); glVertex3f(0.5, -0.5, -0.5);
    glTexCoord2f(1.0, 1.0); glVertex3f(0.5, -0.5, 0.5);
    glTexCoord2f(1.0, 0.0); glVertex3f(0.5, 0.5, 0.5);
    glTexCoord2f(0.0, 0.0); glVertex3f(0.5, 0.5, -0.5);

    // Topo
    glTexCoord2f(0.0, 1.0); glVertex3f(-0.5, 0.5, -0.5);
    glTexCoord2f(1.0, 1.0); glVertex3f(-0.5, 0.5, 0.5);
    glTexCoord2f(1.0, 0.0); glVertex3f(0.5, 0.5, 0.5);
    glTexCoord2f(0.0, 0.0); glVertex3f(0.5, 0.5, -0.5);

    // Base
    glTexCoord2f(0.0, 1.0); glVertex3f(-0.5, -0.5, -0.5);
    glTexCoord2f(1.0, 1.0); glVertex3f(-0.5, -0.5, 0.5);
    glTexCoord2f(1.0, 0.0); glVertex3f(0.5, -0.5, 0.5);
    glTexCoord2f(0.0, 0.0); glVertex3f(0.5, -0.5, -0.5);

    glEnd();
    glPopMatrix(1);
}*/

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
    renderCube(angle, x, y + 2, z);
    renderCube(angle, x, y + 4, z);
    renderCube(angle, x - 2, y, z);
    renderCube(angle, x + 2, y, z);

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

    float moveSpeed = 0.25f;
    float angleRad = camRotY * (M_PI / 32768.0); // converte de f32 para radianos

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

        glFlush(0);
    }

    return 0;
}