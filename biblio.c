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
#define TAMX 500
#define TAMZ 600
#define TAMY 70

//Definição de Constantes do jardim
#define TAMXJARDIM 77
#define TAMZJARDIM TAMZ/4.8
#define XJARDIM -90
#define ZJARDIM 130

//Definição de Constantes da porta
#define TAMPORTAX 38
#define TAMPORTAY 50
#define XPORTA 50 //Posição X do canto esquerdo da porta

//Definição de Constantes das Salas Grandes
#define TAMSALAGRX 80
#define TAMSALAGRZ 65


//Definição de Constantes da Sala diferentona
#define TAMSALADIFX 74
#define TAMSALADIFZ 59

//Definição de Constantes da Sala diferentona
#define TAMSALAINTX 40
#define TAMSALAINTZ 40

//Definição de Constantes das Prateleiras
#define TAMPRAX 
#define TAMPRAZ 70

#define TAMMESAY 16
#define TAMMESAX 15
#define TAMMESAZ 25

// ----- Estruturas para criar os livros nas prateleiras ----------------------------

typedef struct {
    float r, g, b;
} Color;

typedef struct {
    float w, h;
} TamLivro;

// --- Tamanho fixos dos livros ---
TamLivro tamanhos[5] = {
    {0.7f, 5.5f},
    {0.9f, 6.1f},
    {0.6f, 5.0f},
    {0.8f, 5.8f},
    {0.75f, 6.3f}
};

// --- Cores fixas dos livros ---
Color cores[10] = {
    {0.8f, 0.2f, 0.2f},
    {0.2f, 0.3f, 0.8f},
    {0.9f, 0.8f, 0.1f},
    {0.1f, 0.7f, 0.3f},
    {0.6f, 0.2f, 0.7f},
    {0.7f, 0.5f, 0.1f},
    {0.2f, 0.8f, 0.6f},
    {0.8f, 0.4f, 0.2f},
    {0.3f, 0.6f, 0.9f},
    {0.9f, 0.3f, 0.5f}
};

void desenhaEntrada();
void desenhaLeste();
void desenhaPrateleira(float tam_x, float tam_y, float tam_z);
void desenhaPilar(float ang, float x, float y, float z);
void desenhaPilarInterno(float tamx, float tamy, float tamz);
void desenharCena();
void desenhaChao();
void desenhaTeto();
void desenhaSala(float tam_x_sala, float tam_z_sala, float pos_x_sala, float pos_z_sala);
void desenhaSalaInterna(float tam_x_sala, float tam_z_sala, float pos_x_sala, float pos_z_sala);
void desenhaSalaGrande(float tam_x_sala, float tam_z_sala);
void desenhaJardim();
void desenhaVidros(float xinicio, float xfim, float zinicio, float zfim);
void janelaFechada(float tam_x, float tam_y);
void janelaAberta(float tam_x, float tam_y);
void desenhaObjetos();
void desenhaMesa(float tam_x, float tam_y, float tam_z);
void desenhaCadeira(float tam_x, float tam_y, float tam_z);
void desenhaMesaEntrada();
void desenhaBanheiro(float tam_x, float tam_z, int flag);
void desenhaMeio();
void desenhaOeste();
void desenhaFundoDir();
void desenhaPredio();
void salaDif(float tam_x,float tam_z);
void corPilar();
void porta();
void portaVidro(float tam_x, float tam_y);
void chaoSala(float tam_x, float tam_z);
void mesaDif(float tam_x, float tam_y, float tam_z);
void desenhaLivro(float x, float y, float z, float w, float h, float d, Color c);

//Declaração de Variáveis Globis
int projecao=0; //Variável Lógica para Definir o Tipo de Projeção (Perspectiva ou Ortogonal)
float posx=XPORTA+TAMPORTAX/2, posy=30, posz=TAMZ/2+40; //Variáveis que definem a posição da câmera
float oy=30,ox=0,oz=0;         //Variáveis que definem para onde a câmera olha
int lx=0, ly=1,  lz=0;     //Variáveis que definem o eixo da câmera


GLfloat angulox=0.0;
GLfloat anguloy=0.0;

int x_temp=-1; //Armazena posição X do ponteiro do mouse
int y_temp=-1; //Armazena posição Y do ponteiro do mouse
int rotacao = 0; //Controla eixo de rotação do mouse
int i;//controle do for

GLuint texID[3]; // IDs das texturas


const char *textures[] = {
    "tijolo.png",
    "chao-fora.jpeg",
    "banheiro.png",
    "cadeirante.png"
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
    glGenTextures(4, texID); //gera IDs �nicas para texturas no OpenGL.
    for (int i = 0; i < 4; i++) {
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

void corPilar(){
    glColor3ub(105,107,98);
}

void desenhaLivro(float x, float y, float z, float w, float h, float d, Color c)
{
    glPushMatrix();
    glColor3f(c.r, c.g, c.b);
    glTranslatef(x + w/2, y + h/2, z - d/2);
    glScalef(w, h, d);
    glutSolidCube(1);
    glPopMatrix();
}


void desenhaPrateleira(float tam_x, float tam_y, float tam_z)
{
    // --- LADOS ---
    glPushMatrix();
    glColor3f(0.6f,0.3f,0.0f);
    glTranslatef(0, tam_y/2, -tam_z/2);
    glScalef(1, tam_y, tam_z);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.6f,0.3f,0.0f);
    glTranslatef(tam_x + 1, tam_y/2, -tam_z/2);
    glScalef(1, tam_y, tam_z);
    glutSolidCube(1);
    glPopMatrix();

    // --- PRATELEIRAS + LIVROS ---
    for(int i = 0; 0.5f + i*9 <= tam_y; i++)
    {
        float y = 0.5f + i*9;

        // prateleira física
        glPushMatrix();
        glColor3ub(198,185,147);
        glTranslatef(tam_x/2 + 0.5f, y, -tam_z/2);
        glScalef(tam_x, 1, tam_z);
        glutSolidCube(1);
        glPopMatrix();

        // Distribui os livros nas prateleiras
        float xPos = 1.2f;          // início dentro da prateleira
        float z = 0.0f;             // centralizado
        float d = tam_z * 0.8f;     // profundidade do livro

        int idxTam = 0;
        int idxCor = 0;

        // preencher até o final da prateleira
        while (xPos < tam_x - 1.5f)
        {
            TamLivro T = tamanhos[idxTam];
            Color C = cores[idxCor];

            desenhaLivro(xPos, y + 0.5f, z, T.w, T.h, d, C);

            xPos += T.w + 0.1f;  // pequeno espaço entre livros

            // avanço circular
            idxTam = (idxTam + 1) % 5;
            idxCor = (idxCor + 1) % 10;
        }
    }
}

void desenhaPilar(float ang, float x, float y, float z){
    glPushMatrix();
    glColor3ub(87,85,95);
    glTranslatef(x, y, z);
    glRotatef(ang,0,1,0);
    glScalef(20, TAMY, 5);
    glutSolidCube(1);
    glPopMatrix();
}

void desenhaPilarInterno(float tamx, float tamy, float tamz){
    glPushMatrix();
    glScalef(tamx, tamy, tamz);
    glutSolidCube(1);
    glPopMatrix();
}


void camadaPreta(){
    //Primeira camada preta
    glPushMatrix();
    glColor3f(0.27,0.27,0.27);
    glTranslatef(XPORTA + 32, 5, TAMZ/3.2-26);
    glScalef(5,8,30);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.27,0.27,0.27);
    glTranslatef(XPORTA + 40, 5, TAMZ/3.2);
    glRotatef(35,0,1,0);
    glScalef(5,8,30);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.27,0.27,0.27);
    glTranslatef(XPORTA + 62.5, 5, TAMZ/3.2+11.3);
    glRotatef(90,0,1,0);
    glScalef(5,8,32);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.27,0.27,0.27);
    glTranslatef(XPORTA + 54.5, 5, TAMZ/3.2 -43.5);
    glRotatef(90,0,1,0);
    glScalef(5,8,50);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.27,0.27,0.27);
    glTranslatef(XPORTA + 82, 5, TAMZ/3.2+6.7);
    glRotatef(-45,0,1,0);
    glScalef(5,8,15);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.27,0.27,0.27);
    glTranslatef(XPORTA + 86.2, 5, TAMZ/3.2 -32);
    glRotatef(35,0,1,0);
    glScalef(5,8,30);
    glutSolidCube(1);
    glPopMatrix();


}

void camadaBranca(){
    //Primeira camada branca
    glPushMatrix();
    glColor3f(0.85,0.85,0.85);
    glTranslatef(XPORTA + 32, 10, TAMZ/3.2-26);
    glScalef(5,2,30);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.85,0.85,0.85);
    glTranslatef(XPORTA + 40, 10, TAMZ/3.2);
    glRotatef(35,0,1,0);
    glScalef(5,2,30);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.85,0.85,0.85);
    glTranslatef(XPORTA + 62.5, 10, TAMZ/3.2+11.3);
    glRotatef(90,0,1,0);
    glScalef(5,2,32);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.85,0.85,0.85);
    glTranslatef(XPORTA + 54.5, 10, TAMZ/3.2 -43.5);
    glRotatef(90,0,1,0);
    glScalef(5,2,50);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.85,0.85,0.85);
    glTranslatef(XPORTA + 82, 10, TAMZ/3.2+6.7);
    glRotatef(-45,0,1,0);
    glScalef(5,2,15);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.85,0.85,0.85);
    glTranslatef(XPORTA + 86.2, 10, TAMZ/3.2 -32);
    glRotatef(35,0,1,0);
    glScalef(5,2,30);
    glutSolidCube(1);
    glPopMatrix();
}

void desenhaMesaEntrada(){
    camadaPreta();
    camadaBranca();

    glPushMatrix();
    glTranslatef(0,10,0);
    camadaPreta();
    camadaBranca();
    glPopMatrix();

    //Primeira camada branca

}


void desenhaVidros(float xinicio, float xfim, float zinicio, float zfim){
    float i, j;
    glPushMatrix();
    glColor3f(0.2,0.2,0.2);
    glBegin(GL_QUADS);
        glVertex3f(xinicio,0,zinicio);
        glVertex3f(xfim,0,zfim);
        glVertex3f(xfim,6,zfim);
        glVertex3f(xinicio,6,zinicio);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.2,0.2,0.2);
    glBegin(GL_QUADS);
        glVertex3f(xinicio,TAMY-3,zinicio);
        glVertex3f(xfim,TAMY-3,zfim);
        glVertex3f(xfim,TAMY,zfim);
        glVertex3f(xinicio,TAMY,zinicio);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.2,0.2,0.2);
    glBegin(GL_QUADS);
        glVertex3f(xinicio,TAMY/3-2,zinicio);
        glVertex3f(xfim,TAMY/3-2,zfim);
        glVertex3f(xfim,TAMY/3,zfim);
        glVertex3f(xinicio,TAMY/3,zinicio);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.2,0.2,0.2);
    glBegin(GL_QUADS);
        glVertex3f(xinicio,2*TAMY/3,zinicio);
        glVertex3f(xfim,2*TAMY/3,zfim);
        glVertex3f(xfim,2*TAMY/3+4,zfim);
        glVertex3f(xinicio,2*TAMY/3+4,zinicio);
    glEnd();
    glPopMatrix();
    if(xinicio!=xfim){
        for(int i=xinicio-1; i<=xfim; i+=15){
        glPushMatrix();
        glColor3f(0.2,0.2,0.2);
        glBegin(GL_QUADS);
            glVertex3f(i,0,zinicio);
            glVertex3f(i+2,0,zinicio);
            glVertex3f(i+2,TAMY,zinicio);
            glVertex3f(i,TAMY,zinicio);
        glEnd();
        glPopMatrix();
        }

        glPushMatrix();
        glColor3f(0.2,0.2,0.2);
        glBegin(GL_QUADS);
            glVertex3f(xfim-2,0,zinicio);
            glVertex3f(xfim,0,zinicio);
            glVertex3f(xfim,TAMY,zinicio);
            glVertex3f(xfim-2,TAMY,zinicio);
        glEnd();
        glPopMatrix();

    }
    else{
        for(int i=zinicio+1; i>=zfim; i-=15){
        glPushMatrix();
        glColor3f(0.2,0.2,0.2);
        glBegin(GL_QUADS);
            glVertex3f(xinicio,0,i);
            glVertex3f(xinicio,0,i-2);
            glVertex3f(xinicio,TAMY,i-2);
            glVertex3f(xinicio,TAMY,i);
        glEnd();
        glPopMatrix();
        }

        glPushMatrix();
        glColor3f(0.2,0.2,0.2);
        glBegin(GL_QUADS);
            glVertex3f(xinicio,0,zfim+2);
            glVertex3f(xinicio,0,zfim);
            glVertex3f(xinicio,TAMY,zfim);
            glVertex3f(xinicio,TAMY,zfim+2);
        glEnd();
        glPopMatrix();
    }
}

