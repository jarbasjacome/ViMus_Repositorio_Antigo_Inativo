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

#ifndef VIDEOCAPTUREOPENCV_H
#define VIDEOCAPTUREOPENCV_H

#include "Configuration.h"

#include "cv.h"
#include "highgui.h"

#include <boost/thread.hpp>
#include <boost/thread/xtime.hpp>

#include <iostream>

/**
 * This class encapsulates all OpenCV video capture code.
 *
 * @author Jarbas Jácome
 */
class VideoCaptureOpenCV
{
    public:

        /**
         * VideoCaptureOpenCV default constructor.
         */
        VideoCaptureOpenCV();

        /**
         * VideoCaptureOpenCV destructor.
         */
        ~VideoCaptureOpenCV();

        /**
         * Initializes VideoCaptureOpenCV
         */
        void init();

		/**
		 * Get pointer to currentFrame buffer pointer.
		 * This is necessary for GUI to have a direct access to
		 * machine frame buffer.
		 */
		unsigned char* getCurrentFrame(int dev);

        /**
         * Get next avaiable Capture device number.
         */
		int getNextCaptureDevice();

    private:

		// used to make possible to call non-static member functions
		// to use in threads
		static VideoCaptureOpenCV* vidCapOpenCvPtr;

		int nextCapDev;

        cv::VideoCapture* videoCapDevices[10];

        cv::Mat frame[10];
        cv::Mat frameDst[10];
        cv::Mat frameDstSwap[10];

        bool swapBufferOn;

		unsigned char* capturedFrame[10];

		unsigned char* redFrame;

        void grabCapturedFrame(int dev);

        bool flagGrabing;

        void grabThreadFunc();
		inline static void grabThreadFuncStatic()
		{	vidCapOpenCvPtr->grabThreadFunc();	};
        boost::thread* grabThread;
        boost::xtime sleepGrab;
};
#endif // VIDEOCAPTUREOPENCV_H
