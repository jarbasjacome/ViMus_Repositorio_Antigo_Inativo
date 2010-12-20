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

#include "VimusGUIPin.h"

/**
 * VimusGUIPin default constructor.
 */
VimusGUIPin::VimusGUIPin()
{
//    isSelected = false;
    parentIndex = -1;
    indexOnParent = -1;
}

/**
 * VimusGUIPin constructor.
 */
VimusGUIPin::VimusGUIPin (float posX, float posY, float posZ,
          int type, bool isInput, int index, int parentIndex)
{
//    isSelected = false;
    this->parentIndex = parentIndex;
    this->indexOnParent = index;
    this->posX = posX;
    this->posY = posY;
    this->posZ = posZ;
    this->type = type;
    this->isInput = isInput;

    for (int i=0; i<MAX_CONNECTIONS; i++)
	{
        connections[i] = NULL;
		selOutConnections[i] = false;
	}
    numConnections = 0;
}

/**
 * VimusGUIPin destructor.
 */
VimusGUIPin::~VimusGUIPin()
{
}

/**
 * Connect to another pin.
 */
void VimusGUIPin::connect(VimusGUIPin *pin)
{
    if ((numConnections < MAX_CONNECTIONS) &&
        (pin->numConnections < MAX_CONNECTIONS))
    {
        if (pin->isInput)
        {
            for (int i = 0; i<numConnections; i++)
            {
                // check if it is already connected.
                if ((connections[i]->indexOnParent == pin->indexOnParent)
                    && (connections[i]->parentIndex == pin->parentIndex))
                    return;
            }
//            pin->isSelected = false;
//            isSelected = false;
            connections[numConnections] = pin;
			selOutConnections[numConnections] = false;
            numConnections++;

            pin->connections[pin->numConnections] = this;
            pin->numConnections++;
        }
    }
}

/**
 * Disconnect a pin.
 */
void VimusGUIPin::disconnect(VimusGUIPin *pin)
{
    for (int i=0; i<numConnections; i++)
    {
        // check if it is really connected.
        if ((connections[i]->indexOnParent == pin->indexOnParent)
            && (connections[i]->parentIndex == pin->parentIndex))
        {
            removeConnection(i);
            pin->disconnect(this);
            return;
        }
    }
}

void VimusGUIPin::removeConnection(int i)
{
	this->selOutConnections[i] = false;
    for (;i<numConnections;i++)
    {
        if (i < numConnections-1)
        {
            connections[i] = connections[i+1];
        }
        else
        {
            connections[i] = NULL;
            numConnections--;
        }
    }
}

/**
 * Draw pin.
 */
void VimusGUIPin::draw(int renderMode)
{
    glBegin(GL_QUADS);
        glVertex3f (posX, posY, posZ);
        glVertex3f (0.05+posX, posY, posZ);
        glVertex3f (0.05+posX, posY+0.03, posZ);
        glVertex3f (posX, posY+0.03, posZ);
    glEnd();
}

void VimusGUIPin::setParentIndex(int pIndex)
{
    parentIndex = pIndex;
}

void VimusGUIPin::setIndexOnParent(int index)
{
    indexOnParent = index;
}

void VimusGUIPin::setPos(float x, float y, float z)
{
	this->posX = x;
	this->posY = y;
	this->posZ = z;
}