void desenhaChao(){
    //Chão dentro
    glColor3ub(180,133,71);
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
    glVertex3f(-TAMX/2-TAMSALAGRZ+10,TAMY,TAMZ/2 + TAMZ/8);
    glVertex3f(TAMX/2,TAMY,TAMZ/2 + TAMZ/8);
    glVertex3f(TAMX/2,TAMY + TAMY/3,TAMZ/2 + TAMZ/8);
    glVertex3f(-TAMX/2-TAMSALAGRZ+10,TAMY + TAMY/3,TAMZ/2 + TAMZ/8);
    glEnd();
    glPopMatrix();

    //Teto trás
    glColor3f(0.5,0.5,0.5);
    glPushMatrix();
    glBegin(GL_QUADS);
    glVertex3f(-TAMX/2-TAMSALAGRZ+10,TAMY,-TAMZ/2);
    glVertex3f(TAMX/2,TAMY,-TAMZ/2);
    glVertex3f(TAMX/2,TAMY + TAMY/3,-TAMZ/2);
    glVertex3f(-TAMX/2-TAMSALAGRZ+10,TAMY + TAMY/3,-TAMZ/2);
    glEnd();
    glPopMatrix();

    //Teto esquerda
    glColor3f(0.48,0.48,0.48);
    glPushMatrix();
    glBegin(GL_QUADS);
    glVertex3f(-TAMX/2-TAMSALAGRZ+10,TAMY,-TAMZ/2);
    glVertex3f(-TAMX/2-TAMSALAGRZ+10,TAMY,TAMZ/2 + TAMZ/8);
    glVertex3f(-TAMX/2-TAMSALAGRZ+10,TAMY + TAMY/3,TAMZ/2 + TAMZ/8);
    glVertex3f(-TAMX/2-TAMSALAGRZ+10,TAMY + TAMY/3,-TAMZ/2);
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
    //TB trás
    glColor3f(0.54,0.54,0.54);
    glPushMatrix();
    glBegin(GL_QUADS);
    glVertex3f(-TAMX/2-TAMSALAGRZ+10,TAMY,-TAMZ/2);
    glVertex3f(TAMX/2,TAMY,-TAMZ/2);
    glVertex3f(TAMX/2,TAMY,ZJARDIM - TAMZJARDIM);
    glVertex3f(-TAMX/2-TAMSALAGRZ+10,TAMY,ZJARDIM - TAMZJARDIM);
    glEnd();
    glPopMatrix();

    //TB frente
    glColor3f(0.54,0.54,0.54);
    glPushMatrix();
    glBegin(GL_QUADS);
    glVertex3f(-TAMX/2-TAMSALAGRZ+10,TAMY,TAMZ/2 + TAMZ/8);
    glVertex3f(TAMX/2,TAMY,TAMZ/2 + TAMZ/8);
    glVertex3f(TAMX/2,TAMY,ZJARDIM);
    glVertex3f(-TAMX/2-TAMSALAGRZ+10,TAMY,ZJARDIM);
    glEnd();
    glPopMatrix();
    
    //TB direita
    glColor3f(0.54,0.54,0.54);
    glPushMatrix();
    glBegin(GL_QUADS);
    glVertex3f(XJARDIM + TAMXJARDIM,TAMY,ZJARDIM - TAMZJARDIM);
    glVertex3f(TAMX/2,TAMY,ZJARDIM - TAMZJARDIM);
    glVertex3f(TAMX/2,TAMY,ZJARDIM);
    glVertex3f(XJARDIM + TAMXJARDIM,TAMY,ZJARDIM);
    glEnd();
    glPopMatrix();
    
    //TB esquerda
    glColor3f(0.54,0.54,0.54);
    glPushMatrix();
    glBegin(GL_QUADS);
    glVertex3f(-TAMX/2-TAMSALAGRZ+10,TAMY,ZJARDIM - TAMZJARDIM);
    glVertex3f(XJARDIM,TAMY,ZJARDIM - TAMZJARDIM);
    glVertex3f(XJARDIM,TAMY,ZJARDIM);
    glVertex3f(-TAMX/2-TAMSALAGRZ+10,TAMY,ZJARDIM);
    glEnd();
    glPopMatrix();


    //Teto cima
    //TC trás
    glColor3f(0.54,0.54,0.54);
    glPushMatrix();
    glBegin(GL_QUADS);
    glVertex3f(-TAMX/2-TAMSALAGRZ+10,TAMY + TAMY/3,-TAMZ/2);
    glVertex3f(TAMX/2,TAMY + TAMY/3,-TAMZ/2);
    glVertex3f(TAMX/2,TAMY + TAMY/3,ZJARDIM - TAMZJARDIM);
    glVertex3f(-TAMX/2-TAMSALAGRZ+10,TAMY + TAMY/3,ZJARDIM - TAMZJARDIM);
    glEnd();
    glPopMatrix();

    //TC frente
    glColor3f(0.54,0.54,0.54);
    glPushMatrix();
    glBegin(GL_QUADS);
    glVertex3f(-TAMX/2-TAMSALAGRZ+10,TAMY + TAMY/3,TAMZ/2 + TAMZ/8);
    glVertex3f(TAMX/2,TAMY + TAMY/3,TAMZ/2 + TAMZ/8);
    glVertex3f(TAMX/2,TAMY + TAMY/3,ZJARDIM);
    glVertex3f(-TAMX/2-TAMSALAGRZ+10,TAMY + TAMY/3,ZJARDIM);
    glEnd();
    glPopMatrix();
    
    //TB direita
    glColor3f(0.54,0.54,0.54);
    glPushMatrix();
    glBegin(GL_QUADS);
    glVertex3f(XJARDIM + TAMXJARDIM,TAMY + TAMY/3,ZJARDIM - TAMZJARDIM);
    glVertex3f(TAMX/2,TAMY + TAMY/3,ZJARDIM - TAMZJARDIM);
    glVertex3f(TAMX/2,TAMY + TAMY/3,ZJARDIM);
    glVertex3f(XJARDIM + TAMXJARDIM,TAMY + TAMY/3,ZJARDIM);
    glEnd();
    glPopMatrix();
    
    //TB esquerda
    glColor3f(0.54,0.54,0.54);
    glPushMatrix();
    glBegin(GL_QUADS);
    glVertex3f(-TAMX/2-TAMSALAGRZ+10,TAMY + TAMY/3,ZJARDIM - TAMZJARDIM);
    glVertex3f(XJARDIM,TAMY + TAMY/3,ZJARDIM - TAMZJARDIM);
    glVertex3f(XJARDIM,TAMY + TAMY/3,ZJARDIM);
    glVertex3f(-TAMX/2-TAMSALAGRZ+10,TAMY + TAMY/3,ZJARDIM);
    glEnd();
    glPopMatrix();
    
    //Fecha teto do jardim
    //FT frente
    glColor3f(0.54,0.54,0.54);
    glPushMatrix();
    glBegin(GL_QUADS);
    glVertex3f(XJARDIM,TAMY,ZJARDIM);
    glVertex3f(XJARDIM + TAMXJARDIM,TAMY,ZJARDIM);
    glVertex3f(XJARDIM + TAMXJARDIM,TAMY + TAMY/3,ZJARDIM);
    glVertex3f(XJARDIM,TAMY + TAMY/3,ZJARDIM);
    glEnd();
    glPopMatrix();

    //FT trás
    glColor3f(0.54,0.54,0.54);
    glPushMatrix();
    glBegin(GL_QUADS);
    glVertex3f(XJARDIM,TAMY,ZJARDIM - TAMZJARDIM);
    glVertex3f(XJARDIM + TAMXJARDIM,TAMY,ZJARDIM - TAMZJARDIM);
    glVertex3f(XJARDIM + TAMXJARDIM,TAMY + TAMY/3,ZJARDIM - TAMZJARDIM);
    glVertex3f(XJARDIM,TAMY + TAMY/3,ZJARDIM - TAMZJARDIM);
    glEnd();
    glPopMatrix();

    //FT direita
    glColor3f(0.54,0.54,0.54);
    glPushMatrix();
    glBegin(GL_QUADS);
    glVertex3f(XJARDIM + TAMXJARDIM,TAMY,ZJARDIM - TAMZJARDIM);
    glVertex3f(XJARDIM + TAMXJARDIM,TAMY,ZJARDIM);
    glVertex3f(XJARDIM + TAMXJARDIM,TAMY + TAMY/3,ZJARDIM);
    glVertex3f(XJARDIM + TAMXJARDIM,TAMY + TAMY/3,ZJARDIM - TAMZJARDIM);
    glEnd();
    glPopMatrix();

    //FT esquerda
    glColor3f(0.54,0.54,0.54);
    glPushMatrix();
    glBegin(GL_QUADS);
    glVertex3f(XJARDIM,TAMY,ZJARDIM - TAMZJARDIM);
    glVertex3f(XJARDIM,TAMY,ZJARDIM);
    glVertex3f(XJARDIM,TAMY + TAMY/3,ZJARDIM);
    glVertex3f(XJARDIM,TAMY + TAMY/3,ZJARDIM - TAMZJARDIM);
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

    //Chao fora
    glPushMatrix();
    glColor3ub(57,27,0);
    glBegin(GL_QUADS);
        glVertex3f(-TAMX/2-1,0.1,TAMZ/2);
        glVertex3f(-TAMX/8,0.1,TAMZ/2);
        glVertex3f(-TAMX/8,0.1,TAMZ/2+TAMZ/8);
        glVertex3f(-TAMX/2-1,0.1,TAMZ/2+TAMZ/8);
    glEnd();
    glPopMatrix();


    //F1
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texID[0]);
    glColor3f(1,1,1);
    glBegin(GL_QUADS);
        glTexCoord2f(1,0); glVertex3f(-TAMX/2,0,TAMZ/2);
        glTexCoord2f(1,1); glVertex3f(-TAMX/2+TAMSALAGRZ+8,0,TAMZ/2);
        glTexCoord2f(0,1); glVertex3f(-TAMX/2+TAMSALAGRZ+8,3*TAMY/4,TAMZ/2);
        glTexCoord2f(0,0); glVertex3f(-TAMX/2,3*TAMY/4,TAMZ/2);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-TAMX/2,TAMY/2,TAMZ/2);
    corPilar();
    desenhaPilarInterno(3,TAMY,3);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-TAMX/2 + TAMSALAGRZ+8,TAMY/2,TAMZ/2);
    corPilar();
    desenhaPilarInterno(3,TAMY,3);
    glPopMatrix();


    //Vidros da frente
    desenhaVidros(-TAMX/2+TAMSALAGRZ+10,-TAMX/8,TAMZ/2,TAMZ/2);
    
    glPushMatrix();
        glColor3f(0.2,0.2,0.2);
        glBegin(GL_QUADS);
            glVertex3f(-TAMX/8,0,TAMZ/2);
            glVertex3f(-TAMX/8-2,0,TAMZ/2);
            glVertex3f(-TAMX/8-2,TAMY,TAMZ/2);
            glVertex3f(-TAMX/8,TAMY,TAMZ/2);
        glEnd();
    glPopMatrix();


    //F2
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texID[0]);
    glColor3f(1,1,1);
    glBegin(GL_QUADS);
        glTexCoord2f(1,0); glVertex3f(-TAMX/8,0,TAMZ/2);
        glTexCoord2f(1,1); glVertex3f(-TAMX/8,0,TAMZ/2 + TAMZ/8);
        glTexCoord2f(0,1); glVertex3f(-TAMX/8,TAMY,TAMZ/2 + TAMZ/8);
        glTexCoord2f(0,0); glVertex3f(-TAMX/8,TAMY,TAMZ/2);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();


    //F3
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texID[0]);
    glColor3f(1,1,1);
    glBegin(GL_QUADS);
        glTexCoord2f(1,0); glVertex3f(-TAMX/8,0,TAMZ/2 + TAMZ/8);
        glTexCoord2f(1,1); glVertex3f(XPORTA,0,TAMZ/2 + TAMZ/8);
        glTexCoord2f(0,1); glVertex3f(XPORTA,TAMY,TAMZ/2 + TAMZ/8);
        glTexCoord2f(0,0); glVertex3f(-TAMX/8,TAMY,TAMZ/2 + TAMZ/8);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();


    //F4
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texID[0]);
    glColor3f(1,1,1);
    glBegin(GL_QUADS);
        glTexCoord2f(1,0); glVertex3f(XPORTA,0,TAMZ/2);
        glTexCoord2f(1,1); glVertex3f(XPORTA,0,TAMZ/2 + TAMZ/8);
        glTexCoord2f(0,1); glVertex3f(XPORTA,TAMY,TAMZ/2 + TAMZ/8);
        glTexCoord2f(0,0); glVertex3f(XPORTA,TAMY,TAMZ/2);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();


    //F5.1
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texID[0]);
    glColor3f(1,1,1);
    glBegin(GL_QUADS);
        glTexCoord2f(0,0); glVertex3f(XPORTA + 2*TAMPORTAX+3,0,TAMZ/2);
        glTexCoord2f(0,1); glVertex3f(TAMX/2-TAMSALAGRZ,0,TAMZ/2);
        glTexCoord2f(1,1); glVertex3f(TAMX/2-TAMSALAGRZ,TAMY,TAMZ/2);
        glTexCoord2f(1,0); glVertex3f(XPORTA + 2*TAMPORTAX+3,TAMY,TAMZ/2);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    
    

    glPushMatrix();
    corPilar();
    glTranslatef(TAMX/2-TAMSALAGRZ,TAMY/2,TAMZ/2);
    desenhaPilarInterno(6, TAMY, 4);
    glPopMatrix();

    //F5.2
    glPushMatrix();
    glColor3ub(156,149,142);
    glBegin(GL_QUADS);
        glVertex3f(TAMX/2-TAMSALAGRZ,0,TAMZ/2);
        glVertex3f(TAMX/2,0,TAMZ/2);
        glVertex3f(TAMX/2,TAMY/3,TAMZ/2);
        glVertex3f(TAMX/2-TAMSALAGRZ,TAMY/3,TAMZ/2);
    glEnd();
    glPopMatrix();

    
    //Pilar entrada
    desenhaPilar(45, TAMX/2 - 10, TAMY/2,TAMZ/2 + TAMZ/10);
    
    //Pilar frente
    for(int i=0;i<6;i++)
        desenhaPilar(45, -TAMX/6-i*(20), TAMY/2,TAMZ/2 + TAMZ/9-5);

    //Pilar frente canto esquerdo
    desenhaPilar(45, -TAMX/2-TAMSALAGRZ+25, TAMY/2,TAMZ/2 + TAMZ/9-5);
