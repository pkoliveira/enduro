#include <windows.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>


// Variaveis da pista
float ARD = -0.52; // Angulo de inclinacao da reta que desenha a linha direita da pista
float ARE = 0.52;  // Angulo de inclinacao da reta que desenha a linha esquerda da pista

// Flags que controlam a transição da pista
bool PISTA_RETA = true; // Inicia o jogo com a pista reta
bool CURVA_DIREITA = false;
bool CURVA_ESQUERDA = false;

int Intervalo = 0;

int Tempo = 0; // VARIÁVEL PARA TROCA DO CENÁRIO

int VIDAS = 5; // QUANTIDADE DE VIDAS INICIAL

bool RETA_DIREITA = true;
bool RETA_ESQUERDA = true;
bool CURVA_DIREITA_DIREITA = false;
bool CURVA_DIREITA_ESQUERDA =false;
bool CURVA_ESQUERDA_ESQUERDA = false;
bool CURVA_ESQUERDA_DIREITA = false;
bool MOVENDO_RETA_DIREITA = false;
bool MOVENDO_RETA_ESQUERDA = false;
bool MOVENDO_DIREITA_RETA = false;
bool MOVENDO_ESQUERDA_RETA = false;

// VETOR PARA ARMAZENAMENTO DOS PONTOS DA PISTA
float xLadoDireito[111];
float xLadoEsquerdo[111];
int Den_LD = 500;
int Den_LE = 500;

// ESTRUTURA DOS CARROS
typedef struct adversario{
    int i, R, G ,B, PosY, Posicao;
    float PosX, TamX, TamY, Desloc;
    struct adversario *Prox;
} Adversario;

int Velocidade = 2; // VELOCIDADE INICIAL
Adversario *Lista = NULL; // LISTA DE ADVERSÁRIOS

// FLAGS QUE CONTROLAM A DIREÇÃO HORIZONTAL DO CARRO
bool DIREITA = false;
bool ESQUERDA = false;

// FLAGS QUE CONTROLAM O CENÁRIO
bool DIA = true;
bool NEVE = false;
bool NEBLINA = false;
bool NOITE = false;

// VARIÁVEIS QUE CONTROLAM AS CORES DA PISTA
int COR_LINHA_R = 0;
int COR_LINHA_G = 0;
int COR_LINHA_B = 0;

int COR_PISTA_R = 150;
int COR_PISTA_G = 150;
int COR_PISTA_B = 150;

// POSIÇÃO INICIAL DO CARRO
float posInicialX=0.0, posInicialY=-52.0;
float MovPX = 0.0;

int Posicao = 180;

bool COLISAO = false; // FLAG PARA DETECTAR COLISÃO
int TempoColisao = 0;
int acelerar = 20; // VARIÁVEL PARA ACELERAÇÃO

// PROTÓTIPOS DAS FUNÇÕES

// DESENHOS
void DesenhaNaTela(void);
void DesenhaFundo(void);
void DesenhaCenario(void);
void DesenhaPista(void);
void DesenhaCarro(void);
void DesenhaCarros(void);
void DesenhaPosicao(void);
void DesenhaDia(void);
void DesenhaNoite(void);
void DesenhaNeve(void);
void DesenhaNeblina(void);
void DesenhaNevoa(void);
void DesenhaAdversario(int, int, int);
void DesenhaVidas(void);

// ANIMAÇÃO
void Animar(int);

// CARROS E JOGO
void Inicializa(void);
void CalculaPista(void);
void MoverPista(void);
void MoverCarros(void);
void GerarOponente(void);
void TempoDoJogo(void);
void DetectarColisao(Adversario*);
Adversario* CriarCarro(void);

void DesenhaNaTela(void){
    glPushMatrix();
        DesenhaFundo();
        DesenhaCenario();
        DesenhaPista();
        DesenhaCarros();
    glPopMatrix();
    glPushMatrix();
        glTranslated(posInicialX,posInicialY,0.0); // POSIÇÃO DO CARRO NA TELA
        glTranslatef(-MovPX,0,0); // MOVIMENTA O CARRO
        DesenhaCarro();
    glPopMatrix();
    DesenhaNevoa();
    DesenhaPosicao();
    DesenhaVidas();
    glutSwapBuffers();
}

