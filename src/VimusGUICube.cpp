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

#include "VimusGUICube.h"

using namespace std;

/**
 * VimusGUICube constructor.
 */
VimusGUICube :: VimusGUICube (const std::string& label, float posX, float posY, float posZ,
							  bool isFixedOnParent)
{
	this->numObjects = 0;
	for (int i=0; i<MAX_OBJ; i++)
	{
		this->objects[i] = NULL;
	}

	this->numInputs = 0;
    this->numOutputs = 0;
	this->setPins (0, 0);

	this->setLabel(label);

	this->setSizeByLabel();

	this->isMinimized3D = false;
    this->isCube = true;
    this->isFixedOnParent = isFixedOnParent;

    this->posX = posXMini = posX;
    this->posY = posYMini = posY;
    this->posZ = posZMini = posZ;

    rotationReset();
    setCurrentSide(SIDE_PATCH);

    widthNormal = (SCREEN_PLANE_WIDTH)/2;
    heightNormal = (SCREEN_PLANE_HEIGHT)/2;
    lengthNormal = widthNormal;
    setPosNormal (0.0, 0.0, 0.0);

    viewMode = VIEW_MINIMIZED;
    setPosMinimized(posX, posY, posZ);

    if (DEBUG_MODE)
        cout << "\nVimusGUICube constructed.";
}

/**
 * VimusGUICube constructor.
 */
VimusGUICube :: VimusGUICube (const string& label, float posX, float posY, float posZ,
                        VimusGUIObject * objs[MAX_OBJ], int numObjs,
                        bool isFixedOnParent)
{
	this->numInputs = 0;
    this->numOutputs = 0;

    char* input = "inlet";
    char* output = "outlet";
    const char* lab;
    bool found = false;
    int j = 0;

    for (int i=0; i<numObjs; i++)
    {
        objects[i] = objs[i];
		objs[i]->setParent(this, i, this->currentSide);
        lab = objs[i]->label;
        if (lab)
        {
            found = false;
            for (j=0; lab[j] != '\0' && input[j] != '\0' && lab[j]==input[j]; j++);
            if (input[j] == '\0')
            {
                found = true;
            }
            if (found)
                numInputs++;
            else
            {
                found = false;
                for (j=0; lab[j] != '\0' && output[j] != '\0' && lab[j]==output[j]; j++);
                if (output[j] == '\0')
                {
                    found = true;
                }
                if (found)
                    numOutputs++;
            }
        }
    }

	this->numObjects = numObjs;

	this->setLabel(label);

	this->setSizeByLabel();

	this->setPins (numInputs, numOutputs);

	this->isMinimized3D = false;
    this->isCube = true;
    this->isFixedOnParent = isFixedOnParent;

    this->posX = posXMini = posX;
    this->posY = posYMini = posY;
    this->posZ = posZMini = posZ;

    rotationReset();
    setCurrentSide(SIDE_PATCH);

    widthNormal = (SCREEN_PLANE_WIDTH)/2;
    heightNormal = (SCREEN_PLANE_HEIGHT)/2;
    lengthNormal = widthNormal;
    setPosNormal (0.0, 0.0, 0.0);

    viewMode = VIEW_MINIMIZED;
    setPosMinimized(posX, posY, posZ);

}

/**
* VimusGUICube destructor.
*/
VimusGUICube :: ~VimusGUICube()
{
    if (objects)
    {
        for (int i=0; i<MAX_OBJ; i++)
        {
            if (objects[i])
            {
                delete objects[i];
                objects[i] = NULL;
            }

        }
    }
}

/**
 * Update VimusGUICube.
 */
void VimusGUICube ::update()
{
    updatePatch();
    if (viewMode == VIEW_MAXIMIZED || viewMode == VIEW_NORMAL)
    {
        updateRotationState();
    }
}

/**
 * Draw VimusGUICube.
 */
