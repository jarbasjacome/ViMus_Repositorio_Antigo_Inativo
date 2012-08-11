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

#ifndef _VIMUSMACHINEORIENTE_H_
#define _VIMUSMACHINEORIENTE_H_

#include "VimusMachineOpenGLObject.h"
#include "Freenect.h"
#include "OpenALSamplerOriente.h"

#include "GL/glut.h"
#include <boost/thread.hpp>
#include <boost/thread/xtime.hpp>
#include <boost/math/constants/constants.hpp>

class PVector {
public:
    float x;
    float y;
    PVector(float posX, float posY) {
        x=posX;
        y=posY;
    }
    ~PVector(){}
private:
};

/**
 * VimusMachineOriente desenha uma teia de aranha de acordo com o movimento
 * de uma pessoa capturado pelo kinect. A cada novo nó é tocado um pequeno
 * trecho da música Oriente de Gilberto Gil. Esta instalação foi desenvolvida
 * para a exposição GIL70, inaugurada em agosto de 2012 no Rio de Janeiro.
 *
 * @author  Jarbas Jácome
 * @author  André Vallias
 * @author  Aline Brune
 * @date    05.08.2012
 */
class VimusMachineOriente : public VimusMachineOpenGLObject
{
    public:

        /**
         * VimusMachineOriente default constructor.
         */
        VimusMachineOriente(MyFreenectDevice* kin);

        /**
         * VimusMachineOriente destructor.
         */
        ~VimusMachineOriente();

        /**
         * Update VimusMachineOriente.
         */
        void update();

        /**
         * Update VimusMachineOriente.
         */
        void draw();

        /**
         * Update VimusMachineOriente.
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

        OpenALSamplerOriente* audioSampler;

        int posAudio;
        float incAudio;

        boost::xtime tempoAtual;
        boost::xtime inicioToque;
        double tempoPassadoMSegs;

		MyFreenectDevice* kinect;

		GLuint gl_depth_tex;

        float kinectX;
        float kinectY;

        bool preview;

        unsigned char frameAnterior[640*480];

        double kinectAngulo;

        //define número de linhas diagonais da teia
        //linhas diagonais são as que vão de lado ao outro da base
        static const int NUM_RAIOS = 30;
        //define número de pontos de conexão em cada raio
        static const int NUM_PONTOS_RAIO = 50;
        static const int ESPACAMENTO_ESPIRAL = 15;
        static const float CURVA_ESPIRAL = 0.03;
        //define o número máximo de pontos da espiral principal
        static const int MAX_NUM_PONTOS_ESPIRAL = 5000;
        //define número de pontos da base da teia
        static const int NUM_PONTOS_BASE = 4;
        static const int NUM_PONTOS_BASE_CENTRO = NUM_RAIOS;
        static const int TAM_BASE_CENTRO = 10;
        static const float ABAULAMENTO = 5; //DE 0 A 1
        static const int INICIO_TEIA = 20;
        static const int FIM_TEIA = 1200;
        static const int INTERVALO_CONSTRUCAO = 1000;

        static const float TWO_PI = 2*3.1415926535897932384;
        static const float PI = 3.1415926535897932384;
        static const float HALF_PI = 3.1415926535897932384/2;

        //armazena os pontos da espiral principal (a que tem a cola
        //para pegar os insetos).
        //na verdade estes arrays não armazenam os pontos diretamente
        //mas sim, o índice do raio e do ponto no raio onde lá sim estão
        //as coordenadas x e y do ponto.
        //essa indireção é importante para fazer com que os espirais fiquem
        //sempre corretamente "colados" nos raios.
        PVector* espiral[MAX_NUM_PONTOS_ESPIRAL];

        //armazena o estado do desenho do espiral para cada ponto em relaçãconstroiEspiralRandom ()o ao próximo
        //-1 significa que não deve conectar
        //0 significa que ainda não conectou
        //1 a 100 significa que já conectou, uma linha da teia deve ser desenhada, o valor
        //indica a porcentagem de esticamento desse trecho de teia.
        //Inicia com 1% e com o passar do tempo, chega a 100%, condição normal.
        int desenhandoEspiral[MAX_NUM_PONTOS_ESPIRAL];

        int naoLigados[MAX_NUM_PONTOS_ESPIRAL];

        //armazena os pontos de cada raio.
        PVector* raiosIdeal[NUM_RAIOS][NUM_PONTOS_RAIO];

        //armazena os pontos de cada raio na posição de descanso,
        //isto é, quando não estão sendo puxados pela aranha.
        PVector* raios[NUM_RAIOS][NUM_PONTOS_RAIO];

        //armazena os pontos da base da teia
        PVector* pontosBase[NUM_PONTOS_BASE];

        PVector* pontosBaseCentro[NUM_PONTOS_BASE_CENTRO];

        //armazena os angulos em relação ao eixo x que formam
        //linhas do centro até cada ponto base
        //essa informação é importante para traçar os raios do centro
        //até os segmentos de reta que ligam os pontos base da teia
        float angulosPontosBase[NUM_PONTOS_BASE];

        float angulosPontosBaseCentro[NUM_PONTOS_BASE_CENTRO];

        //armazena os ângulos que os raios formam em relação ao eixo x
        float angulosRaios[NUM_RAIOS];

        //armazena o tamanho dos raios
        float tamanhoRaios[NUM_RAIOS];

        //marca posição atual da aranha
        PVector* aranha;

        int tamEspiral;

        //centro da teia
        PVector* centro;

        static const int width = 1280;

        static const int height = 800;

        int ultimaConstrucao;



        void iniciaTeia();

        float random(float maior, float menor);

        float angulo(float x, float y, float xC, float yC);

        float dist(PVector* p1, PVector* p2);

        float dist(float x1, float y1, float x2, float y2);

        void calculaPontoRaioBase (PVector** retorno,float angulo, float cX, float cY);

        void calculaSegmentoBase (PVector** retorno, float angulo);

        void calculaPontoInterseccaoRetas (PVector** retorno, float angulo, float pX, float pY,
                float x1, float y1, float x2, float y2);

        void definePontosEspiral();

        bool dentroBase(float x, float y);

        void colaEspiral(int raio, int pontoRaio);

        void constroiEspiral();

        void constroiEspiralRandom();

        int div(float dividendo, float divisor);

        void desenhaTeia();

        void atualizaPontos();

        void line(float x1, float y1, float x2, float y2);

        float r();

        void desenhaArco(float p1x, float p1y, float p2x, float p2y, float abaula, bool cima);

        float arc (float x, float y, float raio, float inicio, float fim);

};

#endif //_VIMUSMACHINEORIENTE_H_
