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
        vidCapOpenCvPtr->redFrame[i*3] = 0;
        vidCapOpenCvPtr->redFrame[i*3+1] = 0;
        vidCapOpenCvPtr->redFrame[i*3+2] = 255;
	}

	Mat red(VIDEO_WIDTH, VIDEO_HEIGHT, CV_8UC3, vidCapOpenCvPtr->redFrame,0);

    for (int i=0; i<10; i++)
    {
        vidCapOpenCvPtr->frame[i] = red.clone();
        vidCapOpenCvPtr->frameDst[i] = red.clone();
        vidCapOpenCvPtr->frameDstSwap[i] = red.clone();

        vidCapOpenCvPtr->capturedFrame[i] =
                                new unsigned char[VIDEO_WIDTH*VIDEO_HEIGHT*3];

        for (int j=0; j<VIDEO_WIDTH*VIDEO_HEIGHT; j++)
        {
            vidCapOpenCvPtr->capturedFrame[i][j*3] =  100;
            vidCapOpenCvPtr->capturedFrame[i][j*3+1] = 100;
            vidCapOpenCvPtr->capturedFrame[i][j*3+2] = 100;
        }
    }

	vidCapOpenCvPtr->videoCapDevices[0] = new VideoCapture(1);
    if (!vidCapOpenCvPtr->videoCapDevices[0]->isOpened())
    {
        cout << "\nFailed to open video capture device 0\n";
        return;
    }

    boost::thread grabT(VideoCaptureOpenCV::grabThreadFuncStatic);
    vidCapOpenCvPtr->grabThread = &grabT;

    vidCapOpenCvPtr->flagGrabing = false;
    vidCapOpenCvPtr->swapBufferOn = false;

    if (DEBUG_MODE)
        cout << "\nVideoCaptureOpenCV started.";

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
        vidCapOpenCvPtr->videoCapDevices[0]->grab();

        int check;
        IplImage img;

        try
        {
            for (int i = 0; i < vidCapOpenCvPtr->nextCapDev; i++)
            {
                if (vidCapOpenCvPtr->swapBufferOn)
                {
                    vidCapOpenCvPtr->videoCapDevices[i]->retrieve(frame[i], 0);
                    img = frame[i];
                    check = cvCheckArr(&img, 0, 0, 0);
                    resize(vidCapOpenCvPtr->frame[i], vidCapOpenCvPtr->frameDstSwap[i], Size(VIDEO_WIDTH, VIDEO_HEIGHT),0,0, INTER_LINEAR);
                    vidCapOpenCvPtr->swapBufferOn = false;
                }
                else
                {
                    vidCapOpenCvPtr->videoCapDevices[i]->retrieve(frame[i], 0);
                    img = frame[i];
                    check = cvCheckArr(&img, 0, 0, 0);
                    resize(vidCapOpenCvPtr->frame[i], vidCapOpenCvPtr->frameDst[i], Size(VIDEO_WIDTH, VIDEO_HEIGHT),0,0, INTER_LINEAR);
                    vidCapOpenCvPtr->swapBufferOn = true;
                }
            }
        }
        catch (...)
        {
            cout << "\nOpenCV resize error!\n";
        }

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
	vidCapOpenCvPtr->grabCapturedFrame(dev);

    return vidCapOpenCvPtr->capturedFrame[dev];
}
