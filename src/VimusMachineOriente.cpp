/**
 * Copyright 2007-2012 Jarbas Jácome
 *
 * This file is part of ViMus.
 *
 * ViMus is free software: you can re
ribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ViMus is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ViMus.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "VimusMachineOriente.h"

using namespace std;

/**
 * VimusMachineOriente default constructor.
 */
VimusMachineOriente::VimusMachineOriente(MyFreenectDevice* kin)
{
    VimusMachineObject::initObject();

	this->label = string ("oriente");
	this->addInputPin(VimusMachinePin::TYPE_VIDEO);

	this->addOutputPin(VimusMachinePin::TYPE_VIDEO);

	this->ppNullFrame = new unsigned char*;
	(*this->ppNullFrame) = new unsigned char[VIDEO_WIDTH*VIDEO_HEIGHT*3];
	for (int i=0; i<VIDEO_WIDTH*VIDEO_HEIGHT*3; i++)
	{
		(*this->ppNullFrame)[i] = 255;
	}

	// do not assign another valor to it.
	this->ppOutputData = new unsigned char*;
	(*this->ppOutputData) = new unsigned char[VIDEO_WIDTH*VIDEO_HEIGHT*3];
	for (int i=0; i<VIDEO_WIDTH*VIDEO_HEIGHT*3; i++)
	{
		(*this->ppOutputData)[i] = 255;
	}

	// set video output pin
	((VimusMachinePinVideo *)this->outputPins[0])->ppFrame = this->ppOutputData;

	// do not assign another valor to it.
	this->ppInputData = new unsigned char*;
	(*this->ppInputData) = new unsigned char[VIDEO_WIDTH*VIDEO_HEIGHT*3];
	for (int i=0; i<VIDEO_WIDTH*VIDEO_HEIGHT*3; i++)
	{
		(*this->ppInputData)[i] = 255;
	}

	// set video input pin
	//((VimusMachinePinVideo *)this->inputPins[0])->ppFrame = this->ppInputData;

    this->kinect = kin;

	glGenTextures(1, &gl_depth_tex);
	glBindTexture(GL_TEXTURE_2D, gl_depth_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
 	glGenTextures(1, &gl_rgb_tex);
	glBindTexture(GL_TEXTURE_2D, gl_rgb_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    //aloca os arrays na memória RAM
    for (int i=0;i<MAX_NUM_PONTOS_ESPIRAL;i++) {
        desenhandoEspiral[i] = -1;
    }

    iniciaTeia();


    if (DEBUG_MODE)
        cout << "\nVimusMachineOriente constructed.";
}

/**
 * VimusMachineOriente destructor.
 */
VimusMachineOriente::~VimusMachineOriente()
{
}

/**
 * Update VimusMachineOriente.
 */
void VimusMachineOriente::update()
{
}

/**
 * Update VimusMachineOriente.
 */
void VimusMachineOriente::draw(int rendermode)
{
}

/**
 * Update VimusMachineOriente.
 */
void VimusMachineOriente::draw()
{
	static vector<uint8_t> depth(640*480);
	static vector<uint8_t> rgb(640*480*4);

	// using getTiltDegs() in a closed loop is unstable
	/*if(kinect->getState().m_code == TILT_STATUS_STOPPED){
		freenect_angle = kinect->getState().getTiltDegs();
	}*/
	kinect->updateState();
//	printf("\r demanded tilt angle: %+4.2f kinect tilt angle: %+4.2f", freenect_angle, kinect->getState().getTiltDegs());
//	fflush(stdout);

	kinect->getDepth(depth);
	kinect->getRGB(rgb);

	//depth[30]=50;

    //cout << " depthMono= " << (int)depthMono[240*640+320];
//
//	cout << " depth0= " << (int)depth[3*(240*640+320)];
//	cout << " depth1= " << (int)depth[3*(240*640+321)];
//	cout << " depth2= " << (int)depth[3*(240*640+322)];


	int perto = 2048;
	for(int i=0; i<480; i++){
        for(int j=0; j<640; j++){
            if(perto>depth[i*640+j]){
                perto=depth[i*640+j];
                kinectX=j;
                kinectY=480-i;
            }
        }
	}

    kinect->got_frames = 0;

	glPushMatrix();

    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();

    gluLookAt (0, 0, 10.0f, 0, 0, 10.0f - 5.0f, 0.0, 1.0, 0.0);

    glClearColor (0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDisable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);

    glColor4f (1.0f, 1.0f, 1.0f, 1.0f);

	glBindTexture(GL_TEXTURE_2D, gl_depth_tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, 640, 480, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, &depth[0]);

    glBegin(GL_QUADS);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,-1.0f, 0.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,-1.0f, 0.0f);
    glEnd();

    glDisable(GL_TEXTURE_2D);

    glTranslatef(2*kinectX/640.0f - 1, 2*kinectY/480.0f - 1, 0.1f);

    glBegin(GL_LINES);
        glVertex3f( 0.0f, 0.1f, 0.0f);
        glVertex3f( 0.0f,-0.1f, 0.0f);
        glVertex3f(-0.1f, 0.0f, 0.0f);
        glVertex3f( 0.1f, 0.0f, 0.0f);
    glEnd();

	glPopMatrix();

}

/**
 * Get pointer to currentFrame buffer pointer.
 * This is necessary for GUI have a direct access to
 * machine frame buffer.
 */
unsigned char** VimusMachineOriente::getCurrentFramePointer()
{
	return this->ppOutputData;
}

/**
 * Do some action before and/or after connect a pin to
 * another objects pin.
 */
bool VimusMachineOriente::connectOutput (int outPin, VimusMachineObject * dstObj, int inPin)
{
	return dstObj->connectInput(this, outPin, inPin);
}

/**
 * Do some action before and/or after be connected by another
 * objects pin.
 */
bool VimusMachineOriente::connectInput (VimusMachineObject * srcObj, int outPin, int inPin)
{
	bool ret = false;
	if (srcObj->outputPins[outPin]->connect(this->inputPins[inPin]))
	{
		((VimusMachinePinVideo *) this->inputPins[inPin])->ppFrame =
			((VimusMachinePinVideo *) srcObj->outputPins[outPin])->ppFrame;
		this->ppInputData = ((VimusMachinePinVideo *) this->inputPins[inPin])->ppFrame;
		ret = true;
	}
	return ret;
}

/**
 * Do some action before and/or after disconnect an output pin from
 * another objects pin.
 */
void VimusMachineOriente::disconnectOutput (int outPin, VimusMachineObject * dstObj, int inPin)
{
	dstObj->disconnectInput(this, outPin, inPin);
}

/**
 * Do some action before and/or after disconnect an input pin from
 * another objects pin.
 */
void VimusMachineOriente::disconnectInput (VimusMachineObject * srcObj, int outPin, int inPin)
{
	srcObj->outputPins[outPin]->disconnect(this->inputPins[inPin]);
	this->ppInputData = this->ppNullFrame;
}

void VimusMachineOriente::keyBoardFunc(unsigned char key, int x, int y)
{
}

void VimusMachineOriente::specialKeyBoardFunc(int key, int x, int y)
{
}


//loop principal
//a função draw() é chamada várias vezes por segundo
void draw () {

  constroiEspiral();
//
//  int atual = millis();
//  if (atual - ultimaConstrucao > random(INTERVALO_CONSTRUCAO*0.8, INTERVALO_CONSTRUCAO*1.2)) {
//    ultimaConstrucao = atual;
//    constroiEspiralRandom();
//  }
//
//  atualizaPontos();
//
//  background (0); //pinta a tela inteira com a cor preta
//
//  //desenha base do centro da teia
//  stroke(150);
//  for (int i = 0; i<NUM_PONTOS_BASE_CENTRO-1; i++) {
//    line( pontosBaseCentro[i]->x+r(), pontosBaseCentro[i]->y+r(),
//    pontosBaseCentro[i+1]->x+r(), pontosBaseCentro[i+1]->y+r());
//  }
//  line( pontosBaseCentro[NUM_PONTOS_BASE_CENTRO-1]->x+r(),
//  pontosBaseCentro[NUM_PONTOS_BASE_CENTRO-1]->y+r(),
//  pontosBaseCentro[0]->x+r(),
//  pontosBaseCentro[0]->y+r()); //liga último ao primeiro
//
//  //desenha raios
//  stroke(150);
//  float p1x, p1y, p2x, p2y;
//  for (int i=0; i<NUM_RAIOS; i++) {
//    p1x = raios[i][0]->x + r();
//    p1y = raios[i][0]->y + r();
//    for (int j=1; j<NUM_PONTOS_RAIO; j++) {
//      if (raios[i][j]->x>-1) {
//        line (p1x, p1y,
//        raios[i][j]->x+r(), raios[i][j]->y+r());
//        p1x = raios[i][j]->x+r();
//        p1y = raios[i][j]->y+r();
//      }
//    }
//  }
//
//  //desenha espiral
//  stroke(150);
//  for (int i=0; i<tamEspiral-1; i++) {
//    if (desenhandoEspiral[i+1] >= 1) {
//      bool cima=false;
//      if((int)espiral[i+1]->x+1>NUM_RAIOS/2){
//        cima = true;
//      }
//      float abaula = 1.5*abs(cos(TWO_PI*(espiral[i+1]->x/(float)NUM_RAIOS)))+1.5;
//      desenhaArco (raios[(int)espiral[i]->x][(int)espiral[i]->y]->x+r(),
//      raios[(int)espiral[i]->x][(int)espiral[i]->y]->y+r(),
//      raios[(int)espiral[i+1]->x][(int)espiral[i+1]->y]->x+r(),
//      raios[(int)espiral[i+1]->x][(int)espiral[i+1]->y]->y+r(), abaula, cima);
//    }
//  }
//}
//
//void desenhaArco(float p1x, float p1y, float p2x, float p2y, float abaula, bool cima){
//  float teta = TWO_PI-angulo (p2x,p2y,p1x,p1y);
//  if(cima){
//    teta+=PI;
//  }
//  float dist = dist(p1x, p1y, p2x, p2y);
//  float pmx = (p1x+p2x)/2;
//  float pmy = (p1y+p2y)/2;
//  float cx = pmx+dist*abaula*cos(teta-HALF_PI);
//  float cy = pmy+dist*abaula*sin(teta-HALF_PI);
//  float tamElipse = 2*dist(cx,cy,p1x,p1y);
//  float arco = 2*asin(dist/tamElipse);
//  noFill();
//  arc (cx, cy, tamElipse, tamElipse, teta+HALF_PI-arco/2, teta+HALF_PI+arco/2);
}


float VimusMachineOriente::random(float menor, float maior){
    float r = menor + (float)rand()/((float)RAND_MAX/(maior-menor));
    return r;
}

void VimusMachineOriente::iniciaTeia() {

  //sorteia um ponto para ser o centro da teia
  centro = new PVector( random(width/2.0-20, width/2.0+20),
  random(height/2-20, height/2+20));

  //inicializa os pontos da base da teia
  pontosBase[0] = new PVector(width, 0);
  pontosBase[1] = new PVector(0, 0);
  pontosBase[2] = new PVector(0, height);
  pontosBase[3] = new PVector(width, height);

  for(int i=0; i<NUM_PONTOS_BASE_CENTRO; i++){
    float teta = -TWO_PI*i/NUM_PONTOS_BASE_CENTRO;
    pontosBaseCentro[i] = new PVector(centro->x+TAM_BASE_CENTRO*cos(teta)*random(0.90,1),
                                      centro->y+TAM_BASE_CENTRO*sin(teta)*random(1,1.1));
  }

  //calcula os angulos em relação ao eixo x que formam
  //linhas do centro até cada ponto base
  for (int i=0; i<NUM_PONTOS_BASE; i++) {
    angulosPontosBase[i] = angulo (pontosBase[i]->x, pontosBase[i]->y, centro->x, centro->y);
  }

  //calcula os angulos em relação ao eixo x que formam
  //linhas do centro até cada ponto base do centro.
  for (int i=0; i<NUM_PONTOS_BASE_CENTRO; i++) {
    angulosPontosBaseCentro[i] = angulo (pontosBaseCentro[i]->x, pontosBaseCentro[i]->y, centro->x, centro->y);
  }

  //calcula os pontos dos raios que encostam nas bases da teia para todos os raios.
  float incAngulo = TWO_PI/NUM_RAIOS;
  for (int i=0; i<NUM_RAIOS; i++) {
    raiosIdeal[i][0] = new PVector(pontosBaseCentro[i]->x, pontosBaseCentro[i]->y);
    raios[i][0] = new PVector(pontosBaseCentro[i]->x, pontosBaseCentro[i]->y);
    angulosRaios[i] = i*incAngulo+random(-0.04, 0.04);
    raiosIdeal[i][NUM_PONTOS_RAIO-1] = new PVector(0, 0);
    calculaPontoRaioBase (&raiosIdeal[i][NUM_PONTOS_RAIO-1], angulosRaios[i],
    raiosIdeal[i][0]->x, raiosIdeal[i][0]->y);
    raios[i][NUM_PONTOS_RAIO-1] = new PVector(0, 0);
    calculaPontoRaioBase (&raios[i][NUM_PONTOS_RAIO-1], angulosRaios[i],
    raiosIdeal[i][0]->x, raiosIdeal[i][0]->y);
    tamanhoRaios[i] = dist(raiosIdeal[i][0], raiosIdeal[i][NUM_PONTOS_RAIO-1]);
    for (int j=1; j<NUM_PONTOS_RAIO-1; j++) {
      raiosIdeal[i][j] = new PVector(-1, -1);
      raios[i][j] = new PVector(-1, -1);
    }
  }
  ultimaConstrucao=0;

  tamEspiral=0;

  definePontosEspiral();
}

float VimusMachineOriente::dist(PVector* p1, PVector* p2){
    return sqrt(pow(p2->x-p1->x,2)+pow(p2->y-p1->y,2));
}

float VimusMachineOriente::dist(float x1, float y1, float x2, float y2){
    return sqrt(pow(x2-x1,2)+pow(y2-y1,2));
}


void VimusMachineOriente::definePontosEspiral() {
    //variáveis usadas para construção das espirais
    int c = FIM_TEIA; //contador
    float teta, inc, incX, incY;
    int raio, pontoRaio;
    while (c>INICIO_TEIA) {
        c--;
        raio = c%NUM_RAIOS;
        pontoRaio = c/NUM_RAIOS;
        teta = angulosRaios[raio];
        inc = ESPACAMENTO_ESPIRAL*random(0.985, 1.015);
        incX =  cos(teta)*inc*(pontoRaio+CURVA_ESPIRAL*raio);
        incY = -sin(teta)*inc*(pontoRaio+CURVA_ESPIRAL*raio);
        aranha = new PVector (incX+centro->x, incY+centro->y);
        if (dentroBase(aranha->x, aranha->y)) {
          colaEspiral(raio, pontoRaio+1);
          desenhandoEspiral[tamEspiral] = 0;
        }
        else {
          desenhandoEspiral[tamEspiral] = -1;
        }
    }
}

//cola a teia do espiral no ponto onde a aranha está no raio
void VimusMachineOriente::colaEspiral(int raio, int pontoRaio) {
  raiosIdeal[raio][pontoRaio]->x = aranha->x;
  raiosIdeal[raio][pontoRaio]->y = aranha->y;
  espiral[tamEspiral] = new PVector(raio, pontoRaio);
  tamEspiral++;
}
//
//void constroiEspiralRandom () {
//  int c=0;
//  for (int i=0;i<tamEspiral;i++) {
//    if (desenhandoEspiral[i]==0) {
//      naoLigados[c] = i;
//      c++;
//    }
//  }
//  int escolhido = naoLigados[int(random(0, c-1))];
//  desenhandoEspiral[escolhido] = 1;
//  int raioP1, pontoRaioP1, raioP2, pontoRaioP2;
//  raioP1 = (int)espiral[escolhido]->x;
//  pontoRaioP1 = (int)espiral[escolhido]->y;
//  raioP2 = (int)espiral[escolhido-1]->x;
//  pontoRaioP2 = (int)espiral[escolhido-1]->y;
//  raios[raioP1][pontoRaioP1]->x = (raiosIdeal[raioP1][pontoRaioP1]->x + raiosIdeal[raioP2][pontoRaioP2]->x)/2;
//  raios[raioP1][pontoRaioP1]->y = (raiosIdeal[raioP1][pontoRaioP1]->y + raiosIdeal[raioP2][pontoRaioP2]->y)/2;
//  raios[raioP2][pontoRaioP2]->x = (raiosIdeal[raioP1][pontoRaioP1]->x + raiosIdeal[raioP2][pontoRaioP2]->x)/2;
//  raios[raioP2][pontoRaioP2]->y = (raiosIdeal[raioP1][pontoRaioP1]->y + raiosIdeal[raioP2][pontoRaioP2]->y)/2;
//}
//
void VimusMachineOriente::constroiEspiral() {
  if (dentroBase(kinectX, kinectY)) {
    float teta = angulo(kinectX, kinectY, centro->x, centro->y);
    int espirais = NUM_RAIOS* div(dist(kinectX, kinectY, centro->x, centro->y), ESPACAMENTO_ESPIRAL);
    int angulos = div(teta, TWO_PI/NUM_RAIOS);
    int escolhido = espirais + angulos;
    if (escolhido<1) {
      escolhido=1;
    }
    if (espiral[escolhido]!=null && espiral[escolhido-1]!=null && desenhandoEspiral[escolhido] == 0) {
      desenhandoEspiral[escolhido] = 1;
      int raioP1, pontoRaioP1, raioP2, pontoRaioP2;
      raioP1 = (int)espiral[escolhido]->x;
      pontoRaioP1 = (int)espiral[escolhido]->y;
      raioP2 = (int)espiral[escolhido-1]->x;
      pontoRaioP2 = (int)espiral[escolhido-1]->y;
      raios[raioP1][pontoRaioP1]->x = (raiosIdeal[raioP1][pontoRaioP1]->x + raiosIdeal[raioP2][pontoRaioP2]->x)/2;
      raios[raioP1][pontoRaioP1]->y = (raiosIdeal[raioP1][pontoRaioP1]->y + raiosIdeal[raioP2][pontoRaioP2]->y)/2;
      raios[raioP2][pontoRaioP2]->x = (raiosIdeal[raioP1][pontoRaioP1]->x + raiosIdeal[raioP2][pontoRaioP2]->x)/2;
      raios[raioP2][pontoRaioP2]->y = (raiosIdeal[raioP1][pontoRaioP1]->y + raiosIdeal[raioP2][pontoRaioP2]->y)/2;
    }
  }
}
//
//float r() {
//  //return random (-2,2);
//  return random(-0.4, 0.4);
//  //  return 0;
//}
//
//calcula o angulo em relação ao eixo X formado pela reta
//que contém os dois pontos passados como parâmetros (x,y) e (xC, yC).
float VimusMachineOriente::angulo(float x, float y, float xC, float yC) {
  float angulo;
  angulo = atan((yC - y)/(xC - x));
  //localiza em qual quadrante está o ponto
  if (x > xC) {
    if (y < yC) {                   //primeiro quadrante
      angulo = abs(angulo);
    }
    else {                          //quarto quadrante
      angulo = TWO_PI - angulo;
    }
  }
  else {
    if (y > yC) {                   //terceiro quadrante
      angulo = PI + abs(angulo);
    }
    else {                          //segundo quadrante
      angulo = PI - angulo;
    }
  }
  return angulo;
}

//calcula ponto do raio que toca na base da teia de acordo com o ângulo que o raio
//forma com o eixo X.
void VimusMachineOriente::calculaPontoRaioBase (PVector** retorno,float angulo, float cX, float cY) {
    PVector* segBase = new PVector(0, 0);
    calculaSegmentoBase (&segBase, angulo);
    int pontoB1 = int(segBase->x);
    int pontoB2 = int(segBase->y);
    calculaPontoInterseccaoRetas(retorno, angulo, cX, cY,
    pontosBase[pontoB1]->x, pontosBase[pontoB1]->y,
    pontosBase[pontoB2]->x, pontosBase[pontoB2]->y);
}

//calcula o ponto de intersecção da reta (x1, y1) (x2, y2)
//com a reta que passa pelo ponto (pX, pY) e que forma um ângulo "angulo"
//em relação ao eixo X.
//ela é usada para calcular a intersecção dos raios com as linhas que formam
//a base da teia.
void VimusMachineOriente::calculaPontoInterseccaoRetas (PVector** retorno, float angulo, float pX, float pY,
float x1, float y1, float x2, float y2) {
  //pequeno ajuste para evitar valores infinitos como resultado de tangente
  if (angulo == HALF_PI || angulo == 3*HALF_PI) {
    angulo+=0.001;
  }
  //calcula coeficiente de angulação da reta fazendo um pequeno ajuste
  //para evitar divisão por zero quando a reta é vertical
  float a;
  if (x2-x1 == 0) { //reta 100% vertical
    a = (y2-y1)/(x2-x1+0.01);
  }
  else {
    a = (y2-y1)/(x2-x1);
  }
  //calcula coordenada X e Y utilizando equação da reta em função de 2 pontos e
  //em sistema de equações com a equação da reta em função do ângulo e de um ponto.
  (*retorno)->x = (pX*-tan(angulo)-pY+y1-x1*a)/(-tan(angulo) - a);
  (*retorno)->y = -tan(angulo)*((*retorno)->x-pX)+pY;
}

void VimusMachineOriente::calculaSegmentoBase (PVector** retorno, float angulo) {
  int pontoB1=-1;
  int pontoB2=-1;
  int c=0;
  while (pontoB1<0 && c<NUM_PONTOS_BASE-1) {
    if (angulo > angulosPontosBase[c] && angulo < angulosPontosBase[c+1]) {
      pontoB1 = c;
      pontoB2 = c+1;
    }
    c++;
  }
  if (c==NUM_PONTOS_BASE-1 && pontoB1<0) {
    pontoB1 = c;
    pontoB2 = 0;
  }
  (*retorno)->x = pontoB1;
  (*retorno)->y = pontoB2;
}

bool VimusMachineOriente::dentroBase(float x, float y) {
  bool ret = false;
  float anguloAranha = angulo(x, y, centro->x, centro->y);
  PVector* segBase = new PVector(0, 0);
  calculaSegmentoBase(&segBase, anguloAranha);
  int pontoB1 = int(segBase->x);
  int pontoB2 = int(segBase->y);
  PVector* interseccao = new PVector(0, 0);
  calculaPontoInterseccaoRetas (&interseccao, anguloAranha, x, y,
  pontosBase[pontoB1]->x, pontosBase[pontoB1]->y,
  pontosBase[pontoB2]->x, pontosBase[pontoB2]->y);
  float distAranha = dist(x, y, centro->x, centro->y);
  float distInterseccao = dist(interseccao->x, interseccao->y, centro->x, centro->y);
  if (distAranha < distInterseccao) {
    ret = true;
  }
  return ret;
}
//
//void atualizaPontos() {
//  int raioP1, pontoRaioP1, raioP2, pontoRaioP2;
//  float fator1, fator2;
//  for (int i=1; i<tamEspiral; i++) {
//    if (desenhandoEspiral[i]>0 && desenhandoEspiral[i]<100) {
//      desenhandoEspiral[i]+=10;
//      if (desenhandoEspiral[i]>100) {
//        desenhandoEspiral[i]=100;
//      }
//      raioP1 = (int)espiral[i]->x;
//      pontoRaioP1 = (int)espiral[i]->y;
//      raioP2 = (int)espiral[i-1]->x;
//      pontoRaioP2 = (int)espiral[i-1]->y;
//      fator1 = 1+float(desenhandoEspiral[i])/100;
//      fator2 = 1-float(desenhandoEspiral[i])/100;
//      raios[raioP1][pontoRaioP1]->x = (fator1*raiosIdeal[raioP1][pontoRaioP1]->x + fator2*raiosIdeal[raioP2][pontoRaioP2]->x)/2;
//      raios[raioP1][pontoRaioP1]->y = (fator1*raiosIdeal[raioP1][pontoRaioP1]->y + fator2*raiosIdeal[raioP2][pontoRaioP2]->y)/2;
//      raios[raioP2][pontoRaioP2]->x = (fator2*raiosIdeal[raioP1][pontoRaioP1]->x + fator1*raiosIdeal[raioP2][pontoRaioP2]->x)/2;
//      raios[raioP2][pontoRaioP2]->y = (fator2*raiosIdeal[raioP1][pontoRaioP1]->y + fator1*raiosIdeal[raioP2][pontoRaioP2]->y)/2;
//    }
//  }
//}
//
//int div(float dividendo, float divisor) {
//  int c=0;
//  while (dividendo>divisor) {
//    dividendo-=divisor;
//    c++;
//  }
//  return c;
//}
//
//void mousePressed() {
//  background(0);
//  iniciaTeia();
//}
//
//void keyPressed() {
//  constroiEspiral();
//}
//
