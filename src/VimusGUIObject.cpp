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

#include "VimusGUIObject.h"

using namespace std;

/**
 * VimusGUIObject default constructor.
 */
VimusGUIObject :: VimusGUIObject ()
{
    parentCube = NULL;
    parentCubeCurrentSide = 0;
    indexOnParent = -1;
    isCube = false;
    isInterfaceObject = false;
    isFixedOnParent = false;
    isSelected = false;
    label = NULL;
}

/**
 * Set label.
 */
void VimusGUIObject :: setLabel(const string& lab)
{
	this->label = new char[256];
	lab.copy(this->label, lab.size(), 0);
	this->label[lab.size()]='\0';
}

/**
 * Set up input and output Pins.
 */
void VimusGUIObject :: setPins(int numInputs, int numOutputs)
{
    this->numInputs = numInputs;
    this->numOutputs = numOutputs;

	float inputsDist = 0.0;
    float outputsDist = 0.0;
    if (numInputs > 1)
        inputsDist = (length-0.05)/(numInputs-1);
    if (numOutputs > 1)
        outputsDist = (length-0.05)/(numOutputs-1);
    float distTemp = 0.0;

    for (int i=0; i<numInputs; i++)
    {
		inputPins[i] = new VimusGUIPin(posX+distTemp, posY + height - 0.03, posZ, VimusGUIPin::TYPE_CONTROL, true, i, this->indexOnParent);
        distTemp += inputsDist;
    }

    distTemp = 0.0;
    for (int i=0; i<numOutputs; i++)
    {
        outputPins[i] = new VimusGUIPin(posX+distTemp, posY, posZ, VimusGUIPin::TYPE_CONTROL, false, i, this->indexOnParent);
        distTemp += outputsDist;
    }
}

/**
 * Add input Pin.
 */
void VimusGUIObject :: addInputPin()
{
	this->numInputs++;
	float inputsDist = 0.0;
    if (this->numInputs > 1)
        inputsDist = (length-0.05)/(this->numInputs-1);
    float distTemp = 0.0;

    for (int i=0; i<this->numInputs-1; i++)
    {
        inputPins[i]->setPos(posX+distTemp, posY + height - 0.03, posZ);
        distTemp += inputsDist;
    }
	if (this->numInputs > 0)
		inputPins[this->numInputs-1] = new VimusGUIPin (posX + distTemp,
													posY + height - 0.03,
													posZ,
													VimusGUIPin::TYPE_CONTROL,
													true,
													this->numInputs-1,
													this->indexOnParent);
}

/**
 * Add output Pin.
 */
void VimusGUIObject :: addOutputPin()
{
	this->numOutputs++;
	float outputsDist = 0.0;
    if (this->numOutputs > 1)
        outputsDist = (length-0.05)/(this->numOutputs-1);
    float distTemp = 0.0;

    for (int i=0; i<this->numOutputs-1; i++)
    {
        outputPins[i]->setPos(posX+distTemp, posY + height - 0.03, posZ);
        distTemp += outputsDist;
    }
	if (this->numOutputs > 0)
		outputPins[this->numOutputs-1] = new VimusGUIPin(
													posX + distTemp,
													posY,
													posZ,
													VimusGUIPin::TYPE_CONTROL,
													false,
													this->numOutputs-1,
													this->indexOnParent);
}

/**
* VimusGUIObject destructor.
*/
VimusGUIObject :: ~VimusGUIObject()
{
}

/**
 * Move to new position.
 */
/*
void VimusGUIObject::moveTo(float posX, float posY, float posZ)
{
    float movX, movY, movZ;
    movX = posX - this->posX;
    movY = posY - this->posY;
    movZ = posZ - this->posZ;

    for (int i=0; i<numInputs; i++)
    {
        inputPins[i]->posX += movX;
        inputPins[i]->posY += movY;
        inputPins[i]->posZ += movZ;
    }

    for (int i=0; i<numOutputs; i++)
    {
        outputPins[i]->posX += movX;
        outputPins[i]->posY += movY;
        outputPins[i]->posZ += movZ;
    }

    this->posX = posX;
    this->posY = posY;
    this->posZ = posZ;
}
*/

/**
 * Get input VimusGUIPin by index.
 */
VimusGUIPin * VimusGUIObject::getInputByIndex(int index)
{
    VimusGUIPin * pin = NULL;
    if (inputPins[index])
        pin = inputPins[index];
    return pin;
}

/**
 * Get output VimusGUIPin by index.
 */