void VimusGUICube ::draw(int renderMode)
{
    if (viewMode == VIEW_MINIMIZED)
    {
        if (isMinimized3D)
        {
            drawMinimized3D(renderMode);
            if (isFixedOnParent)
            {
                drawGraphOnParent3D();
            }
        }
        else
        {
            drawMinimized2D(renderMode);
            if (isFixedOnParent)
            {
                drawGraphOnParent2D();
            }
        }
    }
    else if (viewMode == VIEW_NORMAL || viewMode == VIEW_MAXIMIZED)
    {
        glPushMatrix();

            glTranslatef(posX, posY, posZ);

            if (!isMinimized3D)
            {
                glColor4f(1.0, 0.0, 0.0, 1.0);
                renderStrokeFontString( -width/2, height/2+0.03, length/2,
                                        GLUT_STROKE_ROMAN, label);
//                renderBitmapString( -width/2, height/2+0.03, length/2,
//                                        GLUT_BITMAP_HELVETICA_18, label);
            }

            glRotatef(rotationAngleX + rotAngleXInc, 1.0, 0.0, 0.0);
            glRotatef(rotationAngleY + rotAngleYInc, 0.0, 1.0, 0.0);

            drawControlPanel(renderMode, 0.0, 0.0, length/2, width, height);

            if (isSelected && viewMode == VIEW_NORMAL)
            {
                glDisable(GL_BLEND);
                glColor4f(0.0, 0.0, 1.0, 1.0);
                glBegin(GL_QUADS);
                    glVertex3f (-width/2, height/2 - 0.02, length/2+0.01);
                    glVertex3f (-width/2 + 0.02, height/2 - 0.02, length/2+0.01);
                    glVertex3f (-width/2 + 0.02, height/2, length/2+0.01);
                    glVertex3f (-width/2, height/2, length/2+0.01);
                glEnd();

                glBegin(GL_QUADS);
                    glVertex3f (0.0, height/2 - 0.02, length/2+0.01);
                    glVertex3f (0.02, height/2 - 0.02, length/2+0.01);
                    glVertex3f (0.02, height/2, length/2+0.01);
                    glVertex3f (0.0, height/2, length/2+0.01);
                glEnd();

                glBegin(GL_QUADS);
                    glVertex3f (width/2, height/2 - 0.02, length/2+0.01);
                    glVertex3f (width/2 - 0.02, height/2 - 0.02, length/2+0.01);
                    glVertex3f (width/2 - 0.02, height/2, length/2+0.01);
                    glVertex3f (width/2, height/2, length/2+0.01);
                glEnd();

                glBegin(GL_QUADS);
                    glVertex3f (width/2, - 0.02, length/2+0.01);
                    glVertex3f (width/2 - 0.02, - 0.02, length/2+0.01);
                    glVertex3f (width/2 - 0.02, 0.0, length/2+0.01);
                    glVertex3f (width/2, 0.0, length/2+0.01);
                glEnd();

                glBegin(GL_QUADS);
                    glVertex3f (width/2, -height/2 + 0.02, length/2+0.01);
                    glVertex3f (width/2 - 0.02, -height/2 + 0.02, length/2+0.01);
                    glVertex3f (width/2 - 0.02, -height/2, length/2+0.01);
                    glVertex3f (width/2, -height/2, length/2+0.01);
                glEnd();

                glBegin(GL_QUADS);
                    glVertex3f (0.0, -height/2 + 0.02, length/2+0.01);
                    glVertex3f (0.02, -height/2 + 0.02, length/2+0.01);
                    glVertex3f (0.02, -height/2, length/2+0.01);
                    glVertex3f (0.0, -height/2, length/2+0.01);
                glEnd();

                glBegin(GL_QUADS);
                    glVertex3f (-width/2, -height/2 + 0.02, length/2);
                    glVertex3f (-width/2 + 0.02, -height/2 + 0.02, length/2);
                    glVertex3f (-width/2 + 0.02, -height/2, length/2);
                    glVertex3f (-width/2, -height/2, length/2);
                glEnd();

                glBegin(GL_QUADS);
                    glVertex3f (-width/2, 0.02, length/2);
                    glVertex3f (-width/2 + 0.02, 0.02, length/2);
                    glVertex3f (-width/2 + 0.02, 0.0, length/2);
                    glVertex3f (-width/2, 0.0, length/2);
                glEnd();

                glEnable(GL_BLEND);
            }
            if ( !( currentSide == SIDE_CONTROLPANEL &&
                    rotationState == ROTATION_STOPPED) ||
                    isMinimized3D)
            {
                glColor4f(0.5, 0.5, 0.5, 0.2);
                glBegin(GL_QUADS);
                    glVertex3f (width/2, height/2, length/2);
                    glVertex3f (-width/2, height/2, length/2);
                    glVertex3f (-width/2, height/2, -length/2);
                    glVertex3f (width/2, height/2, -length/2);
                glEnd();
                glColor4f(1.0, 0.0, 0.0, 1.0);
                glBegin(GL_LINE_LOOP);
                    glVertex3f (width/2, height/2, length/2);
                    glVertex3f (-width/2, height/2, length/2);
                    glVertex3f (-width/2, height/2, -length/2);
                    glVertex3f (width/2, height/2, -length/2);
                glEnd();

                glColor4f(0.5, 0.5, 0.5, 0.2);
                glBegin(GL_QUADS);
                    glVertex3f (width/2, -height/2, length/2);
                    glVertex3f (-width/2, -height/2, length/2);
                    glVertex3f (-width/2, -height/2, -length/2);
                    glVertex3f (width/2, -height/2, -length/2);
                glEnd();
                glColor4f(1.0, 0.0, 0.0, 1.0);
                glBegin(GL_LINE_LOOP);
                    glVertex3f (width/2, -height/2, length/2);
                    glVertex3f (-width/2, -height/2, length/2);
                    glVertex3f (-width/2, -height/2, -length/2);
                    glVertex3f (width/2, -height/2, -length/2);
                glEnd();

            }

            if ((rotationState == ROTATION_STOPPED && currentSide == SIDE_PATCH) ||
                rotationState != ROTATION_STOPPED || isMinimized3D)
            {
                glScalef(scaleX, scaleY, scaleZ);
                drawPatch(renderMode);
            }

        glPopMatrix();
    }
}

