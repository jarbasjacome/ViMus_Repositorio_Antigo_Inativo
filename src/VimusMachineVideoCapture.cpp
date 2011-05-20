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

#include "VimusMachineVideoCapture.h"

using namespace std;

/**
 * VimusMachineVideoCapture default constructor.
 */
VimusMachineVideoCapture::VimusMachineVideoCapture(VideoCaptureOpenCV *vidCap)
{
    VimusMachineObject::initObject();

	this->label = string ("videocap");
	this->addOutputPin(VimusMachinePin::TYPE_VIDEO);

    this->vidCap = vidCap;

    this->deviceNumber = this->vidCap->getNextCaptureDevice();

	this->ppCurrentFrame = new unsigned char*;
	(*this->ppCurrentFrame) = new unsigned char[VIDEO_WIDTH*VIDEO_HEIGHT*3];
	for (int i=0; i<VIDEO_WIDTH*VIDEO_HEIGHT*3; i++)
	{
		(*this->ppCurrentFrame)[i] = 0;
	}

	((VimusMachinePinVideo *)this->outputPins[0])->ppFrame = this->ppCurrentFrame;

    if (DEBUG_MODE)
        cout << "\nVimusMachineVideoCapture constructed.";
}

/**
 * VimusMachineVideoCapture destructor.
 */
VimusMachineVideoCapture::~VimusMachineVideoCapture()
{
    this->vidCap->stopCaptureDevice(this->deviceNumber);
}

/**
 * Update VimusMachineVideoCapture.
 */
void VimusMachineVideoCapture::update()
{
	*(this->ppCurrentFrame) = this->vidCap->getCurrentFrame(this->deviceNumber);
}

/**
 * Get pointer to currentFrame buffer pointer.
 * This is necessary for GUI have a direct access to
 * machine frame buffer.
 */
unsigned char** VimusMachineVideoCapture::getCurrentFramePointer()
{
	return this->ppCurrentFrame;
}

/**
 * Do some action before and/or after connect a pin to
 * another objects pin.
 */
bool VimusMachineVideoCapture::connectOutput (int outPin, VimusMachineObject * dstObj, int inPin)
{
	return dstObj->connectInput(this, outPin, inPin);
}

/**
 * Do some action before and/or after be connected by another
 * objects pin.
 */
bool VimusMachineVideoCapture::connectInput (VimusMachineObject * srcObj, int outPin, int inPin)
{
	return srcObj->outputPins[outPin]->connect(this->inputPins[inPin]);
}

/**
 * Do some action before and/or after disconnect an output pin from
 * another objects pin.
 */
void VimusMachineVideoCapture::disconnectOutput (int outPin, VimusMachineObject * dstObj, int inPin)
{
	dstObj->disconnectInput(this, outPin, inPin);
}

/**
 * Do some action before and/or after disconnect an input pin from
 * another objects pin.
 */
void VimusMachineVideoCapture::disconnectInput (VimusMachineObject * srcObj, int outPin, int inPin)
{
	srcObj->outputPins[outPin]->disconnect(this->inputPins[inPin]);
}