//----------------------//
    //Face Fundo
    //Vidros e pilares fundo
    desenhaVidros(-TAMX/2+2.5,-TAMX/4-3,-TAMZ/2,-TAMZ/2);
    
    glPushMatrix();
    glTranslatef(-TAMX/4-1,TAMY/2,-TAMZ/2);
    corPilar();
    desenhaPilarInterno(4,TAMY,3);
    glPopMatrix();
    
    desenhaVidros(-TAMX/4+2,0,-TAMZ/2,-TAMZ/2);
    
    glPushMatrix();
    glTranslatef(2,TAMY/2,-TAMZ/2);
    corPilar();
    desenhaPilarInterno(4,TAMY,3);
    glPopMatrix();
    
    desenhaVidros(5,TAMX/2-62,-TAMZ/2,-TAMZ/2);
    
    glPushMatrix();
    glTranslatef(TAMX/2-60,TAMY/2,-TAMZ/2);
    corPilar();
    desenhaPilarInterno(4,TAMY,3);
    glPopMatrix();
    
    //T1
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texID[0]);
    glColor3f(1,1,1);
    glBegin(GL_QUADS);
        glTexCoord2f(0,1); glVertex3f(TAMX/2-59,0,-TAMZ/2);
        glTexCoord2f(0,0); glVertex3f(TAMX/2,0,-TAMZ/2);
        glTexCoord2f(1,0); glVertex3f(TAMX/2,TAMY,-TAMZ/2);
        glTexCoord2f(1,1); glVertex3f(TAMX/2-59,TAMY,-TAMZ/2);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(TAMX/2,TAMY/2,-TAMZ/2);
    corPilar();
    desenhaPilarInterno(4,TAMY,4);
    glPopMatrix();


    //----------------------//
    //Face Esquerda

    //Chao fora
    glPushMatrix();
    glColor3ub(57,27,0);
    glBegin(GL_QUADS);
        glVertex3f(-TAMX/2-TAMSALAGRZ+10,0.1,-TAMZ/2);
        glVertex3f(-TAMX/2,0.1,-TAMZ/2);
        glVertex3f(-TAMX/2,0.1,-TAMZ/10);
        glVertex3f(-TAMX/2-TAMSALAGRZ+10,0.1,-TAMZ/10);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    desenhaPilar(-45,-TAMX/2-TAMSALADIFZ+15,TAMY/2,-TAMZ/2+10);
    glTranslatef(0,0,40);
    for(int j=0;j<6;j++){
        desenhaPilar(-45,-TAMX/2-TAMSALADIFZ+15,TAMY/2,-TAMZ/2+50+j*(20));
    }
    glPopMatrix();
    //Vidros e pilares esquerda fundo
    glPushMatrix();
    glTranslatef(-TAMX/2+1.5,TAMY/2,-TAMZ/2+1.5);
    corPilar();
    desenhaPilarInterno(4,TAMY,4);
    glPopMatrix();

    desenhaVidros(-TAMX/2,-TAMX/2,-3*TAMZ/8-2.5,-TAMZ/2+2.5);

    glPushMatrix();
    glTranslatef(-TAMX/2,TAMY/2,-3*TAMZ/8);
    corPilar();
    desenhaPilarInterno(3,TAMY,3);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-TAMX/2,TAMY/2,-TAMZ/4+2);
    corPilar();
    desenhaPilarInterno(3,TAMY,6);
    glPopMatrix();

    
    
    
    //E1
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texID[0]);
    glColor3f(1,1,1);
    glBegin(GL_QUADS);
        glTexCoord2f(0,1); glVertex3f(-TAMX/2,0,-3*TAMZ/8);
        glTexCoord2f(0,0); glVertex3f(-TAMX/2,0,-TAMZ/4);
        glTexCoord2f(1,0); glVertex3f(-TAMX/2,TAMY,-TAMZ/4);
        glTexCoord2f(1,1); glVertex3f(-TAMX/2,TAMY,-3*TAMZ/8);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    

    //E2
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texID[0]);
    glColor3f(1,1,1);
    glBegin(GL_QUADS);
        glTexCoord2f(0,1); glVertex3f(-TAMX/2,0,-TAMZ/4);
        glTexCoord2f(0,0); glVertex3f(-TAMX/2 + TAMX/5,0,-TAMZ/4);
        glTexCoord2f(1,0); glVertex3f(-TAMX/2 + TAMX/5,TAMY,-TAMZ/4);
        glTexCoord2f(1,1); glVertex3f(-TAMX/2,TAMY,-TAMZ/4);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    //E3
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texID[0]);
    glColor3f(1,1,1);
    glBegin(GL_QUADS);
        glTexCoord2f(0,1); glVertex3f(-TAMX/2 + TAMX/5,0,-TAMZ/4);
        glTexCoord2f(0,0); glVertex3f(-TAMX/2 + TAMX/5,0,-TAMZ/7);
        glTexCoord2f(1,0); glVertex3f(-TAMX/2 + TAMX/5,TAMY,-TAMZ/7);
        glTexCoord2f(1,1); glVertex3f(-TAMX/2 + TAMX/5,TAMY,-TAMZ/4);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    
    //E4
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texID[0]);
    glColor3f(1,1,1);
    glBegin(GL_QUADS);
        glTexCoord2f(0,0.3); glVertex3f(-TAMX/2 + TAMX/5,0,-TAMZ/7);
        glTexCoord2f(0,0); glVertex3f(-TAMX/2 + TAMX/6,0,-TAMZ/7);
        glTexCoord2f(1,0); glVertex3f(-TAMX/2 + TAMX/6,TAMY,-TAMZ/7);
        glTexCoord2f(1,0.3); glVertex3f(-TAMX/2 + TAMX/5,TAMY,-TAMZ/7);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    //E5
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texID[0]);
    glColor3f(1,1,1);
    glBegin(GL_QUADS);
        glTexCoord2f(0,1); glVertex3f(-TAMX/2,0,-TAMZ/4+3);
        glTexCoord2f(0,0); glVertex3f(-TAMX/2,0,-TAMZ/10);
        glTexCoord2f(1,0); glVertex3f(-TAMX/2,TAMY,-TAMZ/10);
        glTexCoord2f(1,1); glVertex3f(-TAMX/2,TAMY,-TAMZ/4+3);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-TAMX/2,TAMY/2,-TAMZ/10-1.6);
    corPilar();
    desenhaPilarInterno(3,TAMY,3);
    glPopMatrix();


    //E7
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texID[0]);
    glColor3f(1,1,1);
    glBegin(GL_QUADS);
        glTexCoord2f(0,4); glVertex3f(-TAMX/2-TAMSALAGRZ+10,0,-TAMZ/10);
        glTexCoord2f(0,0); glVertex3f(-TAMX/2-TAMSALAGRZ+10,0,TAMZ/2-60);
        glTexCoord2f(0.3,0); glVertex3f(-TAMX/2-TAMSALAGRZ+10,TAMY/3,TAMZ/2-60);
        glTexCoord2f(0.3,4); glVertex3f(-TAMX/2-TAMSALAGRZ+10,TAMY/3,-TAMZ/10);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    //Chao fora
    glPushMatrix();
    glColor3ub(57,27,0);
    glBegin(GL_QUADS);
        glVertex3f(-TAMX/2-TAMSALAGRZ+10,0.1,TAMZ/2+TAMZ/8);
        glVertex3f(-TAMX/2,0.1,TAMZ/2+TAMZ/8);
        glVertex3f(-TAMX/2,0.1,TAMZ/2-60);
        glVertex3f(-TAMX/2-TAMSALAGRZ+10,0.1,TAMZ/2-60);
    glEnd();
    glPopMatrix();
    //-----------------//
    //Face Direita
    //D1
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texID[0]);
    glColor3f(1,1,1);
    glBegin(GL_QUADS);
        glTexCoord2f(0,1); glVertex3f(TAMX/2,0,-TAMZ/4-TAMZ/8);
        glTexCoord2f(0,0); glVertex3f(TAMX/2,0,-TAMZ/2);
        glTexCoord2f(1,0); glVertex3f(TAMX/2,TAMY,-TAMZ/2);
        glTexCoord2f(1,1); glVertex3f(TAMX/2,TAMY,-TAMZ/4-TAMZ/8);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    
    
    //D2
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texID[0]);
    glColor3f(1,1,1);
    glBegin(GL_QUADS);
        glTexCoord2f(0,1); glVertex3f(TAMX/2 - TAMX/6,0,-TAMZ/4+0.1);
        glTexCoord2f(0,0); glVertex3f(TAMX/2,0,-TAMZ/4+0.1);
        glTexCoord2f(1,0); glVertex3f(TAMX/2,TAMY,-TAMZ/4+0.1);
        glTexCoord2f(1,1); glVertex3f(TAMX/2 - TAMX/6,TAMY,-TAMZ/4+0.1);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    
    //Vidros
    desenhaVidros(TAMX/2 - TAMX/6,TAMX/2 - TAMX/6, 0,-TAMZ/8+1);
    desenhaVidros(TAMX/2 - TAMX/6,TAMX/2 - TAMX/6,-TAMZ/8-1, -TAMZ/4+1);

    glPushMatrix();
    glTranslatef(TAMX/2-TAMX/6,TAMY/2,-TAMZ/8+0.5);
    corPilar();
    desenhaPilarInterno(2,TAMY,3);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(TAMX/2-TAMX/6,TAMY/2,-TAMZ/4+0.5);
    corPilar();
    desenhaPilarInterno(3,TAMY,3);
    glPopMatrix();

    //Pilares fora direita
    for(int i=0;i<7;i++){
        desenhaPilar(-50,TAMX/2-TAMX/12-5,TAMY/2,-15-i*(20));
    }

    //Reboco
    glPushMatrix();
    glColor3ub(112,79,71);
    glTranslatef(TAMX/2-TAMX/12,TAMY/2,+2);
    glScalef(TAMX/6+0.2,TAMY,5);
    glutSolidCube(1);
    glPopMatrix();


    //D5
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texID[0]);
    glColor3f(1,1,1);
    glBegin(GL_QUADS);
        glTexCoord2f(0,3); glVertex3f(TAMX/2,0,0);
        glTexCoord2f(0,0); glVertex3f(TAMX/2,0,TAMZ/2);
        glTexCoord2f(0.3,0); glVertex3f(TAMX/2,TAMY/3,TAMZ/2);
        glTexCoord2f(0.3,3); glVertex3f(TAMX/2,TAMY/3,0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    

    desenhaChao();
    desenhaTeto();
    desenhaEntrada();
    desenhaLeste();
    desenhaJardim();
    desenhaMeio();
    desenhaOeste();
    desenhaFundoDir();
}

void janelaFechada(float tam_x, float tam_y){
    glPushMatrix();
    glColor3f(0.25,0.25,0.25);
    glBegin(GL_QUADS);
        glVertex3f(0, 0, 0);
        glVertex3f(2, 0, 0);
        glVertex3f(2, tam_y, 0);
        glVertex3f(0, tam_y, 0);
    glEnd();
    glPopMatrix();
    
    glPushMatrix();
    glColor3f(0.25,0.25,0.25);
    glBegin(GL_QUADS);
        glVertex3f(tam_x-2, 0, 0);
        glVertex3f(tam_x, 0, 0);
        glVertex3f(tam_x, tam_y, 0);
        glVertex3f(tam_x-2, tam_y, 0);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.25,0.25,0.25);
    glBegin(GL_QUADS);
        glVertex3f(0, tam_y-2, 0);
        glVertex3f(tam_x, tam_y-2, 0);
        glVertex3f(tam_x, tam_y, 0);
        glVertex3f(0, tam_y, 0);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.25,0.25,0.25);
    glBegin(GL_QUADS);
        glVertex3f(0, 0, 0);
        glVertex3f(tam_x, 0, 0);
        glVertex3f(tam_x, 2, 0);
        glVertex3f(0, 2, 0);
    glEnd();
    glPopMatrix();
    
}