void DesenhaFundo(void) {
    if(DIA)
        DesenhaDia();
    else
        if(NOITE)
            DesenhaNoite();
        else
            if(NEVE)
                DesenhaNeve();
            else
                if(NEBLINA)
                    DesenhaNeblina();
}

void DesenhaCenario(void){
    // DESENHA MONTANHAS
    int i;
    if (DIA)
        glColor3ub(139,87,66);
    if (NOITE)
        glColor3ub(60,58,58);
    if (NEVE)
        glColor3ub(112,100,100);
    glPushMatrix();
    for ( i = 0; i < 5; i ++){
        glBegin(GL_POLYGON);
            glVertex2i(-120, 50);glVertex2i(-110, 60);
            glVertex2i(-100, 55);glVertex2i(-95, 70);
            glVertex2i(-75, 60);glVertex2i(-70, 50);
        glEnd();
        glTranslated(60, 0, 0);
    }
    glPopMatrix();

    // DESENHA SOL / LUA
    if(DIA || NOITE){
    glPushMatrix();
    glScaled(1.5,1.5,1.5);
    glTranslated(-30,-20,0);

    if(DIA)
        glColor3ub(255, 255, 0);
    if(NOITE)
        glColor3ub(230, 230, 230);
    glBegin(GL_POLYGON);
        glVertex2i(85, 90); glVertex2i(90,90);
        glVertex2i(93,85); glVertex2i(93,80);
        glVertex2i(90,75); glVertex2i(85,75);
        glVertex2i(82,80); glVertex2i(82,85);
    glEnd();
    glPopMatrix();
    }

    // DESENHA NUVEM
    glColor3ub(255,255,255);
    glPushMatrix();
    for ( i = 0; i < 4; i ++){
        glBegin(GL_POLYGON);
            glVertex2i(-100, 85);glVertex2i(-105, 85);
            glVertex2i(-110, 90);glVertex2i(-105, 95);
            glVertex2i(-100, 95);glVertex2i(-95, 92);
            glVertex2i(-93, 95);glVertex2i(-88, 103);
            glVertex2i(-74, 103);glVertex2i(-70, 98);
            glVertex2i(-65, 95);glVertex2i(-62, 90); glVertex2i(-62, 85);
        glEnd();
        glTranslated(100, 0, 0);
    }
    glPopMatrix();
}

void CalculaPista(void){
    int i, y;
    if(RETA_DIREITA){
        i = 0;
        for( y = -60 ;y < 51; y++, i++)
            xLadoDireito[i] = 60 + ARD*y + ARD * 60; // PASSA PELO PONTO [60,-60] E INCLINAÇÃO ARD
    }
    if(RETA_ESQUERDA){
        i = 110;
        for( y = 50 ;y > -61; y--, i--)
            xLadoEsquerdo[i] = -60 + ARE*y + ARE * 60; // PASSA PELO PONTO [-60,-60] E INCLINAÇÃO ARE
    }
    if(CURVA_DIREITA_ESQUERDA){
        i = 0;
        for( y=100;y <211;y++, i++)
            xLadoEsquerdo[i] = (float)(y*y)/Den_LE - 96;
    }
    if(CURVA_DIREITA_DIREITA){
        i = 110;
        for( y=55;y >-56;y--,i--)
            xLadoDireito[i] = (float)(y*y)/Den_LD + 45;
    }
    if(CURVA_ESQUERDA_ESQUERDA){
        i = 110;
        for( y=55;y >-56;y--,i--)
            xLadoEsquerdo[i] = (float)(y*y)/Den_LE - 45;
    }
    if(CURVA_ESQUERDA_DIREITA){
        i = 0;
        for( y=100;y <211;y++, i++)
            xLadoDireito[i] = (float)(y*y)/Den_LD + 96;
    }
}