VimusGUIPin * VimusGUIObject::getOutputByIndex(int index)
{
    VimusGUIPin * pin = NULL;
    if (outputPins[index])
        pin = outputPins[index];
    return pin;
}

/**
 * Turn minimized3D on.
 */
void VimusGUIObject::enableMinimized3D()
{
    isMinimized3D = true;
}

/**
 * Turn minimized3D off.
 */
void VimusGUIObject::disableMinimized3D()
{
    isMinimized3D = false;
}

/**
 * Set parent.
 */
void VimusGUIObject::setParent(VimusGUIObject * parent, int index,
							   int parentCubeCurrentSide)
{
    this->parentCube = parent;
    this->indexOnParent = index;
	this->parentCubeCurrentSide = parentCubeCurrentSide;
    for (int i=0; i<numInputs; i++)
    {
        inputPins[i]->setParentIndex(index);
    }
    for (int i=0; i<numOutputs; i++)
    {
        outputPins[i]->setParentIndex(index);
    }
}

/**
 * Renders a bitmap string.
 */
void VimusGUIObject::renderBitmapString(
		float x,
		float y,
		float z,
		void *font,
		const char *string)
{
    const char *c;
    glRasterPos3f(x, y, z);
    for (c=string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

/**
 * Renders a stroke font string.
 */
void VimusGUIObject::renderStrokeFontString(
		float x,
		float y,
		float z,
		void *font,
		const char *lab)
{
	const char *c;
    glPushMatrix();
        glLineWidth(1.5);
        glTranslatef(x, y, z);
        glScalef(0.0007, 0.0007, 1.0);
        for (c=lab; *c != '\0'; c++)
        {
            glutStrokeCharacter(font, *c);
        }
        glLineWidth(1.0);
	glPopMatrix();
}

/**
 * Compute string width.
 */
float VimusGUIObject::bitmapStringWidth(void *font, const char *string)
{
    int sWidth = 0;
    float width = 0;
    const char *c;
    for (c=string; *c != '\0'; c++)
    {
        sWidth = sWidth + glutBitmapWidth(font, *c);
    }
    width = 10; //TODO ((float) 2 * sWidth)/ (float) glutGet(GLUT_SCREEN_WIDTH);
    return width;
}

/**
 * Compute stroke font string width.
 */
float VimusGUIObject::strokeFontStringWidth(void *font, const char *string)
{
    int sWidth = 0;
    float width = 0;
    const char *c;
    for (c=string; *c != '\0'; c++)
    {
        sWidth = sWidth + glutStrokeWidth(font, *c); //TODO:
    }
    width = ((float) 2 * sWidth)/ (float) glutGet(GLUT_SCREEN_WIDTH);
    return width;
}

/**
 * Draw graph-on-parent indication in 2D.
 */
void VimusGUIObject::drawGraphOnParent2D()
{
    glPushMatrix();
        glTranslatef(posX, posY, posZ);

        if (isFixedOnParent)
        {
            glColor4f(1.0, 1.0, 1.0, 0.5);
            glBegin(GL_QUADS);
                glVertex3f (length, 0.0, 0.0);
                glVertex3f (length, height, 0.0);
                glVertex3f (length+0.05, height, 0.0);
                glVertex3f (length+0.05, 0.0, 0.0);
            glEnd();
        }
    glPopMatrix();
}

/**
 * Draw graph-on-parent indication in 3D.
 */
void VimusGUIObject::drawGraphOnParent3D()
{
    glPushMatrix();
        glTranslatef(posX, posY, posZ);

        if (isFixedOnParent)
        {
            glTranslatef(length + length/16, height/2, 0.0);
            glPushMatrix();
                glScalef(length/8, height, width);

                glColor4f(1.0, 1.0, 1.0, 0.2);
                glutSolidCube(1.0);
            glPopMatrix();
        }

    glPopMatrix();
}

void VimusGUIObject::setPosControlPanel(float x, float y, float z)
{
    posXControlPanel = x;
    posYControlPanel = y;
    posZControlPanel = z;
}

void VimusGUIObject::setSizeControlPanel(float w, float h)
{
    widthControlPanel = w;
    heightControlPanel = h;

    lengthControlPanel = w;

    scaleX = w/(SCREEN_PLANE_WIDTH);
    scaleY = h/(SCREEN_PLANE_HEIGHT);
    scaleZ = lengthControlPanel/(SCREEN_PLANE_WIDTH);
}
