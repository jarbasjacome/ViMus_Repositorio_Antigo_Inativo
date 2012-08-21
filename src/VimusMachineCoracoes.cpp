/**
 * Copyright 2007-2012 Jarbas Jácome
 *
 * This file is part of ViMus.
 *
 * ViMus is free software: you can redistribute it and/or modify
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

#include "VimusMachineCoracoes.h"

using namespace std;

/**
 * VimusMachineCoracoes default constructor.
 */
VimusMachineCoracoes::VimusMachineCoracoes(MyFreenectDevice* kin)
{
    VimusMachineObject::initObject();

	this->label = string ("coracoes");
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

    srand((unsigned)time(0));

    this->kinect = kin;

    preview = false;

	glGenTextures(1, &gl_depth_tex);
	glBindTexture(GL_TEXTURE_2D, gl_depth_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    audioSampler = new OpenALSamplerCoracoes();

    kinectAngulo=0;
    kinect->setTiltDegrees(kinectAngulo);

    //instancia a classe Coracao em um array de 200 objetos.
    for (int i=0; i<Coracao::NUM_MAX_CORACOES; i++) {
        coracoes[i]= new Coracao();
    }

    coracoes[0]->nasce(Coracao::width/2, Coracao::height/2, Coracao::LARGURA_INICIAL, Coracao::ALTURA_INICIAL, 0, 0, 0, 0, 0);
    numCoracoes = 1;
    geracao=1;
    geracaoFinal=1;
    opacidade = 1;
    volume = 1;

    boost::xtime_get(&(this->tempoInicio), boost::TIME_UTC);

    if (DEBUG_MODE)
        cout << "\nVimusMachineCoracoes constructed.";
}

/**
 * VimusMachineCoracoes destructor.
 */
VimusMachineCoracoes::~VimusMachineCoracoes()
{
}

/**
 * Update VimusMachineCoracoes.
 */
void VimusMachineCoracoes::update()
{
    boost::xtime_get(&(this->tempoAtual), boost::TIME_UTC);
    this->tempoPassadoMSegs = (this->tempoAtual.nsec - this->tempoInicio.nsec) / 1000000.0f;
    this->tempoPassadoMSegs += (this->tempoAtual.sec - this->tempoInicio.sec)*1000;
    if (tempoPassadoMSegs > 5000) {
        geracaoFinal = Coracao::ULTIMA_GERACAO*(1 - (kinectZ-DIST_MIN) / (DIST_MAX-DIST_MIN));
        if (geracaoFinal>Coracao::ULTIMA_GERACAO) {
            geracaoFinal=Coracao::ULTIMA_GERACAO;
        }
    }

    if (geracao<geracaoFinal) {
        if (!coracoes[numCoracoes-1]->dividindo) {
            divideCoracoes();
        }
    }

    if (geracao == Coracao::ULTIMA_GERACAO) {
        if(audioSampler->getSecondOffset(0)==0){
            audioSampler->playSample(0);
        }

        boost::xtime_get(&(this->tempoAtual), boost::TIME_UTC);
        this->tempoPassadoMSegs = (this->tempoAtual.nsec - this->tempoFrenesi.nsec) / 1000000.0f;
        this->tempoPassadoMSegs += (this->tempoAtual.sec - this->tempoFrenesi.sec)*1000;
        if (tempoPassadoMSegs > INTERVALO_FRENESI) {
            for (int i=0; i<numCoracoes; i++) {
                coracoes[i]->velX += random (-1*Coracao::VEL_FRENESI, Coracao::VEL_FRENESI);
                coracoes[i]->velY += random (-1*Coracao::VEL_FRENESI, Coracao::VEL_FRENESI);
            }

            if (geracaoFinal<geracao)  {
                opacidade-=0.005;
                if (opacidade<0) {
                    opacidade = 0;
                    audioSampler->stopSample(0);
                    reinicia();
                }
            } else {
                opacidade+=0.005;
                if (opacidade>1) {
                    opacidade = 1;
                }
            }
            volume = opacidade;
            audioSampler->setGain(0, volume);

            boost::xtime_get(&(this->tempoFrenesi), boost::TIME_UTC);
        }
    } else {
        if(audioSampler->getSecondOffset(0)!=0){
            audioSampler->stopSample(0);
            audioSampler->setPlaybackPos(0, 0);
        }
    }
}

/**
 * Update VimusMachineCoracoes.
 */
void VimusMachineCoracoes::draw(int rendermode)
{
}

/**
 * Update VimusMachineCoracoes.
 */
void VimusMachineCoracoes::draw()
{
    static vector<uint8_t> depth(640*480);

	//TODO: colocar em threads do boost
	kinect->updateState();

	kinect->getDepth(depth);

    kinect->got_frames = 0;

    int areaSensivel = 20;
    float distMedia=0;
    for(int i=480/2-areaSensivel; i<480/2+areaSensivel; i++){
        for(int j=640/2-areaSensivel; j<640/2+areaSensivel; j++){
            if (depth[i*640+j] > 255) {
                distMedia+=255;
            } else {
                distMedia+=depth[i*640+j];
            }
            //depth[i*640+j] += 30;
        }
    }
    distMedia= distMedia/(areaSensivel*areaSensivel*4);

    kinectZ = distMedia;

    //cout << "distMedia = " << distMedia;

	glPushMatrix();

    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();

    gluLookAt (0, 0, 10.0f, 0, 0, 10.0f - 5.0f, 0.0, 1.0, 0.0);

    glClearColor (0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //glRotatef(90, 0, 0, 1);

    if (preview) {

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

    }

    glPushMatrix();
    glEnable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
    glTranslatef(-1, 1, 0);
    glScalef(2.0f/Coracao::width, -2.0f/Coracao::height, 1);
    glColor4f(1, 0, 0, opacidade);

    //Detecta colisões entre os corações
    float distMinima;
    float distancia;
    float dX;
    float dY;
    float angulo;
    float targetX;
    float targetY;
    float ax;
    float ay;
    for (int i=0; i<numCoracoes; i++) {
        if (coracoes[i]->ativo) {
          for (int j=i; j<numCoracoes; j++) {
            distMinima = coracoes[i]->largura/2 + coracoes[j]->largura/2;
            distancia = dist(coracoes[i]->posX, coracoes[i]->posY, coracoes[j]->posX, coracoes[j]->posY);
            if (i!=j && coracoes[j]->ativo && distancia<distMinima) {
              //retirado do exemplo do Processing Bouncy Bubbles
              dX = coracoes[j]->posX-coracoes[i]->posX;
              dY = coracoes[j]->posY-coracoes[i]->posY;
              angulo = atan2(dY, dX);
              targetX = coracoes[i]->posX + cos(angulo) * distMinima;
              targetY = coracoes[i]->posY + sin(angulo) * distMinima;
              ax = (targetX - coracoes[i]->posX) * Coracao::MOLA;
              ay = (targetY - coracoes[i]->posY) * Coracao::MOLA;
              coracoes[i]->velX -= ax;
              coracoes[i]->velY -= ay;
              coracoes[j]->velX += ax;
              coracoes[j]->velY += ay;
              coracoes[i]->velGiro = random(-1*Coracao::VEL_GIRO, Coracao::VEL_GIRO);
              coracoes[j]->velGiro = random(-1*Coracao::VEL_GIRO, Coracao::VEL_GIRO);
            }
          }
        }
        coracoes[i]->atualiza();

        c=coracoes[i];

        if (c->dividiu) {
            c->dividiu = false;
            coracoes[numCoracoes]->nasce(c->posX-c->largura*cos(c->anguloDiv)/2, c->posY-c->largura*sin(c->anguloDiv)/2,
                            c->largura*Coracao::DIMINUICAO*random(0.95, 1.05),
                            c->altura*Coracao::DIMINUICAO*random(0.95, 1.05),
                            c->velX-Coracao::VEL_INICIAL*cos(c->anguloDiv), c->velY-Coracao::VEL_INICIAL*sin(c->anguloDiv),
                            c->anguloDiv, random(-0.2/2,0.2/2), geracao);
            numCoracoes++;
            coracoes[numCoracoes]->nasce(c->posX+c->largura*cos(c->anguloDiv)/2, c->posY+c->largura*sin(c->anguloDiv)/2,
                            c->largura*Coracao::DIMINUICAO*random(0.95, 1.05),
                            c->altura*Coracao::DIMINUICAO*random(0.95, 1.05),
                            c->velX+Coracao::VEL_INICIAL*cos(c->anguloDiv), c->velY+Coracao::VEL_INICIAL*sin(c->anguloDiv),
                            c->anguloDiv, random(-0.2/2,0.2/2), geracao);
            numCoracoes++;
        }

        coracoes[i]->desenha();
    }


    glPopMatrix();

    glPopMatrix();
}

/**
 * Get pointer to currentFrame buffer pointer.
 * This is necessary for GUI have a direct access to
 * machine frame buffer.
 */
unsigned char** VimusMachineCoracoes::getCurrentFramePointer()
{
	return this->ppOutputData;
}

/**
 * Do some action before and/or after connect a pin to
 * another objects pin.
 */
bool VimusMachineCoracoes::connectOutput (int outPin, VimusMachineObject * dstObj, int inPin)
{
	return dstObj->connectInput(this, outPin, inPin);
}

/**
 * Do some action before and/or after be connected by another
 * objects pin.
 */
bool VimusMachineCoracoes::connectInput (VimusMachineObject * srcObj, int outPin, int inPin)
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
void VimusMachineCoracoes::disconnectOutput (int outPin, VimusMachineObject * dstObj, int inPin)
{
	dstObj->disconnectInput(this, outPin, inPin);
}

/**
 * Do some action before and/or after disconnect an input pin from
 * another objects pin.
 */
void VimusMachineCoracoes::disconnectInput (VimusMachineObject * srcObj, int outPin, int inPin)
{
	srcObj->outputPins[outPin]->disconnect(this->inputPins[inPin]);
	this->ppInputData = this->ppNullFrame;
}

void VimusMachineCoracoes::divideCoracoes() {
  if (geracao<Coracao::ULTIMA_GERACAO) {
    for (int i=0; i<numCoracoes; i++) {
      coracoes[i]->divide();
    }
    geracao++;
    if (geracao == Coracao::ULTIMA_GERACAO) {
      boost::xtime_get(&(this->tempoFrenesi), boost::TIME_UTC);
      opacidade = 1;
      volume = 1;
    }
  }
}

void VimusMachineCoracoes::keyBoardFunc(unsigned char key, int x, int y)
{
    switch (key) {
        case 'y':
            if(preview) {
                preview=false;
            } else{
                preview=true;
            }
            break;
        case ' ':
            geracaoFinal++;
            if (geracaoFinal>Coracao::ULTIMA_GERACAO) {
                geracaoFinal = Coracao::ULTIMA_GERACAO;
            }
            break;
        case 'q':
            reinicia();
            break;
    }
}

void VimusMachineCoracoes::reinicia() {
    for (int i=0; i<Coracao::NUM_MAX_CORACOES; i++) {
        coracoes[i]->inicia();
    }
    coracoes[0]->nasce(Coracao::width/2, Coracao::height/2, Coracao::LARGURA_INICIAL, Coracao::ALTURA_INICIAL, 0, 0, 0, 0, 0);
    numCoracoes = 1;
    geracao=1;
    geracaoFinal=1;
    opacidade = 1;
    volume = 1;
}

void VimusMachineCoracoes::specialKeyBoardFunc(int key, int x, int y)
{
}

float VimusMachineCoracoes::random(float menor, float maior){
    float r = menor + (float)rand()/((float)RAND_MAX/(maior-menor));
    return r;
}

float VimusMachineCoracoes::dist(float x1, float y1, float x2, float y2){
    return sqrt(pow(x2-x1,2)+pow(y2-y1,2));
}

float VimusMachineCoracoes::r() {
  //return random (-2,2);
  return random(-0.4, 0.4);
  //return 0;
}
