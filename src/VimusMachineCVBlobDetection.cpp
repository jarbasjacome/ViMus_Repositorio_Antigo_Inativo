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

    glGenTextures(NUM_MAPS, &texNames[0]);

    mapa[0] = imread("/home/jandila/mapa01.jpg", 1);
    mapa[1] = imread("/home/jandila/mapa02.jpg", 1);
    mapa[2] = imread("/home/jandila/mapa03.jpg", 1);
    mapa[3] = imread("/home/jandila/mapa04.jpg", 1);
    mapa[4] = imread("/home/jandila/mapa05.jpg", 1);

    // win width divided by win height
    float winXratio = (float) glutGet(GLUT_WINDOW_WIDTH)/
                                (float) glutGet(GLUT_WINDOW_HEIGHT);

    // TODO: plug
    winXratio = 1.3333f;


    for (int i=0; i<NUM_MAPS; i++)
    {
        Size siz = mapa[i].size();

        // map width divided by map height
        float mapXratio = (float) siz.width/ (float) siz.height;

        mapWidth[i] = mapHeight[i] = 1.0f;
        if (mapXratio > winXratio)
        {
            mapHeight[i] = (1.0f/mapXratio) * winXratio;
        }
        else if (mapXratio < winXratio)
        {
            mapWidth[i] = mapXratio * (1.0f/winXratio);
        }

        resize(mapa[i], mapaResized[i], Size(4096, 4096),0,0, INTER_AREA);

        mapaData[i] = mapaResized[i].data;

        glBindTexture(GL_TEXTURE_2D, texNames[i]);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 4096, 4096, 0, GL_BGR,
                                    GL_UNSIGNED_BYTE, this->mapaData[i]);

    }

    currentMap = 0;
    lastMap = 0;
    mapsPosition = 0;

    state = STATE_USER_INPUT;

    distance = MAX_ZOOM;
    distanceOrigin = MAX_ZOOM;
    camZ = MAX_ZOOM;
    camZOrigin = MAX_ZOOM;

    originId1 = -1;
    originId2 = -1;

    resetPosArrays();

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

        Point2f tempOrigin1;
        int tempOrigin1Id = 0;

        float radius;
        int countBlobs = 0;
        int nextId = -1;
        int currBlobId = -1;

        for (int i=0; i<numContours; i++)
        {
            int area = (int) abs(contourArea(v.at(i)));
            if (area > MIN_BLOB_SIZE)
            {
                int j=0;
                countBlobs++;
                minEnclosingCircle(v.at(i), p, radius);
                //verify if p is a new Point
                for (j = 0; j<MAX_NUM_BLOBS; j++)
                {
                    if (blobsId[j])
                    {
                        int difX = blobs[j].x - p.x;
                        int difY = blobs[j].y - p.y;
                        if (abs(difX) <= MIN_BLOB_SIZE*2 &&
                            abs(difY) <= MIN_BLOB_SIZE*2)
                        {
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
                    numBlobs++;

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

                        distance = sqrt(    pow(origin1.x - origin2.x, 2) +
                                            pow(origin1.y - origin2.y, 2) );

                        distanceOrigin = distance;

//                        cout << "\n camZ = " << camZ;
                        camZOrigin = camZ;

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

        for (int i=0; i<MAX_NUM_BLOBS; i++)
        {
            blobs[i] = blobsSwap[i];
            blobsId[i] = blobsIdSwap[i];
            blobsSwap[i] = Point(-500,-500);
            blobsIdSwap[i] = false;
        }

        numBlobs = countBlobs;

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

    updateState();

    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();

    gluLookAt (-posX, -posY, camZ, -posX, -posY, camZ - 5.0f, 0.0, 1.0, 0.0);

    glClearColor (0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDisable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);

    glColor4f (1.0f, 1.0f, 1.0f, 1.0f);

    glTranslatef(-mapsPosition, 0.0f, 0.0f);

    for (int i=0; i<NUM_MAPS; i++)
    {
        glBindTexture(GL_TEXTURE_2D, texNames[i]);

        glBegin(GL_QUADS);
            glTexCoord2f(1.0f, 0.0f); glVertex3f( mapWidth[i], mapHeight[i], 0.0f);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-mapWidth[i], mapHeight[i], 0.0f);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-mapWidth[i],-mapHeight[i], 0.0f);
            glTexCoord2f(1.0f, 1.0f); glVertex3f( mapWidth[i],-mapHeight[i], 0.0f);
        glEnd();

        if (i<NUM_MAPS - 1)
            glTranslatef(mapWidth[i]+mapWidth[i+1]+SPACE_BEETWEEN_MAPS,0.0f,0.0f);
    }

	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

    glPopMatrix();
}

void VimusMachineCVBlobDetection::updateState()
{
    float step;
    switch (state)
    {
    case STATE_SLIDING_LEFT:
        step = -mapWidth[currentMap] - mapWidth[lastMap] - SPACE_BEETWEEN_MAPS;
        moveTo(step,0);
        if (posX <= step + 0.05f)
        {
            setState(STATE_USER_INPUT);
        }
        break;
    case STATE_SLIDING_RIGHT:
        step = mapWidth[currentMap] + mapWidth[lastMap] + SPACE_BEETWEEN_MAPS;
        moveTo(step,0);
        if (posX >= step - 0.05f)
        {
            setState(STATE_USER_INPUT);
        }
        break;
    case STATE_USER_INPUT:
        treatBlobsGestures();
        break;
    }
}

/**
 * Treat user blobs gestures.
 * Calculates zoom and translation based on one blob position and
 * two blobs distance.
 */
void VimusMachineCVBlobDetection::treatBlobsGestures()
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

    if (numBlobs == 1)
    {
        if (camZ/MAX_ZOOM == 1.0)
        {

            float difference = posXarray[AVERAGE_ARRAY_SIZE-1]+1 - (posXorigin+1);

            if (difference > SLIDE_VALUE)
            {
                setState(STATE_SLIDING_RIGHT);
            }
            else if (difference < - (float) SLIDE_VALUE)
            {
                setState(STATE_SLIDING_LEFT);
            }

        }
        // put the current frame point position value on the earliest position
        moveTo( posXorigin + ((float)(p1Origin.x - p1.x)/(float) VIDEO_WIDTH),//*((camZ+10) + 25)/25;
                posYorigin + ((float)(p1Origin.y - p1.y)/(float) VIDEO_HEIGHT));//*((camZ+10) + 25)/25;
        checkLimits();


    }

    if (numBlobs == 2 && blob2)
    {

        distance = sqrt( pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2) );

        strStream->clear();
        (*strStream) << "Distance = " << distance;
        renderBitmapString(-1.0,-0.9,0,GLUT_BITMAP_9_BY_15, strStream);
        strStream->clear();
        (*strStream) << " camZ = " <<  camZ;
        renderBitmapString(-1.0,-1.0,0,GLUT_BITMAP_9_BY_15, strStream);

        // subtract oldest zoom value from the sum of zoom values array
        zoomLastSum -= zoomArray[0];

        // makes the zoom values queue walks one step
        for (int i=0; i<AVERAGE_ARRAY_SIZE-1; i++)
        {
            zoomArray[i] = zoomArray[i+1];
        }

        // put the current frame zoom value as the earliest one in the queue
        zoomArray[AVERAGE_ARRAY_SIZE-1] = camZOrigin + (MAX_ZOOM - MIN_ZOOM) *
                                    (distanceOrigin - distance) / VIDEO_WIDTH;

        // add current frame zoom value to the sum of all zoom values
        zoomLastSum += zoomArray[AVERAGE_ARRAY_SIZE-1];

        // calculate the average position to get a smooth zoom
        camZ = zoomLastSum / AVERAGE_ARRAY_SIZE;

        if (camZ < 2.0f)
        {
            camZ  = 2.0f;
            camZOrigin = 2.0f;
        }
        if (camZ > MAX_ZOOM)
        {
            camZ = MAX_ZOOM;
            camZOrigin = MAX_ZOOM;
        }

        checkLimits();
    }

}