void MoverPista(){
    if(MOVENDO_RETA_DIREITA){
            if(RETA_ESQUERDA && !CURVA_DIREITA_ESQUERDA){
                RETA_ESQUERDA = false;
                CURVA_DIREITA_ESQUERDA = true;
            }
            if(Den_LE > 310)
                Den_LE -= 10;
            else
                Den_LE = 300;

            if(ARD < -0.2)
                ARD += 0.025;
            else{
                if(RETA_DIREITA && !CURVA_DIREITA_DIREITA){
                    RETA_DIREITA = false;
                    CURVA_DIREITA_DIREITA = true;
                }
                if(Den_LD >249)
                    Den_LD -= 50;
                else
                    Den_LD = 190;
            }
        }else
            if(MOVENDO_RETA_ESQUERDA){
                if(RETA_DIREITA && !CURVA_ESQUERDA_DIREITA){
                    RETA_DIREITA = false;
                    CURVA_ESQUERDA_DIREITA = true;
                }
                if(Den_LD < -310)
                    Den_LD += 10;
                else
                    Den_LD = -300;
                if(ARE > 0.2)
                    ARE -= 0.025;
                else{
                   if(RETA_ESQUERDA && !CURVA_ESQUERDA_ESQUERDA){
                        RETA_ESQUERDA = false;
                        CURVA_ESQUERDA_ESQUERDA = true;
                   }
                   if(Den_LE < -249)
                        Den_LE += 50;
                    else
                        Den_LE = -190;
                }


            }else
                if(MOVENDO_ESQUERDA_RETA){
                    if(Den_LE > -489)
                        Den_LE -= 50;
                    else{
                        Den_LE = -500;
                        if(CURVA_ESQUERDA_ESQUERDA && !RETA_ESQUERDA){
                            CURVA_ESQUERDA_ESQUERDA = false;
                            RETA_ESQUERDA = true;
                            ARE = 0.2;
                        }
                    }
                    if(ARE < 0.52)
                        ARE += 0.025;
                    else
                        ARE = 0.521;
                    if(Den_LD > -490)
                        Den_LD -= 10;
                    else{
                        Den_LD = -500;
                        if(CURVA_ESQUERDA_DIREITA && !RETA_DIREITA){
                            RETA_DIREITA = true;
                            CURVA_ESQUERDA_DIREITA = false;
                            ARD = -0.52;
                        }
                    }
                }else
                    if(MOVENDO_DIREITA_RETA){
                        if(Den_LD < 489)
                            Den_LD += 50;
                        else{
                            Den_LD = 500;
                            if(CURVA_DIREITA_DIREITA && !RETA_DIREITA){
                                CURVA_DIREITA_DIREITA = false;
                                RETA_DIREITA = true;
                                ARD = -0.2;
                            }
                        }
                        if(ARD > -0.52)
                            ARD -= 0.025;
                        else
                            ARD = -0.521;
                        if(Den_LE < 489)
                            Den_LE += 10;
                        else{
                            Den_LE = 500;
                            if(CURVA_DIREITA_ESQUERDA && !RETA_ESQUERDA){
                                CURVA_DIREITA_ESQUERDA = false;
                                RETA_ESQUERDA = true;
                                ARE = 0.52;
                            }
                        }
                    }

}

void DesenhaPista(void){
    CalculaPista();
    int y;
    glColor3ub(COR_PISTA_R,COR_PISTA_G,COR_PISTA_B);
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(0 , -60);
        for( y = -60 ;y < 51; y++){
            glVertex2f(xLadoDireito[y+60] , y);
        }
        for( y = 50 ;y > -61; y--)
            glVertex2f(xLadoEsquerdo[y+60] , y);
    glEnd();
    glColor3ub(COR_LINHA_R,COR_LINHA_G,COR_LINHA_B);
    glBegin(GL_LINE_STRIP);
        //glVertex2f(0 , -60);
        for( y = -60 ;y < 51; y++){
            glVertex2f(xLadoDireito[y+60] , y);
        }
    glEnd();
    glBegin(GL_LINE_STRIP);
        for( y = 50 ;y > -61; y--)
            glVertex2f(xLadoEsquerdo[y+60] , y);
    glEnd();
}

void DesenhaCarros(){
    Adversario *Aux;
    Aux = Lista;
    while(Aux != NULL){
        glPushMatrix();
            glTranslatef(Aux->PosX, Aux->PosY,0);
            glScalef(Aux->TamX, Aux->TamY, 1);
            if(Aux->PosY < 51)
                DesenhaAdversario(Aux->R, Aux->G, Aux->B);
        glPopMatrix();
        Aux = Aux->Prox;
    }
}

