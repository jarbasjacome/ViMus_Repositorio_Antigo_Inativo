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

#ifndef _VIMUSMACHINEPIN_H_
#define _VIMUSMACHINEPIN_H_

#include <stdlib.h>
#include <iostream>
#include "Configuration.h"

/**
 * VimusMachinePin represents an object input or output pin in ViMus Machine.
 *
 * @author Jarbas Jácome
 */
class VimusMachinePin
{
    public:

        const static int TYPE_CONTROL = 0;
        const static int TYPE_AUDIO = 1;
        const static int TYPE_VIDEO = 2;

        /**
         * VimusMachinePin default constructor.
         */
        VimusMachinePin();

        /**
         * VimusMachinePin destructor.
         */
        ~VimusMachinePin();

        /**
         * Connect to another pin.
         */
		bool connect(VimusMachinePin *pin);

        /**
         * Disconnect a pin.
         */
        void disconnect(VimusMachinePin *pin);

        bool isInput;

        int type;

        int numConnections;

        const static int MAX_CONNECTIONS = 30;
        VimusMachinePin * connections[MAX_CONNECTIONS];

        int parentIndex;

        int indexOnParent;

        void setParentIndex(int pIndex);

        void setIndexOnParent(int index);

        void removeConnection(int index);

    private:


};

#endif //_VIMUSMACHINEPIN_H_
