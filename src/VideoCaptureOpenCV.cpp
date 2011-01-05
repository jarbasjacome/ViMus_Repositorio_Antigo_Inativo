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

#include "VideoCaptureOpenCV.h"

using namespace cv;

/**
 * VimusCaptureOpenCV singleton object, to make it possible to
 * to call non-static member functions to use in threads
 */
VideoCaptureOpenCV* VideoCaptureOpenCV::vidCapOpenCvPtr =
                                                new VideoCaptureOpenCV();

/**
 * Constructor
 */
VideoCaptureOpenCV::VideoCaptureOpenCV()
{
}

/**
 * Get next avaiable Capture device number.
 */
int VideoCaptureOpenCV::getNextCaptureDevice()
{
    int ret = vidCapOpenCvPtr->nextCapDev;
    vidCapOpenCvPtr->nextCapDev += 1;

    if (DEBUG_MODE)
        cout << "\ngetCaptureDeviceNumber = " << ret;

    return ret;
}

/**
 * Initializes VideoCaptureOpenCV
 */
void VideoCaptureOpenCV::init()
{

    vidCapOpenCvPtr->nextCapDev = 0;

    for (int i=0; i<10; i++)
    {
        vidCapOpenCvPtr->videoCapDevices[i] = NULL;
    }

	vidCapOpenCvPtr->redFrame = new unsigned char[VIDEO_WIDTH*VIDEO_HEIGHT*3];

	for (int i=0; i<VIDEO_WIDTH*VIDEO_HEIGHT; i++)
	{
        vidCapOpenCvPtr->redFrame[i*3] = 255;
        vidCapOpenCvPtr->redFrame[i*3+1] = 255;
        vidCapOpenCvPtr->redFrame[i*3+2] = 255;
	}

    for (int i=0; i<10; i++)
    {
        vidCapOpenCvPtr->capturedFrame[i] =
                                new unsigned char[VIDEO_WIDTH*VIDEO_HEIGHT*3];

        for (int j=0; j<VIDEO_WIDTH*VIDEO_HEIGHT; j++)
        {
            vidCapOpenCvPtr->capturedFrame[i][j*3] =  100;
            vidCapOpenCvPtr->capturedFrame[i][j*3+1] = 100;
            vidCapOpenCvPtr->capturedFrame[i][j*3+2] = 100;
        }
    }

//SEM_CAMERA
/*
	vidCapOpenCvPtr->videoCapDevices[0] = new VideoCapture(0);
	vidCapOpenCvPtr->videoCapDevices[0]->set(CV_CAP_PROP_FRAME_WIDTH, 320);
    vidCapOpenCvPtr->videoCapDevices[0]->set(CV_CAP_PROP_FRAME_HEIGHT, 240);
    vidCapOpenCvPtr->videoCapDevices[0]->grab();

    vidCapOpenCvPtr->videoCapDevices[1] = new VideoCapture(1);
	vidCapOpenCvPtr->videoCapDevices[1]->set(CV_CAP_PROP_FRAME_WIDTH, 320);
    vidCapOpenCvPtr->videoCapDevices[1]->set(CV_CAP_PROP_FRAME_HEIGHT, 240);
    vidCapOpenCvPtr->videoCapDevices[1]->grab();
    boost::thread grabT(VideoCaptureOpenCV::grabThreadFuncStatic);
    vidCapOpenCvPtr->grabThread = &grabT;

    boost::thread resizeT(VideoCaptureOpenCV::resizeThreadFuncStatic);
    vidCapOpenCvPtr->resizeThread = &resizeT;

    vidCapOpenCvPtr->flagGrabing = false;
    vidCapOpenCvPtr->swapBufferOn = false;

    if (DEBUG_MODE)
        cout << "\nVideoCaptureOpenCV started.";
*/

}

/**
 * Destructor
 */
VideoCaptureOpenCV::~VideoCaptureOpenCV()
{
//    vidCapOpenCvPtr->grabThread->join(); //TODO:
}

