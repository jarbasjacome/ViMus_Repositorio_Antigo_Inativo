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

#ifndef _VIMUSMACHINELANTERNAMAGICA_H_
#define _VIMUSMACHINELANTERNAMAGICA_H_

#include "VimusMachineOpenGLObject.h"

#include "cv.h"
#include "highgui.h"

#include "GL/glut.h"

#include <iostream>

/**
 * VimusMachineLanternaMagica is funk soul brother.
 *
 * @author  Jarbas Jácome
 * @date    05.22.2011
 */
class VimusMachineLanternaMagica : public VimusMachineOpenGLObject
{
    public:

        /**
         * VimusMachineLanternaMagica default constructor.
         */
        VimusMachineLanternaMagica();

        /**
         * VimusMachineLanternaMagica destructor.
         */
        ~VimusMachineLanternaMagica();

        /**
         * Update VimusMachineLanternaMagica.
         */
        void update();

        /**
         * Draw VimusMachineLanternaMagica.
         */
        void draw();

        /**
         * Draw VimusMachineLanternaMagica.
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

        /**
         * TODO: PUT THIS OUT OF THIS OBJECT!
         * Renders a bitmap string.
         */
        void renderBitmapString(
                float x,
                float y,
                float z,
                void *font,
                std::ostringstream * strStream);

    private:

		unsigned char** ppInputData;

		unsigned char** ppOutputData;

		unsigned char** ppNullFrame;

        cv::VideoCapture video;

        cv::Mat frame;
        cv::Mat frameDest;

		unsigned char* capturedFrame;

        GLuint texName;

        float camZ;

        const static float MAX_ZOOM = 10.5f;
        const static float MIN_ZOOM = 3.0f;

};

#endif //_VIMUSMACHINELANTERNAMAGICA_H_
