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

#include "VimusMachineNormalObject.h"

using namespace std;

/**
 * VimusMachineNormalObject default constructor.
 */
VimusMachineNormalObject :: VimusMachineNormalObject ()
{
    VimusMachineObject::initObject();
    if (DEBUG_MODE)
        cout << "\nVimusMachineNormalObject constructed.";
}

/**
 * VimusMachineNormalObject constructor.
 */
VimusMachineNormalObject :: VimusMachineNormalObject (const string& label, int numInputs, int numOutputs)
{
    this->label = label;
	this->setPins (numInputs, numOutputs);
}

/**
 * VimusMachineNormalObject destructor.
 */
VimusMachineNormalObject :: ~VimusMachineNormalObject()
{
}

/**
 * Update VimusMachineNormalObject.
 */
void VimusMachineNormalObject::update()
{
}

/**
 * Do some action before and/or after connect a pin to
 * another objects pin.
 */
bool VimusMachineNormalObject::connectOutput (int outPin, VimusMachineObject * dstObj, int inPin)
{
	return dstObj->connectInput(this, outPin, inPin);
}

/**
 * Do some action before and/or after be connected by another
 * objects pin.
 */
bool VimusMachineNormalObject::connectInput (VimusMachineObject * srcObj, int outPin, int inPin)
{
	return srcObj->outputPins[outPin]->connect(this->inputPins[inPin]);
}

/**
 * Do some action before and/or after disconnect an output pin from
 * another objects pin.
 */
void VimusMachineNormalObject::disconnectOutput (int outPin, VimusMachineObject * dstObj, int inPin)
{
	dstObj->disconnectInput(this, outPin, inPin);
}

/**
 * Do some action before and/or after disconnect an input pin from
 * another objects pin.
 */
void VimusMachineNormalObject::disconnectInput (VimusMachineObject * srcObj, int outPin, int inPin)
{
	srcObj->outputPins[outPin]->disconnect(this->inputPins[inPin]);
}
