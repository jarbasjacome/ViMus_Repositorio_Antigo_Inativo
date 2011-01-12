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

#include "VimusMachineCVThreshold.h"

using namespace std;
using namespace cv;

/**
 * VimusMachineCVThreshold default constructor.
 */
VimusMachineCVThreshold::VimusMachineCVThreshold()
{
    VimusMachineObject::initObject();

	this->label = string ("pixel_cvthreshold");
	this->addInputPin(VimusMachinePin::TYPE_VIDEO);
	this->addInputPin(VimusMachinePin::TYPE_CONTROL);

	this->addOutputPin(VimusMachinePin::TYPE_VIDEO);

	this->ppNullFrame = new unsigned char*;
	(*this->ppNullFrame) = new unsigned char[VIDEO_WIDTH*VIDEO_HEIGHT*3];
	for (int i=0; i<VIDEO_WIDTH*VIDEO_HEIGHT*3; i++)
	{
		(*this->ppNullFrame)[i] = 0;
	}

	// do not assign another valor to it.
	this->ppOutputData = new unsigned char*;
	(*this->ppOutputData) = new unsigned char[VIDEO_WIDTH*VIDEO_HEIGHT*3];
	for (int i=0; i<VIDEO_WIDTH*VIDEO_HEIGHT*3; i++)
	{
		(*this->ppOutputData)[i] = 0;
	}

	// set video output pin
	((VimusMachinePinVideo *)this->outputPins[0])->ppFrame = this->ppOutputData;

	// do not assign another valor to it.
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

//	this->frame->data = (*this->ppInputData);
//	this->frameDst->data = this->frame->data;
//    (*this->ppOutputData) = this->frameDst->data;

	this->frame.data = (*this->ppInputData);
	this->frameDst.data = this->frame.data;
    (*this->ppOutputData) = this->frameDst.data;

    if (DEBUG_MODE)
        cout << "\nVimusMachineCVThreshold constructed.";
}

/**
 * VimusMachineCVThreshold destructor.
 */
VimusMachineCVThreshold::~VimusMachineCVThreshold()
{
}

/**
 * Update VimusMachineCVThreshold.
 */
void VimusMachineCVThreshold::update()
{
//	for (int i=0; i<VIDEO_WIDTH*VIDEO_HEIGHT*3; i++)
//	{
//		(*this->ppOutputData)[i] = (*this->ppInputData)[i] + 150;
//	}
    try
    {
        this->frame.data = (*this->ppInputData);
        threshold(this->frame, this->frameDst, 220, 255, CV_THRESH_BINARY);
        (*this->ppOutputData) = this->frameDst.data;
    }
    catch  (...)
    {
    }
}

/**
 * Get pointer to currentFrame buffer pointer.
 * This is necessary for GUI have a direct access to
 * machine frame buffer.
 */
unsigned char** VimusMachineCVThreshold::getCurrentFramePointer()
{
	return this->ppOutputData;
}

/**
 * Do some action before and/or after connect a pin to
 * another objects pin.
 */
bool VimusMachineCVThreshold::connectOutput (int outPin, VimusMachineObject * dstObj, int inPin)
{
	return dstObj->connectInput(this, outPin, inPin);
}

/**
 * Do some action before and/or after be connected by another
 * objects pin.
 */
bool VimusMachineCVThreshold::connectInput (VimusMachineObject * srcObj, int outPin, int inPin)
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
void VimusMachineCVThreshold::disconnectOutput (int outPin, VimusMachineObject * dstObj, int inPin)
{
	dstObj->disconnectInput(this, outPin, inPin);
}

/**
 * Do some action before and/or after disconnect an input pin from
 * another objects pin.
 */
void VimusMachineCVThreshold::disconnectInput (VimusMachineObject * srcObj, int outPin, int inPin)
{
	srcObj->outputPins[outPin]->disconnect(this->inputPins[inPin]);
	this->ppInputData = this->ppNullFrame;
}
