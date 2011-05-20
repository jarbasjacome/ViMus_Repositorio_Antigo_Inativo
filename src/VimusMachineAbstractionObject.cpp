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

#include "VimusMachineAbstractionObject.h"

using namespace std;

/**
 * VimusGUICube constructor.
 */
VimusMachineAbstractionObject :: VimusMachineAbstractionObject (const string& label)
{
	this->label = label;
	this->numInputs = 0;
    this->numOutputs = 0;
	this->numObjects = 0;
	this->isAbstractionObject = true;

	for (int i=0; i<MAX_OBJ; i++)
	{
		this->objects[i] = NULL;
	}

//	this->setPins (0, 0);
//	this->setLabel(label);

    if (DEBUG_MODE)
        cout << "\nVimusMachineAbstractionObject constructed.";
}


/**
* VimusMachineAbstractionObject destructor.
*/
VimusMachineAbstractionObject :: ~VimusMachineAbstractionObject()
{
	if (objects)
    {
        for (int i=0; i<MAX_OBJ; i++)
        {
            if (objects[i] != NULL)
            {
                delete objects[i];
                objects[i] = NULL;
            }

        }
    }
}

/**
 * Update VimusMachineAbstractionObject.
 */
void VimusMachineAbstractionObject ::update()
{
    updatePatch();
}

/**
 * Add a machine object to abstraction.
 */
void VimusMachineAbstractionObject::addObject(VimusMachineObject * object)
{
    if (numObjects < MAX_OBJ)
    {
        objects[numObjects] = object;
		objects[numObjects]->setParent(this, numObjects);

		string input ("inlet");
		string output ("outlet");
		if (!input.compare(objects[numObjects]->label))
		{
			this->addInputPin();
		}
		else if (!output.compare(objects[numObjects]->label))
		{
			this->addOutputPin();
		}
        numObjects++;
    }

}

/**
 * Update patch.
 */
void VimusMachineAbstractionObject::updatePatch()
{
    for (int i=0; i<numObjects; i++)
    {
        objects[i]->update();
    }
}


/**
 * Connect an output to another object input.
 */
//void VimusMachineAbstractionObject::connect(VimusMachinePin * output, VimusMachinePin * input)
//{
//    if (!output->isInput && input->isInput)
//        output->connect(input);
//}

/**
 * Remove a machine object from abstraction.
 */
void VimusMachineAbstractionObject::removeObject(int i)
{
	delete objects[i];
    for (;i<numObjects;i++)
    {
        if (i < numObjects-1)
        {
            objects[i] = objects[i+1];
			objects[i]->indexOnParent=i;
			for (int j=0; j<objects[i]->numInputs; j++)
			{
				objects[i]->inputPins[j]->parentIndex=i;
			}
			for (int j=0; j<objects[i]->numOutputs; j++)
			{
				objects[i]->outputPins[j]->parentIndex=i;
			}
        }
        else
        {
            objects[i] = NULL;
            numObjects--;
        }
    }
}

/**
 * Do some action before and/or after connect a pin to
 * another objects pin.
 */
bool VimusMachineAbstractionObject::connectOutput (int outPin, VimusMachineObject * dstObj, int inPin)
{
	return dstObj->connectInput(this, outPin, inPin);
}

/**
 * Do some action before and/or after be connected by another
 * objects pin.
 */
bool VimusMachineAbstractionObject::connectInput (VimusMachineObject * srcObj, int outPin, int inPin)
{
	return srcObj->outputPins[outPin]->connect(this->inputPins[inPin]);
}

/**
 * Do some action before and/or after disconnect an output pin from
 * another objects pin.
 */
void VimusMachineAbstractionObject::disconnectOutput (int outPin, VimusMachineObject * dstObj, int inPin)
{
	dstObj->disconnectInput(this, outPin, inPin);
}

/**
 * Do some action before and/or after disconnect an input pin from
 * another objects pin.
 */
void VimusMachineAbstractionObject::disconnectInput (VimusMachineObject * srcObj, int outPin, int inPin)
{
	srcObj->outputPins[outPin]->disconnect(this->inputPins[inPin]);
}
