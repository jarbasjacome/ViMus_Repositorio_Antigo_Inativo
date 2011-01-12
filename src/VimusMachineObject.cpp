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

#include "VimusMachineObject.h"

using namespace std;

/**
 * VimusMachineObject default constructor.
 */
VimusMachineObject :: VimusMachineObject ()
{
	this->initObject();
}

/**
* VimusMachineObject destructor.
*/
VimusMachineObject :: ~VimusMachineObject()
{
}

/**
 * Initializes object default values.
 */
void VimusMachineObject :: initObject()
{
    parentAbsObj = NULL;
    indexOnParent = -1;
    isAbstractionObject = false;
    isInterfaceObject = false;
    label = "";
	numInputs = numOutputs = 0;
}

/**
 * Get input Pin by index.
 */
VimusMachinePin * VimusMachineObject::getInputByIndex(int index)
{
    VimusMachinePin * pin = NULL;
    if (inputPins[index])
        pin = inputPins[index];
    return pin;
}

/**
 * Get output Pin by index.
 */
VimusMachinePin * VimusMachineObject::getOutputByIndex(int index)
{
    VimusMachinePin * pin = NULL;
    if (outputPins[index])
        pin = outputPins[index];
    return pin;
}

/**
 * Set parent.
 */
void VimusMachineObject::setParent(VimusMachineObject * parent, int index)
{
    parentAbsObj = parent;
    indexOnParent = index;
    for (int i=0; i<numInputs; i++)
    {
        inputPins[i]->setParentIndex(index);
    }
    for (int i=0; i<numOutputs; i++)
    {
        outputPins[i]->setParentIndex(index);
    }
}

/**
 * Add input Pin.
 */
void VimusMachineObject :: addInputPin()
{
	this->numInputs++;
	if (this->numInputs > 0)
		inputPins[this->numInputs-1] = new VimusMachinePinControl (
												true,
												this->numInputs-1);
}

/**
 * Add output Pin.
 */
void VimusMachineObject :: addOutputPin()
{
	this->numOutputs++;
	if (this->numOutputs > 0)
		outputPins[this->numOutputs-1] = new VimusMachinePinControl(
												false,
												this->numOutputs-1);
}

/**
 * Add input Pin.
 */
void VimusMachineObject :: addInputPin(int type)
{
	this->numInputs++;
	if (this->numInputs > 0)
	{
		if (type == VimusMachinePin::TYPE_VIDEO)
			inputPins[this->numInputs-1] = new VimusMachinePinVideo (
												true,
												this->numInputs-1);
		else
			inputPins[this->numInputs-1] = new VimusMachinePinControl (
												true,
												this->numInputs-1);
	}
}

/**
 * Add output Pin.
 */
void VimusMachineObject :: addOutputPin(int type)
{
	this->numOutputs++;
	if (this->numOutputs > 0)
	{
		if (type == VimusMachinePin::TYPE_VIDEO)
		{
			outputPins[this->numOutputs-1] = new VimusMachinePinVideo (
												false,
												this->numOutputs-1);
		}
		else
		{
			outputPins[this->numOutputs-1] = new VimusMachinePinControl (
												false,
												this->numOutputs-1);
		}
	}
}

/**
 * Set up input and output Pins.
 */
void VimusMachineObject :: setPins(int numInputs, int numOutputs)
{
    this->numInputs = numInputs;
    this->numOutputs = numOutputs;

    for (int i=0; i<numInputs; i++)
    {
        inputPins[i] = new VimusMachinePinControl(true, i);
    }

    for (int i=0; i<numOutputs; i++)
    {
        outputPins[i] = new VimusMachinePinControl(false, i);
    }
}