void janelaAberta(float tam_x, float tam_y){
    glPushMatrix();
    glColor3f(0.25,0.25,0.25);
    glBegin(GL_QUADS);
        glVertex3f(tam_x-2, 5, 10);
        glVertex3f(tam_x, 5, 10);
        glVertex3f(tam_x, tam_y, 0);
        glVertex3f(tam_x-2, tam_y, 0);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.25,0.25,0.25);
    glBegin(GL_QUADS);
        glVertex3f(0, tam_y-2, 0);
        glVertex3f(tam_x, tam_y-2, 0);
        glVertex3f(tam_x, tam_y, 0);
        glVertex3f(0, tam_y, 0);
    glEnd();
    glPopMatrix();
    
    glPushMatrix();
    glColor3f(0.25,0.25,0.25);
    glBegin(GL_QUADS);
        glVertex3f(0, 5, 10);
        glVertex3f(2, 5, 10);
        glVertex3f(2, tam_y, 0);
        glVertex3f(0, tam_y, 0);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.25,0.25,0.25);
    glBegin(GL_QUADS);
        glVertex3f(2, 5, 10);
        glVertex3f(tam_x-2, 5, 10);
        glVertex3f(tam_x-2, 7.4, 9);
        glVertex3f(2, 7.4, 9);
    glEnd();
    glPopMatrix();
}


void chaoSala(float tam_x, float tam_z){
    glPushMatrix();
    glColor3ub(134,81,81);
    glBegin(GL_QUADS);
        glVertex3f(0, 0, 0);
        glVertex3f(tam_x, 0, 0);
        glVertex3f(tam_x, 0, -tam_z);
        glVertex3f(0, 0, -tam_z);
    glEnd();
    glPopMatrix();
}

void desenhaSalaGrande(float tam_x, float tam_z){
    //Chao
    glPushMatrix();
    glTranslatef(0,0.11,0);
    chaoSala(tam_x,tam_z);
    glPopMatrix();

    //Frente da sala grande
    glPushMatrix();
    glColor3ub(156,149,142);
    glBegin(GL_QUADS);
        glVertex3f(0, 0, 0);
        glVertex3f(tam_x-25, 0, 0);
        glVertex3f(tam_x-25, TAMY, 0);
        glVertex3f(0, TAMY, 0);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glColor3ub(156,149,142);
    glBegin(GL_QUADS);
        glVertex3f(tam_x-5, 0, 0);
        glVertex3f(tam_x, 0, 0);
        glVertex3f(tam_x, TAMY, 0);
        glVertex3f(tam_x-5, TAMY, 0);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glColor3ub(156,149,142);
    glBegin(GL_QUADS);
        glVertex3f(tam_x-25, TAMPORTAY, 0);
        glVertex3f(tam_x-5, TAMPORTAY, 0);
        glVertex3f(tam_x-5, TAMY, 0);
        glVertex3f(tam_x-25, TAMY, 0);
    glEnd();
    glPopMatrix();

    //Porta
    glPushMatrix();
    glColor3f(0.75,0.8,0.8);
    glBegin(GL_QUADS);
        glVertex3f(tam_x-25, 0, 0);
        glVertex3f(tam_x-5, 0, 0);
        glVertex3f(tam_x-5, TAMPORTAY, 0);
        glVertex3f(tam_x-25, TAMPORTAY, 0);
    glEnd();
    glPopMatrix();

    //Maçaneta porta
    glPushMatrix();
    glColor3f(0.3,0.3,0.2);
    glTranslatef(tam_x-20, TAMY/3-5, 0);
    glutSolidSphere(1.5,20,20);
    glPopMatrix();


    //Parede direita da sala grande
    glPushMatrix();
    glColor3ub(156,149,142);
    glBegin(GL_QUADS);
        glVertex3f(tam_x, 0, 0);
        glVertex3f(tam_x, 0, -tam_z);
        glVertex3f(tam_x, TAMY, -tam_z);
        glVertex3f(tam_x, TAMY, 0);
    glEnd();
    glPopMatrix();

    //Parede esquerda da sala grande
    glPushMatrix();
    glColor3ub(156,149,142);
    glBegin(GL_QUADS);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, -tam_z);
        glVertex3f(0, TAMY, -tam_z);
        glVertex3f(0, TAMY, 0);
    glEnd();
    glPopMatrix();

    //Parede fundo
    glPushMatrix();
    glColor3ub(156,149,142);
    glBegin(GL_QUADS);
        glVertex3f(0, 0, -tam_z);
        glVertex3f(tam_x, 0, -tam_z);
        glVertex3f(tam_x, TAMY/3, -tam_z);
        glVertex3f(0, TAMY/3, -tam_z);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glColor3ub(115,59,18);
    glTranslatef(tam_x/2, TAMY/3, -tam_z);
    glScalef(tam_x-0.1, 2, 5);
    glutSolidCube(1);
    glPopMatrix();

    //Janelas abertas
    glPushMatrix();
    glTranslatef(0, TAMY/3, -tam_z);
    glTranslatef(tam_x/2,0,0);
    glRotatef(180,0,1,0);
    janelaAberta(tam_x/4, TAMY/2.5);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, TAMY/3, -tam_z);
    glTranslatef(tam_x/4,0,0);
    glRotatef(180,0,1,0);
    janelaAberta(tam_x/4, TAMY/2.5);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, TAMY - (2*TAMY/3 - TAMY/2.5)- 1, -tam_z);
    janelaFechada(tam_x/4, 2*TAMY/3 - TAMY/2.5+1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(tam_x/4, TAMY - (2*TAMY/3 - TAMY/2.5)- 1, -tam_z);
    janelaFechada(tam_x/4, 2*TAMY/3 - TAMY/2.5 + 1);
    glPopMatrix();
    
    //Janelas fechadas
    glPushMatrix();
    glTranslatef(tam_x/2, TAMY/3, -tam_z);
    janelaFechada(tam_x/4, 2*TAMY/3);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(3*tam_x/4, TAMY/3, -tam_z);
    janelaFechada(tam_x/4, 2*TAMY/3);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(tam_x/2, TAMY - (2*TAMY/3 - TAMY/2.5)- 1, -tam_z);
    janelaFechada(tam_x/2, 2*TAMY/3 - TAMY/2.5 + 1);
    glPopMatrix();

    
    //Objetos
    //Mesa
    glPushMatrix();
    glColor3ub(200,137,40);
    glTranslatef(tam_x/2-10,0,-tam_z/2+2);
    glRotatef(90,0,1,0);
    desenhaMesa(TAMMESAX,TAMMESAY,TAMMESAZ);
    glPopMatrix();

    //Cadeira
    glPushMatrix();
    glColor3ub(200,120,115);
    glTranslatef(tam_x/2-4,0,-tam_z/2-20);
    desenhaCadeira(10,30,10);
    glPopMatrix();
}

void desenhaMesa(float tam_x,float tam_y, float tam_z){
    
    glPushMatrix();
    glTranslatef(1,tam_y/2,1);
    glScalef(2,tam_y,2);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(tam_x-1,tam_y/2,1);
    glScalef(2,tam_y,2);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(tam_x-1,tam_y/2,tam_z-1);
    glScalef(2,tam_y,2);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1,tam_y/2,tam_z-1);
    glScalef(2,tam_y,2);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(tam_x/2,tam_y,tam_z/2);
    glScalef(tam_x,2,tam_z);
    glutSolidCube(1);
    glPopMatrix();
}

void desenhaCadeira(float tam_x, float tam_y, float tam_z){
    desenhaMesa(tam_x,tam_y/3,tam_z);
    glPushMatrix();
    glTranslatef(0.5,tam_y/2,0.5);
    glScalef(2,tam_y,2);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(tam_x-0.5,tam_y/2,0.5);
    glScalef(2,tam_y,2);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(tam_x/2,3*tam_y/4-tam_y/12,0.5);
    glScalef(tam_x,tam_y/3,1);
    glutSolidCube(1);
    glPopMatrix();
}

void desenhaMesaRed(float comp, float altura){
    glPushMatrix();
    glColor3ub(195,197,221);
    glTranslatef(0,altura/2,0);
    glScalef(2,altura,2);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,altura,0);
    glScalef(1,0.01,1);
    glutSolidSphere(comp/2,10,10);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,0.5,0);
    glScalef(2*comp/3,1,1);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,0.5,0);
    glScalef(1,1,2*comp/3);
    glutSolidCube(1);
    glPopMatrix();
}

void desenhaMesaRed2(float comp, float altura){
    desenhaMesaRed(comp,altura);
    glColor3ub(142,141,158);
    glPushMatrix();
    glPushMatrix();
    glTranslatef(0,altura+5,0);
    glScalef(5*comp/7,10,1);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,altura+5,0);
    glScalef(1,10,5*comp/7);
    glutSolidCube(1);
    glPopMatrix();
    glPopMatrix();
}

void salaEstudos(){
    //Sala de estudos
    //Frente
    glPushMatrix();
    glColor3ub(156,149,142);
    glBegin(GL_QUADS);
        glVertex3f(TAMX/2-TAMSALAGRZ,0,TAMZ/2);
        glVertex3f(TAMX/2-TAMSALAGRZ, 0, TAMZ/2-TAMZ/8-35+TAMSALAGRX);
        glVertex3f(TAMX/2-TAMSALAGRZ, TAMY, TAMZ/2-TAMZ/8-35+TAMSALAGRX);
        glVertex3f(TAMX/2-TAMSALAGRZ, TAMY, TAMZ/2);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glColor3ub(156,149,142);
    glBegin(GL_QUADS);
        glVertex3f(TAMX/2-TAMSALAGRZ,0,TAMZ/2-TAMZ/8-55+TAMSALAGRX);
        glVertex3f(TAMX/2-TAMSALAGRZ, 0, TAMZ/2-TAMZ/8-60+TAMSALAGRX);
        glVertex3f(TAMX/2-TAMSALAGRZ, TAMY, TAMZ/2-TAMZ/8-60+TAMSALAGRX);
        glVertex3f(TAMX/2-TAMSALAGRZ, TAMY, TAMZ/2-TAMZ/8-55+TAMSALAGRX);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glColor3ub(156,149,142);
    glBegin(GL_QUADS);
        glVertex3f(TAMX/2-TAMSALAGRZ, TAMPORTAY, TAMZ/2-TAMZ/8-35+TAMSALAGRX);
        glVertex3f(TAMX/2-TAMSALAGRZ, TAMPORTAY, TAMZ/2-TAMZ/8-60+TAMSALAGRX);
        glVertex3f(TAMX/2-TAMSALAGRZ, TAMY, TAMZ/2-TAMZ/8-60+TAMSALAGRX);
        glVertex3f(TAMX/2-TAMSALAGRZ, TAMY, TAMZ/2-TAMZ/8-35+TAMSALAGRX);
    glEnd();
    glPopMatrix();


    //Porta
    glPushMatrix();
    glColor3f(0.75,0.8,0.8);
    glBegin(GL_QUADS);
        glVertex3f(TAMX/2-TAMSALAGRZ, 0, TAMZ/2-TAMZ/8-35+TAMSALAGRX);
        glVertex3f(TAMX/2-TAMSALAGRZ, 0, TAMZ/2-TAMZ/8-55+TAMSALAGRX);
        glVertex3f(TAMX/2-TAMSALAGRZ, TAMPORTAY, TAMZ/2-TAMZ/8-55+TAMSALAGRX);
        glVertex3f(TAMX/2-TAMSALAGRZ, TAMPORTAY, TAMZ/2-TAMZ/8-35+TAMSALAGRX);
    glEnd();
    glPopMatrix();

    //Maçaneta porta
    glPushMatrix();
    glColor3f(0.3,0.3,0.2);
    glTranslatef(TAMX/2-TAMSALAGRZ, TAMY/3-5, TAMZ/2-TAMZ/8-50+TAMSALAGRX);
    glutSolidSphere(1.5,20,20);
    glPopMatrix();
    
    //Fundo
    glPushMatrix();
    glColor3ub(160,153,142);
    glBegin(GL_QUADS);
        glVertex3f(TAMX/2-0.1,0,TAMZ/2);
        glVertex3f(TAMX/2-0.1, 0, TAMZ/2-TAMZ/8-60+TAMSALAGRX);
        glVertex3f(TAMX/2-0.1, TAMY/3, TAMZ/2-TAMZ/8-60+TAMSALAGRX);
        glVertex3f(TAMX/2-0.1, TAMY/3, TAMZ/2);
    glEnd();
    glPopMatrix();

    //Batentes
    glPushMatrix();
    glColor3ub(115,59,18);
    glTranslatef(TAMX/2, TAMY/3, (TAMZ/2+TAMZ/2-TAMZ/8-60+TAMSALAGRX)/2);
    glScalef(5, 2, TAMZ/2-(TAMZ/2-TAMZ/8-60+TAMSALAGRX));
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glColor3ub(115,59,18);
    glTranslatef(TAMX/2-TAMSALAGRZ/2+2, TAMY/3, TAMZ/2);
    glScalef(TAMSALAGRZ, 2, 5);
    glutSolidCube(1);
    glPopMatrix();


    //Janelas sul
    for(int i=0;i<3;i++){
        glPushMatrix();
        glTranslatef(TAMX/2-TAMSALAGRZ+2+i*(TAMSALAGRZ/3), TAMY/3+1,TAMZ/2);
        janelaFechada(TAMSALAGRZ/3,TAMY/2.5-1);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(TAMX/2-TAMSALAGRZ+2+i*(TAMSALAGRZ/3), TAMY/3 + TAMY/2.5,TAMZ/2);
        janelaFechada(TAMSALAGRZ/3,2*TAMY/3 - TAMY/2.5);
        glPopMatrix();

    }

    //Janelas leste
    for(int i=0;i<3;i++){
        if(i==2){
            glPushMatrix();
            glRotatef(90,0,1,0);
            glTranslatef(-TAMZ/2+i*(TAMSALAGRX/4), TAMY/3+1,TAMX/2);
            janelaFechada(TAMSALAGRX/4-4,TAMY/2.5);
            glPopMatrix();

            glPushMatrix();
            glRotatef(90,0,1,0);
            glTranslatef(-TAMZ/2+i*(TAMSALAGRX/4),TAMY/3 + TAMY/2.5, TAMX/2);
            janelaFechada(TAMSALAGRX/4-4,2*TAMY/3 - TAMY/2.5);
            glPopMatrix();
            
        }
        else{
            glPushMatrix();
            glRotatef(90,0,1,0);
            glTranslatef(-TAMZ/2+i*(TAMSALAGRX/4), TAMY/3+1,TAMX/2);
            janelaFechada(TAMSALAGRX/4,TAMY/2.5);
            glPopMatrix();

            glPushMatrix();
            glRotatef(90,0,1,0);
            glTranslatef(-TAMZ/2+i*(TAMSALAGRX/4),TAMY/3 + TAMY/2.5, TAMX/2);
            janelaFechada(TAMSALAGRZ/3,2*TAMY/3 - TAMY/2.5);
            glPopMatrix();
        }
    }
}