/**
 * Set position at normal view mode.
 */
void VimusGUICube::setPosNormal(float posX, float posY, float posZ)
{
    this->posXNormal = posX;
    this->posYNormal = posY;
    this->posZNormal = posZ;
}

/**
 * Set position at minimized view mode.
 */
void VimusGUICube::setPosMinimized(float posX, float posY, float posZ)
{
    this->posXMini = posX;
    this->posYMini = posY;
    this->posZMini = posZ;
}

/**
 * Adds Y axis rotation angle.
 */
void VimusGUICube::addAngleY(float angle)
{
    this->rotationAngleY += angle;
}

/**
 * Adds X axis rotation angle.
 */
void VimusGUICube::addAngleX(float angle)
{
    this->rotationAngleX += angle;
}

/**
 * Set rotation state.
 */
void VimusGUICube::setRotationState(int newState)
{
    if (newState == ROTATION_STOPPED)
    {
        rotationState = ROTATION_STOPPED;
        disablePatch3D();
    }
    else if (newState == ROTATION_FREE)
    {
        rotationState = ROTATION_FREE;
        enablePatch3D();
    }
    else if (newState == ROTATION_AUTO)
    {
        rotationState = ROTATION_AUTO;
        enablePatch3D();
    }
}

/**
 * Get rotation state.
 */
int VimusGUICube::getRotationState()
{
    return this->rotationState;
}

void VimusGUICube::rotateTo(int side)
{
    switch (side)
    {
    case SIDE_CONTROLPANEL:
        rotateTo(0.0, 0.0, 0.0);
        setCurrentSide(SIDE_CONTROLPANEL);
        break;
    case SIDE_INPUT:
        rotateTo(90.0, 0.0, 0.0);
        setCurrentSide(SIDE_INPUT);
        break;
    case SIDE_OUTPUT:
        rotateTo(-90.0, 0.0, 0.0);
        setCurrentSide(SIDE_OUTPUT);
        break;
    case SIDE_PATCH:
        rotateTo(0.0, 90.0, 0.0);
        setCurrentSide(SIDE_PATCH);
        break;
    }
}

