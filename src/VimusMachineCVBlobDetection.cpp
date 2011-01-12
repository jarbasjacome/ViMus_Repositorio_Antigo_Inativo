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

#include "VimusMachineCVBlobDetection.h"

using namespace std;
using namespace cv;

/**
 * VimusMachineCVBlobDetection default constructor.
 */
VimusMachineCVBlobDetection::VimusMachineCVBlobDetection()
{
    VimusMachineObject::initObject();

	this->label = string ("videoCVBlob");
	this->addInputPin(VimusMachinePin::TYPE_VIDEO);
	this->addInputPin(VimusMachinePin::TYPE_CONTROL);

	this->addOutputPin(VimusMachinePin::TYPE_VIDEO);

	this->ppNullFrame = new unsigned char*;
	(*this->ppNullFrame) = new unsigned char[VIDEO_WIDTH*VIDEO_HEIGHT*3];
	for (int i=0; i<VIDEO_WIDTH*VIDEO_HEIGHT*3; i++)
	{
		(*this->ppNullFrame)[i] = 0;
	}

	// do not assign another value to it.
	this->ppOutputData = new unsigned char*;
	(*this->ppOutputData) = new unsigned char[VIDEO_WIDTH*VIDEO_HEIGHT*3];
	for (int i=0; i<VIDEO_WIDTH*VIDEO_HEIGHT*3; i++)
	{
		(*this->ppOutputData)[i] = 0;
	}

	// set video output pin
	((VimusMachinePinVideo *)this->outputPins[0])->ppFrame = this->ppOutputData;

	// do not assign another value to it.
	this->ppInputData = new unsigned char*;
	(*this->ppInputData) = new unsigned char[VIDEO_WIDTH*VIDEO_HEIGHT*3];
	for (int i=0; i<VIDEO_WIDTH*VIDEO_HEIGHT*3; i++)
	{
		(*this->ppInputData)[i] = 0;
	}

	// set video input pin
	//((VimusMachinePinVideo *)this->inputPins[0])->ppFrame = this->ppInputData;

	this->frame = (* new Mat(VIDEO_WIDTH, VIDEO_HEIGHT, CV_8UC3));
	this->frameDst = (* new Mat(VIDEO_WIDTH, VIDEO_HEIGHT, CV_8UC3));

	this->frame.data = (*this->ppInputData);
	this->frameDst.data = this->frame.data;
    (*this->ppOutputData) = this->frameDst.data;

    if (DEBUG_MODE)
        cout << "\nVimusMachineCVBlobDetection constructed.";

    for (int i=0; i<MAX_NUM_BLOBS; i++)
    {
        blobs[i] = Point(-500,-500);
        blobsSwap[i] = Point(-500,-500);
        blobsId[i] = false;
        blobsIdSwap[i] = false;
    }

    distance = 6;
    cam_z = 6;

}

/**
 * VimusMachineCVBlobDetection destructor.
 */
VimusMachineCVBlobDetection::~VimusMachineCVBlobDetection()
{
}

/**
 * Update VimusMachineCVBlobDetection.
 */
