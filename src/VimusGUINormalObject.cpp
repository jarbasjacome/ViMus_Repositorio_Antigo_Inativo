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

#include "VimusGUINormalObject.h"

using namespace std;

/**
 * VimusGUINormalObject default constructor.
 */
VimusGUINormalObject :: VimusGUINormalObject () : VimusGUIObject ()
{
    if (DEBUG_MODE)
        cout << "\nVimusGUINormalObject constructed.";
}

/**
 * VimusGUINormalObject constructor.
 */
VimusGUINormalObject :: VimusGUINormalObject (const string& label, float posX,
											  float posY, float posZ,
					                        int numInputs, int numOutputs)
{
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

    if (DEBUG_MODE)
        cout << "\nVimusGUINormalObject constructed.";
}

/**
 * VimusGUINormalObject destructor.
 */
VimusGUINormalObject :: ~VimusGUINormalObject()
{
}

/**
 * Draw minimized VimusGUINormalObject in 2D.
 */
void VimusGUINormalObject::drawMinimized2D(int renderMode)
{
    setSizeByLabel();

    glColor4f(1.0, 0.0, 0.0, 1.0);
    if (isSelected)
        glColor4f(0.3, 0.3, 1.0, 1.0);

    glPushMatrix();
        glTranslatef(posX, posY, posZ);

        renderStrokeFontString(0.03, 0.07, 0.0, GLUT_STROKE_ROMAN, label);
        //renderBitmapString(0.03, 0.07, 0.0, GLUT_BITMAP_HELVETICA_18, label);

        if (renderMode == GL_SELECT)
            glBegin(GL_QUADS);
        else
            glBegin(GL_LINE_LOOP);
            glVertex3f (0.0, 0.0, 0.0);
            glVertex3f (length, 0.0, 0.0);
            glVertex3f (length, height, 0.0);
            glVertex3f (0.0, height, 0.0);
        glEnd();

    glPopMatrix();

    if (renderMode == GL_SELECT)
        glPushName(0);

    //draw inputs
    for (int i=0; i<numInputs; i++)
    {
        if (renderMode == GL_SELECT)
            glPushName(i);

        inputPins[i]->draw(renderMode);

        if (renderMode == GL_SELECT)
            glPopName();
    }

    if (renderMode == GL_SELECT)
        glPopName();

    if (renderMode == GL_SELECT)
        glPushName(1);

    //draw outputs
    for (int i=0; i<numOutputs; i++)
    {
        if (renderMode == GL_SELECT)
            glPushName(i);

        outputPins[i]->draw(renderMode);

        if (renderMode == GL_SELECT)
            glPopName();
    }

    if (renderMode == GL_SELECT)
        glPopName();
}

/**
 * Draw minimized VimusGUINormalObject in 3D.
 */
void VimusGUINormalObject::drawMinimized3D(int renderMode)
{
    setSizeByLabel();

    glColor4f(1.0, 0.0, 0.0, 1.0);
    if (isSelected)
        glColor4f(0.3, 0.3, 1.0, 1.0);

    glPushMatrix();
        glTranslatef(posX, posY, posZ);

        renderStrokeFontString(0.03, 0.07, width/2, GLUT_STROKE_ROMAN, label);
        //renderBitmapString(0.03, 0.07, width/2, GLUT_BITMAP_HELVETICA_18, label);

        glTranslatef(length/2, height/2, 0.0);
        glPushMatrix();
            glScalef(length, height, width);
            glutWireCube(1.0);
        glPopMatrix();

    glPopMatrix();

    //draw inputs
    for (int i=0; i<numInputs; i++)
    {
        inputPins[i]->draw(renderMode);
    }

    //draw outputs
    for (int i=0; i<numOutputs; i++)
    {
        outputPins[i]->draw(renderMode);
    }
}

/**
 * Update VimusGUINormalObject.
 */
void VimusGUINormalObject::update()
{
}

/**
 * Draw VimusGUINormalObject.
 */
void VimusGUINormalObject::draw(int renderMode)
{
    if (isMinimized3D)
    {
        drawMinimized3D(renderMode);
    }
    else
    {
        drawMinimized2D(renderMode);
    }
}

/**
 * Draw VimusGUINormalObject side.
 */
void VimusGUINormalObject::drawSide(int renderMode, int side)
{
}

/**
 * Compute width by label size.
 */
void VimusGUINormalObject::setSizeByLabel()
{
    width = 0.2;
    height = 0.2;
    if (label == NULL)
        this->setLabel(" ");
    length = strokeFontStringWidth(GLUT_STROKE_ROMAN, label)*0.7;
}

void VimusGUINormalObject::translate(float x, float y, float z)
{
    posX += x;
    posY += y;
    posZ += z;

    for (int i=0; i<numInputs; i++)
    {
        inputPins[i]->posX += x;
        inputPins[i]->posY += y;
        inputPins[i]->posZ += z;
    }

    for (int i=0; i<numOutputs; i++)
    {
        outputPins[i]->posX += x;
        outputPins[i]->posY += y;
        outputPins[i]->posZ += z;
    }
}

void VimusGUINormalObject::keyBoardFunc(unsigned char key, int x, int y)
{
}

void VimusGUINormalObject::specialKeyBoardFunc(int key, int x, int y)
{
}