void VimusGUICube::rotateTo(float angleXInc, float angleYInc, float angleZInc)
{
    rotAngleXInc = rotationAngleX - angleXInc;
    rotAngleXFactor = fmod((float) rotAngleXInc, (float) 360.0);
    if (rotAngleXFactor > 180)
        rotAngleXFactor = - (360 - rotAngleXFactor);
    else if (rotAngleXFactor < -180)
        rotAngleXFactor = 360 + rotAngleXFactor;
    rotAngleXFactor = rotAngleXFactor/rotNumFrames;

    rotAngleYInc = rotationAngleY - angleYInc;
    rotAngleYFactor = fmod((float) rotAngleYInc, (float) 360.0);
    if (rotAngleYFactor > 180)
        rotAngleYFactor = - (360 - rotAngleYFactor);
    else if (rotAngleYFactor < -180)
        rotAngleYFactor = 360 + rotAngleYFactor;
    rotAngleYFactor = rotAngleYFactor/rotNumFrames;

    rotAngleZInc = rotationAngleZ - angleZInc;
    rotAngleZFactor = fmod((float) rotAngleZInc, (float) 360.0);
    if (rotAngleZFactor > 180)
        rotAngleZFactor = - (360 - rotAngleZFactor);
    else if (rotAngleZFactor < -180)
        rotAngleZFactor = 360 + rotAngleZFactor;
    rotAngleZFactor = rotAngleZFactor/rotNumFrames;

    setRotationState(ROTATION_AUTO);
}

/**
 * Turn left.
 */
void VimusGUICube::turnLeft()
{
    if (rotationState == ROTATION_STOPPED)
    {
        switch (currentSide)
        {
        case SIDE_PATCH:
            rotateTo (SIDE_CONTROLPANEL);
            enablePatch3D();
            break;
        case SIDE_CONTROLPANEL:
            rotateTo (SIDE_PATCH);
            enablePatch3D();
            break;
        case SIDE_INPUT:
            rotateTo (SIDE_PATCH);
            enablePatch3D();
            break;
        case SIDE_OUTPUT:
            rotateTo (SIDE_PATCH);
            enablePatch3D();
            break;
        }
    }
}

/**
 * Turn right.
 */
void VimusGUICube::turnRight()
{
    if (rotationState == ROTATION_STOPPED)
    {
        switch (currentSide)
        {
        case SIDE_CONTROLPANEL:
            rotateTo (SIDE_PATCH);
            enablePatch3D();
            break;
        case SIDE_PATCH:
            rotateTo (SIDE_CONTROLPANEL);
            enablePatch3D();
            break;
        case SIDE_INPUT:
            rotateTo (SIDE_PATCH);
            enablePatch3D();
            break;
        case SIDE_OUTPUT:
            rotateTo (SIDE_PATCH);
            enablePatch3D();
            break;
        }
    }
}

/**
 * Turn up.
 */
void VimusGUICube::turnUp()
{
    if (rotationState == ROTATION_STOPPED)
    {
        switch (currentSide)
        {
        case SIDE_INPUT:
            rotateTo (SIDE_CONTROLPANEL);
            enablePatch3D();
            break;
        case SIDE_CONTROLPANEL:
            rotateTo (SIDE_OUTPUT);
            enablePatch3D();
            break;
        case SIDE_PATCH:
            rotateTo (SIDE_OUTPUT);
            enablePatch3D();
            break;
        }
    }
}

/**
 * Turn down.
 */
void VimusGUICube::turnDown()
{
    if (rotationState == ROTATION_STOPPED)
    {
        switch (currentSide)
        {
        case SIDE_OUTPUT:
            rotateTo (SIDE_CONTROLPANEL);
            enablePatch3D();
            break;
        case SIDE_CONTROLPANEL:
            rotateTo (SIDE_INPUT);
            enablePatch3D();
            break;
        case SIDE_PATCH:
            rotateTo (SIDE_INPUT);
            enablePatch3D();
            break;
        }
    }
}

