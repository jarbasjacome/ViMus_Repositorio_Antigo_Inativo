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

    posXorigin = 0;
    posYorigin = 0;
    posX = 0;
    posY = 0;

    blobPressed = false;

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
        int numBlobs = 0;
        int nextId = -1;
        int currBlobId = -1;

        for (int i=0; i<MAX_NUM_BLOBS; i++)
        {
            blobs[i] = blobsSwap[i];
            blobsId[i] = blobsIdSwap[i];
            blobsSwap[i] = Point(-500,-500);
            blobsIdSwap[i] = false;
        }

        for (int i=0; i<numContours; i++)
        {
            int area = (int) abs(contourArea(v.at(i)));
            if (area > 20)
            {
                int j=0;
                numBlobs++;
                minEnclosingCircle(v.at(i), p, radius);
                //verify if p is a new Point
                for (j = 0; j<MAX_NUM_BLOBS; j++)
                {
                    if (blobsId[j])
                    {
                        int difX = blobs[j].x - p.x;
                        int difY = blobs[j].y - p.y;
                        if (abs(difX) <= 20 &&
                            abs(difY) <= 20)
                        {
                            //set current found point position for next position prevention;
                            blobsSwap[j].x = p.x;
                            blobsSwap[j].y = p.y;
                            blobsIdSwap[j] = true;
                            currBlobId = j;
                            break;
                        }
                    }
                }
                if (j==MAX_NUM_BLOBS)
                {
                    for (int k=0; k<MAX_NUM_BLOBS; k++)
                    {
                        if (!blobsId[k])
                        {
                            nextId = k;
                            break;
                        }
                    }
                    blobsSwap[nextId] = p;
                    blobsIdSwap[nextId] = true;
                    currBlobId = nextId;
                    if (numBlobs == 1)
                    {
                        origin1 = p;
                        originId1 = nextId;

                        posXorigin = posX;
                        posYorigin = posY;
                        blobPressed = true;
                    }
                    else if (numBlobs == 2)
                    {
                        origin2 = p;
                        originId2 = nextId;
                    }
                }
                drawInfo(frame, "ID=", currBlobId, Point(p.x, p.y-30), &infoStream, &infoString);
            }
        }

        // check if blob released! equivalent to mouseReleased.
        if (numBlobs == 0 && blobPressed == true)
        {
            blobPressed = false;
        }

        drawInfo(frame, "Number = ", numBlobs, Point(20, 20), &infoStream, &infoString);

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

    calculateZoom();

    gluLookAt (0.0, 0.0, cam_z*0.2, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    glTranslatef (posX, posY, 0);

    glBegin(GL_LINES);

    for (int i=0; i<11; i++)
    {
        glVertex2f(-1, i*0.2 - 1);
        glVertex2f(1, i*0.2 - 1);
        glVertex2f(i*0.2 - 1, -1);
        glVertex2f(i*0.2 - 1, 1);
    }

    glEnd();

    glPopMatrix();
}

/**
 * Calculates zoom based on two blob distance.
 */
void VimusMachineCVBlobDetection::calculateZoom()
{
    bool blob2 = false;

    ostringstream * strStream = new ostringstream;

    Point p1(-100,-100);
    Point p2(-100,-100);
    int i = 0;

    for (i=0; i<this->MAX_NUM_BLOBS; i++)
    {
        if (this->blobsId[i])
        {
            p1 = this->blobs[i];
            if (i == originId1)
            {
                p1Origin = origin1;
            }
            else if (i == originId2)
            {
                p1Origin = origin2;
            }

            (*strStream) << " p1Ox = " << p1Origin.x <<
                            " p1Oy = " << p1Origin.y <<
                            " p1Ox - p1.x = " << p1Origin.x - p1.x <<
                            " p1Oy - p1.y = " << p1Origin.y - p1.y;
            renderBitmapString(-1.0,-0.8,0,GLUT_BITMAP_9_BY_15, strStream);

            posX = posXorigin + ((float) (p1Origin.x - p1.x) / (float) VIDEO_WIDTH)*2;
            posY = posYorigin + ((float) (p1Origin.y - p1.y) / (float) VIDEO_HEIGHT)*2;

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

            if (i == originId1)
            {
                p2Origin = origin1;
            }
            else if (i == originId2)
            {
                p2Origin = origin2;
            }


            break;
        }
    }

    if (blob2)
    {
        distance = sqrt( pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2) );

        strStream->clear();
        (*strStream) << "Distance = " << distance <<
                        " p1Ox = " << p1Origin.x <<
                        " p1Oy = " << p1Origin.y <<
                        " p2Ox = " << p2Origin.x <<
                        " p3Oy = " << p2Origin.y;
        renderBitmapString(-1.0,-1.0,0,GLUT_BITMAP_9_BY_15, strStream);

    }

    cam_z = abs (13 - distance/25);

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

/**
 * TODO: PUT THIS OUT OF THIS OBJECT!
 * Renders a bitmap string.
 */
void VimusMachineCVBlobDetection::renderBitmapString(
		float x,
		float y,
		float z,
		void *font,
		ostringstream * strStream)
{
    string str;
    str = strStream->str();
    glRasterPos3f(x, y, z);
    for (int i=0; i<(int)str.size(); i++)
    {
        glutBitmapCharacter(font, str.at(i));
    }
}
