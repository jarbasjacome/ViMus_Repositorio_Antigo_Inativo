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

#include "VimusGUIVideoPreview.h"

using namespace std;

/**
 * VimusGUIVideoPreview default constructor.
 */
VimusGUIVideoPreview::VimusGUIVideoPreview() : VimusGUIObject ()
{
    if (DEBUG_MODE)
        cout << "\nVimusGUIVideoPreview constructed.";
}

/**
 * VimusGUIVideoPreview constructor.
 */
VimusGUIVideoPreview::VimusGUIVideoPreview(float posX,
										   float posY,
										   float posZ,
										   unsigned char ** ppFrame
										   ) : VimusGUIObject ()
{
	this->setLabel("video");
    this->isFixedOnParent = true;

    this->posX = posX;
    this->posY = posY;
    this->posZ = posZ;

    this->width = (SCREEN_PLANE_WIDTH)/5;
    this->height = (SCREEN_PLANE_HEIGHT)/5;
    this->length = (SCREEN_PLANE_WIDTH)/5;

    this->scaleX = 1.0;
    this->scaleY = 1.0;
    this->scaleZ = 1.0;

    isMinimized3D = false;
    isCube = false;

	this->setPins(1,1);

    this->setPosControlPanel(-0.95, -0.95, 0.0);
    this->setSizeControlPanel(1.9, 1.9);

	// exactly the same pp of VimusMachineVideoPreview
	this->ppCurrentFrame = ppFrame;

    glGenTextures(1, &texName);
    glBindTexture(GL_TEXTURE_2D, texName);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, VIDEO_WIDTH, VIDEO_HEIGHT, 0, GL_BGR, GL_UNSIGNED_BYTE, (*this->ppCurrentFrame));

}

/**
 * VimusGUIVideoPreview destructor.
 */
VimusGUIVideoPreview::~VimusGUIVideoPreview()
{
}

/**
 * Update VimusGUIVideoPreview.
 */
void VimusGUIVideoPreview::update()
{
}

/**
 * Draw VimusGUIVideoPreview side.
 */
void VimusGUIVideoPreview::drawSide(int renderMode, int side)
{
    // desenhar caixa de video no painel de controle.
}

/**
 * Draw VimusGUIVideoPreview.
 */
void VimusGUIVideoPreview::draw(int renderMode)
{

	if (parentCubeCurrentSide == SIDE_CONTROLPANEL)
    {
        glColor4f(1.0, 0.0, 0.0, 1.0);
        if (isSelected)
            glColor4f(0.3, 0.3, 1.0, 1.0);

        glPushMatrix();
            glTranslatef(posXControlPanel, posYControlPanel, posZControlPanel);

            if (renderMode == GL_SELECT)
                glBegin(GL_QUADS);
            else
                glBegin(GL_LINE_LOOP);
                glVertex3f (0.0, 0.0, 0.0);
                glVertex3f (lengthControlPanel, 0.0, 0.0);
                glVertex3f (lengthControlPanel, heightControlPanel, 0.0);
                glVertex3f (0.0, heightControlPanel, 0.0);
            glEnd();

			this->drawFrame(0.0, 0.0, 0.0, lengthControlPanel, heightControlPanel);

			glFlush ();

        glPopMatrix();

        if (renderMode == GL_SELECT)
            glPushName(0);

    }
	else if (parentCubeCurrentSide == SIDE_PATCH)
    {

        glColor4f(1.0, 0.0, 0.0, 1.0);
        if (isSelected)
            glColor4f(0.3, 0.3, 1.0, 1.0);

        glPushMatrix();
            glTranslatef(posX, posY, posZ);

            if (renderMode == GL_SELECT)
                glBegin(GL_QUADS);
            else
                glBegin(GL_LINE_LOOP);
                glVertex3f (0.0, 0.0, 0.0);
                glVertex3f (length, 0.0, 0.0);
                glVertex3f (length, height, 0.0);
                glVertex3f (0.0, height, 0.0);
            glEnd();

			this->drawFrame(0.0,0.0,0.0,length, height);

        glPopMatrix();

		glColor4f(1.0, 0.0, 0.0, 1.0);
        if (isSelected)
            glColor4f(0.3, 0.3, 1.0, 1.0);

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

        if (isFixedOnParent)
        {
            drawGraphOnParent2D();
        }
    }
}

void VimusGUIVideoPreview::translate(float x, float y, float z)
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

/**
 * Draw currrent video frame.
 */
void VimusGUIVideoPreview :: drawFrame(	float posX, float posY, float posZ,
										float width, float height)
{
	glDisable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	if (*(this->ppCurrentFrame))
	{
		glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
        glBindTexture(GL_TEXTURE_2D, texName);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, VIDEO_WIDTH, VIDEO_HEIGHT, 0, GL_BGR, GL_UNSIGNED_BYTE, (*this->ppCurrentFrame));

		glBegin(GL_QUADS);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(posX, posY, posZ);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(posX+width, posY, posZ);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(posX+width, posY+height, posZ);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(posX, posY+height, posZ);
		glEnd();
	}

	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

}

void VimusGUIVideoPreview::keyBoardFunc(unsigned char key, int x, int y)
{
}

void VimusGUIVideoPreview::specialKeyBoardFunc(int key, int x, int y)
{
}
