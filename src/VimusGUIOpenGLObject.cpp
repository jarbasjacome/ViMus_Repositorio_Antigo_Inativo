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

#include "VimusGUIOpenGLObject.h"

using namespace std;

/**
 * VimusGUIOpenGLObject constructor.
 */
VimusGUIOpenGLObject :: VimusGUIOpenGLObject (  const string& label, float posX,
                                                float posY, float posZ,
                                                int numInputs, int numOutputs,
                                                VimusMachineOpenGLObject* machineObj)
{
    //VimusGUINormalObject::VimusGUINormalObject( label, posX, posY, posZ, numInputs, numOutputs);

    isMinimized3D = false;
    isCube = false;

	this->posX = posX;
    this->posY = posY;
    this->posZ = posZ;

    this->scaleX = 1.0;
    this->scaleY = 1.0;
    this->scaleZ = 1.0;

	this->setLabel(label);

	this->setSizeByLabel();

	this->setPins(numInputs, numOutputs);

    this->machineGLObj = machineObj;

    if (DEBUG_MODE)
        cout << "\nVimusGUIOpenGLObject constructed.";
}

/**
 * VimusGUIOpenGLObject destructor.
 */
VimusGUIOpenGLObject :: ~VimusGUIOpenGLObject()
{
}

/**
 * Update VimusGUIOpenGLObject.
 */
void VimusGUIOpenGLObject::update()
{
}

/**
 * Update VimusGUIOpenGLObject.
 */
void VimusGUIOpenGLObject::draw(int rendermode)
{
    VimusGUINormalObject::draw(rendermode);
}

void VimusGUIOpenGLObject::keyBoardFunc(unsigned char key, int x, int y)
{
    this->machineGLObj->keyBoardFunc(key,x,y);
}

void VimusGUIOpenGLObject::specialKeyBoardFunc(int key, int x, int y)
{
    this->machineGLObj->specialKeyBoardFunc(key,x,y);
}
