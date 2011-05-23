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

#ifndef _VIMUSMACHINECVBLOBDETECTION_H_
#define _VIMUSMACHINECVBLOBDETECTION_H_

#include "VimusMachineOpenGLObject.h"

#include "cv.h"
#include "highgui.h"

#include "GL/glut.h"

#include <iostream>

/**
 * VimusMachineCVBlobDetection calls OpenCV cvBlobsLib blob detection function.
 *
 * @author  Jarbas Jácome
 * @date    06.01.2011
 */
class VimusMachineCVBlobDetection : public VimusMachineOpenGLObject
{
    public:

        /**
         * VimusMachineCVBlobDetection default constructor.
         */
        VimusMachineCVBlobDetection();

        /**
         * VimusMachineCVBlobDetection destructor.
         */
        ~VimusMachineCVBlobDetection();

        /**
         * Update VimusMachineCVBlobDetection.
         */
        void update();

        /**
         * Draw VimusMachineCVBlobDetection.
         */
        void draw();

        /**
         * Draw VimusMachineCVBlobDetection.
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

        void keyBoardFunc(unsigned char key, int x, int y);
		void specialKeyBoardFunc(int key, int x, int y);


    private:

		unsigned char** ppInputData;

		unsigned char** ppOutputData;

		unsigned char** ppNullFrame;

		cv::Mat frame;
		cv::Mat frameDst;

        std::ostringstream infoStream;
        std::string infoString;

        cv::Mat frameGray;
        cv::Mat frameThresh;

        std::vector<std::vector<cv::Point> > v;

        static const int MAX_NUM_BLOBS = 30;
        cv::Point blobs[MAX_NUM_BLOBS];
        cv::Point blobsSwap[MAX_NUM_BLOBS];
        bool blobsId[MAX_NUM_BLOBS];
        bool blobsIdSwap[MAX_NUM_BLOBS];

        float distance;
        float distanceOrigin;

        float camZ;
        float camZOrigin;

        const static float MAX_ZOOM = 10.5f;
        const static float MIN_ZOOM = 3.0f;

        const static int AVERAGE_ARRAY_SIZE = 20;
        // this value determines how smooth will zoom and move object

        // array to accumulate zoom values to make its move smooth
        float zoomArray[AVERAGE_ARRAY_SIZE];
        // store last sum of zoom values for next average position
        float zoomLastSum;

        float posX;
        float posY;

        float posXorigin;
        float posYorigin;

        // array to accumulate object positions values to make its move smooth
        float posXarray[AVERAGE_ARRAY_SIZE];
        float posYarray[AVERAGE_ARRAY_SIZE];

        // store last sum of positions for next average position
        float posXlastSum;
        float posYlastSum;

        int numBlobs;

        const static int NUM_MAPS = 5;

        cv::Mat mapa[NUM_MAPS];
        cv::Mat mapaResized[NUM_MAPS];
        unsigned char * mapaData[NUM_MAPS];
        float mapWidth[NUM_MAPS];
        float mapHeight[NUM_MAPS];

        GLuint texNames[NUM_MAPS];

        float slideToLeft;
        float slideToRight;

        int state;
        const static int STATE_SLIDING_RIGHT = 0;
        const static int STATE_SLIDING_LEFT = 1;
        const static int STATE_USER_INPUT = 2;
        const static int NUM_OF_STATES = 3;

        int currentMap;
        int lastMap;

        float mapsPosition;

        // minimum distance needed to consider a slide gesture.
        const static float SLIDE_VALUE = 0.2f;

        const static float SPACE_BEETWEEN_MAPS = 0.8f;

        const static int MIN_BLOB_SIZE = 20;

        bool blobPressed;

        cv::Point origin1;
        int originId1;

        cv::Point origin2;
        int originId2;

        cv::Point p1Origin;
        cv::Point p2Origin;

        /**
         * Treat user blobs gestures.
         * Calculates zoom and translation based on one blob position and
         * two blobs distance.
         */
        void treatBlobsGestures();

        /**
         * TODO: PUT THIS OUT OF THIS OBJECT!
         * Renders a bitmap string.
         */
        void drawInfo ( cv::Mat frame,
                        const char* infoName,
                        int infoValue,
                        cv::Point pt,
                        std::ostringstream* infoStream,
                        std::string* infoString );

        /**
         * Calculates the posX and posY limits based on zoom value.
         * If zoom is high, user can move more.
         */
        void checkLimits();

        /**
         * Moves camera softly to a new position.
         */
        void moveTo(float x, float y);

        void setState(int state);

        void updateState();

        void resetPosArrays();
};

#endif //_VIMUSMACHINECVBLOBDETECTION_H_