void desenhaLeste(){

    salaEstudos();

    for(int i=0; i<3;i++){
        glPushMatrix();
        glTranslatef(TAMX/2-TAMSALAGRZ -0.1,0, TAMZ/2 -TAMZ/8 -60 - i*(TAMSALAGRX +0.1));
        glRotatef(-90,0,1,0);
        desenhaSalaGrande(TAMSALAGRX,TAMSALAGRZ);
        glPopMatrix();
    }


}

void desenhaFundoDir(){
    glPushMatrix();
    glTranslatef(TAMX/2-60,0,-TAMZ/4-TAMZ/8);
    glRotatef(-90,0,1,0);
    desenhaSalaGrande(TAMZ/8,60);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(TAMX/2-60,0,-TAMZ/2+0.1);
    glRotatef(-90,0,1,0);
    desenhaBanheiro(72,60-0.1,2);
    glPopMatrix();

    glPushMatrix();
    glColor3ub(68,62,52);
    glTranslatef(TAMX/2-30,TAMY/2,-TAMZ/2+73.6);
    desenhaPilarInterno(62,TAMY,2.9);
    glPopMatrix();
}

void desenhaOeste(){
    
    glPushMatrix();
    glTranslatef(-TAMX/2+45.2,0,-TAMZ/7-0.1);
    glScalef(-1,1,1);
    desenhaBanheiro(45,62,1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-TAMX/2+49.3,TAMY/2,-TAMZ/7);
    corPilar();
    desenhaPilarInterno(8,TAMY,2);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-TAMX/2+TAMX/6-30,0,-TAMZ/7-0.1);
    desenhaBanheiro(45,62,0);
    glPopMatrix();

    //Salas Oeste
    for(int i=0;i<5;i++){
        glPushMatrix();
        glTranslatef(-TAMX/2+10.1,0,TAMZ/2-60-i*(60));
        glRotatef(90,0,1,0);
        desenhaSalaGrande(60,TAMSALAGRZ);
        glPopMatrix();
    }
    
    //Sala diferentona
    glPushMatrix();
    glTranslatef(-TAMX/2+TAMSALAGRZ+8,0,TAMZ/2-TAMSALADIFZ-1);
    glRotatef(180,0,1,0);
    salaDif(TAMSALADIFX,TAMSALADIFZ);
    glPopMatrix();

}

void portaVidro(float tam_x, float tam_y){
    glPushMatrix();
    janelaFechada(tam_x/2, tam_y);
    janelaFechada(tam_x/4+1, tam_y);
    glTranslatef(0,tam_y,0);
    janelaFechada(tam_x, TAMY-tam_y);
    janelaFechada(3*tam_x/4, TAMY-tam_y);
    janelaFechada(tam_x/2, TAMY-tam_y);
    janelaFechada(tam_x/4, TAMY-tam_y);
    glTranslatef(0,-tam_y,0);
    glTranslatef(tam_x/2+0.2,0,0);
    janelaFechada(tam_x/2, tam_y);
    janelaFechada(tam_x/4+1, tam_y);
    glPopMatrix();
}


void salaDif(float tam_x, float tam_z){

    //Chao
    glPushMatrix();
    glTranslatef(0,0.11,0);
    chaoSala(tam_x,tam_z);
    glPopMatrix();

    //Frente
    glPushMatrix();
    glColor3ub(156,149,142);
    glBegin(GL_QUADS);
        glVertex3f(0, 0, 0);
        glVertex3f(5, 0, 0);
        glVertex3f(5, TAMY, 0);
        glVertex3f(0, TAMY, 0);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(25,0,0);
    glRotatef(180,0,1,0);
    porta();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(25,0,0);
    porta();
    glPopMatrix();

    
    glPushMatrix();
    glColor3ub(156,149,142);
    glBegin(GL_QUADS);
        glVertex3f(45, 0, 0);
        glVertex3f(tam_x, 0, 0);
        glVertex3f(tam_x, TAMY, 0);
        glVertex3f(45, TAMY, 0);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glColor3ub(156,149,142);
    glBegin(GL_QUADS);
        glVertex3f(5, TAMPORTAY, 0);
        glVertex3f(tam_x, TAMPORTAY, 0);
        glVertex3f(tam_x, TAMY, 0);
        glVertex3f(5, TAMY, 0);
    glEnd();
    glPopMatrix();

    //Direita
    glPushMatrix();
    glColor3ub(156,149,142);
    glBegin(GL_QUADS);
        glVertex3f(tam_x, 0, 0);
        glVertex3f(tam_x, 0, -tam_z);
        glVertex3f(tam_x, TAMY, -tam_z);
        glVertex3f(tam_x, TAMY, 0);
    glEnd();
    glPopMatrix();

    //Esquerda
    glPushMatrix();
    glColor3ub(156,149,142);
    glBegin(GL_QUADS);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, -tam_z);
        glVertex3f(0, TAMY, -tam_z);
        glVertex3f(0, TAMY, 0);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texID[0]);
    glColor3f(0.9,0.9,0.9);
    glBegin(GL_QUADS);
        glTexCoord2f(0,1); glVertex3f(-0.1,0,-1);
        glTexCoord2f(0,0); glVertex3f(-0.1, 0, -tam_z+1);
        glTexCoord2f(1,0); glVertex3f(-0.1,TAMY,-tam_z+1);
        glTexCoord2f(1,1); glVertex3f(-0.1,TAMY,-1);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPushMatrix();
    corPilar();
    glTranslatef(0.5,TAMY/2,-0.5);
    desenhaPilarInterno(4,TAMY,4);
    glPopMatrix();
    //Fundo
    glPushMatrix();
    glColor3ub(156,149,142);
    glBegin(GL_QUADS);
        glVertex3f(0, 0, -tam_z);
        glVertex3f(tam_x, 0, -tam_z);
        glVertex3f(tam_x, 3*TAMY/4, -tam_z);
        glVertex3f(0, 3*TAMY/4, -tam_z);
    glEnd();
    glPopMatrix();

    //Janelas
    glPushMatrix();
    glTranslatef(0,3*TAMY/4,-tam_z);
    janelaFechada(tam_x/3,TAMY/4);
    glTranslatef(tam_x/3,0,0);
    janelaFechada(tam_x/3,TAMY/4);
    glTranslatef(tam_x/3,0,0);
    janelaFechada(tam_x/3-2,TAMY/4);
    glPopMatrix();
}

void desenhaMeio(){
    //Salas Meio
    for(int j=0;j<3;j++){
        desenhaSalaInterna(50,40,-TAMX/2 + TAMX/5+0.01+j*(50.01),ZJARDIM-TAMZJARDIM-95);

    }
    //desenhaSalaInterna(45,40,XJARDIM+TAMXJARDIM-70.1,ZJARDIM-TAMZJARDIM-95);
    //desenhaSalaInterna(45,40,XJARDIM+TAMXJARDIM-115.2,ZJARDIM-TAMZJARDIM-95);
    //desenhaSalaInterna(55,40,XJARDIM+TAMXJARDIM-170.3,ZJARDIM-TAMZJARDIM-95);

    //Mureta das salas internas
    glPushMatrix();
    glColor3ub(68,73,30);
    glTranslatef(-75,TAMY/3+0.5,ZJARDIM-TAMZJARDIM-136);
    glScalef(150, 1.5, 6);
    glutSolidCube(1);
    glPopMatrix();
    
    glPushMatrix();
    glColor3f(0.6,0.6,0.55);
    glTranslatef(-75,TAMY/6,ZJARDIM-TAMZJARDIM-137);
    glScalef(150, TAMY/3+0.5, 4);
    glutSolidCube(1);
    glPopMatrix();

    //Pilar colado nas salas internas
    glPushMatrix();
    glColor3ub(68,62,52);
    glTranslatef(1,TAMY/2,ZJARDIM-TAMZJARDIM-137);
    desenhaPilarInterno(3,TAMY,3.5);
    glPopMatrix();


}

void desenhaJardim(){
    //Parte verde do jardim
    glColor3ub(0,200,0);
    glPushMatrix();
    glBegin(GL_QUADS);
    glVertex3f(XJARDIM, 0.4, ZJARDIM);
    glVertex3f(XJARDIM + TAMXJARDIM, 0.4, ZJARDIM);
    glVertex3f(XJARDIM + TAMXJARDIM, 0.4, ZJARDIM - TAMZJARDIM);
    glVertex3f(XJARDIM, 0.4, ZJARDIM - TAMZJARDIM);
    glEnd();
    glPopMatrix();

    //Paredes de vidro do jardim
    //Parede frente
    desenhaVidros(XJARDIM-20, XJARDIM+TAMXJARDIM+20,ZJARDIM+30, ZJARDIM+30);

    //Parede direita
    desenhaVidros(XJARDIM+TAMXJARDIM+20, XJARDIM+TAMXJARDIM+20,ZJARDIM, ZJARDIM - TAMZJARDIM-17);

    //Porta vidro
    glPushMatrix();
    glTranslatef(XJARDIM+TAMXJARDIM+20,0,ZJARDIM+30);
    glRotatef(90,0,1,0);
    portaVidro(29,TAMPORTAY);
    glPopMatrix();

    //Parede fundo
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texID[0]);
    glColor3f(0.9,0.9,0.9);
    glBegin(GL_QUADS);
        glTexCoord2f(0,3); glVertex3f(XJARDIM+TAMXJARDIM+20,0,ZJARDIM - TAMZJARDIM-17);
        glTexCoord2f(0,0); glVertex3f(XJARDIM-20,0,ZJARDIM - TAMZJARDIM-17);
        glTexCoord2f(0.5,0); glVertex3f(XJARDIM-20,TAMY/3,ZJARDIM - TAMZJARDIM-17);
        glTexCoord2f(0.5,3); glVertex3f(XJARDIM+TAMXJARDIM+20,TAMY/3,ZJARDIM - TAMZJARDIM-17);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.6,0.6,0.55);
    glBegin(GL_QUADS);
        glVertex3f(XJARDIM+TAMXJARDIM+20,0,ZJARDIM - TAMZJARDIM-17.1);
        glVertex3f(XJARDIM-20,0,ZJARDIM - TAMZJARDIM-17.1);
        glVertex3f(XJARDIM-20,TAMY/3,ZJARDIM - TAMZJARDIM-17.1);
        glVertex3f(XJARDIM+TAMXJARDIM+20,TAMY/3,ZJARDIM - TAMZJARDIM-17.1);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.7,0.6,0.55);
    glTranslatef(XJARDIM+TAMXJARDIM/2-0.1, TAMY/3, ZJARDIM - TAMZJARDIM-15);
    glScalef(TAMXJARDIM+40,2,10);
    glutSolidCube(1);
    glPopMatrix();
    

    //Parede esquerda
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texID[0]);
    glColor3f(0.9,0.9,0.9);
    glBegin(GL_QUADS);
        glTexCoord2f(0,3); glVertex3f(XJARDIM-20,0,ZJARDIM - TAMZJARDIM-17);
        glTexCoord2f(0,0); glVertex3f(XJARDIM-20,0,ZJARDIM+30);
        glTexCoord2f(0.5,0); glVertex3f(XJARDIM-20,TAMY/3,ZJARDIM+30);
        glTexCoord2f(0.5,3); glVertex3f(XJARDIM-20,TAMY/3,ZJARDIM - TAMZJARDIM-17);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.61,0.61,0.55);
    glBegin(GL_QUADS);
        glVertex3f(XJARDIM-20.1,0,ZJARDIM - TAMZJARDIM-17);
        glVertex3f(XJARDIM-20.1,0,ZJARDIM+30);
        glVertex3f(XJARDIM-20.1,TAMY/3,ZJARDIM+30);
        glVertex3f(XJARDIM-20.1,TAMY/3,ZJARDIM - TAMZJARDIM-17);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.7,0.6,0.55);
    glTranslatef(XJARDIM-21, TAMY/3, ZJARDIM - TAMZJARDIM/2 +4.9);
    glScalef(10,2,TAMZJARDIM+50);
    glutSolidCube(1);
    glPopMatrix();



    //SALAS AO REDOR DO JARDIM
    //Salas fundo
    for(int i=0;i<2;i++){
        desenhaSala(TAMSALAINTX, TAMSALAINTZ+5, XJARDIM-20+i*(TAMSALAINTX+0.01), ZJARDIM -TAMZJARDIM - 17.1);
    }

    desenhaSala(36.5, TAMSALAINTZ+5, XJARDIM-20+2*(TAMSALAINTX+0.01), ZJARDIM -TAMZJARDIM - 17.1);
    
    
    
    //Salas esquerda
    glPushMatrix();
    glRotatef(90,0,1,0);
    for(int i=0;i<5;i++){
        desenhaSala(34,TAMSALAINTZ+5, -ZJARDIM -30+i*(34+0.01), XJARDIM-20);
    }
    glPopMatrix();
    
    //Pilar Ponta
    glPushMatrix();
    glColor3ub(68,62,52);
    glTranslatef(XJARDIM-24,TAMY/2,ZJARDIM-TAMZJARDIM-18);
    desenhaPilarInterno(7,TAMY,5);
    glPopMatrix();


    

}