void DesenhaVidas(){
    int i;
    glPushMatrix();
    glColor3ub(255, 0, 0);
    glTranslated(35,-80,0);
    glScalef(0.5,0.7,1);
    for (i = 0; i < VIDAS; i ++){
        glTranslated(15,0,0);
        glBegin(GL_LINE_LOOP);
            glVertex2i(-5, 5);glVertex2i(-3, 5);
            glVertex2i(0, 2);glVertex2i(3, 5);
            glVertex2i(5, 5);glVertex2i(6, 3);
            glVertex2i(6, 1);glVertex2i(0, -3);
            glVertex2i(-6, 1);glVertex2i(-6, 3);
            glVertex2i(-5, 5);
        glEnd();
    }
    glPopMatrix();
}

void DesenhaPosicao(void){
    char Text[4], *c;
    int i;
    // DESENHA ÁREA DOS DADOS
    glColor3ub(0,0,0);
    glBegin(GL_QUAD_STRIP);
        glVertex2f(-200, -60);
        glVertex2f(200, -60);
        glVertex2f(-200, -200);
        glVertex2f(200, -200);
    glEnd();
    int aux = Posicao;

    Text[0] = aux / 100 + 48;
    aux = aux % 100;
    Text[1] = aux / 10 + 48;
    aux = aux % 10;
    Text[2] = aux + 48;

    c = Text;

    // DESENHA O CONTORNO DA ÁREA
    glColor3ub(255,0,0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(-80, -65);
        glVertex2f(80, -65);
        glVertex2f(80, -95);
        glVertex2f(-80, -95);
    glEnd();

	if(VIDAS <= 0){
        char mensagem[]={'D','E','R','R','O','T','A','!',' ',':','(','\0'};
        char *Msg = mensagem;

        glColor3f(1.0, 1.0, 1.0);
        glRasterPos3f(-20, -82,0);
        for (; *Msg != '\0'; Msg++)
        {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *Msg);
        }
    }else {
        if(Posicao > 0){
            char mensagem[]={'P','o','s','i','c','a','o',':',' ','#','\0'};
            char *Msg = mensagem;
            char vidas[]={'V','i','d','a','s','\0'};
            char *Lif = vidas;

            glColor3f(1.0, 1.0, 1.0);
            glRasterPos3f(-70, -82,0);
            for (; *Msg != '\0'; Msg++){
                glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *Msg);
            }
            for (i = 0; i < 3; c++, i++){
                glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
            }

            glRasterPos3f(20, -82,0);
            for (; *Lif != '\0'; Lif++){
                glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *Lif);
            }
        }else{
            char mensagem[]={'V','I','T','O','R','I','A','!','\0'};
            char *Msg = mensagem;
            glColor3f(1.0, 1.0, 1.0);
            glRasterPos3f(-20, -82,0);
            for (; *Msg != '\0'; Msg++)
            {
                glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *Msg);
            }
            char o;
            scanf("%c", &o);
            exit(-1);
        }
    }
}

void DesenhaAdversario(int R, int G, int B){

    if(!NOITE){
        //PNEUS
        glColor3ub(40, 40, 40);
        glBegin(GL_QUADS);
            glVertex2i(-9, -7); glVertex2i(-6,-7); glVertex2i(-6,0); glVertex2i(-9,0);
        glEnd();
        glBegin(GL_QUADS);
            glVertex2i(9, -7); glVertex2i(6,-7); glVertex2i(6,0); glVertex2i(9,0);
        glEnd();
        // CARROCERIA
        glColor3ub(R,G,B);
        glBegin(GL_POLYGON);
            glVertex2i(-8, -5); glVertex2i(8,-5); glVertex2i(8,0);
            glVertex2i(5,5); glVertex2i(-5,5); glVertex2i(-8,0);
        glEnd();
    }
    //VIDRO
    glColor3ub(200, 200, 200);
    glBegin(GL_QUADS);
        glVertex2i(6,1); glVertex2i(4,4); glVertex2i(-4,4); glVertex2i(-6,1);
    glEnd();
    //FAROIS TRASEIROS
    glColor3ub(236, 19, 19);
    glBegin(GL_QUADS);
        glVertex2i(-7,-4); glVertex2i(-6, -4); glVertex2i(-6,-1); glVertex2i(-7,-1);
    glEnd();
    glBegin(GL_QUADS);
        glVertex2i(7,-4); glVertex2i(6,-4); glVertex2i(6,-1); glVertex2i(7,-1);
    glEnd();
    //PLACA
    glColor3ub(200, 200, 200);
    glBegin(GL_QUADS);
        glVertex2i(-2,-4); glVertex2i(2,-4); glVertex2i(2,-2); glVertex2i(-2,-2);
    glEnd();
}

