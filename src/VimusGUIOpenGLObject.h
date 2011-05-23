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

#ifndef _VIMUSGUIOPENGLOBJECT_H_
#define _VIMUSGUIOPENGLOBJECT_H_

#include "Configuration.h"

#include "VimusGUINormalObject.h"
#include "VimusMachineOpenGLObject.h"

#include <stdlib.h>
#include <string>
#include <GL/glut.h>

/**
 * VimusGUIOpenGLObject represents an OpenGL call object.
 *
 * @author Jarbas Jácome
 */
class VimusGUIOpenGLObject : public VimusGUINormalObject
{
    public:

        /**
         * VimusGUIOpenGLObject constructor.
         */
        VimusGUIOpenGLObject(	const std::string& label, float posX,
								float posY, float posZ,
								int numInputs, int numOutputs,
								VimusMachineOpenGLObject* machineObj);

        /**
         * VimusGUIOpenGLObject destructor.
         */
        ~VimusGUIOpenGLObject();

        /**
         * Update VimusGUIOpenGLObject.
         */
        void update();

        /**
         * Draw VimusGUIOpenGLObject.
         */
        void draw(int rendermode);

        void keyBoardFunc(unsigned char key, int x, int y);

		void specialKeyBoardFunc(int key, int x, int y);

    private:

        VimusMachineOpenGLObject* machineGLObj;

};

#endif //_VIMUSGUIOPENGLOBJECT_H_