void desenhaSala(float tam_x_sala, float tam_z_sala, float pos_x_sala, float pos_z_sala){

    //Parede direita
    glPushMatrix();
    glColor3f(0.8,0.75,0.75);
    glBegin(GL_QUADS);
        glVertex3f(pos_x_sala + tam_x_sala, 0, pos_z_sala);
        glVertex3f(pos_x_sala + tam_x_sala, 0, pos_z_sala - tam_z_sala);
        glVertex3f(pos_x_sala + tam_x_sala, TAMY, pos_z_sala - tam_z_sala);
        glVertex3f(pos_x_sala + tam_x_sala, TAMY, pos_z_sala);
    glEnd();
    glPopMatrix();

    //Parede esquerda
    glPushMatrix();
    glColor3f(0.7,0.75,0.75);
    glBegin(GL_QUADS);
        glVertex3f(pos_x_sala, 0, pos_z_sala - tam_z_sala);
        glVertex3f(pos_x_sala, 0, pos_z_sala);
        glVertex3f(pos_x_sala, TAMY, pos_z_sala);
        glVertex3f(pos_x_sala, TAMY, pos_z_sala - tam_z_sala);
    glEnd();
    glPopMatrix();

    //Parede frente
    glPushMatrix();
    glColor3f(0.75,0.75,0.75);
    glBegin(GL_QUADS);
        glVertex3f(pos_x_sala + tam_x_sala -20, 0, pos_z_sala - tam_z_sala);
        glVertex3f(pos_x_sala, 0, pos_z_sala - tam_z_sala);
        glVertex3f(pos_x_sala, TAMY/3, pos_z_sala - tam_z_sala);
        glVertex3f(pos_x_sala + tam_x_sala -20, TAMY/3, pos_z_sala - tam_z_sala);
    glEnd();
    glPopMatrix();

    //Porta
    glPushMatrix();
    glColor3f(0.75,0.8,0.8);
    glBegin(GL_QUADS);
        glVertex3f(pos_x_sala + tam_x_sala -1, 0, pos_z_sala - tam_z_sala);
        glVertex3f(pos_x_sala +tam_x_sala -19, 0, pos_z_sala - tam_z_sala);
        glVertex3f(pos_x_sala +tam_x_sala -19, TAMY/2 +9, pos_z_sala - tam_z_sala);
        glVertex3f(pos_x_sala + tam_x_sala -1, TAMY/2 +9, pos_z_sala - tam_z_sala);
    glEnd();
    glPopMatrix();

    //Maçaneta porta
    glPushMatrix();
    glColor3f(0.3,0.3,0.2);
    glTranslatef(pos_x_sala + tam_x_sala -16, TAMY/3-5, pos_z_sala - tam_z_sala +0.5);
    glutSolidSphere(1.5,20,20);
    glPopMatrix();

    //Parede fundo
    glPushMatrix();
    glColor3f(0.75,0.75,0.75);
    glBegin(GL_QUADS);
        glVertex3f(pos_x_sala + tam_x_sala, TAMY/2+10, pos_z_sala - tam_z_sala);
        glVertex3f(pos_x_sala + tam_x_sala -20, TAMY/2+10, pos_z_sala - tam_z_sala);
        glVertex3f(pos_x_sala + tam_x_sala -20, TAMY, pos_z_sala - tam_z_sala);
        glVertex3f(pos_x_sala + tam_x_sala, TAMY, pos_z_sala - tam_z_sala);
    glEnd();
    glPopMatrix();

    //Detalhes preto porta
    glPushMatrix();
    glColor3f(0.2,0.2,0.2);
    glBegin(GL_QUADS);
        glVertex3f(pos_x_sala + tam_x_sala, 0, pos_z_sala - tam_z_sala);
        glVertex3f(pos_x_sala + tam_x_sala -1, 0, pos_z_sala - tam_z_sala);
        glVertex3f(pos_x_sala + tam_x_sala -1, TAMY/2+10, pos_z_sala - tam_z_sala);
        glVertex3f(pos_x_sala + tam_x_sala, TAMY/2+10, pos_z_sala - tam_z_sala);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.2,0.2,0.2);
    glBegin(GL_QUADS);
        glVertex3f(pos_x_sala + tam_x_sala, TAMY/2+9, pos_z_sala - tam_z_sala);
        glVertex3f(pos_x_sala + tam_x_sala -20, TAMY/2+9, pos_z_sala - tam_z_sala);
        glVertex3f(pos_x_sala + tam_x_sala -20, TAMY/2+10, pos_z_sala - tam_z_sala);
        glVertex3f(pos_x_sala + tam_x_sala, TAMY/2+10, pos_z_sala - tam_z_sala);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.2,0.2,0.2);
    glBegin(GL_QUADS);
        glVertex3f(pos_x_sala + tam_x_sala -19, 0, pos_z_sala - tam_z_sala);
        glVertex3f(pos_x_sala + tam_x_sala -20, 0, pos_z_sala - tam_z_sala);
        glVertex3f(pos_x_sala + tam_x_sala -20, TAMY/2+10, pos_z_sala - tam_z_sala);
        glVertex3f(pos_x_sala + tam_x_sala -19, TAMY/2+10, pos_z_sala - tam_z_sala);
    glEnd();
    glPopMatrix();

    //Detalhes preto janela frente
    glPushMatrix();
    glColor3f(0.2,0.2,0.2);
    glBegin(GL_QUADS);
        glVertex3f(pos_x_sala + tam_x_sala-20, TAMY/3, pos_z_sala - tam_z_sala);
        glVertex3f(pos_x_sala + tam_x_sala -21, TAMY/3, pos_z_sala - tam_z_sala);
        glVertex3f(pos_x_sala + tam_x_sala -21, TAMY, pos_z_sala - tam_z_sala);
        glVertex3f(pos_x_sala + tam_x_sala-20, TAMY, pos_z_sala - tam_z_sala);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.2,0.2,0.2);
    glBegin(GL_QUADS);
        glVertex3f(pos_x_sala + (tam_x_sala-20)/2, 2*TAMY/3, pos_z_sala - tam_z_sala);
        glVertex3f(pos_x_sala + (tam_x_sala -20)/2 -1, 2*TAMY/3, pos_z_sala - tam_z_sala);
        glVertex3f(pos_x_sala + (tam_x_sala -20)/2 -1, TAMY, pos_z_sala - tam_z_sala);
        glVertex3f(pos_x_sala + (tam_x_sala-20)/2, TAMY, pos_z_sala - tam_z_sala);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.2,0.2,0.2);
    glBegin(GL_QUADS);
        glVertex3f(pos_x_sala + tam_x_sala -20, TAMY/3, pos_z_sala - tam_z_sala);
        glVertex3f(pos_x_sala, TAMY/3, pos_z_sala - tam_z_sala);
        glVertex3f(pos_x_sala, TAMY/3 +1, pos_z_sala - tam_z_sala);
        glVertex3f(pos_x_sala + tam_x_sala -20, TAMY/3+1, pos_z_sala - tam_z_sala);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.2,0.2,0.2);
    glBegin(GL_QUADS);
        glVertex3f(pos_x_sala + tam_x_sala -20, TAMY-1, pos_z_sala - tam_z_sala);
        glVertex3f(pos_x_sala, TAMY-1, pos_z_sala - tam_z_sala);
        glVertex3f(pos_x_sala, TAMY, pos_z_sala - tam_z_sala);
        glVertex3f(pos_x_sala + tam_x_sala -20, TAMY, pos_z_sala - tam_z_sala);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.2,0.2,0.2);
    glBegin(GL_QUADS);
        glVertex3f(pos_x_sala + tam_x_sala -20, 2*TAMY/3-1, pos_z_sala - tam_z_sala);
        glVertex3f(pos_x_sala, 2*TAMY/3-1, pos_z_sala - tam_z_sala);
        glVertex3f(pos_x_sala, 2*TAMY/3+1, pos_z_sala - tam_z_sala);
        glVertex3f(pos_x_sala + tam_x_sala -20, 2*TAMY/3+1, pos_z_sala - tam_z_sala);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.2,0.2,0.2);
    glBegin(GL_QUADS);
        glVertex3f(pos_x_sala +1, TAMY/3, pos_z_sala - tam_z_sala);
        glVertex3f(pos_x_sala, TAMY/3, pos_z_sala - tam_z_sala);
        glVertex3f(pos_x_sala, TAMY, pos_z_sala - tam_z_sala);
        glVertex3f(pos_x_sala +1, TAMY, pos_z_sala - tam_z_sala);
    glEnd();
    glPopMatrix();
    
    //Detalhes preto janela atrás
    glPushMatrix();
    glColor3f(0.2,0.2,0.2);
    glBegin(GL_QUADS);
        glVertex3f(pos_x_sala + tam_x_sala, TAMY/3, pos_z_sala);
        glVertex3f(pos_x_sala + tam_x_sala -1, TAMY/3, pos_z_sala);
        glVertex3f(pos_x_sala + tam_x_sala -1, TAMY, pos_z_sala);
        glVertex3f(pos_x_sala + tam_x_sala, TAMY, pos_z_sala);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.2,0.2,0.2);
    glBegin(GL_QUADS);
        glVertex3f(pos_x_sala-0.2, TAMY/3, pos_z_sala);
        glVertex3f(pos_x_sala+1, TAMY/3, pos_z_sala);
        glVertex3f(pos_x_sala+1, TAMY, pos_z_sala);
        glVertex3f(pos_x_sala-0.2, TAMY, pos_z_sala);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.2,0.2,0.2);
    glBegin(GL_QUADS);
        glVertex3f(pos_x_sala+tam_x_sala/2-1, TAMY/3, pos_z_sala);
        glVertex3f(pos_x_sala+tam_x_sala/2+1, TAMY/3, pos_z_sala);
        glVertex3f(pos_x_sala+tam_x_sala/2+1, 3*TAMY/4, pos_z_sala);
        glVertex3f(pos_x_sala+tam_x_sala/2-1, 3*TAMY/4, pos_z_sala);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.2,0.2,0.2);
    glBegin(GL_QUADS);
        glVertex3f(pos_x_sala, TAMY-2, pos_z_sala);
        glVertex3f(pos_x_sala + tam_x_sala, TAMY-2, pos_z_sala);
        glVertex3f(pos_x_sala + tam_x_sala, TAMY, pos_z_sala);
        glVertex3f(pos_x_sala, TAMY, pos_z_sala);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.2,0.2,0.2);
    glBegin(GL_QUADS);
        glVertex3f(pos_x_sala, 3*TAMY/4, pos_z_sala);
        glVertex3f(pos_x_sala + tam_x_sala, 3*TAMY/4, pos_z_sala);
        glVertex3f(pos_x_sala + tam_x_sala, 3*TAMY/4 +3, pos_z_sala);
        glVertex3f(pos_x_sala, 3*TAMY/4 +3, pos_z_sala);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.2,0.2,0.2);
    glBegin(GL_QUADS);
        glVertex3f(pos_x_sala +tam_x_sala/2+1, TAMY/3, pos_z_sala);
        glVertex3f(pos_x_sala + tam_x_sala-1, TAMY/3, pos_z_sala);
        glVertex3f(pos_x_sala + tam_x_sala-1, TAMY/3 +3, pos_z_sala);
        glVertex3f(pos_x_sala +tam_x_sala/2+1, TAMY/3 +3, pos_z_sala);
    glEnd();
    glPopMatrix();



    //Janelas para fora
    glPushMatrix();
    glColor3f(0.25,0.25,0.25);
    glBegin(GL_QUADS);
        glVertex3f(pos_x_sala+tam_x_sala/2-1, TAMY/3+5, pos_z_sala+10);
        glVertex3f(pos_x_sala+tam_x_sala/2-3, TAMY/3+5, pos_z_sala+10);
        glVertex3f(pos_x_sala+tam_x_sala/2-3, 3*TAMY/4, pos_z_sala);
        glVertex3f(pos_x_sala+tam_x_sala/2-1, 3*TAMY/4, pos_z_sala);
    glEnd();
    glPopMatrix();
    
    glPushMatrix();
    glColor3f(0.25,0.25,0.25);
    glBegin(GL_QUADS);
        glVertex3f(pos_x_sala+1, TAMY/3+5, pos_z_sala+10);
        glVertex3f(pos_x_sala+3, TAMY/3+5, pos_z_sala+10);
        glVertex3f(pos_x_sala+3, 3*TAMY/4, pos_z_sala);
        glVertex3f(pos_x_sala+1, 3*TAMY/4, pos_z_sala);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.25,0.25,0.25);
    glBegin(GL_QUADS);
        glVertex3f(pos_x_sala+3, TAMY/3+5, pos_z_sala+10);
        glVertex3f(pos_x_sala+tam_x_sala/2-3, TAMY/3+5, pos_z_sala+10);
        glVertex3f(pos_x_sala+tam_x_sala/2-3, TAMY/3+7.4, pos_z_sala+9);
        glVertex3f(pos_x_sala+3, TAMY/3+7.4, pos_z_sala+9);
    glEnd();
    glPopMatrix();

    //Mesa e cadeira dentro da sala
    glPushMatrix();
    glColor3ub(139,69,19);
    glTranslatef(pos_x_sala + tam_x_sala/2,0,pos_z_sala - tam_z_sala/2);
    desenhaMesaRed(25,TAMMESAY-1);
    glColor3ub(150,75,30);
    glTranslatef(5,0,16);
    glRotatef(180,0,1,0);
    desenhaCadeira(13,TAMMESAY+5,10);
    glPopMatrix();

    
    

}