void VideoCaptureOpenCV::grabThreadFunc()
{
    while(1)
    {
        vidCapOpenCvPtr->flagGrabing = true;

//        for (int i= 0; i<10; i++)
//        {
//            if (vidCapOpenCvPtr->videoCapDevices[i] != 0)
//                vidCapOpenCvPtr->videoCapDevices[i]->grab();
//        }

        vidCapOpenCvPtr->videoCapDevices[0]->grab();
        vidCapOpenCvPtr->videoCapDevices[1]->grab();

        vidCapOpenCvPtr->flagGrabing = false;

//        boost::xtime_get(&(vidCapOpenCvPtr->sleepGrab), boost::TIME_UTC);
//        vidCapOpenCvPtr->sleepGrab.nsec += 10000000;
//        boost::thread::sleep(vidCapOpenCvPtr->sleepGrab);

    }
}

void VideoCaptureOpenCV::resizeThreadFunc()
{
    while(1)
    {
        int check;
        IplImage img;

        vidCapOpenCvPtr->flagResizing = true;

        try
        {
            if (vidCapOpenCvPtr->swapBufferOn)
            {
                int i = 0;
                vidCapOpenCvPtr->videoCapDevices[i]->retrieve(frame[i], 0);
                img = frame[i];
                check = cvCheckArr(&img, 0, 0, 0);
                resize(vidCapOpenCvPtr->frame[i], vidCapOpenCvPtr->frameDstSwap[i], Size(VIDEO_WIDTH, VIDEO_HEIGHT),0,0, INTER_LINEAR);
                i = 1;
                vidCapOpenCvPtr->videoCapDevices[i]->retrieve(frame[i], 0);
                img = frame[i];
                check = cvCheckArr(&img, 0, 0, 0);
                resize(vidCapOpenCvPtr->frame[i], vidCapOpenCvPtr->frameDstSwap[i], Size(VIDEO_WIDTH, VIDEO_HEIGHT),0,0, INTER_LINEAR);
                vidCapOpenCvPtr->swapBufferOn = false;
            }
            else
            {
                int i = 0;
                vidCapOpenCvPtr->videoCapDevices[i]->retrieve(frame[i], 0);
                img = frame[i];
                check = cvCheckArr(&img, 0, 0, 0);
                resize(vidCapOpenCvPtr->frame[i], vidCapOpenCvPtr->frameDst[i], Size(VIDEO_WIDTH, VIDEO_HEIGHT),0,0, INTER_LINEAR);
                i = 1;
                vidCapOpenCvPtr->videoCapDevices[i]->retrieve(frame[i], 0);
                img = frame[i];
                check = cvCheckArr(&img, 0, 0, 0);
                resize(vidCapOpenCvPtr->frame[i], vidCapOpenCvPtr->frameDst[i], Size(VIDEO_WIDTH, VIDEO_HEIGHT),0,0, INTER_LINEAR);
                vidCapOpenCvPtr->swapBufferOn = true;
            }
        }
        catch (...)
        {
        }

        vidCapOpenCvPtr->flagResizing = false;

//        boost::xtime_get(&(vidCapOpenCvPtr->sleepResize), boost::TIME_UTC);
//        vidCapOpenCvPtr->sleepResize.nsec += 10000000;
//        boost::thread::sleep(vidCapOpenCvPtr->sleepResize);

    }
}

void VideoCaptureOpenCV::grabCapturedFrame(int dev)
{
    int i=dev;
    if (vidCapOpenCvPtr->swapBufferOn)
    {
        vidCapOpenCvPtr->capturedFrame[i] = (unsigned char *) vidCapOpenCvPtr->frameDst[i].data;
    }
    else
    {
        vidCapOpenCvPtr->capturedFrame[i] = (unsigned char *) vidCapOpenCvPtr->frameDstSwap[i].data;
    }
}

unsigned char* VideoCaptureOpenCV :: getCurrentFrame(int dev)
{
	//vidCapOpenCvPtr->grabCapturedFrame(dev);//SEM_CAMERA

	return vidCapOpenCvPtr->redFrame;
    //return vidCapOpenCvPtr->capturedFrame[dev];//SEM_CAMERA
}
