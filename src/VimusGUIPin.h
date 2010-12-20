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

#ifndef _VIMUSGUIPIN_H_
#define _VIMUSGUIPIN_H_

#include "Configuration.h"
#include <stdlib.h>
#include <GL/glut.h>

/**
 * VimusGUIPin represents an object input or output pin.
 *
 * @author Jarbas Jácome
 * @author Mavix
 */
class VimusGUIPin
{
    public:

        const static int TYPE_CONTROL = 0;
        const static int TYPE_AUDIO = 1;
        const static int TYPE_VIDEO = 2;

        float posX;
        float posY;
        float posZ;

        //bool isSelected;

        /**
         * VimusGUIPin default constructor.
         */
        VimusGUIPin();

        /**
         * VimusGUIPin constructor.
         */
        VimusGUIPin (   float posX, float posY, float posZ,
                int type, bool input, int index, int parentIndex);

        /**
         * VimusGUIPin destructor.
         */
        ~VimusGUIPin();

        /**
         * Connect to another pin.
         */
        void connect(VimusGUIPin *pin);

        /**
         * Disconnect a pin.
         */
        void disconnect(VimusGUIPin *pin);

        /**
         * Draw pin.
         */
        void draw(int renderMode);

        bool isInput;

        int type;

        int numConnections;

        const static int MAX_CONNECTIONS = 30;
        VimusGUIPin * connections[MAX_CONNECTIONS];

		bool selOutConnections[MAX_CONNECTIONS];

        int parentIndex;

        int indexOnParent;

        void setParentIndex(int pIndex);

        void setIndexOnParent(int index);

        void removeConnection(int index);

		void setPos(float x, float y, float z);

    private:


};

#endif //_VIMUSGUIPIN_H_