void VimusMachineCVBlobDetection::update()
{
    this->frame.data = (*this->ppInputData);

    try
    {

        cvtColor(frame, frameGray, CV_BGR2GRAY);

        threshold( frameGray, frameThresh,  200, 255, CV_THRESH_BINARY);

        int numContours=0;

        findContours(frameThresh, v, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
        numContours = v.size();
        Point2f p;
        float radius;
        int c = 0;

        for (int i=0; i<MAX_NUM_BLOBS; i++)
        {
            blobs[i] = blobsSwap[i];
            blobsId[i] = blobsIdSwap[i];
            blobsSwap[i] = Point(-500,-500);
            blobsIdSwap[i] = false;
        }

        for (int i=0; i<numContours; i++)
        {
            int j=0;
            int area = (int) abs(contourArea(v.at(i)));
            if (area > 5)
            {
                c++;
                minEnclosingCircle(v.at(i), p, radius);
                if (numBlobs > 0)
                {
                    for (j = 0; j<MAX_NUM_BLOBS; j++)
                    {
                        if (blobsId[j])
                        {
                            if (abs(blobs[j].x - p.x) <= 5 ||
                                abs(blobs[j].y - p.y) <= 5)
                            {
                                blobsSwap[j].x = p.x;
                                blobsSwap[j].y = p.y;
                                blobsIdSwap[j] = true;
                                break;
                            }
                        }
                    }
                    if (j==MAX_NUM_BLOBS)
                    {
                        j=nextId;
                        blobsSwap[nextId] = p;
                        blobsIdSwap[nextId] = true;
                        blobs[nextId] = p;
                        blobsId[nextId] = true;
                    }
                }
                else
                {
                    blobsSwap[0] = p;
                    blobsIdSwap[0] = true;
                    blobs[0] = p;
                    blobsId[0] = true;
                    nextId = 1;
                    numBlobs++;
                }
//                drawInfo(frame, "Contour ", j, Point(20, i*20+40), &infoStream, &infoString);
//                drawInfo(frame, "Area = ", area, Point(150, i*20+40), &infoStream, &infoString);
//                drawInfo(frame, "posX = ", p.x, Point(p.x - 60, p.y), &infoStream, &infoString);
//                drawInfo(frame, "posY = ", p.y, Point(p.x + 60, p.y), &infoStream, &infoString);
                drawInfo(frame, "ID=", j, Point(p.x, p.y-30), &infoStream, &infoString);
            }

            for (int k=0; k<MAX_NUM_BLOBS; k++)
            {
                if (!blobsId[k] && !blobsIdSwap[k])
                {
                    nextId = k;
                    break;
                }
            }

            numBlobs = c;

        }

        drawInfo(frame, "Number = ", c, Point(20, 20), &infoStream, &infoString);

        (*this->ppOutputData) = this->frame.data;
    }
    catch  (...)
    {
        cout << "\nError at ViMusMachineCVBlobDetection update.";
    }
}

/**
 * Draw VimusMachineCVBlobDetection.
 */
void VimusMachineCVBlobDetection::draw()
{
    glPushMatrix();

    bool blob2 = false;

    Point p1(-100,-100);
    Point p2(-100,-100);
    int i = 0;

    for (i=0; i<this->MAX_NUM_BLOBS; i++)
    {
        if (this->blobsId[i])
        {
            p1 = this->blobs[i];
            break;
        }
    }
    i++;
    for (int j = i;j<this->MAX_NUM_BLOBS; j++)
    {
        if (this->blobsId[j])
        {
            p2 = this->blobs[j];
            blob2 = true;
            break;
        }
    }

    if (blob2)
    {
        distance = sqrt( pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2) );
    }

    cam_z = abs (13 - distance/25);

//    if (cam_z < 1)
//        cam_z = 10;

    gluLookAt (0.0, 0.0, cam_z*0.2, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    glTranslatef(   abs(1 - (float) p1.x / (float) VIDEO_WIDTH) - 0.5,
                    abs(1- (float) p1.y / (float) VIDEO_HEIGHT) - 0.5,
                    0);

//    glutWireTeapot(0.5);

    glBegin(GL_LINES);

    for (int i=0; i<11; i++)
    {
        glVertex2f(-1, i*0.2 - 1); glVertex2f(1, i*0.2 - 1);
        glVertex2f(i*0.2 - 1, -1); glVertex2f(i*0.2 - 1, 1);
    }

    glEnd();


    glPopMatrix();

}

/**
 * Draw VimusMachineCVBlobDetection.
 */
void VimusMachineCVBlobDetection::draw(int rendermode)
{
}

/**
 * Get pointer to currentFrame buffer pointer.
 * This is necessary for GUI have a direct access to
 * machine frame buffer.
 */
unsigned char** VimusMachineCVBlobDetection::getCurrentFramePointer()
{
	return this->ppOutputData;
}

/**
 * Do some action before and/or after connect a pin to
 * another objects pin.
 */
bool VimusMachineCVBlobDetection::connectOutput (int outPin, VimusMachineObject * dstObj, int inPin)
{
	return dstObj->connectInput(this, outPin, inPin);
}

/**
 * Do some action before and/or after be connected by another
 * objects pin.
 */
bool VimusMachineCVBlobDetection::connectInput (VimusMachineObject * srcObj, int outPin, int inPin)
{
	bool ret = false;
	if (srcObj->outputPins[outPin]->connect(this->inputPins[inPin]))
	{
		((VimusMachinePinVideo *) this->inputPins[inPin])->ppFrame =
			((VimusMachinePinVideo *) srcObj->outputPins[outPin])->ppFrame;
		this->ppInputData = ((VimusMachinePinVideo *) this->inputPins[inPin])->ppFrame;
		ret = true;
	}
	return ret;
}

/**
 * Do some action before and/or after disconnect an output pin from
 * another objects pin.
 */
void VimusMachineCVBlobDetection::disconnectOutput (int outPin, VimusMachineObject * dstObj, int inPin)
{
	dstObj->disconnectInput(this, outPin, inPin);
}

/**
 * Do some action before and/or after disconnect an input pin from
 * another objects pin.
 */
void VimusMachineCVBlobDetection::disconnectInput (VimusMachineObject * srcObj, int outPin, int inPin)
{
	srcObj->outputPins[outPin]->disconnect(this->inputPins[inPin]);
	this->ppInputData = this->ppNullFrame;
}

/**
 * Draw debug info on video using OpenCV drawing and text funtions.
 */
void VimusMachineCVBlobDetection::drawInfo ( Mat frame,
                const char* infoName,
                int infoValue,
                Point pt,
                std::ostringstream* infoStream,
                std::string* infoString )
{
    infoStream->str("");
    infoStream->clear();
    (*infoStream) << infoName << infoValue;
    (*infoString) = infoStream->str();
    putText(frame, (*infoString), pt, FONT_HERSHEY_PLAIN,1, CV_RGB(255,0,0),1,8,false);
}
