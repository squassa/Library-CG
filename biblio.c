//gcc biblio.c -o biblio -lglut -lGL -lGLU -lm
#include <stdlib.h>
//#include <windows.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <stdio.h>
#include <math.h>
#include <stdarg.h>
#include <unistd.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"



//Definição de Constantes do predio
#define TAMX 300
#define TAMZ 400
#define TAMY 80

//Definição de Constantes do jardim
#define TAMXJARDIM TAMX/4
#define TAMYJARDIM TAMZ/4

//Definição de Constantes da porta
#define TAMPORTAX 50
#define TAMPORTAY 40
#define XPORTA 20 //Posição X do canto esquerdo da porta



void desenhaPrateleira(double x, double y, double z);
void desenharCena();
void desenhaChao();
void desenhaTeto();
void desenhaPredio();

//Declaração de Variáveis Globis
int projecao=0; //Variável Lógica para Definir o Tipo de Projeção (Perspectiva ou Ortogonal)
float posx=0, posy=30, posz=300; //Variáveis que definem a posição da câmera
float oy=30,ox=0,oz=0;         //Variáveis que definem para onde a câmera olha
int lx=0, ly=1,  lz=0;     //Variáveis que definem o eixo da câmera


GLfloat angulox=0.0;
GLfloat anguloy=0.0;

int x_temp=-1; //Armazena posição X do ponteiro do mouse
int y_temp=-1; //Armazena posição Y do ponteiro do mouse
int rotacao = 0; //Controla eixo de rotação do mouse
int i;//controle do for

GLuint texID[2]; // IDs das texturas


const char *textures[] = {
    "tijolo.png",
    "chao-fora.jpeg"
};



void loadTexture(const char *filename, GLuint texID) {
    int w, h, channels; //w*h -> resolu��o da imagem, channels: n. de canais de cor
    unsigned char *data = stbi_load(filename, &w, &h, &channels, 0); //Carrega a imagem do arquivo
    if (!data) { //Verifica se imagem foi carregada corretamente
        printf("Erro ao carregar %s\n", filename);
        exit(1);
    }
    glBindTexture(GL_TEXTURE_2D, texID);//Associa a textura texid �s pr�ximas opera��es do OpenGL envolvendo textura
    
    //Efeito "esticar" textura para preencher o pol�gono
    /*GL_TEXTURE_WRAP_S: eixo horizontal da textura (U).
	GL_TEXTURE_WRAP_T: eixo vertical da textura (V).
	GL_CLAMP_TO_EDGE: limita a amostra da textura � borda da imagem (evita �vazamento� de pixels ao redor).*/
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    //Efeito "repetir" textura para preencher o pol�gono
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Repeti��o no eixo S (horizontal)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Repeti��o no eixo T (vertical)
    
	//Deixar comentado acima o efeito que N�O for utilizar
    /*********************/
    
    
    /*GL_TEXTURE_MIN_FILTER: quando a textura for reduzida (minifica��o).
	GL_TEXTURE_MAG_FILTER: quando a textura for ampliada (magnifica��o).
	GL_LINEAR: faz interpola��o linear (suaviza os pixels, evita blocos vis�veis)*/
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLenum format = (channels == 4) ? GL_RGBA : GL_RGB; //Define o formato da textura de acordo com channels
    
	//envia os dados da imagem para a GPU como textura 2D
	glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);//Libera a mem�ria
}


void initTextures() {
    glGenTextures(2, texID); //gera IDs �nicas para texturas no OpenGL.
    for (int i = 0; i < 2; i++) {
        loadTexture(textures[i], texID[i]);
    }
}


void stroke_output(GLfloat x, GLfloat y, char *format,...)//função para escrever em 3d
{
    va_list args;
    char buffer[200], *p;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);
    glPushMatrix();
    glScalef(0.05, 0.05, 0.05);
    for (p = buffer; *p; p++)
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
    glPopMatrix();
}


