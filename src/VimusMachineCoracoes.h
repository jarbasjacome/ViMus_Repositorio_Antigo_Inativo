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

#ifndef _VIMUSMACHINECORACOES_H_
#define _VIMUSMACHINECORACOES_H_

#include "VimusMachineOpenGLObject.h"
#include "Freenect.h"
#include "OpenALSamplerCoracoes.h"
#include "PVector.h"

#include "GL/glut.h"
#include <boost/thread.hpp>
#include <boost/thread/xtime.hpp>
#include <boost/math/constants/constants.hpp>

class Coracao {

public:

  static const int ULTIMA_GERACAO = 10;
  static const int NUM_MAX_CORACOES = 1025; //deve ser 2^ULTIMA_GERACAO + 1

  static const float ATRITO = 0.6;
  static const float MOLA = 0.05;
  static const float LARGURA_INICIAL = 500.0;
  static const float ALTURA_INICIAL = 500.0;
  static const float DIMINUICAO = 0.8;
  static const float VEL_INICIAL = 8.0;
  static const float VEL_FRENESI = 4;
  static const float VEL_GIRO = 0.2;

  static const float TWO_PI = 2*3.1415926535897932384;
  static const float PI = 3.1415926535897932384;
  static const float HALF_PI = 3.1415926535897932384/2;

    boost::xtime tempoAtual;
    boost::xtime tempoBatimento;
    double tempoPassadoMSegs;
    float batimento;

  GLfloat ctrlpoints[4][3];

  GLfloat ctrlpointsESQ[4][3];

  //indica o quanto o coração está se dividindo
  float divisao;

  //guarda o estado atual do coração
  bool dividindo;

  //flag para divisão
  bool dividiu;

  //guarda o ângulo que indica a direção na qual o coração irá se dividir
  float anguloDiv;

  float posX;
  float posY;
  float largura;
  float altura;
  float velX;
  float velY;

  float velGiro;

  bool ativo;

  int geracao;

//  static const int width = 800;
//  static const int height = 1280;

    static const int width = 1280;
    static const int height = 800;

    Coracao () {
        GLfloat ctrlp[4][3] = {
            { 436, 342, 0.0}, { 627, 66, 0.0},
            {978, 514, 0.0}, {417, 736, 0.0}};
        for(int i=0; i<4;i++){
            for(int j=0; j<3; j++){
                ctrlpoints[i][j] = ctrlp[i][j];
            }
        }
        GLfloat ctrlpESQ[4][3] = {
            { 417, 736, 0.0}, { 273, 768, 0.0},
            {204, 217, 0.0}, {436, 342, 0.0}};
        for(int i=0; i<4;i++){
            for(int j=0; j<3; j++){
                ctrlpointsESQ[i][j] = ctrlpESQ[i][j];
            }
        }
        inicia();
    }

    void inicia(){
        batimento=random(0,1);
        dividiu=false;
        divisao=0;
        dividindo=false;
        ativo=false;
        velX = velY = 0;
        anguloDiv = 0;
    }

    float random(float menor, float maior){
        float r = menor + (float)rand()/((float)RAND_MAX/(maior-menor));
        return r;
    }

    float r() {
      //return random (-2,2);
      return random(-1, 1);
      //return 0;
    }

  void atualiza() {
    if (ativo) {

        boost::xtime_get(&(this->tempoAtual), boost::TIME_UTC);
        this->tempoPassadoMSegs = (this->tempoAtual.nsec - this->tempoBatimento.nsec) / 1000000.0f;
        this->tempoPassadoMSegs += (this->tempoAtual.sec - this->tempoBatimento.sec)*1000;
        if (tempoPassadoMSegs > 10) {
            boost::xtime_get(&(this->tempoBatimento), boost::TIME_UTC);
            batimento+=0.03;
            if (batimento>1) {
                batimento=0;
            }
        }


      posX += velX;
      posY += velY;
      if (velX>0) {
        velX -= ATRITO;
        if (velX<0) velX=0;
      }
      else if (velX<0) {
        velX += ATRITO;
        if (velX>0) velX=0;
      }
      if (velY>0) {
        velY -= ATRITO;
        if (velY<0) velY=0;
      }
      else if (velY<0) {
        velY += ATRITO;
        if (velY>0) velY=0;
      }

      if (dividindo) {
        divisao+=largura/4;
        if (divisao>largura) {
          divisao=0;
          dividindo=false;
          dividiu=true;
          ativo = false;
        }
      }
      else {
        anguloDiv+=velGiro;
        velGiro+=random(-0.01, 0.01);
        if (velGiro > 0) {
          velGiro-=0.01;
          if (velGiro<0) velGiro=0;
        }
        if (velGiro < 0) {
          velGiro+=0.01;
          if (velGiro>0) velGiro=0;
        }
        if (geracao==ULTIMA_GERACAO) {
          if (anguloDiv > 0) {
            anguloDiv-=0.02;
            if (anguloDiv<0) anguloDiv=0;
          }
          if (anguloDiv < 0) {
            anguloDiv+=0.02;
            if (anguloDiv>0) anguloDiv=0;
          }
        }
      }
    }
  }