void desenhaSalaInterna(float tam_x_sala, float tam_z_sala, float pos_x_sala, float pos_z_sala){
    //Parede direita
    glPushMatrix();
    glColor3f(0.8,0.75,0.75);
    glBegin(GL_QUADS);
        glVertex3f(pos_x_sala + tam_x_sala, 0, pos_z_sala);
        glVertex3f(pos_x_sala + tam_x_sala, 0, pos_z_sala - tam_z_sala);
        glVertex3f(pos_x_sala + tam_x_sala, TAMY, pos_z_sala - tam_z_sala);
        glVertex3f(pos_x_sala + tam_x_sala, TAMY, pos_z_sala);
    glEnd();
    glPopMatrix();

    //Parede esquerda
    glPushMatrix();
    glColor3f(0.7,0.75,0.75);
    glBegin(GL_QUADS);
    glVertex3f(pos_x_sala, 0, pos_z_sala);
        glVertex3f(pos_x_sala, 0, pos_z_sala - tam_z_sala);
        glVertex3f(pos_x_sala, TAMY, pos_z_sala - tam_z_sala);
        glVertex3f(pos_x_sala, TAMY, pos_z_sala);
    glEnd();
    glPopMatrix();

    //Parede fundo
    glPushMatrix();
    glColor3f(0.6,0.6,0.55);
    glBegin(GL_QUADS);
        glVertex3f(pos_x_sala,0,pos_z_sala - tam_z_sala);
        glVertex3f(pos_x_sala+tam_x_sala,0,pos_z_sala - tam_z_sala);
        glVertex3f(pos_x_sala+tam_x_sala,TAMY/3,pos_z_sala - tam_z_sala);
        glVertex3f(pos_x_sala,TAMY/3,pos_z_sala - tam_z_sala);
    glEnd();
    glPopMatrix();

    //Parede frente
    glPushMatrix();
    glColor3f(0.75,0.75,0.75);
    glBegin(GL_QUADS);
        glVertex3f(pos_x_sala + tam_x_sala -20, 0, pos_z_sala);
        glVertex3f(pos_x_sala, 0, pos_z_sala);
        glVertex3f(pos_x_sala, TAMY/3, pos_z_sala);
        glVertex3f(pos_x_sala + tam_x_sala -20, TAMY/3, pos_z_sala);
    glEnd();
    glPopMatrix();

    //Porta
    glPushMatrix();
    glColor3f(0.75,0.8,0.8);
    glBegin(GL_QUADS);
        glVertex3f(pos_x_sala + tam_x_sala -1, 0, pos_z_sala);
        glVertex3f(pos_x_sala +tam_x_sala -19, 0, pos_z_sala);
        glVertex3f(pos_x_sala +tam_x_sala -19, TAMY/2 +9, pos_z_sala);
        glVertex3f(pos_x_sala + tam_x_sala -1, TAMY/2 +9, pos_z_sala);
    glEnd();
    glPopMatrix();

    //Maçaneta porta
    glPushMatrix();
    glColor3f(0.3,0.3,0.2);
    glTranslatef(pos_x_sala + tam_x_sala -16, TAMY/3-5, pos_z_sala +0.5);
    glutSolidSphere(1.5,20,20);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.75,0.75,0.75);
    glBegin(GL_QUADS);
        glVertex3f(pos_x_sala + tam_x_sala, TAMY/2+10, pos_z_sala);
        glVertex3f(pos_x_sala + tam_x_sala -20, TAMY/2+10, pos_z_sala);
        glVertex3f(pos_x_sala + tam_x_sala -20, TAMY, pos_z_sala);
        glVertex3f(pos_x_sala + tam_x_sala, TAMY, pos_z_sala);
    glEnd();
    glPopMatrix();

    //Detalhes preto porta
    glPushMatrix();
    glColor3f(0.2,0.2,0.2);
    glBegin(GL_QUADS);
        glVertex3f(pos_x_sala + tam_x_sala, 0, pos_z_sala);
        glVertex3f(pos_x_sala + tam_x_sala -1, 0, pos_z_sala);
        glVertex3f(pos_x_sala + tam_x_sala -1, TAMY/2+10, pos_z_sala);
        glVertex3f(pos_x_sala + tam_x_sala, TAMY/2+10, pos_z_sala);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.2,0.2,0.2);
    glBegin(GL_QUADS);
        glVertex3f(pos_x_sala + tam_x_sala, TAMY/2+9, pos_z_sala);
        glVertex3f(pos_x_sala + tam_x_sala -20, TAMY/2+9, pos_z_sala);
        glVertex3f(pos_x_sala + tam_x_sala -20, TAMY/2+10, pos_z_sala);
        glVertex3f(pos_x_sala + tam_x_sala, TAMY/2+10, pos_z_sala);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.2,0.2,0.2);
    glBegin(GL_QUADS);
        glVertex3f(pos_x_sala + tam_x_sala -19, 0, pos_z_sala);
        glVertex3f(pos_x_sala + tam_x_sala -20, 0, pos_z_sala);
        glVertex3f(pos_x_sala + tam_x_sala -20, TAMY/2+10, pos_z_sala);
        glVertex3f(pos_x_sala + tam_x_sala -19, TAMY/2+10, pos_z_sala);
    glEnd();
    glPopMatrix();

    //Detalhes preto janela frente
    glPushMatrix();
    glColor3f(0.2,0.2,0.2);
    glBegin(GL_QUADS);
        glVertex3f(pos_x_sala + tam_x_sala-20, TAMY/3, pos_z_sala);
        glVertex3f(pos_x_sala + tam_x_sala -21, TAMY/3, pos_z_sala);
        glVertex3f(pos_x_sala + tam_x_sala -21, TAMY, pos_z_sala);
        glVertex3f(pos_x_sala + tam_x_sala-20, TAMY, pos_z_sala);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.2,0.2,0.2);
    glBegin(GL_QUADS);
        glVertex3f(pos_x_sala + (tam_x_sala-20)/2, 2*TAMY/3, pos_z_sala);
        glVertex3f(pos_x_sala + (tam_x_sala -20)/2 -1, 2*TAMY/3, pos_z_sala);
        glVertex3f(pos_x_sala + (tam_x_sala -20)/2 -1, TAMY, pos_z_sala);
        glVertex3f(pos_x_sala + (tam_x_sala-20)/2, TAMY, pos_z_sala);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.2,0.2,0.2);
    glBegin(GL_QUADS);
        glVertex3f(pos_x_sala + tam_x_sala -20, TAMY/3, pos_z_sala);
        glVertex3f(pos_x_sala, TAMY/3, pos_z_sala);
        glVertex3f(pos_x_sala, TAMY/3 +1, pos_z_sala);
        glVertex3f(pos_x_sala + tam_x_sala -20, TAMY/3+1, pos_z_sala);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.2,0.2,0.2);
    glBegin(GL_QUADS);
        glVertex3f(pos_x_sala + tam_x_sala -20, TAMY-1, pos_z_sala);
        glVertex3f(pos_x_sala, TAMY-1, pos_z_sala);
        glVertex3f(pos_x_sala, TAMY, pos_z_sala);
        glVertex3f(pos_x_sala + tam_x_sala -20, TAMY, pos_z_sala);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.2,0.2,0.2);
    glBegin(GL_QUADS);
        glVertex3f(pos_x_sala + tam_x_sala -20, 2*TAMY/3-1, pos_z_sala);
        glVertex3f(pos_x_sala, 2*TAMY/3-1, pos_z_sala);
        glVertex3f(pos_x_sala, 2*TAMY/3+1, pos_z_sala);
        glVertex3f(pos_x_sala + tam_x_sala -20, 2*TAMY/3+1, pos_z_sala);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.2,0.2,0.2);
    glBegin(GL_QUADS);
        glVertex3f(pos_x_sala +1, TAMY/3, pos_z_sala);
        glVertex3f(pos_x_sala, TAMY/3, pos_z_sala);
        glVertex3f(pos_x_sala, TAMY, pos_z_sala);
        glVertex3f(pos_x_sala +1, TAMY, pos_z_sala);
    glEnd();
    glPopMatrix();

    //Janelas fundo
      //duas de baixo
    glPushMatrix();
    glTranslatef(pos_x_sala,TAMY/3+1,pos_z_sala-tam_z_sala);
    janelaFechada(tam_x_sala/2, 2*TAMY/5);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(pos_x_sala+tam_x_sala/2,TAMY/3+1,pos_z_sala-tam_z_sala);
    janelaFechada(tam_x_sala/2, 2*TAMY/5);
    glPopMatrix();

      //duas de cima
    glPushMatrix();
    glTranslatef(pos_x_sala,11*TAMY/15 +1,pos_z_sala-tam_z_sala);
    janelaFechada(tam_x_sala/2, 4*TAMY/15);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(pos_x_sala+tam_x_sala/2,11*TAMY/15 +1,pos_z_sala-tam_z_sala);
    janelaFechada(tam_x_sala/2, 4*TAMY/15);
    glPopMatrix();

    //Mesa dentro da sala
    glPushMatrix();
    glColor3ub(139,69,19);
    glTranslatef(pos_x_sala + tam_x_sala/2,0,pos_z_sala - tam_z_sala/2);
    desenhaMesaRed(25,TAMMESAY-1);
    glColor3ub(150,75,30);
    glTranslatef(-5,0,-16);
    desenhaCadeira(13,TAMMESAY+5,10);
    glTranslatef(5,0,16);
    glTranslatef(5,0,16);
    glRotatef(180,0,1,0);
    desenhaCadeira(13,TAMMESAY+5,10);
    glPopMatrix();
    
}

void mesaDif(float tam_x, float tam_y, float tam_z){
    glPushMatrix();
    glTranslatef(0.25,tam_y/2,0.25);
    glScalef(0.5,tam_y,0.5);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(tam_x-0.25,tam_y/2,0.25);
    glScalef(0.5,tam_y,0.5);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(tam_x-0.25,tam_y/2,tam_z-0.25);
    glScalef(0.5,tam_y,0.5);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.25,tam_y/2,tam_z-0.25);
    glScalef(0.5,tam_y,0.5);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(tam_x/2,tam_y,tam_z/2);
    glScalef(tam_x,1,tam_z);
    glutSolidCube(1);
    glPopMatrix();
}

void corPretoAzul(){
    glColor3ub(47,49,59);
}

void desenhaSofa(){
    float tamx=10,tamy=28,tamz=60;
    corPretoAzul();
    glPushMatrix();
    glTranslatef(0,tamy/2,0);
    desenhaPilarInterno(2.5,tamy,tamz);
    glTranslatef(tamx/2,-tamy/4,0);
    desenhaPilarInterno(tamx,tamy/2,tamz);
    glPopMatrix();

    //Almofadas
    glPushMatrix();
    glColor3ub(112,64,61);
    glTranslatef(tamx/2,tamy/2,0);
    desenhaPilarInterno(3*tamx/4,2,tamz-4);
    glPopMatrix();
}