// Inicializa parâmetros de rendering
void Inicializa(void)
{
    // Define a cor de fundo da janela de visualização como preta
    glClearColor(0, 255, 255, 0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glutWarpPointer(400,300);
    glutSetCursor(GLUT_CURSOR_NONE);
}

void DISPLAY(void)
{
    glMatrixMode(GL_PROJECTION);/*glMatrixMode()- define qual matriz será alterada. SEMPRE defina o tipo de apresentação
                              (Ortogonal ou Perspectiva) na matriz PROJECTION.*/
    glLoadIdentity();//"Limpa" ou "transforma" a matriz em identidade, reduzindo possíveis erros.
    if (projecao==1)
    {
        glOrtho(-200, 200, -200, 200, -200, 200);  //Define a projeção como ortogonal
        if (rotacao=1)
            glRotatef(angulox,1.0,0.0,0.0);
        if (rotacao=2)
            glRotatef(anguloy,0.0,1.0,0.0);
    }
    if (projecao == 0)
    {
        gluPerspective(90,1,1,500); //Define a projeção como perspectiva
        if (rotacao=1)
            glRotatef(angulox,1.0,0.0,0.0);
        if (rotacao=2)
            glRotatef(anguloy,0.0,1.0,0.0);
    }

    glMatrixMode(GL_MODELVIEW);/*glMatrixMode()- define qual matriz será alterada. SEMPRE defina a câmera
                              (Ortogonal ou Perspectiva) na matriz MODELVIEW (onde o desenho ocorrerá).*/
    glLoadIdentity(); ////"Limpa" ou "transforma" a matriz em identidade, reduzindo possíveis erros.

    gluLookAt(posx,posy,posz,ox,oy,oz,lx,ly,lz); //Define a pos da câmera, para onde olha e qual eixo está na vertical.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); /* "limpa" um buffer particular ou combinações de buffers,
                                                         onde buffer é uma área de armazenamento para informações da imagem.
                                                        Nesse caso, está "limpando os buffers para suportarem animações */
    desenharCena();
	//Sleep (10);
    glutSwapBuffers();
    glutPostRedisplay();
}

void desenhaPrateleira(double x, double y, double z)
{
    glColor3ub(128,128,128);
    glPushMatrix();
    glTranslatef(x,y,z);
    glScalef(5,0.5,1);
    glutSolidCube(20);
    glPopMatrix();


}

void desenhaChao(){
    //Chão dentro
    glColor3ub(150,75,0);
    glPushMatrix();
    glTranslatef(0,0.04,0);
    glScalef(TAMX,0.1,TAMZ);
    glutSolidCube(1);
    glPopMatrix();

    //Chão fora
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texID[1]);
    glColor3f(1,1,1);
    glBegin(GL_QUADS);
        glTexCoord2f(0,0); glVertex3f(XPORTA,0.4,TAMZ/2);
        glTexCoord2f(12,0); glVertex3f(XPORTA, 0.4,TAMZ/2 + TAMZ/8);
        glTexCoord2f(12,12); glVertex3f(TAMX/2,0.4,TAMZ/2 + TAMZ/8);
        glTexCoord2f(0,12); glVertex3f(TAMX/2,0.4,TAMZ/2);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    //Jardim
    glColor3ub(0,200,0);
    glPushMatrix();
    glTranslatef(0,0.05,0);
    glScalef(TAMXJARDIM,0.1,TAMYJARDIM);
    glutSolidCube(1);
    glPopMatrix();
}