/**
* Enable ROTATION_FREE rotation mode.
*/
void VimusGUICube::enableFreeRotation()
{
    if (rotationState == ROTATION_STOPPED)
        setRotationState(ROTATION_FREE);
}

/**
 * Disable ROTATION_FREE rotation mode.
 */
void VimusGUICube::disableFreeRotation()
{
    if (rotationState == ROTATION_FREE)
    {
        setRotationState(ROTATION_STOPPED);

        float rotAngleX = fmod((float) (rotationAngleX + rotAngleXInc), (float) 360.0);
        float rotAngleY = fmod((float) (rotationAngleY + rotAngleYInc), (float) 360.0);

        if ((rotAngleX < 45.0 && rotAngleX > -45.00) ||
            rotAngleX > 315.00 ||
            rotAngleX < -315.00)
        {
            if ((rotAngleY < 45.0 && rotAngleY > -45.00) ||
                rotAngleY > 315.00 ||
                rotAngleY < -315.00)
            {
                rotateTo(SIDE_CONTROLPANEL);
            }
            else
            {
                rotateTo(SIDE_PATCH);
            }
        }
        else if (  (rotAngleX < 135.0 && rotAngleX > 45.00)     ||
                   (rotAngleX < -225.0 && rotAngleX > -315.00)     )
        {
            rotateTo(SIDE_INPUT);
        }
        else if (  (rotAngleX < -45.0 && rotAngleX > -135.00) ||
                   (rotAngleX < 315.0 && rotAngleX > 225.00)       )
        {
            rotateTo(SIDE_OUTPUT);
        }
        else
        {
            rotateTo(SIDE_PATCH);
        }
    }
}

/**
 * Returns if it is in ROTATION_FREE rotation state.
 */
bool VimusGUICube::isFreeRotation()
{
    if (rotationState == ROTATION_FREE)
        return true;
    return false;
}

/**
 * Draw patch lines.
 */
void VimusGUICube::drawLines(int renderMode)
{
    if ((rotationState == ROTATION_STOPPED && currentSide == SIDE_PATCH)
            || rotationState != ROTATION_STOPPED)
    {
        int numCon = 0;
        float bypass = 0.0;
        for (int i=0; i<numObjects; i++)
        {
            if (renderMode == GL_SELECT)
                glPushName(i);
            bypass = -objects[i]->width;
            for (int j=0; j<objects[i]->numOutputs; j++)
            {
                if (renderMode == GL_SELECT)
                    glPushName(j);
                tempOutputPin = objects[i]->outputPins[j];
                numCon = tempOutputPin->numConnections;
                for (int k=0; k<numCon; k++)
                {
                    if (renderMode == GL_SELECT)
                        glPushName(k);

                    glLineWidth(tempOutputPin->type+1.0);

                    glColor4f(1.0, 0.0, 0.0, 1.0);
//                    if (tempOutputPin->isSelected && tempOutputPin->connections[k]->isSelected)
					if (tempOutputPin->selOutConnections[k])
                    {
                        glColor4f(0.3, 0.3, 1.0, 1.0);
                    }

                    glBegin(GL_LINES);
                        if (tempOutputPin->posY < tempOutputPin->connections[k]->posY)
                        {
                            glVertex3f (tempOutputPin->posX+0.025,
                                        tempOutputPin->posY+0.015,
                                        tempOutputPin->posZ);
                            glVertex3f (tempOutputPin->posX+0.025,
                                        tempOutputPin->posY+0.015,
                                        tempOutputPin->posZ+bypass);
                            glVertex3f (tempOutputPin->posX+0.025,
                                        tempOutputPin->posY+0.015,
                                        tempOutputPin->posZ+bypass);
                            glVertex3f (tempOutputPin->connections[k]->posX+0.025,
                                        tempOutputPin->connections[k]->posY+0.015,
                                        tempOutputPin->connections[k]->posZ+bypass);
                            glVertex3f (tempOutputPin->connections[k]->posX+0.025,
                                        tempOutputPin->connections[k]->posY+0.015,
                                        tempOutputPin->connections[k]->posZ+bypass);
                            glVertex3f (tempOutputPin->connections[k]->posX+0.025,
                                        tempOutputPin->connections[k]->posY+0.015,
                                        tempOutputPin->connections[k]->posZ);
                        }
                        else
                        {
                            glVertex3f (tempOutputPin->posX+0.025,
                                        tempOutputPin->posY+0.015,
                                        tempOutputPin->posZ);
                            glVertex3f (tempOutputPin->connections[k]->posX+0.025,
                                        tempOutputPin->connections[k]->posY+0.015,
                                        tempOutputPin->connections[k]->posZ);
                        }
                    glEnd();

                    if (renderMode == GL_SELECT)
                        glPopName();
                }
                if (renderMode == GL_SELECT)
                    glPopName();
            }
            if (renderMode == GL_SELECT)
                glPopName();
        }
        glLineWidth(1.0);
    }
}