void desenhaObjetos(){

    //Sofá esquerda
    glPushMatrix();
    glTranslatef(-TAMX/8-2,0,TAMZ/2-34);
    glRotatef(180,0,1,0);
    desenhaSofa();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-TAMX/8-35,0,TAMZ/2-34);
    desenhaSofa();
    glRotatef(180,0,1,0);
    desenhaSofa();
    glPopMatrix();

    //Mesas esquerda
    glPushMatrix();
    glColor3ub(23,22,18);
    glTranslatef(-TAMX/8 -80,0,TAMZ/2-70);
    mesaDif(12,TAMMESAY+3,45);
    glPopMatrix();

    //Prateleiras entrada
    for(int i=0;i<4;i++){
        glPushMatrix();
        glTranslatef(85,0,30 + i*(30));
        desenhaPrateleira(42, 4*TAMY/5, 10);
        glPopMatrix();
    
        glPushMatrix();
        glTranslatef(129,0,30 + i*(30));
        desenhaPrateleira(42, 4*TAMY/5, 10);
        glPopMatrix();
    }


    //Prateleiras meio
    for(int i=0;i<5;i++){
        glPushMatrix();
        glTranslatef(100,0,-TAMZ/4+20+i*(30));
        desenhaPrateleira(45, 4*TAMY/5, 10);
        glPopMatrix();
    }

    //Mesas meio
    glPushMatrix();
    glTranslatef(XJARDIM+TAMXJARDIM+45,0,ZJARDIM-TAMZJARDIM/2+30);
    for(int j=0;j<3;j++){
        if(j==1){
            glRotatef(45,0,1,0);
            desenhaMesaRed(30,18);
            glRotatef(-45,0,1,0);
            glTranslatef(0,0,-60);
        }
        else{
            desenhaMesaRed(30,18);
            glTranslatef(0,0,-60);
        }
    }
    glPopMatrix();

    //Prateleiras fundo
    for(int i=0;i<13;i++){
        glPushMatrix();
        glTranslatef(-TAMX/2 + 50 + i*(30), 0,-TAMZ/2 + 110);
        glRotatef(90,0,1,0);
        desenhaPrateleira(36, 4*TAMY/5, 10);
        glPopMatrix();
    
        glPushMatrix();
        glTranslatef(-TAMX/2 + 50 + i*(30), 0,-TAMZ/2 + 73);
        glRotatef(90,0,1,0);
        desenhaPrateleira(36, 4*TAMY/5, 10);
        glPopMatrix();
    }

    //Mesas meio/fundo
    glPushMatrix();
    glTranslatef(-10,0,-155);
    for(int j=0;j<3;j++){
        if(j==1){
            glRotatef(45,0,1,0);
            desenhaMesaRed2(30,18);
            glRotatef(-45,0,1,0);
            glTranslatef(-50,0,0);
        }
        else{
            desenhaMesaRed2(30,18);
            glTranslatef(-50,0,0);
        }
    }
    glPopMatrix();

    //Mesas fundo
    glPushMatrix();
    glTranslatef(-10,0,-155);
    for(int j=0;j<3;j++){
        if(j==1){
            glRotatef(45,0,1,0);
            desenhaMesaRed2(30,18);
            glRotatef(-45,0,1,0);
            glTranslatef(-50,0,0);
        }
        else{
            desenhaMesaRed2(30,18);
            glTranslatef(-50,0,0);
        }
    }
    glPopMatrix();
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

    desenhaPredio();
    desenhaObjetos();

}

void corPreto(){
    glColor3ub(15,11,10);
}

void corMadeira(){
    glColor3ub(130,86,27);
}

void desenhaMesinhas(){
    float tamy=20.5;
    glPushMatrix();
    corPreto();
    glTranslatef(0,tamy/2,0);
    desenhaPilarInterno(0.5,tamy,0.5);
    glTranslatef(0,-tamy/2+0.5,0);
    desenhaPilarInterno(15,1,0.5);
    desenhaPilarInterno(0.5,1,15);
    corMadeira();
    glTranslatef(0,tamy,0);
    desenhaPilarInterno(20,1,20);
    glPopMatrix();
}

void desenhaPuff(){
    glPushMatrix();
    float tamx=10,tamy=13,tamz=10;
    glColor3ub(47,42,61);
    glTranslatef(0,tamy/2,0);
    glScalef(tamx,tamy,tamz);
    glutSolidCube(1);
    glPopMatrix();
}

void desenhaEntrada(){
    glPushMatrix();
    glColor3f(0.8,0.8,0.8);
    glBegin(GL_QUADS);
        glVertex3f(XPORTA, 0, TAMZ/2 - TAMZ/12);
        glVertex3f(XPORTA, TAMY, TAMZ/2 - TAMZ/12);
        glVertex3f(-TAMX/8, TAMY, TAMZ/2 - TAMZ/12);
        glVertex3f(-TAMX/8, 0, TAMZ/2 - TAMZ/12);
    glEnd();
    glPopMatrix();
    
    glPushMatrix();
    glColor3ub(57,62,71);
    glBegin(GL_QUADS);
        glVertex3f(XPORTA, 0, TAMZ/2 - TAMZ/12 - 15);
        glVertex3f(XPORTA, TAMY, TAMZ/2 - TAMZ/12 - 15);
        glVertex3f(-TAMX/8, TAMY, TAMZ/2 - TAMZ/12 - 15);
        glVertex3f(-TAMX/8, 0, TAMZ/2 - TAMZ/12 - 15);
    glEnd();
    glPopMatrix();

    //Detalhes da parede da entrada dentro
    
    glPushMatrix();
    corPilar();
    glTranslatef(XPORTA-TAMX/8+6, 3*TAMY/4,TAMZ/2 - TAMZ/12 - 15);
    for(int j=0;j<4;j++){
        desenhaPilarInterno(114,3,2);
        glTranslatef(0,-10,0);
    }
    glPopMatrix();

    //Mesinhas e puffs
    glPushMatrix();
    glTranslatef(XPORTA-10,0,TAMZ/2-TAMZ/12-20);
    for(int j=0;j<5;j++){
        desenhaMesinhas();
        glTranslatef(0,0,-12);
        desenhaPuff();
        glTranslatef(0,0,12);
        glTranslatef(-21,0,0);
    }
    glPopMatrix();


    //---------------//
    glPushMatrix();
    glColor3f(0.8,0.8,0.8);
    glBegin(GL_QUADS);
        glVertex3f(XPORTA, 0, TAMZ/2 - TAMZ/12 - 15);
        glVertex3f(XPORTA, TAMY, TAMZ/2 - TAMZ/12 - 15);
        glVertex3f(XPORTA, TAMY, TAMZ/2 - TAMZ/12);
        glVertex3f(XPORTA, 0, TAMZ/2 - TAMZ/12);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.8,0.8,0.8);
    glBegin(GL_QUADS);
        glVertex3f(-TAMX/8, 0, TAMZ/2 - TAMZ/12 - 15);
        glVertex3f(-TAMX/8, TAMY, TAMZ/2 - TAMZ/12 - 15);
        glVertex3f(-TAMX/8, TAMY, TAMZ/2);
        glVertex3f(-TAMX/8, 0, TAMZ/2);
    glEnd();
    glPopMatrix();
    
    glPushMatrix();
    corPilar();
    glTranslatef(-TAMX/8,TAMY/2,TAMZ/2);
    desenhaPilarInterno(3,TAMY,3);
    glPopMatrix();

    glPushMatrix();
    corPilar();
    glTranslatef(-TAMX/8,TAMY/2,TAMZ/2-TAMZ/12-15);
    desenhaPilarInterno(3,TAMY,3);
    glPopMatrix();

    glPushMatrix();
    corPilar();
    glTranslatef(-TAMX/8+1.6,TAMY/2,TAMZ/2-TAMZ/12);
    desenhaPilarInterno(3,TAMY,3);
    glPopMatrix();

    glPushMatrix();
    corPilar();
    glTranslatef(XPORTA,TAMY/2,TAMZ/2-TAMZ/12-7.5);
    desenhaPilarInterno(3,TAMY,14.99);
    glPopMatrix();

    //Banheiros entrada
    glPushMatrix();
    glTranslatef(XPORTA-0.1,0,TAMZ/2);
    glRotatef(180,0,1,0);
    desenhaBanheiro(53,74,1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-TAMX/8+0.1,0,TAMZ/2);
    glScalef(1,1,-1);
    desenhaBanheiro(60,74,0);
    glPopMatrix();

    //Mesa entrada
    desenhaMesaEntrada();

    //Porta entrada
    glPushMatrix();
    glTranslatef(XPORTA,0,TAMZ/2);
    portaVidro(TAMPORTAX,TAMPORTAY);
    glPopMatrix();

    glPushMatrix();
    corPilar();
    glTranslatef(XPORTA+TAMPORTAX+1,TAMY/2,TAMZ/2-1);
    desenhaPilarInterno(3,TAMY,4);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(XPORTA+TAMPORTAX+2,0,TAMZ/2);
    portaVidro(TAMPORTAX,TAMPORTAY);
    glPopMatrix();

    glPushMatrix();
    corPilar();
    glTranslatef(XPORTA + 2*TAMPORTAX+3,TAMY/2,TAMZ/2);
    desenhaPilarInterno(4, TAMY, 4);
    glPopMatrix();
}

void desenhaBanheiro(float tam_x, float tam_z, int flag){
    //Parede esquerda
    glPushMatrix();
    glColor3f(0.82,0.82,0.82);
    glBegin(GL_QUADS);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, -tam_z);
        glVertex3f(0, TAMY, -tam_z);
        glVertex3f(0, TAMY, 0);
    glEnd();
    glPopMatrix();
    //Parede direita
    glPushMatrix();
    glColor3f(0.75,0.75,0.75);
    glBegin(GL_QUADS);
        glVertex3f(tam_x, 0, 0);
        glVertex3f(tam_x, 0, -tam_z);
        glVertex3f(tam_x, TAMY, -tam_z);
        glVertex3f(tam_x, TAMY, 0);
    glEnd();
    glPopMatrix();

    //Fundo
    glPushMatrix();
    glColor3f(0.79,0.79,0.79);
    glBegin(GL_QUADS);
        glVertex3f(0, 0, -tam_z);
        glVertex3f(tam_x, 0, -tam_z);
        glVertex3f(tam_x, TAMY, -tam_z);
        glVertex3f(0, TAMY, -tam_z);
    glEnd();
    glPopMatrix();

    //Frente
    glPushMatrix();
    glColor3f(0.8,0.8,0.8);
    glBegin(GL_QUADS);
        glVertex3f(0, 0, 0);
        glVertex3f(5, 0, 0);
        glVertex3f(5, TAMY, 0);
        glVertex3f(0, TAMY, 0);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.8,0.8,0.8);
    glBegin(GL_QUADS);
        glVertex3f(5, TAMPORTAY, 0);
        glVertex3f(25, TAMPORTAY, 0);
        glVertex3f(25, TAMY, 0);
        glVertex3f(5, TAMY, 0);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.8,0.8,0.8);
    glBegin(GL_QUADS);
        glVertex3f(25, 0, 0);
        glVertex3f(tam_x, 0, 0);
        glVertex3f(tam_x, TAMY, 0);
        glVertex3f(25, TAMY, 0);
    glEnd();
    glPopMatrix();

    //Chão
    glPushMatrix();
    glColor3f(0.84,0.84,0.84);
    glBegin(GL_QUADS);
        glVertex3f(0, 0.1,0);
        glVertex3f(tam_x, 0.1, 0);
        glVertex3f(tam_x, 0.1, -tam_z);
        glVertex3f(0, 0.1, -tam_z);
    glEnd();
    glPopMatrix();

    //Porta
    glPushMatrix();
    glTranslatef(5,0,0);
    porta();
    glPopMatrix();

    if(flag==0){
        glPushMatrix();
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texID[2]);
        glColor3f(1,1,1);
        glBegin(GL_QUADS);
            glTexCoord2f(0.5,0); glVertex3f(12.5,TAMPORTAY-13,0.2);
            glTexCoord2f(0,0); glVertex3f(17.5, TAMPORTAY-13,0.2);
            glTexCoord2f(0,1); glVertex3f(17.5,TAMPORTAY-6,0.2);
            glTexCoord2f(0.5,1); glVertex3f(12.5,TAMPORTAY-6,0.2);
        glEnd();
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();
    }
    else{
        if(flag==1){
            glPushMatrix();
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, texID[2]);
            glColor3f(1,1,1);
            glBegin(GL_QUADS);
                glTexCoord2f(0.5,0); glVertex3f(12.5,TAMPORTAY-13,0.2);
                glTexCoord2f(1,0); glVertex3f(17.5, TAMPORTAY-13,0.2);
                glTexCoord2f(1,1); glVertex3f(17.5,TAMPORTAY-6,0.2);
                glTexCoord2f(0.5,1); glVertex3f(12.5,TAMPORTAY-6,0.2);
            glEnd();
            glDisable(GL_TEXTURE_2D);
            glPopMatrix();
        }
        else{
            glPushMatrix();
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, texID[3]);
                glColor3f(1,1,1);
                glBegin(GL_QUADS);
                    glTexCoord2f(0,0); glVertex3f(12.5,TAMPORTAY-13,0.2);
                    glTexCoord2f(1,0); glVertex3f(17.5, TAMPORTAY-13,0.2);
                    glTexCoord2f(1,1); glVertex3f(17.5,TAMPORTAY-6,0.2);
                    glTexCoord2f(0,1); glVertex3f(12.5,TAMPORTAY-6,0.2);
                glEnd();
                glDisable(GL_TEXTURE_2D);
                glPopMatrix();
        }
    }

}

void porta(){
    //Porta
    glPushMatrix();
    glColor3ub(136,91,59);
    glBegin(GL_QUADS);
        glVertex3f(0, 0, 0);
        glVertex3f(20, 0, 0);
        glVertex3f(20, TAMPORTAY, 0);
        glVertex3f(0, TAMPORTAY, 0);
    glEnd();
    glPopMatrix();

    //Maçaneta porta
    glPushMatrix();
    glColor3f(0.3,0.3,0.2);
    glTranslatef(5, TAMY/3-5, 0);
    glutSolidSphere(1.5,20,20);
    glPopMatrix();
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