void VimusMachineCVBlobDetection::setState(int nextState)
{
    switch (state)
    {
    case STATE_USER_INPUT:
        if (nextState == STATE_SLIDING_LEFT)
        {
            if (currentMap < NUM_MAPS-1)
            {
                lastMap = currentMap;
                currentMap++;
                state = STATE_SLIDING_LEFT;
            }
        }
        else if (nextState == STATE_SLIDING_RIGHT)
        {
            if (currentMap > 0)
            {
                lastMap = currentMap;
                currentMap--;
                state = STATE_SLIDING_RIGHT;
            }
        }
        break;
    case STATE_SLIDING_LEFT:
    case STATE_SLIDING_RIGHT:
        if (nextState == STATE_USER_INPUT)
        {
            mapsPosition = 0;
            int c = 0;
            while (c < currentMap)
            {
                mapsPosition += mapWidth[c] + mapWidth[c+1] + SPACE_BEETWEEN_MAPS;
                c++;
            }

            resetPosArrays();

            state = STATE_USER_INPUT;
        }
        break;
    }
}

void VimusMachineCVBlobDetection::resetPosArrays()
{
    posXorigin = 0;
    posYorigin = 0;
    posX = 0;
    posY = 0;
    for (int i=0; i<AVERAGE_ARRAY_SIZE; i++)
    {
        posXarray[i] = 0;
        posYarray[i] = 0;
        zoomArray[i] = MAX_ZOOM;
    }
    posXlastSum = 0;
    posYlastSum = 0;
    zoomLastSum = MAX_ZOOM*AVERAGE_ARRAY_SIZE;
}