/**
 * Draw VimusGUICube side.
 */
void VimusGUICube::drawSide(int renderMode, int side)
{
    if (side == SIDE_CONTROLPANEL)
    {
        drawControlPanel(   renderMode,
                            posXControlPanel + widthControlPanel/2,
                            posYControlPanel + heightControlPanel/2,
                            posZControlPanel,
                            widthControlPanel, heightControlPanel);
    }
}


/**
 * Draw control panel.
 */
void VimusGUICube::drawControlPanel(int renderMode, float x, float y, float z,
                                 float w, float h)
{

    glPushMatrix();
        glTranslatef(x, y, z);

        //draw controlpanel
        glColor4f(0.5, 0.5, 0.5, 0.4);
        glBegin(GL_QUADS);
            glVertex3f (-w/2, -h/2, 0.0);
            glVertex3f (w/2, -h/2, 0.0);
            glVertex3f (w/2, h/2, 0.0);
            glVertex3f (-w/2, h/2, 0.0);
        glEnd();
        glColor4f(1.0, 0.0, 0.0, 1.0);
        glBegin(GL_LINE_LOOP);
            glVertex3f (-w/2, -h/2, 0.0);
            glVertex3f (w/2, -h/2,  0.0);
            glVertex3f (w/2, h/2,  0.0);
            glVertex3f (-w/2, h/2,  0.0);
        glEnd();

        //glColor4f(1.0, 0.0, 0.0, 1.0);
        //renderStrokeFontString( -w/2, +h/2+0.03, 0.0, GLUT_STROKE_ROMAN, label);

        glScalef(scaleX, scaleY, scaleZ);

        for (int i=0; i<numObjects; i++)
        {
            if (objects[i]->isFixedOnParent)
            {
                if (currentSide != SIDE_CONTROLPANEL)
                {
                    objects[i]->drawSide(renderMode, SIDE_CONTROLPANEL);
                }
                else
                {
                    objects[i]->draw(renderMode);
                }
            }
        }

    glPopMatrix();
}

/**
 * Upate VimusGUICube rotation state.
 */
void VimusGUICube::updateRotationState()
{
    if (rotationState == ROTATION_AUTO)
    {
        if (rotCurrentFrame < rotNumFrames)
        {
            rotAngleXInc -= rotAngleXFactor;
            rotAngleYInc -= rotAngleYFactor;
            rotAngleZInc -= rotAngleZFactor;
            rotCurrentFrame++;
        }
        else
        {
            setRotationState(ROTATION_STOPPED);
            rotAngleXInc = 0.0;
            rotAngleYInc = 0.0;
            rotAngleZInc = 0.0;
            rotCurrentFrame = 0;
        }
    }
}

/**
 * Restore VimusGUICube.
 */
void VimusGUICube::restore()
{
    if (parentCube != NULL)
    {
        posX = posXNormal;
        posY = posYNormal;
        posZ = posZNormal;

        scaleX = widthNormal/(SCREEN_PLANE_WIDTH);
        scaleY = heightNormal/(SCREEN_PLANE_HEIGHT);
        scaleZ = lengthNormal/(SCREEN_PLANE_WIDTH);

        width = widthNormal;
        height = heightNormal;
        length = lengthNormal;

        viewMode = VIEW_NORMAL;
    }
}