void desenhaTeto(){
    //Teto opção 1
    /*glColor3ub(100,100,100);
    glPushMatrix();
    glTranslatef(0,TAMY + TAMY/6,TAMZ/16);
    glScalef(TAMX,TAMY/3,TAMZ + TAMZ/8);
    glutSolidCube(1);
    glPopMatrix();
    */

    //Teto opção 2
    //Teto frente
    glColor3f(0.5,0.5,0.5);
    glPushMatrix();
    glBegin(GL_QUADS);
    glVertex3f(-TAMX/2,TAMY,TAMZ/2 + TAMZ/8);
    glVertex3f(TAMX/2,TAMY,TAMZ/2 + TAMZ/8);
    glVertex3f(TAMX/2,TAMY + TAMY/3,TAMZ/2 + TAMZ/8);
    glVertex3f(-TAMX/2,TAMY + TAMY/3,TAMZ/2 + TAMZ/8);
    glEnd();
    glPopMatrix();

    //Teto trás
    glColor3f(0.5,0.5,0.5);
    glPushMatrix();
    glBegin(GL_QUADS);
    glVertex3f(-TAMX/2,TAMY,-TAMZ/2);
    glVertex3f(TAMX/2,TAMY,-TAMZ/2);
    glVertex3f(TAMX/2,TAMY + TAMY/3,-TAMZ/2);
    glVertex3f(-TAMX/2,TAMY + TAMY/3,-TAMZ/2);
    glEnd();
    glPopMatrix();

    //Teto esquerda
    glColor3f(0.48,0.48,0.48);
    glPushMatrix();
    glBegin(GL_QUADS);
    glVertex3f(-TAMX/2,TAMY,-TAMZ/2);
    glVertex3f(-TAMX/2,TAMY,TAMZ/2 + TAMZ/8);
    glVertex3f(-TAMX/2,TAMY + TAMY/3,TAMZ/2 + TAMZ/8);
    glVertex3f(-TAMX/2,TAMY + TAMY/3,-TAMZ/2);
    glEnd();
    glPopMatrix();


    //Teto direita
    glColor3f(0.52,0.52,0.52);
    glPushMatrix();
    glBegin(GL_QUADS);
    glVertex3f(TAMX/2,TAMY,-TAMZ/2);
    glVertex3f(TAMX/2,TAMY,TAMZ/2 + TAMZ/8);
    glVertex3f(TAMX/2,TAMY + TAMY/3,TAMZ/2 + TAMZ/8);
    glVertex3f(TAMX/2,TAMY + TAMY/3,-TAMZ/2);
    glEnd();
    glPopMatrix();

    //Teto baixo
    glColor3f(0.54,0.54,0.54);
    glPushMatrix();
    glBegin(GL_QUADS);
    glVertex3f(-TAMX/2,TAMY,-TAMZ/2);
    glVertex3f(TAMX/2,TAMY,-TAMZ/2);
    glVertex3f(TAMX/2,TAMY,TAMZ/2 + TAMZ/8);
    glVertex3f(-TAMX/2,TAMY,TAMZ/2 + TAMZ/8);
    glEnd();
    glPopMatrix();

    //Teto cima
    glColor3f(0.56,0.56,0.56);
    glPushMatrix();
    glBegin(GL_QUADS);
    glVertex3f(-TAMX/2,TAMY + TAMY/3,-TAMZ/2);
    glVertex3f(TAMX/2,TAMY + TAMY/3,-TAMZ/2);
    glVertex3f(TAMX/2,TAMY + TAMY/3,TAMZ/2 + TAMZ/8);
    glVertex3f(-TAMX/2,TAMY + TAMY/3,TAMZ/2 + TAMZ/8);
    glEnd();
    glPopMatrix();

    //Escreve biblioteca    
    glColor3ub(0,0,0);
    glPushMatrix();
    glTranslatef(XPORTA + TAMPORTAX, TAMY + 7, TAMZ/2 + TAMZ/8 + 2);
    stroke_output(0, 0, "BIBLIOTECA");
    glPopMatrix();
    

}


