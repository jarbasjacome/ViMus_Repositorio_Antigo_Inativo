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

#include "VimusMachinePinVideo.h"

using namespace std;

/**
 * VimusMachinePinVideo constructor.
 */
VimusMachinePinVideo::VimusMachinePinVideo (bool isInput, int index)
{
    parentIndex = -1;
    indexOnParent = index;
	this->type = VimusMachinePin::TYPE_VIDEO;
    this->isInput = isInput;

    for (int i=0; i<MAX_CONNECTIONS; i++)
        connections[i] = NULL;
    numConnections = 0;
    if (DEBUG_MODE)
        cout << "\nVimusMachinePinVideo constructed.";
}

/**
 * VimusMachinePinVideo destructor.
 */
VimusMachinePinVideo::~VimusMachinePinVideo()
{
}
