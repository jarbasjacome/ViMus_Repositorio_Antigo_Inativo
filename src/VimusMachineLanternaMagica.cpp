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

#include "VimusMachineLanternaMagica.h"

using namespace std;
using namespace cv;

/**
 * VimusMachineLanternaMagica default constructor.
 */
VimusMachineLanternaMagica::VimusMachineLanternaMagica()
{
    VimusMachineObject::initObject();

	this->label = string ("lanterna");
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

    if (DEBUG_MODE)
        cout << "\nVimusMachineLanternaMagica constructed.";

//    // win width divided by win height
//    float winXratio = (float) glutGet(GLUT_WINDOW_WIDTH)/
//                                (float) glutGet(GLUT_WINDOW_HEIGHT);
//    // TODO: plug
//    winXratio = 1.3333f;
//
//    mapaData[i] = mapaResized[i].data;
//

    this->capturedFrame = new unsigned char[VIDEO_WIDTH*VIDEO_HEIGHT*3];

	for (int i=0; i<VIDEO_WIDTH*VIDEO_HEIGHT; i++)
	{
        this->capturedFrame[i*3+1] = 255;
        this->capturedFrame[i*3] = 0;
        this->capturedFrame[i*3+2] = 0;
	}

	Mat red(VIDEO_WIDTH, VIDEO_HEIGHT, CV_8UC3, capturedFrame,0);

	this->frame = red.clone();
	this->frameDest = red.clone();

    glGenTextures(1, &texName);

    glBindTexture(GL_TEXTURE_2D, texName);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, VIDEO_WIDTH, VIDEO_HEIGHT, 0, GL_BGR,
                                GL_UNSIGNED_BYTE, this->capturedFrame);

    try
    {
        video = VideoCapture("/dados/jabahpureza/mother_popcorn_01_raw_i420_noaudio.avi");
        //video = VideoCapture("/usr/share/doc/opencv-doc/examples/c/tree.avi");
    }
    catch ( ... )
    {
        cout << "erro opencv videocapture";
    }

}

/**
 * VimusMachineLanternaMagica destructor.
 */
VimusMachineLanternaMagica::~VimusMachineLanternaMagica()
{
}

/**
 * Update VimusMachineLanternaMagica.
 */
void VimusMachineLanternaMagica::update()
{

    int check;
    IplImage img;

    try
    {
        video.grab();
        video.retrieve(this->frame, 0);
        img = this->frame;
        check = cvCheckArr(&img, 0, 0, 0);
        resize(this->frame, this->frameDest, Size(VIDEO_WIDTH, VIDEO_HEIGHT),0,0, INTER_LINEAR);
        this->capturedFrame = (unsigned char *) this->frameDest.data;
    }
    catch ( ... )
    {
        cout << "\nOpenCV retrieve error!\n";
    }

}

/**
 * Draw VimusMachineLanternaMagica.
 */
void VimusMachineLanternaMagica::draw()
{
    glPushMatrix();

    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();

    gluLookAt (0, 0, 10.0f, 0, 0, 10.0f - 5.0f, 0.0, 1.0, 0.0);

	glDisable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);

    glColor4f (1.0f, 1.0f, 1.0f, 1.0f);

    glBindTexture(GL_TEXTURE_2D, texName);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, VIDEO_WIDTH, VIDEO_HEIGHT, 0, GL_BGR,
                                GL_UNSIGNED_BYTE, this->capturedFrame);

    glScalef(0.8,1,1);


    glBegin(GL_QUADS);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,-1.0f, 0.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,-1.0f, 0.0f);
    glEnd();

	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

    glPopMatrix();
}


/**
 * Draw VimusMachineLanternaMagica.
 */
void VimusMachineLanternaMagica::draw(int rendermode)
{
}

/**
 * Get pointer to currentFrame buffer pointer.
 * This is necessary for GUI have a direct access to
 * machine frame buffer.
 */
unsigned char** VimusMachineLanternaMagica::getCurrentFramePointer()
{
	return this->ppOutputData;
}

/**
 * Do some action before and/or after connect a pin to
 * another objects pin.
 */
bool VimusMachineLanternaMagica::connectOutput (int outPin, VimusMachineObject * dstObj, int inPin)
{
	return dstObj->connectInput(this, outPin, inPin);
}

/**
 * Do some action before and/or after be connected by another
 * objects pin.
 */
bool VimusMachineLanternaMagica::connectInput (VimusMachineObject * srcObj, int outPin, int inPin)
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
void VimusMachineLanternaMagica::disconnectOutput (int outPin, VimusMachineObject * dstObj, int inPin)
{
	dstObj->disconnectInput(this, outPin, inPin);
}

/**
 * Do some action before and/or after disconnect an input pin from
 * another objects pin.
 */
void VimusMachineLanternaMagica::disconnectInput (VimusMachineObject * srcObj, int outPin, int inPin)
{
	srcObj->outputPins[outPin]->disconnect(this->inputPins[inPin]);
	this->ppInputData = this->ppNullFrame;
}

/**
 * TODO: PUT THIS OUT OF THIS OBJECT!
 * Renders a bitmap string.
 */
void VimusMachineLanternaMagica::renderBitmapString(
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