/**
 * Restore VimusGUICube.
 */
void VimusGUICube::restoreToControlPanel()
{
    restore(    posXControlPanel, posYControlPanel, posZControlPanel,
                widthControlPanel, heightControlPanel, lengthControlPanel  );
}

/**
 * Restore VimusGUICube.
 */
void VimusGUICube::restoreToPatch()
{
    restore (posX, posY, posZ, width, height, length);
}

/**
 * Restore VimusGUICube.
 */
void VimusGUICube::restore(float x, float y, float z, float w, float h, float len)
{
    if (parentCube != NULL)
    {
        switch (currentSide)
        {
        case SIDE_PATCH:
            posX = x + len/2;
            posY = y + h/2;
            posZ = z - w/2;
            break;
        case SIDE_CONTROLPANEL:
            posX = x + w/2;
            posY = y + h/2;
            posZ = z - len/2;
            break;
        case SIDE_INPUT:
            posX = x + w/2;
            posY = y + len/2;
            posZ = z - h/2;
            break;
        case SIDE_OUTPUT:
            posX = x + w/2;
            posY = y + len/2;
            posZ = z - h/2;
            break;
        }

        scaleX = w/(SCREEN_PLANE_WIDTH);
        scaleY = h/(SCREEN_PLANE_HEIGHT);
        scaleZ = len/(SCREEN_PLANE_WIDTH);

        width = w;
        height = h;
        length = len;

        viewMode = VIEW_NORMAL;
    }
}


/**
 * Maximize VimusGUICube.
 */
void VimusGUICube::maximize()
{
    width = SCREEN_PLANE_WIDTH;
    height = SCREEN_PLANE_HEIGHT;
    length = width;

    posX = 0.0;
    posY = 0.0;
    posZ = 0.0;

    scaleX = 1.0;
    scaleY = 1.0;
    scaleZ = 1.0;

    viewMode = VIEW_MAXIMIZED;
}

/**
 * Minimize VimusGUICube.
 */
void VimusGUICube::minimize()
{
    if (parentCube != NULL)
    {
        posX = posXMini;
        posY = posYMini;
        posZ = posZMini;

        setSizeByLabel();

        if (((VimusGUICube *) parentCube)->currentSide == SIDE_PATCH)
            rotateTo(SIDE_PATCH);
        else if (((VimusGUICube *) parentCube)->currentSide == SIDE_CONTROLPANEL)
            rotateTo(SIDE_CONTROLPANEL);

        viewMode = VIEW_MINIMIZED;
    }
}

/**
 * Turn patch 3D view mode on.
 */
void VimusGUICube::enablePatch3D()
{
    for (int i=0; i<numObjects; i++)
    {
        objects[i]->enableMinimized3D();
        if (objects[i]->isCube)
            ((VimusGUICube *) objects[i])->enablePatch3D();
    }
}

/**
 * Turn patch 3D view mode off.
 */
void VimusGUICube::disablePatch3D()
{
    for (int i=0; i<numObjects; i++)
    {
        objects[i]->disableMinimized3D();
        if (objects[i]->isCube)
            ((VimusGUICube *) objects[i])->disablePatch3D();
    }
}

/**
 * Turn minimized3D on.
 */
void VimusGUICube::addObject(VimusGUIObject * object)
{
    if (numObjects < MAX_OBJ)
    {
        objects[numObjects] = object;
		objects[numObjects]->setParent(this, numObjects, this->currentSide);

		string input ("inlet");
		string output ("outlet");
		string lab (objects[numObjects]->label);
		if (!input.compare(lab))
		{
			this->addInputPin();
		}
		else if (!output.compare(lab))
		{
			this->addOutputPin();
		}
        numObjects++;
    }
}

/**
 * Update patch.
 */
void VimusGUICube::updatePatch()
{
    for (int i=0; i<numObjects; i++)
    {
        objects[i]->update();
    }
}

/**
 * Draw patch.
 */
