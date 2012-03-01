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
using namespace std;

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
    int dev = 1;

    //find first available device.
    while (dev < NUM_MAX_DEVICES)
    {
        if (vidCapOpenCvPtr->videoCapDevices[dev] == NULL)
        {
            break;
        }
        else
        {
            dev++;
        }
    }

    if (dev == NUM_MAX_DEVICES)
    {
        dev = -1;
        if (DEBUG_MODE)
            cout << "\nViMus: All " << NUM_MAX_DEVICES << " VideoCapture devices are busy.\n";
    }
    else
    {
        try
        {
            vidCapOpenCvPtr->videoCapDevices[dev] = new VideoCapture(dev);
            if (!vidCapOpenCvPtr->videoCapDevices[dev]->isOpened())
            {
                if (DEBUG_MODE)
                    cout << "\nViMus: Failed to open video capture device " << dev << "\n\n\n";
                vidCapOpenCvPtr->videoCapDevices[dev] = NULL;
            }
        }
        catch ( ... )
        {
            cout << "\nOpenCV Exception when trying to open VideoCapture device " << dev << "\n";
            vidCapOpenCvPtr->videoCapDevices[dev] = NULL;
        }
    }

    return dev;
}

/**
 * Stop a capture device.
 */
void VideoCaptureOpenCV::stopCaptureDevice(int dev)
{
    if (vidCapOpenCvPtr->videoCapDevices[dev] != NULL && vidCapOpenCvPtr->videoCapDevices[dev]->isOpened())
    {
        // If we delete VideoCapture the grabThreadFund will immediatelly try to
        // grab a frame of it and it will find anything so...
        // Before delete VideoCapture, we have to mark its position in
        // videoCapDevices[dev] as NULL, so grabThreadFunc will ignore this position.

        // Save temporally VideoCapture reference.
        VideoCapture* vidCap = vidCapOpenCvPtr->videoCapDevices[dev];

        // Turn videoCapDevices[dev] NULL, so grabThreadFunc is safe.
        vidCapOpenCvPtr->videoCapDevices[dev] = NULL;

        // Finally delete VideoCapture.
        delete vidCap;
    }
}

/**
 * Initializes VideoCaptureOpenCV
 */
void VideoCaptureOpenCV::init()
{

    for (int i=0; i<NUM_MAX_DEVICES; i++)
    {
        vidCapOpenCvPtr->videoCapDevices[i] = NULL;
    }

	vidCapOpenCvPtr->redFrame = new unsigned char[VIDEO_WIDTH*VIDEO_HEIGHT*3];

	for (int i=0; i<VIDEO_WIDTH*VIDEO_HEIGHT; i++)
	{
        vidCapOpenCvPtr->redFrame[i*3+1] = 0;
        vidCapOpenCvPtr->redFrame[i*3] = 0;
        vidCapOpenCvPtr->redFrame[i*3+2] = 255;
	}

	Mat red(VIDEO_WIDTH, VIDEO_HEIGHT, CV_8UC3, vidCapOpenCvPtr->redFrame,0);

    for (int i=0; i<NUM_MAX_DEVICES; i++)
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

/*
//    LANTERNA MAGICA nao precisa de captura
//    TODO: apenas ligar essas threads quando estiver ocorrendo captura.

    boost::thread grabT(VideoCaptureOpenCV::grabThreadFuncStatic);
    vidCapOpenCvPtr->grabThread = &grabT;

    vidCapOpenCvPtr->elapsedTime = 0;
    boost::xtime_get(&(vidCapOpenCvPtr->lastTimeSys), boost::TIME_UTC);
//*/

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

    for (int i=0; i<NUM_MAX_DEVICES; i++)
    {
        if (vidCapOpenCvPtr->videoCapDevices[i] != NULL)
        {
            delete vidCapOpenCvPtr->videoCapDevices[i];
        }
    }

}

void VideoCaptureOpenCV::grabThreadFunc()
{
    while(1)
    {
        for (int i=0; i<NUM_MAX_DEVICES; i++)
        {
            if (vidCapOpenCvPtr->videoCapDevices[i] != NULL)
            {
                vidCapOpenCvPtr->videoCapDevices[i]->grab();
            }
        }

        int check;
        IplImage img;

        try
        {
            for (int i = 0; i < NUM_MAX_DEVICES; i++)
            {
                if (vidCapOpenCvPtr->videoCapDevices[i] != NULL)
                {
                    vidCapOpenCvPtr->videoCapDevices[i]->retrieve(frame[i], 0);
                    img = frame[i];
                    check = cvCheckArr(&img, 0, 0, 0);
                    if (vidCapOpenCvPtr->swapBufferOn)
                    {
                        resize(vidCapOpenCvPtr->frame[i], vidCapOpenCvPtr->frameDstSwap[i], Size(VIDEO_WIDTH, VIDEO_HEIGHT),0,0, INTER_LINEAR);
                    }
                    else
                    {
                        resize(vidCapOpenCvPtr->frame[i], vidCapOpenCvPtr->frameDst[i], Size(VIDEO_WIDTH, VIDEO_HEIGHT),0,0, INTER_LINEAR);
                    }
                }
            }
            if (vidCapOpenCvPtr->swapBufferOn)
            {
                vidCapOpenCvPtr->swapBufferOn = false;
            }
            else
            {
                vidCapOpenCvPtr->swapBufferOn = true;
            }
        }
        catch (...)
        {
            cout << "\nOpenCV resize error!\n";
        }

        boost::xtime_get(&(vidCapOpenCvPtr->sleepGrab), boost::TIME_UTC);
        vidCapOpenCvPtr->elapsedTime = vidCapOpenCvPtr->sleepGrab.nsec - vidCapOpenCvPtr->lastTimeSys.nsec;
        if (vidCapOpenCvPtr->elapsedTime < 17000000 && vidCapOpenCvPtr->elapsedTime > 0)
        {
            vidCapOpenCvPtr->sleepGrab.nsec += 17000000 - vidCapOpenCvPtr->elapsedTime;
            boost::thread::sleep(vidCapOpenCvPtr->sleepGrab);
        }
        boost::xtime_get(&(vidCapOpenCvPtr->lastTimeSys), boost::TIME_UTC);

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