void DesenhaCarro(void){
    glPushMatrix();
    if(NOITE) {
        // FAROIS
        glColor3ub(244, 243, 164);
        glBegin(GL_POLYGON);
            glVertex2i(-5,1); glVertex2i(-4, 1); glVertex2i(-3,8); glVertex2i(-10,8);
        glEnd();
        glBegin(GL_POLYGON);
            glVertex2i(5,1); glVertex2i(4,1); glVertex2i(3,8); glVertex2i(10,8);
        glEnd();
    }
    //PNEUS
    glColor3ub(40, 40, 40);
    glBegin(GL_QUADS);
        glVertex2i(-9, -7); glVertex2i(-6,-7); glVertex2i(-2,0); glVertex2i(-9,0);
    glEnd();
    glBegin(GL_QUADS);
        glVertex2i(9, -7); glVertex2i(6,-7); glVertex2i(2,0); glVertex2i(9,0);
    glEnd();
    // CARROCERIA
        glColor3ub(3, 22, 120);
        glBegin(GL_POLYGON);
            glVertex2i(-8, -5); glVertex2i(8,-5); glVertex2i(8,0);
            glVertex2i(5,5); glVertex2i(-5,5); glVertex2i(-8,0);
        glEnd();
    // RETROVISOR
    glColor3ub(3, 22, 120);
    glBegin(GL_POLYGON);
        glVertex2i(-7, 1); glVertex2i(-9,1);
        glVertex2i(-9, 4); glVertex2i(-7,4);
        glVertex2i(-7, 3); glVertex2i(-5,3);
        glVertex2i(-5, 2); glVertex2i(-7,2);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2i(7, 1); glVertex2i(9, 1);
        glVertex2i(9, 4); glVertex2i(7, 4);
        glVertex2i(7, 3); glVertex2i(5, 3);
        glVertex2i(5, 2); glVertex2i(7, 2);
    glEnd();
    glColor3ub(180, 180, 180);
    glPointSize(4.0);
    glBegin(GL_POINTS);
        glVertex2f(-8,2.5);
    glEnd();
    glBegin(GL_POINTS);
        glVertex2f(8,2.5);
    glEnd();
    //ESCAPE
    glColor3ub(200, 200, 200);
    glBegin(GL_POINTS);
        glVertex2f(4,-5);
    glEnd();

    //VIDRO
    glColor3ub(200, 200, 200);
    glBegin(GL_QUADS);
        glVertex2i(6,1); glVertex2i(4,4); glVertex2i(-4,4); glVertex2i(-6,1);
    glEnd();
    //FAROIS TRASEIROS
    glColor3ub(236, 19, 19);
    glBegin(GL_QUADS);
        glVertex2i(-7,-4); glVertex2i(-6, -4); glVertex2i(-6,-1); glVertex2i(-7,-1);
    glEnd();
    glBegin(GL_QUADS);
        glVertex2i(7,-4); glVertex2i(6,-4); glVertex2i(6,-1); glVertex2i(7,-1);
    glEnd();
    //PLACA
    glColor3ub(200, 200, 200);
    glBegin(GL_QUADS);
        glVertex2i(-2,-4); glVertex2i(2,-4); glVertex2i(2,-2); glVertex2i(-2,-2);
    glEnd();
    glPopMatrix();
}