void VimusGUICube::drawPatch(int renderMode)
{
    glPushMatrix();
        glLoadName(1);
        glRotatef(-90.0, 0.0, 1.0, 0.0);

        if (currentSide == SIDE_CONTROLPANEL)
        {
            for (int i=0; i<numObjects; i++)
            {
                if (!objects[i]->isFixedOnParent)
                    objects[i]->draw(renderMode);
            }
        }
        else
        {
            for (int i=0; i<numObjects; i++)
            {
                if (renderMode == GL_SELECT)
                    glPushName(i);

                objects[i]->draw(renderMode);

                if (renderMode == GL_SELECT)
                    glPopName();
            }

            glLoadName(2);
            drawLines(renderMode);
        }
    glPopMatrix();
}

/**
 * Set VimusGUICube current visible side.
 */
void VimusGUICube::setCurrentSide(int side)
{
    switch (side)
    {
    case SIDE_PATCH:
        currentSide = SIDE_PATCH;
        rotationAngleX = 0.0;
        rotationAngleY = 90.0;
        rotationAngleZ = 0.0;
        setFixedOnParentSide(SIDE_PATCH);
        break;
    case SIDE_CONTROLPANEL:
        currentSide = SIDE_CONTROLPANEL;
        rotationAngleX = 0.0;
        rotationAngleY = 0.0;
        rotationAngleZ = 0.0;
        setFixedOnParentSide(SIDE_CONTROLPANEL);
        break;
    case SIDE_INPUT:
        currentSide = SIDE_INPUT;
        rotationAngleX = 90.0;
        rotationAngleY = 0.0;
        rotationAngleZ = 0.0;
        setFixedOnParentSide(SIDE_PATCH);
        break;
    case SIDE_OUTPUT:
        currentSide = SIDE_OUTPUT;
        rotationAngleX = -90.0;
        rotationAngleY = 0.0;
        rotationAngleZ = 0.0;
        setFixedOnParentSide(SIDE_PATCH);
        break;
    }
}

void VimusGUICube::rotationReset()
{
    rotAngleXInc = 0.0;
    rotAngleYInc = 0.0;
    rotAngleZInc = 0.0;
    rotNumFrames = 20;
    rotCurrentFrame = 0;
    rotationState = ROTATION_STOPPED;
}

void VimusGUICube::setFixedOnParentSide(int side)
{
    for (int i=0; i<numObjects; i++)
    {
        objects[i]->parentCubeCurrentSide = side;
        if (objects[i]->isFixedOnParent)
        {
            if (objects[i]->isCube)
            {
                ((VimusGUICube *) objects[i])->setCurrentSide(side);
                if (side == SIDE_CONTROLPANEL)
                    ((VimusGUICube *) objects[i])->restoreToControlPanel();

                else if (side == SIDE_PATCH)
                    ((VimusGUICube *) objects[i])->minimize();
            }
        }
    }
}

void VimusGUICube::translate(float x, float y, float z)
{
    VimusGUINormalObject::translate(x, y, z);
    posXMini += x;
    posYMini += y;
    posZMini += z;
}

void VimusGUICube::removeObject(int i)
{
    if (objects[i] != NULL)
        delete objects[i];
    for (;i<numObjects;i++)
    {
        if (i < numObjects-1)
        {
            objects[i] = objects[i+1];
			objects[i]->indexOnParent=i;
			for (int j=0; j<objects[i]->numInputs; j++)
			{
				objects[i]->inputPins[j]->parentIndex=i;
			}
			for (int j=0; j<objects[i]->numOutputs; j++)
			{
				objects[i]->outputPins[j]->parentIndex=i;
			}
        }
        else
        {
            objects[i] = NULL;
            numObjects--;
        }
    }
}

void VimusGUICube::keyBoardFunc(unsigned char key, int x, int y)
{
    for (int i=0; i<numObjects; i++)
    {
        objects[i]->keyBoardFunc(key, x, y);
    }
}

void VimusGUICube::specialKeyBoardFunc(int key, int x, int y)
{
    for (int i=0; i<numObjects; i++)
    {
        objects[i]->specialKeyBoardFunc(key, x, y);
    }
}
