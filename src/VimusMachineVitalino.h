/**
 * Copyright 2007-2011 Jarbas Jácome
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

#ifndef _VIMUSMACHINEVITALINO_H_
#define _VIMUSMACHINEVITALINO_H_

#include "VimusMachineOpenGLObject.h"

#include "GL/glut.h"
#include <boost/thread.hpp>
#include <boost/thread/xtime.hpp>

/**
 * VimusMachineVitalino is a class built for 3D sculptures modeling
 * using extrusion intersection of two video inputs positioned
 * perpendicularly.
 *
 * @author  Jarbas Jácome
 * @date    03.06.2010
 */
class VimusMachineVitalino : public VimusMachineOpenGLObject
{
    public:

        /**
         * VimusMachineVitalino default constructor.
         */
        VimusMachineVitalino();

        /**
         * VimusMachineVitalino destructor.
         */
        ~VimusMachineVitalino();

        /**
         * Update VimusMachineVitalino.
         */
        void update();

        /**
         * Update VimusMachineVitalino.
         */
        void draw();

        /**
         * Update VimusMachineVitalino.
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

		unsigned char** ppInputData[2];

		unsigned char** ppOutputData;

		unsigned char** ppNullFrame;

		int* pontos2;
		int numPontos2;

		const static int BARRO_WIDTH = 64;
		const static int BARRO_HEIGHT = 64;
		const static int BARRO_DEPTH = 64;

		float rotInc;

		bool barro[BARRO_WIDTH][BARRO_HEIGHT][BARRO_DEPTH][2];

		int numVoxels;

        boost::xtime xtimeRecomecar;

        GLfloat light_ambient[4];
        GLfloat light_diffuse[4];
        GLfloat light_specular[4];
        GLfloat light_position[4];

        GLfloat mat_ambient[4];
        GLfloat mat_diffuse[4];
        GLfloat mat_specular[4];
        GLfloat high_shininess[1];

        void drawVoxel(int raz, float adj, float x, float y, float z, float color);

};

#endif //_VIMUSMACHINEVITALINO_H_