void DesenhaDia(void) {
    glClearColor(0.4, 0.6, 1,0); // COR DO FUNDO
    glClear(GL_COLOR_BUFFER_BIT);// LIMPA AS CORES
    glColor3ub(61,153,68);     // DEFINE A COR DO CHÃO
    glBegin(GL_POLYGON);       // DESENHA O CHÃO
        glVertex2i(-200, 50);
        glVertex2i(200,50);
        glVertex2i(200,-200);
        glVertex2i(-200,-200);
    glEnd();

    COR_PISTA_R = COR_PISTA_G = COR_PISTA_B = 164; // MUDAR A COR DA PISTA
}

void DesenhaNoite(void) {
    glClearColor(0.3,0.3,0.3,0); // COR DO FUNDO
    glClear(GL_COLOR_BUFFER_BIT);// LIMPA AS CORES
    glColor3ub(0,0,0);     // DEFINE A COR DO CHÃO
    glBegin(GL_POLYGON);       // DESENHA O CHÃO
        glVertex2i(-200, 50);
        glVertex2i(200,50);
        glVertex2i(200,-200);
        glVertex2i(-200,-200);
    glEnd();

    COR_PISTA_R = COR_PISTA_G = COR_PISTA_B = 0; // MUDAR A COR DA PISTA
}

void DesenhaNeve(void){
    glClearColor(0.2,0.6,1.0,0); // COR DO FUNDO
    glClear(GL_COLOR_BUFFER_BIT);// LIMPA AS CORES
    glColor3ub(255,255,255);     // DEFINE A COR DO CHÃO
    glBegin(GL_POLYGON);       // DESENHA O CHÃO
        glVertex2i(-200, 50);
        glVertex2i(200,50);
        glVertex2i(200,-200);
        glVertex2i(-200,-200);
    glEnd();


    COR_PISTA_R = COR_PISTA_G = COR_PISTA_B = 255; // MUDAR A COR DA PISTA
}

void DesenhaNevoa(void){
    if(NEBLINA){
        glColor3ub(186,186,186);     // DEFINE A COR DO CHÃO
        glBegin(GL_POLYGON);       // DESENHA O CHÃO
            glVertex2i(-200, 100);
            glVertex2i(200,100);
            glVertex2i(200,10);
            glVertex2i(-200,10);
        glEnd();
    }
}

void DesenhaNeblina(void){
    glClearColor(0,0,0,0); // COR DO FUNDO
    glClear(GL_COLOR_BUFFER_BIT);// LIMPA AS CORES
    glColor3ub(0,0,0);     // DEFINE A COR DO CHÃO
    glBegin(GL_POLYGON);       // DESENHA O CHÃO
        glVertex2i(-100, 20);
        glVertex2i(100,20);
        glVertex2i(100,-100);
        glVertex2i(-100,-100);
    glEnd();
}

void Teclado (unsigned char tecla, int x, int y){
	if (tecla == 27)// QUANDO ESC FOR PRESSIONADO O PROGRAMA FINALIZA
		exit(0);
	switch (tecla) {
        case '1':
            COR_PISTA_R = 164;
            COR_PISTA_G = 164;
            COR_PISTA_B = 164;
            COR_LINHA_R = 13;
            COR_LINHA_G = 62;
            COR_LINHA_B = 31;
            DIA=true;
            NOITE=NEBLINA=NEVE=false;
            glutPostRedisplay();
            break;
        case '2':
            COR_PISTA_R = 0;
            COR_PISTA_G = 0;
            COR_PISTA_B = 0;
            COR_LINHA_R = 255;
            COR_LINHA_G = 255;
            COR_LINHA_B = 255;
            NOITE=true;
            DIA=NEBLINA=NEVE=false;
            glutPostRedisplay();
            break;
        case '3':
            COR_PISTA_R = 255;
            COR_PISTA_G = 255;
            COR_PISTA_B = 255;
            COR_LINHA_R = 0;
            COR_LINHA_G = 0;
            COR_LINHA_B = 0;
            NEVE=true;
            NOITE=NEBLINA=DIA=false;
            glutPostRedisplay();
            break;
        case '4':
            COR_PISTA_R = 47;
            COR_PISTA_G = 79;
            COR_PISTA_B = 79;
            COR_LINHA_R = 255;
            COR_LINHA_G = 255;
            COR_LINHA_B = 255;
            NEBLINA=true;
            NOITE=DIA=NEVE=false;
            glutPostRedisplay();
            break;
	}
}

