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

#ifndef _VIMUSGUINORMALOBJECT_H_
#define _VIMUSGUINORMALOBJECT_H_

#include "Configuration.h"

#include "VimusGUIObject.h"

#include <stdlib.h>
#include <string>
#include <iostream>

#include <GL/glut.h>

/**
 * VimusGUINormalObject represents a node (unit generator or object)
 * of a VimusPatch.
 *
 * @author Jarbas Jácome
 * @author Mavix
 */
class VimusGUINormalObject : public VimusGUIObject
{
    public:

        /**
         * VimusGUINormalObject default constructor.
         */
        VimusGUINormalObject();

        /**
         * VimusGUINormalObject constructor.
         */
        VimusGUINormalObject (float posX, float posY, float posZ);

        /**
         * VimusGUINormalObject constructor.
         */
        VimusGUINormalObject(	const std::string& label, float posX,
								float posY, float posZ,
								int numInputs, int numOutputs);

        /**
         * VimusGUINormalObject destructor.
         */
        ~VimusGUINormalObject();

        /**
         * Update VimusGUINormalObject.
         */
        void update();

        /**
         * Draw VimusGUINormalObject.
         */
        void draw(int renderMode);

        /**
         * Draw VimusGUINormalObject side.
         */
        void drawSide(int renderMode, int side);

        /**
         * Draw minimized VimusGUINormalObject in 2D.
         */
        void drawMinimized2D(int renderMode);

        /**
         * Translate.
         */
        void translate(float x, float y, float z);


        /**
         * Draw minimized VimusGUINormalObject in 3D.
         */
        void drawMinimized3D(int renderMode);

        /**
         * Compute width by label size.
         */
        void setSizeByLabel();

        void keyBoardFunc(unsigned char key, int x, int y);

		void specialKeyBoardFunc(int key, int x, int y);

    private:

};

#endif //_VIMUSGUINORMALOBJECT_H_