void desenhaPredio(){

    //PARTE EXTERNA
    //Face Frente
    //F1
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texID[0]);
    glColor3f(1,1,1);
    glBegin(GL_QUADS);
        glTexCoord2f(0,0); glVertex3f(-TAMX/2,0,TAMZ/2);
        glTexCoord2f(2,0); glVertex3f(XPORTA,0,TAMZ/2);
        glTexCoord2f(2,1); glVertex3f(XPORTA,TAMY,TAMZ/2);
        glTexCoord2f(0,1); glVertex3f(-TAMX/2,TAMY,TAMZ/2);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    //F2
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texID[0]);
    glColor3f(1,1,1);
    glBegin(GL_QUADS);
        glTexCoord2f(0,0); glVertex3f(-TAMX/8,0,TAMZ/2);
        glTexCoord2f(1,0); glVertex3f(-TAMX/8,0,TAMZ/2 + TAMZ/8);
        glTexCoord2f(1,1); glVertex3f(-TAMX/8,TAMY,TAMZ/2 + TAMZ/8);
        glTexCoord2f(0,1); glVertex3f(-TAMX/8,TAMY,TAMZ/2);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    //F3
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texID[0]);
    glColor3f(1,1,1);
    glBegin(GL_QUADS);
        glTexCoord2f(0,0); glVertex3f(-TAMX/8,0,TAMZ/2 + TAMZ/8);
        glTexCoord2f(1,0); glVertex3f(XPORTA,0,TAMZ/2 + TAMZ/8);
        glTexCoord2f(1,1); glVertex3f(XPORTA,TAMY,TAMZ/2 + TAMZ/8);
        glTexCoord2f(0,1); glVertex3f(-TAMX/8,TAMY,TAMZ/2 + TAMZ/8);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    //F4
    glColor3f(0.8,0.8,0.8);
    glPushMatrix();
    glBegin(GL_QUADS);
    glVertex3f(XPORTA,0,TAMZ/2);
    glVertex3f(XPORTA,0,TAMZ/2 + TAMZ/8);
    glVertex3f(XPORTA,TAMY,TAMZ/2 + TAMZ/8);
    glVertex3f(XPORTA,TAMY,TAMZ/2);
    glEnd();
    glPopMatrix();


    //F5.1
    glColor3f(0.78,0.78,0.78);
    glPushMatrix();
    glBegin(GL_QUADS);
    glVertex3f(TAMX/2,0,TAMZ/2);
    glVertex3f(XPORTA + TAMPORTAX,0,TAMZ/2);
    glVertex3f(XPORTA + TAMPORTAX,TAMY,TAMZ/2);
    glVertex3f(TAMX/2,TAMY,TAMZ/2);
    glEnd();
    glPopMatrix();
    //F5.2
    glColor3f(0.78,0.78,0.78);
    glPushMatrix();
    glBegin(GL_QUADS);
    glVertex3f(XPORTA,TAMPORTAY,TAMZ/2);
    glVertex3f(XPORTA + TAMPORTAX,TAMPORTAY,TAMZ/2);
    glVertex3f(XPORTA + TAMPORTAX,TAMY,TAMZ/2);
    glVertex3f(XPORTA,TAMY,TAMZ/2);
    glEnd();
    glPopMatrix();

    //Face Trás
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texID[0]);
    glColor3f(1,1,1);
    glBegin(GL_QUADS);
        glTexCoord2f(0,0); glVertex3f(-TAMX/2,0,-TAMZ/2);
        glTexCoord2f(6,0); glVertex3f(TAMX/2,0,-TAMZ/2);
        glTexCoord2f(6,1); glVertex3f(TAMX/2,TAMY,-TAMZ/2);
        glTexCoord2f(0,1); glVertex3f(-TAMX/2,TAMY,-TAMZ/2);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    //Face Esquerda
    //E1
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texID[0]);
    glColor3f(1,1,1);
    glBegin(GL_QUADS);
        glTexCoord2f(0,0); glVertex3f(-TAMX/2,0,-TAMZ/2);
        glTexCoord2f(1,0); glVertex3f(-TAMX/2,0,-TAMZ/4);
        glTexCoord2f(1,1); glVertex3f(-TAMX/2,TAMY,-TAMZ/4);
        glTexCoord2f(0,1); glVertex3f(-TAMX/2,TAMY,-TAMZ/2);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    //E2
    glColor3f(0.7,0.7,0.7);
    glPushMatrix();
    glBegin(GL_QUADS);
    glVertex3f(-TAMX/2,0,-TAMZ/4);
    glVertex3f(-TAMX/2 + TAMX/5,0,-TAMZ/4);
    glVertex3f(-TAMX/2 + TAMX/5,TAMY,-TAMZ/4);
    glVertex3f(-TAMX/2,TAMY,-TAMZ/4);
    glEnd();
    glPopMatrix();

    //E3
    glColor3f(0.68,0.68,0.68);
    glPushMatrix();
    glBegin(GL_QUADS);
    glVertex3f(-TAMX/2 + TAMX/5,0,-TAMZ/4);
    glVertex3f(-TAMX/2 + TAMX/5,0,-TAMZ/7);
    glVertex3f(-TAMX/2 + TAMX/5,TAMY,-TAMZ/7);
    glVertex3f(-TAMX/2 + TAMX/5,TAMY,-TAMZ/4);
    glEnd();
    glPopMatrix();

    
    //E4
    glColor3f(0.66,0.66,0.66);
    glPushMatrix();
    glBegin(GL_QUADS);
    glVertex3f(-TAMX/2 + TAMX/5,0,-TAMZ/7);
    glVertex3f(-TAMX/2 + TAMX/15,0,-TAMZ/7);
    glVertex3f(-TAMX/2 + TAMX/15,TAMY,-TAMZ/7);
    glVertex3f(-TAMX/2 + TAMX/5,TAMY,-TAMZ/7);
    glEnd();
    glPopMatrix();

    //E5
    glColor3f(0.67,0.67,0.67);
    glPushMatrix();
    glBegin(GL_QUADS);
    glVertex3f(-TAMX/2 + TAMX/15,0,-TAMZ/7);
    glVertex3f(-TAMX/2 + TAMX/15,0,-TAMZ/10);
    glVertex3f(-TAMX/2 + TAMX/15,TAMY,-TAMZ/10);
    glVertex3f(-TAMX/2 + TAMX/15,TAMY,-TAMZ/7);
    glEnd();
    glPopMatrix();

    //E6
    glColor3f(0.7,0.7,0.7);
    glPushMatrix();
    glBegin(GL_QUADS);
    glVertex3f(-TAMX/2 + TAMX/15,0,-TAMZ/10);
    glVertex3f(-TAMX/2,0,-TAMZ/10);
    glVertex3f(-TAMX/2,TAMY,-TAMZ/10);
    glVertex3f(-TAMX/2 + TAMX/15,TAMY,-TAMZ/10);
    glEnd();
    glPopMatrix();

    //E7
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texID[0]);
    glColor3f(1,1,1);
    glBegin(GL_QUADS);
        glTexCoord2f(0,0); glVertex3f(-TAMX/2,0,-TAMZ/10);
        glTexCoord2f(4,0); glVertex3f(-TAMX/2,0,TAMZ/2);
        glTexCoord2f(4,1); glVertex3f(-TAMX/2,TAMY,TAMZ/2);
        glTexCoord2f(0,1); glVertex3f(-TAMX/2,TAMY,-TAMZ/10);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    //Face Direita
    //D1
    glColor3f(0.83,0.83,0.83);
    glPushMatrix();
    glBegin(GL_QUADS);
    glVertex3f(TAMX/2,0,-TAMZ/4);
    glVertex3f(TAMX/2,0,-TAMZ/2);
    glVertex3f(TAMX/2,TAMY,-TAMZ/2);
    glVertex3f(TAMX/2,TAMY,-TAMZ/4);
    glEnd();
    glPopMatrix();
    
    //D2
    glColor3f(0.73,0.73,0.73);
    glPushMatrix();
    glBegin(GL_QUADS);
    glVertex3f(TAMX/2 - TAMX/6,0,-TAMZ/4);
    glVertex3f(TAMX/2,0,-TAMZ/4);
    glVertex3f(TAMX/2,TAMY,-TAMZ/4);
    glVertex3f(TAMX/2 - TAMX/6,TAMY,-TAMZ/4);
    glEnd();
    glPopMatrix();
    
    //D3
    glColor3f(0.68,0.68,0.68);
    glPushMatrix();
    glBegin(GL_QUADS);
    glVertex3f(TAMX/2 - TAMX/6,0,0);
    glVertex3f(TAMX/2 - TAMX/6,0,-TAMZ/4);
    glVertex3f(TAMX/2 - TAMX/6,TAMY,-TAMZ/4);
    glVertex3f(TAMX/2 - TAMX/6,TAMY,0);
    glEnd();
    glPopMatrix();

    //D4
    glColor3f(0.73,0.73,0.73);
    glPushMatrix();
    glBegin(GL_QUADS);
    glVertex3f(TAMX/2,0,0);
    glVertex3f(TAMX/2 - TAMX/6,0,0);
    glVertex3f(TAMX/2 - TAMX/6,TAMY,0);
    glVertex3f(TAMX/2,TAMY,0);
    glEnd();
    glPopMatrix();

    //D5
    glColor3f(0.83,0.83,0.83);
    glPushMatrix();
    glBegin(GL_QUADS);
    glVertex3f(TAMX/2,0,0);
    glVertex3f(TAMX/2,0,TAMZ/2);
    glVertex3f(TAMX/2,TAMY,TAMZ/2);
    glVertex3f(TAMX/2,TAMY,0);
    glEnd();
    glPopMatrix();

    desenhaChao();
    desenhaTeto();
}