void Teclado_Especial(GLint tecla, int x, int y) {
    switch (tecla) {
        case GLUT_KEY_RIGHT:
                DIREITA = true;
                ESQUERDA = false;
            glutPostRedisplay();
            break;
        case GLUT_KEY_LEFT:
                DIREITA = false;
                ESQUERDA = true;
            glutPostRedisplay();
            break;
        case GLUT_KEY_UP:
            if(Velocidade < 5) // EVITA O TURBO MUITO LOUCO
                Velocidade++;
        break;
        case GLUT_KEY_DOWN:
            if(Velocidade > 1) // EVITA QUE O CARRO PARE
                Velocidade--;
        break;
    }
}

void Controlador(){
    if(!COLISAO){
        if(DIREITA && MovPX > xLadoEsquerdo[0] +20)
            MovPX -= 0.8 * Velocidade;
        if(ESQUERDA && MovPX < xLadoDireito[0] -20)
            MovPX += 0.8 * Velocidade;
    }
}

Adversario* CriarCarro(){
    Adversario *Carro;
    //srand(time(NULL));
    Carro = (Adversario*)malloc(sizeof(Adversario));
    if(Carro == NULL){
        printf("Problema ao reservar memoria, the program will be terminated !");
        exit(-1);
    }
    Carro->Posicao = rand() % (3);
    Carro->TamX = 0.2;
    Carro->TamY = 0.2;
    Carro->PosY = 50;
    Carro->PosX = xLadoEsquerdo[110];
    Carro->Prox = NULL;
    Carro->i = 110;
    Carro->Desloc = 4;
    Carro->R = rand() % (254);
    Carro->G = rand() % (254);
    Carro->B = rand() % (254);
    //printf("dff = %d", Carro->velocidade);
    return Carro;
}

void MoverCarros(){
    Adversario *Aux, *Anterior;
    if(COLISAO){
        TempoColisao++;
        Velocidade = -1;
        if(TempoColisao == 30){
            //COLISAO = false;
            Velocidade = 0;
            //TempoColisao =0;
        }
        if(TempoColisao == 35){
            Velocidade = 2;
            COLISAO = false;
            TempoColisao =0;
            VIDAS--;
        }
    }
    Aux = Lista;
    if(Lista == NULL)
        return;
    Anterior = Lista;
    do{
        DetectarColisao(Aux);
        if(Aux->PosY < -80 && Aux->Prox == NULL){
            free(Aux);
            Aux = NULL;
            Anterior->Prox = NULL;
            Posicao--;
        }else{
                Aux->i -= Velocidade;

            if(Aux->i < 0) // VERIFICA O FINAL DA PISTA
                Aux->i = 0;
            Aux->PosY -= Velocidade;
            switch(Aux->Posicao){
                case 0:
                    Aux->PosX = xLadoEsquerdo[Aux->i] + Aux->Desloc;
                    Aux->Desloc += 0.15 * Velocidade;
                    break;
                case 1:
                    Aux->PosX = xLadoEsquerdo[Aux->i] + Aux->Desloc;
                    Aux->Desloc += 0.5 * Velocidade;
                    break;
                case 2:
                    Aux->PosX = xLadoDireito[Aux->i] - Aux->Desloc;
                    Aux->Desloc += 0.15 * Velocidade;
                    break;
            }
            Aux->TamX += 0.00727273 * Velocidade;
            Aux->TamY += 0.00727273 * Velocidade;
            Anterior = Aux;
            Aux = Aux->Prox;
        }
    }while(Aux != NULL);
        if(acelerar == 40 && !COLISAO){
            Velocidade++;
            acelerar = 0;
        }
}

void GerarOponente(){
    Adversario *Aux;
    if(Intervalo > 40){
        Aux = CriarCarro();
        Aux->Prox = Lista;
        Lista = Aux;
        Intervalo = 0;
    }
}