  void desenha() {
    if (ativo) {
      glPushMatrix();
      glEnable(GL_MAP1_VERTEX_3);
      glTranslatef(posX, posY, 0.0);
      glRotatef(anguloDiv*360/TWO_PI, 0, 0, 1);
      glScalef(1+divisao/largura, 1, 1);
      glScalef((largura+r())/500, (altura+r())/500, 1);
      if (batimento<0.5) {
        glScalef(1+fabs(sin(2*batimento*TWO_PI))/15,1+fabs(sin(2*batimento*TWO_PI))/15,1);
      }
      glTranslatef(-1000/2, -1000/2, 0.0);
      glLineWidth(3);
      glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpoints[0][0]);
      glBegin(GL_LINE_STRIP);
      for (int i = 0; i <= 30; i++)
         glEvalCoord1f((GLfloat) i/30.0);
      glEnd();
      glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpointsESQ[0][0]);
      glBegin(GL_LINE_STRIP);
      for (int i = 0; i <= 30; i++)
         glEvalCoord1f((GLfloat) i/30.0);
      glEnd();
      glPopMatrix();
    }
  }

  void divide() {
    if (dividindo==false) {
      dividindo=true;
      divisao=0;
      anguloDiv = random(-1*HALF_PI, HALF_PI);
    }
  }

  void nasce(float x, float y, float l, float a, float vX, float vY, float angulo, float vGiro, int ger) {
    inicia();
    geracao=ger;
    posX = x;
    posY = y;
    largura = l;
    altura = a;
    velX = vX;
    velY = vY;
    velGiro = vGiro;
    anguloDiv = angulo;
    dividiu=false;
    ativo = true;
  }
};

/**
 * VimusMachineCoracoes desenha um coração que se multiplica em vários
 * a medida em que o visitante se aproxima do kinect. Esta instalação
 * foi desenvolvida para a exposição GIL70, inaugurada em agosto de
 * 2012 no Rio de Janeiro.
 *
 * @author  Jarbas Jácome
 * @author  Adriana Calcanhoto
 * @author  Aline Brune
 * @author  André Vallias
 * @date    19.08.2012
 */
class VimusMachineCoracoes : public VimusMachineOpenGLObject
{
    public:

        /**
         * VimusMachineCoracoes default constructor.
         */
        VimusMachineCoracoes(MyFreenectDevice* kin);

        /**
         * VimusMachineCoracoes destructor.
         */
        ~VimusMachineCoracoes();

        /**
         * Update VimusMachineCoracoes.
         */
        void update();

        /**
         * Update VimusMachineCoracoes.
         */
        void draw();

        /**
         * Update VimusMachineCoracoes.
         */
        void draw(int rendermode);

		/**
		 * Get pointer to currentFrame buffer pointer.
		 * This is necessary for GUI have a direct access to
		 * machine frame buffer.
		 */
		unsigned char** getCurrentFramePointer();

		/**
		 * Do some action before and/or after connect a pin to
		 * another objects pin.
		 */
		bool connectOutput (int outPin, VimusMachineObject * dstObj, int inPin);

		/**
		 * Do some action before and/or after be connected by another
		 * objects pin.
		 */
		bool connectInput (VimusMachineObject * srcObj, int outPin, int inPin);

		/**
		 * Do some action before and/or after disconnect an output pin from
		 * another objects pin.
		 */
		void disconnectOutput (int outPin, VimusMachineObject * dstObj, int inPin);

		/**
		 * Do some action before and/or after disconnect an input pin from
		 * another objects pin.
		 */
		void disconnectInput (VimusMachineObject * srcObj, int outPin, int inPin);

        void keyBoardFunc(unsigned char key, int x, int y);

		void specialKeyBoardFunc(int key, int x, int y);


    private:

		unsigned char** ppInputData;

		unsigned char** ppOutputData;

		unsigned char** ppNullFrame;

        OpenALSamplerCoracoes* audioSampler;
        float volume;

        boost::xtime tempoAtual;
        boost::xtime tempoInicio;
        //guarda o momento em segundos que ocorreu o último pulso de frenesi
        boost::xtime tempoFrenesi;
        double tempoPassadoMSegs;

        static const float TWO_PI = 2*3.1415926535897932384;
        static const float PI = 3.1415926535897932384;
        static const float HALF_PI = 3.1415926535897932384/2;

		MyFreenectDevice* kinect;

		GLuint gl_depth_tex;

        float kinectX;
        float kinectY;
        float kinectZ;

        static const float DIST_MAX = 130;
        static const float DIST_MIN = 70;

        bool preview;

        unsigned char frameAnterior[640*480];

        double kinectAngulo;

        //define intervalo de tempo em segundos entre pulso de frenesi e outro
        static const float INTERVALO_FRENESI = 20;

        float opacidade;

        Coracao* c;

        Coracao* coracoes[Coracao::NUM_MAX_CORACOES];

        int numCoracoes;

        int geracao;

        int geracaoFinal;

        float random(float maior, float menor);

        float dist(float x1, float y1, float x2, float y2);

        float r();

        void divideCoracoes();

        void reinicia();

};

#endif //_VIMUSMACHINECORACOES_H_