void desenharCena ()
{
    /*Crosta Terrestre*/
    glColor3ub(100,200,0);
    glPushMatrix();
    glTranslatef(0,-0.02,0);
    glScalef(60,0.01,60);
    glutSolidCube(20);
    glPopMatrix();
    /******/

    //desenhaPrateleira(0,0,0);
    desenhaPredio();

    
  
}

void MOUSE_Button(int botao, int estado, int x, int y)
{
    //botão - recebe o código do botão pressionado
    //estado - recebe se está pressionado ou não
    //x, y - recebem respectivamente as posições do cursor do mouse
    switch (botao)
    {
    case GLUT_LEFT_BUTTON:
        if (estado == GLUT_DOWN)
        {
            projecao=1;
            posx=0;
            posy=0;
            posz=20;
            ox=0,oy=0,oz=0;
            lx=0, ly=1,lz=0;
            glutPostRedisplay();
        }
        break;

    case GLUT_RIGHT_BUTTON:
        if(estado == GLUT_DOWN)
        {
            projecao=0;
            posx=0;
            posy=30;
            posz=200;
            oy=30;
            ox=0;
            oz=0;
            lx=0, ly=1, lz=0;
            glutPostRedisplay();
        }
        break;
    }
}

void MOUSE(int x, int y)
{
    if (x > x_temp)
    {
        anguloy=anguloy+(x/100);
        if(anguloy > 360.0)
            anguloy = anguloy-360;
        rotacao = 2;
        glutPostRedisplay();
    }
    if (x < x_temp)
    {
        anguloy=anguloy-(x/100);
        if(anguloy > 360.0)
            anguloy = anguloy-360;
        rotacao = 2;
        glutPostRedisplay();
    }
    x_temp=x;
    if (y>y_temp)
    {
        angulox=angulox+(y/100);
        if(angulox > 360.0)
            angulox = angulox-360;
        rotacao = 1;
       glutPostRedisplay();
    }
    if (y<y_temp)
    {
        angulox=angulox-(y/100);
        if(angulox > 360.0)
            angulox = angulox-360;
        rotacao = 1;
        glutPostRedisplay();
    }
    y_temp = y;
    if(x>=500 || x<=300 || y>=400 || y<=200) //delimitar uma area de acesso ao mouse de 600x450
    {
        glutWarpPointer(400,300);
        glutPostRedisplay();
    }
    
}