void TempoDoJogo(){
    switch(Tempo){
        case 500: // NEVE
            DIA = false;
            NEVE = true;
            COR_LINHA_R = 0;
            COR_LINHA_G = 0;
            COR_LINHA_B = 0;
            COR_PISTA_R = 255;
            COR_PISTA_G = 255;
            COR_PISTA_B = 255;
            break;
        case 1000: // DIA
            NEVE = false;
            DIA = true;
            COR_LINHA_R = 0;
            COR_LINHA_G = 0;
            COR_LINHA_B = 0;
            COR_PISTA_R = 150;
            COR_PISTA_G = 150;
            COR_PISTA_B = 150;
            break;
        case 1500: // NOITE
            DIA = false;
            NOITE = true;
            COR_LINHA_R = 255;
            COR_LINHA_G = 255;
            COR_LINHA_B = 255;
            COR_PISTA_R = 0;
            COR_PISTA_G = 0;
            COR_PISTA_B = 0;
            break;
        case 1750: // NEBLINA
            NOITE = false;
            NEBLINA = true;
            COR_LINHA_R = 255;
            COR_LINHA_G = 255;
            COR_LINHA_B = 255;
            COR_PISTA_R = 25;
            COR_PISTA_G = 25;
            COR_PISTA_B = 25;
            break;
        case 2000:// NOITE
            NEBLINA = false;
            NOITE = true;
            COR_LINHA_R = 255;
            COR_LINHA_G = 255;
            COR_LINHA_B = 255;
            COR_PISTA_R = 0;
            COR_PISTA_G = 0;
            COR_PISTA_B = 0;
            break;
        case 2250: // DIA
            NOITE = false;
            DIA = true;
            Tempo = 0;
            COR_LINHA_R = 0;
            COR_LINHA_G = 0;
            COR_LINHA_B = 0;
            COR_PISTA_R = 150;
            COR_PISTA_G = 150;
            COR_PISTA_B = 150;
            break;
    }
}

void DetectarColisao(Adversario *Aux){
    int i;
    float X1,X2,Y2,Y3;
    X1 = -MovPX -10;
    X2 = -MovPX +10;
    Y3 = -40;
    float X,Y;

    Y = Aux->PosY;
    X = Aux->PosX - 8;
    for( i = 0;i< 2;i++){
    if(X > X1 && X < X2)
        if(Y < Y3 && Y > -60){
            COLISAO = true;
        }
        X = Aux->PosX + 8;
    }
}

void Animar(int iteracoes){
    if (VIDAS > 0 && Posicao > 0){
        Intervalo += Velocidade;
        GerarOponente();

        if(iteracoes == 30){
            MOVENDO_RETA_DIREITA = true;
        }
        if(iteracoes == 100){
            MOVENDO_RETA_DIREITA = false;
            MOVENDO_DIREITA_RETA = true;
        }
        if(iteracoes == 600){
            MOVENDO_DIREITA_RETA = false;
            MOVENDO_RETA_ESQUERDA = true;
            Den_LD = Den_LE = -500;
        }
        if(iteracoes == 800){
            MOVENDO_RETA_ESQUERDA = false;
            MOVENDO_ESQUERDA_RETA = true;
        }
        if(iteracoes == 1000){
            MOVENDO_ESQUERDA_RETA = false;
            Den_LD = Den_LE = 500;
            iteracoes = 0;
        }

        if(Lista == NULL){
            Lista = CriarCarro();
        }
        MoverCarros();
        MoverPista();
        TempoDoJogo();
        Controlador();
        glutPostRedisplay();
        iteracoes++;
        Tempo++;
        glutTimerFunc(30,Animar,iteracoes);
    }
}

void Inicializa(void) {
	// Define a janela de visualização 2D
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-100.0,100.0,-100.0,100.0);//Exibição Bidimensional
}

int main(int argc, char** argv) {
    srand(time(NULL));
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1000,800);
    glutInitWindowPosition(500,100);
    glutCreateWindow("Enduro [ Prabhat ]");
    //glutFullScreen();
    Inicializa();
    glutDisplayFunc(DesenhaNaTela);
    glutKeyboardFunc( Teclado );
    glutSpecialFunc(Teclado_Especial);
    glutTimerFunc(30,Animar,0);
    glutMainLoop();
}
