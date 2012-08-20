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

  static const int ULTIMA_GERACAO = 9;
  static const int NUM_MAX_CORACOES = 513; //deve ser 2^ULTIMA_GERACAO + 1

  static const float ATRITO = 0.6;
  static const float MOLA = 0.07;
  static const float LARGURA_INICIAL = 300;
  static const float ALTURA_INICIAL = 300;
  static const float DIMINUICAO = 0.8;
  static const float VEL_INICIAL = 8;
  static const float VEL_FRENESI = 4;
  static const float VEL_GIRO = 0.2;

  static const float TWO_PI = 2*3.1415926535897932384;
  static const float PI = 3.1415926535897932384;
  static const float HALF_PI = 3.1415926535897932384/2;

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

  Coracao () {
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
      return random(-0.4, 0.4);
      //return 0;
    }

    int div(float dividendo, float divisor) {
      int c=0;
      while (dividendo>divisor) {
        dividendo-=divisor;
        c++;
      }
      return c;
    }

  void atualiza() {
    if (ativo) {
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
      glTranslatef(posX, posY, 0.0);
      glRotatef(anguloDiv*360/TWO_PI, 0, 0, 1);
      glScalef(1+divisao/largura, 1, 1);
      glColor4f(1, 0, 0, 1);
      float ranLarg = r();
      float ranAlt = r();
      glColor4f(1, 0, 0, 1);
      //glutWireCube(largura);
      glBegin(GL_LINE_STRIP);
          glVertex3f( largura*0.5+ranLarg,-altura*0.5+ranAlt, 0);
          glVertex3f( largura*0.5+ranLarg, altura*0.5+ranAlt, 0);
          glVertex3f(-largura*0.5+ranLarg, altura*0.5+ranAlt, 0);
          glVertex3f(-largura*0.5+ranLarg,-altura*0.5+ranAlt, 0);
      glEnd();
      glPopMatrix();
    }
  }

  void divide() {
    if (dividindo==false) {
      dividindo=true;
      divisao=0;
      anguloDiv = random(-HALF_PI, HALF_PI);
    }
  }

  void nasce(float x, float y, float l, float a, float vX, float vY, float angulo, float vGiro, int ger) {
    geracao=ger;
    posX = x;
    posY = y;
    largura = l;
    altura = a;
    velX = vX;
    velY = vY;
    velGiro = vGiro;
    anguloDiv = angulo;
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

        bool preview;

        unsigned char frameAnterior[640*480];

        double kinectAngulo;

        //define intervalo de tempo em segundos entre pulso de frenesi e outro
        static const float INTERVALO_FRENESI = 50;

        float opacidade;

        Coracao* coracoes[Coracao::NUM_MAX_CORACOES];

        int numCoracoes;

        int geracao;

        static const int width = 800;
        static const int height = 600;

        float random(float maior, float menor);

        float dist(float x1, float y1, float x2, float y2);

        float r();

        int div(float dividendo, float divisor);

        void divideCoracoes();

};

#endif //_VIMUSMACHINECORACOES_H_