/**
 * Moves camera softly to a new position.
 */
void VimusMachineCVBlobDetection::moveTo(float x, float y)
{
        // subtract oldest position value from the sum of positions array
        posXlastSum -= posXarray[0];
        posYlastSum -= posYarray[0];

        // makes the positions queue walks one step
        for (int i=0; i<AVERAGE_ARRAY_SIZE-1; i++)
        {
            posXarray[i] = posXarray[i+1];
            posYarray[i] = posYarray[i+1];
        }

        // put the current frame point position value on the earliest position
        posXarray[AVERAGE_ARRAY_SIZE-1] = x;
        posYarray[AVERAGE_ARRAY_SIZE-1] = y;

        // add current frame point position to the sum of all positions
        posXlastSum += posXarray[AVERAGE_ARRAY_SIZE-1];
        posYlastSum += posYarray[AVERAGE_ARRAY_SIZE-1];

        // calculate the average position to get a smooth object move
        posX = posXlastSum / AVERAGE_ARRAY_SIZE;
        posY = posYlastSum / AVERAGE_ARRAY_SIZE;

//        posXarray[AVERAGE_ARRAY_SIZE-1] = posX;
//        posYarray[AVERAGE_ARRAY_SIZE-1] = posY;
}

/**
 * Calculates the posX and posY limits based on zoom value.
 * If zoom is high, user can move more.
 */
void VimusMachineCVBlobDetection::checkLimits()
{
    float zoomRatioX = 1 - camZ/MAX_ZOOM - (1 - mapWidth[currentMap]);
    float zoomRatioY = 1 - camZ/MAX_ZOOM - (1 - mapHeight[currentMap]);

    if (zoomRatioX < 0)
    {
        posX = 0;
    }
    else if (abs(posX) > zoomRatioX)
    {
        if (posX > 0)
        {
            posX = zoomRatioX;
        }
        else
        {
            posX = -zoomRatioX;
        }
    }
    if (zoomRatioY < 0)
    {
        posY = 0;
    }
    else if (abs(posY) > zoomRatioY)
    {
        if (posY > 0)
        {
            posY = zoomRatioY;
        }
        else
        {
            posY = -zoomRatioY;
        }
    }
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

void VimusMachineCVBlobDetection::keyBoardFunc(unsigned char key, int x, int y)
{
}

void VimusMachineCVBlobDetection::specialKeyBoardFunc(int key, int x, int y)
{
}
