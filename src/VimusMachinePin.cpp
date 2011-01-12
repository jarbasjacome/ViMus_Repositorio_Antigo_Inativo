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

#include "VimusMachinePin.h"

using namespace std;

/**
 * VimusMachinePin default constructor.
 */
VimusMachinePin::VimusMachinePin()
{
    parentIndex = -1;
    indexOnParent = -1;
}

/**
 * VimusMachinePin destructor.
 */
VimusMachinePin::~VimusMachinePin()
{
}

/**
 * Connect to another pin.
 */
bool VimusMachinePin::connect(VimusMachinePin *pin)
{
	bool connected = false;
	if (numConnections >= MAX_CONNECTIONS)
	{
	    cout << "number of connected pins exceed./n"; //TODO EXCEPTION
		return false;
	}
	//TODO: accept multiple input connections in some cases.
	if (pin->numConnections > 0)
    {
		cout << "already connected with another pin./n";  //TODO EXCEPTION
		return false;
	}
    if (pin->isInput)
    {
		if (this->type != pin->type)
		{
			 cout << "can not connect diferent types./n";
			return false;
		}
        for (int i = 0; i<numConnections; i++)
        {
            // check if it is already connected.
            if ((connections[i]->indexOnParent == pin->indexOnParent)
                && (connections[i]->parentIndex == pin->parentIndex))
			{
				cout << "already connected./n";
                return false;
			}
        }
        connections[numConnections] = pin;
        numConnections++;

        pin->connections[pin->numConnections] = this;
        pin->numConnections++;

		connected = true;
    }
	return connected;
}

/**
 * Disconnect a pin.
 */
void VimusMachinePin::disconnect(VimusMachinePin *pin)
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

void VimusMachinePin::removeConnection(int i)
{
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

void VimusMachinePin::setParentIndex(int pIndex)
{
    parentIndex = pIndex;
}

void VimusMachinePin::setIndexOnParent(int index)
{
    indexOnParent = index;
}
