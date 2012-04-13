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
#include <boost/thread/xtime.hpp>

#include "OpenALSampler.h"
#include "OpenALCapture.h"

#include <iostream>

#include <math.h>

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

        void keyBoardFunc(unsigned char key, int x, int y);
        void specialKeyBoardFunc(int key, int x, int y);

    private:

		unsigned char** ppInputData;

		unsigned char** ppOutputData;

		unsigned char** ppNullFrame;

        const static int NUM_VIDEOS = 14;

        cv::VideoCapture video[NUM_VIDEOS];

        bool videoPlaying;

        int currVideo;

        cv::Mat frame;
        cv::Mat frameDest;

		unsigned char* capturedFrame;
		unsigned char* distorcedFrame;

		int videoEffect;
		const static int VIDEO_EFFECT_OFF = 0;
		const static int VIDEO_EFFECT_RED = 1;
		const static int VIDEO_EFFECT_CONTRAST_GAIN = 2;
		const static int VIDEO_EFFECT_CONTRAST_GAIN_INV = 3;
		const static int VIDEO_EFFECT_CONTRAST_TEMPO = 4;
		const static int VIDEO_EFFECT_CONTRAST_TEMPO_INV = 5;
		const static int VIDEO_EFFECT_WAVE = 6;

//		const static int VIDEO_EFFECT_BRIGHTNESS = 5;
//		const static int VIDEO_EFFECT_BRIGHTNESS_INV = 6;

//555555555
//
//jaj
//55
//
//
//
//555

		int lastVideoEffect;
		int currEffect;
		const static int EFFECT_OFF = 0;
		const static int EFFECT_GAIN = 1;
		const static int EFFECT_GAIN_INV = 2;
		const static int EFFECT_TEMPO = 3;
		const static int EFFECT_TEMPO_INV = 4;
		const static int NUM_EFFECTS = 5;

		ALfloat currPitch;

		int waveDrawer;
		const static int WAVE_DRAWER_OFF = 0;
		const static int WAVE_DRAWER_LINES = 1;
		const static int WAVE_DRAWER_LINES_3D = 2;
		const static int WAVE_DRAWER_CIRCLES = 3;
		const static int WAVE_DRAWER_CIRCLES_3D = 4;
		const static int WAVE_DRAWER_CIRCLES_FLOWER = 5;
		const static int WAVE_DRAWER_CIRCLES_FLOWER_3D = 6;
		const static int NUM_OF_DRAWER_EFFECTS = 7;

        GLuint texName;

        float camZ;

        const static float MAX_ZOOM = 10.5f;
        const static float MIN_ZOOM = 3.0f;

        boost::xtime currSysTime2;
        boost::xtime lastSysTime2;
        boost::xtime startSysTime2;
        double pastTimeMSecs;
        double sampleStartTimeMSecs;

        double timePast;
        int currFrame;

        OpenALSampler * audioSampler;

        int timeStamps[NUM_VIDEOS][20];

        int currMeasure;

        int repeatMode;
        static const int REPEAT_MODE_GROOVE = 0;
        static const int REPEAT_MODE_MEASURE = 1;
        static const int REPEAT_MODE_OFF = 2;
        static const int NUM_OF_REPEAT_MODES = 3;

        int schedMeasureChange;
        int schedGrooveChange;

        OpenALCapture* audioCapture;

		unsigned char contrasTransform [256];

        void playCurrVideo();
        void nextTrack();
        void prevTrack();

        void changeContrast (float bright, float contrast);

        void setCurrVideo(int video);

        /**
         * Renders a stroke font string.
         */
        void renderStrokeFontString(
                float x,
                float y,
                float z,
                void *font,
                const char *lab,
                float stroke);

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

};

#endif //_VIMUSMACHINELANTERNAMAGICA_H_