void keyboard(unsigned char tecla, int x, int y)
{
    switch(tecla)
    {
    case 's':
        posx=posx+(5*cos((anguloy+90)/57.32));
        posz=posz+(5*sin((anguloy+90)/57.32));
        ox=ox+(5*cos((anguloy+90)/57.32));
        oz=oz+(5*sin((anguloy+90)/57.32));
        glutPostRedisplay();
        break;

    case 'w':
        posx=posx-(5*cos((anguloy+90)/57.32));
        posz=posz-(5*sin((anguloy+90)/57.32));
        ox=ox-(5*cos((anguloy+90)/57.32));
        oz=oz-(5*sin((anguloy+90)/57.32));
        glutPostRedisplay();
        break;

    case 'd':
        posx=posx+(5*sin((anguloy+90)/57.32));
        posz=posz-(5*cos((anguloy+90)/57.32));
        ox=ox+(5*sin((anguloy+90)/57.32));
        oz=oz-(5*cos((anguloy+90)/57.32));
        glutPostRedisplay();
        break;

    case 'a':
        posx=posx-(5*sin((anguloy+90)/57.32));
        posz=posz+(5*cos((anguloy+90)/57.32));
        ox=ox-(5*sin((anguloy+90)/57.32));
        oz=oz+(5*cos((anguloy+90)/57.32));
        glutPostRedisplay();
        break;
    case 27:
        exit(0);
        break;
    default:
        break;
    }
}

void TeclasEspeciais (int key, int x, int y)
{
    //Key - recebe o código ASCII da tecla
//x, y - recebem respectivamente as posições do cursor do mouse (permite tratar os dois dispositivos)
    if (key==GLUT_KEY_PAGE_UP)
    {
        posy+=5;
        oy+=5;
    }
    else if (key==GLUT_KEY_PAGE_DOWN)
    {
        posy-=5;
        oy-=5;
    }
    glutPostRedisplay();
}

int main(int argc, char**argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL); /*Define as características do espaço vetorial.
                                                                        //  Nesse caso, permite animações (sem cintilações), cores compostas por Verm. Verde e Azul,
                                                                //  Buffer que permite trablhar com profundidade e elimina faces escondidas.*/
    glutInitWindowSize(800,600);
    glutInitWindowPosition(10, 10);
    glutCreateWindow("Biblioteca CG");
    Inicializa();
    initTextures();
    glutDisplayFunc(DISPLAY);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(MOUSE_Button);
    glutPassiveMotionFunc(MOUSE);
    glutSpecialFunc(TeclasEspeciais);
    glutMainLoop();
    return 0;
}