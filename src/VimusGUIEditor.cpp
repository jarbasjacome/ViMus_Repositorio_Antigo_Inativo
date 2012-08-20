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

#include "VimusGUIEditor.h"

using namespace std;

VimusGUIEditor::VimusGUIEditor()
{
    pickFlag = false;
    fpsFlag = false;

    this->pastTime = 0;
    boost::xtime_get(&(this->lastSysTime), boost::TIME_UTC);

    fpsAcum = 0;

    zoomState = ZOOM_STOPPED;
    resetZoom();

    isEditMode = true;
    editorState = EDITOR_READY;
    resetEditorState();
    connectingPin = NULL;

    mainCube = new VimusGUICube("ViMus", 0.0, 0.0, 0.0, true);
	currentCube = mainCube;
    mainCube->maximize();
	mainCube->setCurrentSide(VimusGUIObject::SIDE_CONTROLPANEL);

    if (DEBUG_MODE)
        cout << "\nVimusGUIEditor constructed.";

    this->viewPortMode = VIEWPORT_EDITOR;

    glEnable(GL_SCISSOR_TEST);//TODO: verify performance implications
}

VimusGUIEditor::~VimusGUIEditor()
{

}

void VimusGUIEditor::mouseFunc(int button, int state, int x, int y)
{
    if (state == GLUT_DOWN)
    {
        if (button == GLUT_RIGHT_BUTTON)
        {
            if (editorState == EDITOR_READY)
            {
                if (!currentCube->isFreeRotation() && currentCube->rotationState
                        == currentCube->ROTATION_STOPPED)
                {
                    currentCube->enableFreeRotation();
                    glutSetCursor(GLUT_CURSOR_CYCLE);
                }
            }
        }
        else if (button == GLUT_LEFT_BUTTON)
        {
            doPick(x, y);
        }
    }
    if (state == GLUT_UP)
    {
        if (button == GLUT_RIGHT_BUTTON)
        {
                if (currentCube->isFreeRotation())
                {
                    currentCube->disableFreeRotation();
                    glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
                }
        }
        else if (button == GLUT_LEFT_BUTTON)
        {
            if (editorState == EDITOR_MOVING_SELECTED)
                setEditorState(EDITOR_READY);
            else if (editorState == EDITOR_CONNECTING)
                doPick(x, y);
        }
    }
}

void VimusGUIEditor::motionFunc(int x, int y)
{
    if (currentCube->isFreeRotation()) {
        currentCube->addAngleX(y - lastMouseY);
        currentCube->addAngleY(x - lastMouseX);
    }

    if (editorState == EDITOR_MOVING_SELECTED)
    {
        for (int i=0; i<currentCube->numObjects; i++)
        {
            if (currentCube->objects[i]->isSelected)
            {
//                screenCoordToSceneCoord(x, y, &posCursorXTemp,
//                                          &posCursorYTemp, &posCursorZTemp);
//                screenCoordToSceneCoord(lastMouseX, lastMouseY, &posCursorX,
//                                          &posCursorY, &posCursorZ);
//                currentCube->objects[i]->translate(
//                    (posCursorXTemp - posCursorX)/2,
//                    (posCursorYTemp - posCursorY)/2,
//                    0.0);

                currentCube->objects[i]->translate(
                    ((float)(x - lastMouseX))/viewport[2] * 2.5,
                    -((float)(y - lastMouseY))/viewport[3] * 2.5,
                    0.0);

//                currentCube->objects[i]->translate(
//                    2*((float)(x - lastMouseX))/glutGet(GLUT_WINDOW_WIDTH),
//                    2*-((float)(y - lastMouseY))/glutGet(GLUT_WINDOW_HEIGHT),
//                    0.0);
            }
        }
    }

    lastMouseX = x;
    lastMouseY = y;
}

void VimusGUIEditor::passiveMotionFunc(int x, int y)
{
    lastMouseX = x;
    lastMouseY = y;
}

void VimusGUIEditor::keyBoardFunc(unsigned char key, int x, int y)
{
	switch (key)
	{
        case '1':
            zoomInTo(0);//currentCube->maximize();
            break;

        case '2':
            zoomInTo(1);//currentCube->restore();
            break;

        case '3':
            zoomInTo(2);//currentCube->minimize();
            break;

        case '4':
            zoomInTo(3);//currentCube->minimize();
            break;

        case '5':
            zoomInTo(4);//currentCube->minimize();
            break;

        case '6':
            zoomInTo(5);//currentCube->minimize();
            break;

        case '7':
            zoomInTo(6);//currentCube->minimize();
            break;

        case '8':
            zoomInTo(7);
            break;

        case '9':
            zoomInTo(8);
            break;

        case '0':
            zoomInTo(9);
            break;

        case '-':
            zoomOut();
            break;

        case 'f':
            if (!fpsFlag)
                showFps();
            else
                hideFps();
            break;
		case 'v':
			this->createObject("video", -this->currentCube->numObjects/10.0,
                                                                    -0.6, 0.0);
			break;
		case 'c':
			this->createObject("videocap", -0.5, -0.6, 0.0);
			break;
		case 'p':
			this->createObject("pixelAdd", 0.0, -0.6, 0.0);
			break;
		case 't':
			this->createObject("pixelCVThreshold", 0.0, -0.6, 0.0);
			break;
		case 'b':
			this->createObject("pixelCVBlob", 0.0, -0.6, 0.0);
			break;
		case 's':
			this->createObject("mangue", 0.0, -0.6, 0.0);
			break;
        case 'm':
            this->createObject("vitalino", 0.0, -0.6, 0.0);
            break;
        case 'l':
            this->createObject("lanterna", 0.2, -0.5, 0.0);
            break;
        case 'o':
            this->createObject("oriente", -0.2, -0.5, 0.0);
            break;
        case 'r':
            this->createObject("coracoes", 0.2, 0.5, 0.0);
            break;
        case 127:
		case 8:
            removeSelected();

	}
	this->mainCube->keyBoardFunc(key,x,y);
}

void VimusGUIEditor::specialKeyBoardFunc(int key, int x, int y)
{
	switch (key)
	{
		case GLUT_KEY_F1:
           	break;

		case GLUT_KEY_F2:
			break;

	    case GLUT_KEY_F5:
            this->viewPortMode++;
            if (this->viewPortMode == VIEWPORT_NUM_OF_MODES)
            {
                this->viewPortMode = 0;
            }
        break;

        case GLUT_KEY_UP:
            currentCube->turnDown();
			break;
        case GLUT_KEY_DOWN:
            currentCube->turnUp();
			break;
        case GLUT_KEY_LEFT:
            currentCube->turnRight();
			break;
        case GLUT_KEY_RIGHT:
            currentCube->turnLeft();
			break;
	}
	this->mainCube->specialKeyBoardFunc(key,x,y);
}

/**
 * This function is called every time OS asks window to be redrawed
 */
void VimusGUIEditor::displayFunc()
{
    // initialize viewing values
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();

    //"near" and "far" are distance values from camera origin point.
    gluPerspective(PERSP_FOV, PERSP_ASPECT, PERSP_NEAR, PERSP_FAR);
}

void VimusGUIEditor::updateFps()
{
//    currTime = clock();
//    timePast = (float) (currTime - lastTime)/ (float) CLOCKS_PER_SEC;
//    if ( timePast > 1.0f)
//    {
//        fps = fpsAcum + (timePast - 1.0f);
//        lastTime = clock();
//        fpsAcum = 0;
//    }
//    else
//    {
//        fpsAcum+= 1.0f;
//    }

    boost::xtime_get(&(this->currSysTime), boost::TIME_UTC);
    this->pastTime = this->currSysTime.nsec - this->lastSysTime.nsec +
                      this->currSysTime.sec*1000000000 - this->lastSysTime.sec*1000000000;
    if (this->pastTime > 1000000000)
    {
        fps = fpsAcum + (this->pastTime - 1000000000.0f)/1000000000.0f;
        boost::xtime_get(&(this->lastSysTime), boost::TIME_UTC);
        fpsAcum = 0;
    }
    else
    {
        fpsAcum+= 1.0f;
    }
}

void VimusGUIEditor::drawFps()
{
    ostringstream fpsStr;
    string fpsS;
    fpsStr << "FPS: " << fps;
    fpsS = fpsStr.str();
    glRasterPos3f(-1.0, -1.0, 0.0);
    for (int i=0; i<(int)fpsS.size(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, fpsS.at(i));
    }
}

void VimusGUIEditor::update()
{
    //TODO: check if it has machine
	this->machine->update();

    if (fpsFlag) updateFps();
    if (zoomState != ZOOM_STOPPED)
    {
        updateZoomState();
    }
    mainCube->update();
}

void VimusGUIEditor::drawEditor()
{
    //buffer must be cleaned before drawing and buffer swap.
    glClearColor (0.0, 0.0, 0.1, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    glPushMatrix();

        glMatrixMode (GL_MODELVIEW);
        glLoadIdentity ();

        if (zoomState != ZOOM_STOPPED)
        {
            gluLookAt ( camEyeX + camEyeXTrans,
                        camEyeY + camEyeYTrans,
                        camEyeZ + camEyeZTrans,
                        camTargetX + camTargetXTrans,
                        camTargetY + camTargetYTrans,
                        camTargetZ + camTargetZTrans,
                        0.0, 1.0, 0.0);
        }
        else
        {
            //(eyex, eyey, eyez) is camera origin point.
            //(centerx, centery, centerz) is camera target point.
            //(upx, upy, upz) is  camera up side direction.
            gluLookAt (0.0, 0.0, CAM_EYE_Z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
        }

        currentCube->draw(GL_RENDER);

        if (editorState == EDITOR_CONNECTING)
        {
            if (connectingPin != NULL)
            {
                screenCoordToSceneCoord(lastMouseX, lastMouseY, &posCursorX,
                                            &posCursorY, &posCursorZ);
                glColor4f(1.0, 0.0, 0.0, 1.0);
                glBegin(GL_LINES);
                    glVertex3f (connectingPin->posX+0.025,
                                connectingPin->posY+0.015,
                                0.0);
                    glVertex3f (posCursorX, posCursorY, posCursorZ);
                glEnd();
            }
        }

    glPopMatrix();

    glColor4f(1.0, 1.0, 1.0, 1.0);
    glDisable(GL_BLEND);

    if (pickFlag)
    {
        glSelectBuffer (BUFFER, selectBuf);
        (void) glRenderMode (GL_SELECT);

        glInitNames();
        glPushName(0);

        glMatrixMode (GL_PROJECTION);
        glPushMatrix ();
        glLoadIdentity ();
        //  create 5x5 pixel picking region near cursor location
        gluPickMatrix ( (GLdouble) pickPosX,
                        (GLdouble) (viewport[3] - pickPosY),
                        5.0, 5.0, viewport);
        gluPerspective(PERSP_FOV, PERSP_ASPECT, PERSP_NEAR, PERSP_FAR);

        glMatrixMode (GL_MODELVIEW);
        glLoadIdentity ();

        gluLookAt (0.0, 0.0, CAM_EYE_Z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

        currentCube->draw(GL_SELECT);

        glMatrixMode (GL_PROJECTION);
        glPopMatrix ();
//        glFlush ();

        hits = glRenderMode (GL_RENDER);
        processHits (hits, selectBuf);

        glutPostRedisplay();
        pickFlag = false;
    }

    isShiftKeyPressed = false;

    if (fpsFlag) drawFps();
}

void VimusGUIEditor::drawOutput()
{
    this->machine->drawOpenGLObjects();
}

void VimusGUIEditor::draw()
{
    this->windowWidth = glutGet(GLUT_WINDOW_WIDTH);
    this->windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
    this->screen0w = this->screen1w = glutGet(GLUT_WINDOW_WIDTH)/2;
    this->screen0h = this->screen1h = glutGet(GLUT_WINDOW_HEIGHT);

//*
//  TODO: Make possible to change this is real-time.
    this->screen0w = 1280;
    this->screen1w = 1024;
    this->screen0h = this->screen1h = 800;
//*/

    switch (this->viewPortMode)
    {
        case VIEWPORT_EDITOR:
            glutSetCursor(GLUT_CURSOR_LEFT_ARROW);

            glViewport(0, 0, windowWidth, windowHeight);
            glGetIntegerv( GL_VIEWPORT, viewport );
            glScissor(0, 0, windowWidth, windowHeight);
            this->drawEditor();
        break;
        case VIEWPORT_OUTPUT:
            glutSetCursor(GLUT_CURSOR_NONE);

            glViewport(0, 0, windowWidth, windowHeight);
            glGetIntegerv( GL_VIEWPORT, viewport );
            glScissor(0, 0, windowWidth, windowHeight);
            this->drawOutput();
        break;
        case VIEWPORT_EDITOR_OUTPUT:
            glutSetCursor(GLUT_CURSOR_LEFT_ARROW);

            glViewport(0, 0, screen0w, screen0h);
            glGetIntegerv( GL_VIEWPORT, viewport );
            glScissor(0, 0, screen0w, screen0h);
            this->drawEditor();

            glViewport(screen0w, 0, screen1w, screen1h);
            glGetIntegerv( GL_VIEWPORT, viewport );
            glScissor(screen0w, 0, screen1w, screen1h);
            this->drawOutput();
         break;
        case VIEWPORT_OUTPUT_EDITOR:
            glutSetCursor(GLUT_CURSOR_LEFT_ARROW);

            glViewport(0, 0, screen0w, screen0h);
            glGetIntegerv( GL_VIEWPORT, viewport );
            glScissor(0, 0, screen0w, screen0h);
            this->drawOutput();

            glViewport(screen0w, 0, screen1w, screen1h);
            glGetIntegerv( GL_VIEWPORT, viewport );
            glScissor(screen0w, 0, screen1w, screen1h);
            this->drawEditor();

         break;
    }


    glutSwapBuffers();
}

void VimusGUIEditor::screenCoordToSceneCoord(int screenX, int screenY, GLdouble *sceneX,
                                          GLdouble *sceneY, GLdouble *sceneZ)
{
	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
	glGetDoublev( GL_PROJECTION_MATRIX, projection );
//	glGetIntegerv( GL_VIEWPORT, viewport );

	winX = (float) screenX;
	winY = (float) viewport[3] - (float) screenY;
	glReadPixels(  screenX, int(winY), 1, 1,
                    GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );
	gluUnProject( winX, winY, winZ, modelview, projection, viewport, sceneX, sceneY, sceneZ);
}

/**
 * processHits prints out the contents of the
 * selection array.
 */
void VimusGUIEditor::processHits (GLint hits, GLuint buffer[])
{
    int i, j;
    GLuint numNames, *ptr;
    numNames = 0;

    int pinBufIndex = -1;
    int objectBufIndex = -1;
    int lineBufIndex = -1;
    int indexCount = 0;

    if (DEBUG_MODE)
        cout << "\nhits = " << hits;

    ptr = (GLuint *) buffer;

    for (i = 0; i < hits; i++) /*  for each hit  */
    {
        numNames = *ptr;

        if (DEBUG_MODE)
            cout << "\n number of names for this hit = " << numNames;

        ptr++; indexCount++;

        if (DEBUG_MODE)
            cout << "\n  z1 is " << (float) *ptr/0x7fffffff;

        ptr++;
        indexCount++;

        if (DEBUG_MODE)
            cout << "\n z2 is " << (float) *ptr/0x7fffffff;

        ptr++;
        indexCount++;

        if (DEBUG_MODE)
        {
            cout << "   names are ";
            cout << *ptr << " ";
        }

        if (*ptr == 1)
        {
            if (numNames == 2)
                objectBufIndex = indexCount-3;
            else if (numNames == 4)
                pinBufIndex = indexCount-3;
        } else if (*ptr == 2)
        {
            if (numNames == 4)
                lineBufIndex = indexCount-3;
        }

        ptr++; indexCount++;

        for (j = 1; j < (int) numNames; j++) /*  for each name */
        {
            if (DEBUG_MODE)
                cout << *ptr << " ";

            ptr++; indexCount++;
        }
        if (DEBUG_MODE)
            cout << "\n";
    }

    if (isEditMode)
    {
        switch (editorState)
        {
        case EDITOR_READY:
            if (hits == 0)
            {
                unselectAll();
                return;
            }
            if (pinBufIndex >= 0)
            {
                numNames = buffer[pinBufIndex];
                if (DEBUG_MODE)
                    cout << "\npino! numNames: " << numNames << " \n";

                if (buffer[pinBufIndex+5] == 0) // clicked on input pin
                    selectObject(buffer[objectBufIndex+4]);
                else if (buffer[pinBufIndex+5] == 1) // clicked on output pin
                    pickOutputConnectPin(buffer[pinBufIndex+4],buffer[pinBufIndex+6]);
            }
            else if (objectBufIndex >=0)
            {
                numNames = buffer[objectBufIndex];

                if (DEBUG_MODE)
                    cout << "objeto! numNames: " << numNames << " \n";

                selectObject(buffer[objectBufIndex+4]);
            }
            else if (lineBufIndex >=0)
            {
                numNames = buffer[lineBufIndex];
                unselectAll();
/*
				currentCube->objects[buffer[lineBufIndex+4]]->
                    getOutputByIndex(buffer[lineBufIndex+5])->isSelected = true;
                currentCube->objects[buffer[lineBufIndex+4]]->
                    getOutputByIndex(buffer[lineBufIndex+5])->
                    connections[buffer[lineBufIndex+6]]->isSelected = true;
*/
				if (!currentCube->objects[buffer[lineBufIndex+4]]->
					getOutputByIndex(buffer[lineBufIndex+5])->
					selOutConnections[buffer[lineBufIndex+6]])
				{
					currentCube->objects[buffer[lineBufIndex+4]]->
						getOutputByIndex(buffer[lineBufIndex+5])->
						selOutConnections[buffer[lineBufIndex+6]] = true;
				} else
				{
					currentCube->objects[buffer[lineBufIndex+4]]->
						getOutputByIndex(buffer[lineBufIndex+5])->
						selOutConnections[buffer[lineBufIndex+6]] = false;
				}
				if (DEBUG_MODE)
                    cout << "linha! numNames: " << numNames << " \n";
            }
            break;

        case EDITOR_CONNECTING:
            if (hits == 0)
            {
                setEditorState(EDITOR_READY);
                return;
            }
            if (pinBufIndex >= 0)
            {
                numNames = buffer[pinBufIndex];
                if (DEBUG_MODE)
                    cout << "pino! numNames: " << numNames << " \n";

                if (buffer[pinBufIndex+5] == 0) // clicked on input pin
                    pickInputConnectPin(buffer[pinBufIndex+4],buffer[pinBufIndex+6]);
                else if (buffer[pinBufIndex+5] == 1) // clicked on input pin
                    setEditorState(EDITOR_READY);
            }
            else if (objectBufIndex >=0)
            {
                numNames = buffer[objectBufIndex];
                if (DEBUG_MODE)
                    cout << "objeto! numNames: " << numNames << " \n";

                setEditorState(EDITOR_READY);
            }
            else if (lineBufIndex >=0)
            {
                numNames = buffer[lineBufIndex];
                if (DEBUG_MODE)
                    cout << "linha! numNames: " << numNames << " \n";

                setEditorState(EDITOR_READY);
            }
            break;

        }
    }
}

/**
 * Upate VimusGUIEditor zoom state.
 */
void VimusGUIEditor::updateZoomState()
{
    switch (zoomState)
    {
    case ZOOM_IN:
        if ( zoomCurrentFrame < zoomNumFrames)
        {
            camEyeXTrans += camEyeXFactor;
            camEyeYTrans += camEyeYFactor;
            camEyeZTrans += camEyeZFactor;
            camTargetXTrans += camTargetXFactor;
            camTargetYTrans += camTargetYFactor;
            camTargetZTrans += camTargetZFactor;
            zoomCurrentFrame++;
        }
        else {
            setZoomState(ZOOM_STOPPED);
            currentCube = (VimusGUICube *) currentCube->objects[zoomObjRef];
            currentCube->maximize();
            resetZoom();
        }
        break;
    case ZOOM_OUT:
        if (zoomCurrentFrame < zoomNumFrames)
        {
            camEyeXTrans -= camEyeXFactor;
            camEyeYTrans -= camEyeYFactor;
            camEyeZTrans -= camEyeZFactor;
            camTargetXTrans -= camTargetXFactor;
            camTargetYTrans -= camTargetYFactor;
            camTargetZTrans -= camTargetZFactor;
            zoomCurrentFrame++;
        }
        else {
            setZoomState(ZOOM_STOPPED);
            if (currentCube->currentSide == VimusGUIObject::SIDE_PATCH)
                ((VimusGUICube *) currentCube->objects[zoomObjRef])->minimize();
            else if (currentCube->currentSide == VimusGUIObject::SIDE_CONTROLPANEL)
                ((VimusGUICube *) currentCube->objects[zoomObjRef])->restoreToControlPanel();
            resetZoom();
        }
        break;
    };
}

/**
 * Set zoom state.
 */
void VimusGUIEditor::setZoomState(int newState)
{
    switch (newState)
    {
    case ZOOM_STOPPED:
        zoomState = ZOOM_STOPPED;
        currentCube->disablePatch3D();
        break;
    case ZOOM_IN:
        zoomState = ZOOM_IN;
        computeZoomValues(zoomObjRef);
        camEyeXTrans = 0.0;
        camEyeYTrans = 0.0;
        camEyeZTrans = 0.0;
        camTargetXTrans = 0.0;
        camTargetYTrans = 0.0;
        camTargetZTrans = 0.0;
        currentCube->enablePatch3D();
        if (currentCube->currentSide == VimusGUIObject::SIDE_PATCH)
        {
            ((VimusGUICube *) currentCube->objects[zoomObjRef])->restoreToPatch();
        }
        else if (currentCube->currentSide == VimusGUIObject::SIDE_CONTROLPANEL)
        {
            ((VimusGUICube *) currentCube->objects[zoomObjRef])->restoreToControlPanel();
        }
        break;
    case ZOOM_OUT:
        zoomState = ZOOM_OUT;
        computeZoomValues(zoomObjRef);

        if (currentCube->currentSide == VimusGUIObject::SIDE_PATCH)
        {
            camEyeXTrans = currentCube->objects[zoomObjRef]->posX +
                            currentCube->objects[zoomObjRef]->length/2;
            camEyeYTrans = currentCube->objects[zoomObjRef]->posY +
                            currentCube->objects[zoomObjRef]->height/2;
            camEyeZTrans = currentCube->objects[zoomObjRef]->posZ +
                            currentCube->objects[zoomObjRef]->width/2 +
    camEyeZ*currentCube->objects[zoomObjRef]->length/currentCube->length - camEyeZ;
            camTargetXTrans = camEyeXTrans;
            camTargetYTrans = camEyeYTrans;
            camTargetZTrans = camEyeZTrans;
            currentCube->enablePatch3D();
            ((VimusGUICube *) currentCube->objects[zoomObjRef])->restoreToPatch();
        }
        else if (currentCube->currentSide == VimusGUIObject::SIDE_CONTROLPANEL)
        {
            camEyeXTrans = currentCube->objects[zoomObjRef]->posXControlPanel +
                            currentCube->objects[zoomObjRef]->width/2;
            camEyeYTrans = currentCube->objects[zoomObjRef]->posYControlPanel +
                            currentCube->objects[zoomObjRef]->height/2;
            camEyeZTrans = currentCube->objects[zoomObjRef]->posZControlPanel +
                            currentCube->objects[zoomObjRef]->length/2 +
    camEyeZ*currentCube->objects[zoomObjRef]->width/currentCube->width - camEyeZ;
            camTargetXTrans = camEyeXTrans;
            camTargetYTrans = camEyeYTrans;
            camTargetZTrans = camEyeZTrans;
            currentCube->enablePatch3D();
            ((VimusGUICube *) currentCube->objects[zoomObjRef])->restoreToControlPanel();
        }
        break;
    }
}

/**
 * Zoom to object.
 */
void VimusGUIEditor::zoomInTo (int cubeIndex)
{
    if (cubeIndex >= currentCube->numObjects || cubeIndex < 0)
        return;
    if ((currentCube->currentSide == VimusGUIObject::SIDE_PATCH ||
        currentCube->currentSide == VimusGUIObject::SIDE_CONTROLPANEL) &&
        zoomState == ZOOM_STOPPED &&
        currentCube->rotationState == VimusGUICube::ROTATION_STOPPED)
    {
        resetZoom();
        if (currentCube->objects[cubeIndex])
        {
            if (currentCube->objects[cubeIndex]->isCube)
            {
                if (!((currentCube->currentSide == VimusGUIObject::SIDE_CONTROLPANEL) &&
                    !currentCube->objects[cubeIndex]->isFixedOnParent))
                {
                    zoomObjRef = cubeIndex;
                    setZoomState(ZOOM_IN);
                }
            }
        }
    }
}

/**
 * Zoom out from current cube.
 */
void VimusGUIEditor::zoomOut()
{
    if (currentCube->parentCube != NULL)
    {
        if (zoomState == ZOOM_STOPPED &&
            currentCube->rotationState == VimusGUICube::ROTATION_STOPPED)
        {
            int tempIndex = currentCube->indexOnParent;
            currentCube = (VimusGUICube *) currentCube->parentCube;

            if (currentCube->currentSide == VimusGUIObject::SIDE_PATCH)
                ((VimusGUICube *) currentCube->objects[tempIndex])->minimize();
            else if (currentCube->currentSide == VimusGUIObject::SIDE_CONTROLPANEL)
                ((VimusGUICube *) currentCube->objects[tempIndex])->restoreToControlPanel();

            zoomOutFrom(tempIndex);
        }
    }
}

/**
 * Zoom out from object.
 */
void VimusGUIEditor::zoomOutFrom (int cubeIndex)
{
    resetZoom();
    zoomObjRef = cubeIndex;
    setZoomState(ZOOM_OUT);
}

/**
 * Compute zoom values.
 */
void VimusGUIEditor::computeZoomValues (int cubeIndex)
{
    if (currentCube->objects[cubeIndex])
    {
        float cubeL, cubeH, cubeW, cubeX, cubeY, cubeZ;

        if (currentCube->currentSide == VimusGUIObject::SIDE_PATCH)
        {
            cubeW = currentCube->objects[cubeIndex]->width;
            cubeH = currentCube->objects[cubeIndex]->height;
            cubeL = currentCube->objects[cubeIndex]->length;

            cubeX = currentCube->objects[cubeIndex]->posX + cubeL/2;
            cubeY = currentCube->objects[cubeIndex]->posY + cubeH/2;
            cubeZ = currentCube->objects[cubeIndex]->posZ + cubeW/2;

            camEyeXFactor = (cubeX - currentCube->posX)/zoomNumFrames;
            camEyeYFactor = (cubeY - currentCube->posY)/zoomNumFrames;
            camEyeZFactor = (camEyeZ*cubeL/currentCube->length - camEyeZ)/zoomNumFrames
                            + (cubeZ - currentCube->posZ)/zoomNumFrames;
            camTargetXFactor = (cubeX - currentCube->posX)/zoomNumFrames;
            camTargetYFactor = (cubeY - currentCube->posY)/zoomNumFrames;
            camTargetZFactor = (cubeZ - currentCube->posZ)/zoomNumFrames;
        }
        else if (currentCube->currentSide == VimusGUIObject::SIDE_CONTROLPANEL)
        {
            cubeW = currentCube->objects[cubeIndex]->widthControlPanel;
            cubeH = currentCube->objects[cubeIndex]->heightControlPanel;
            cubeL = currentCube->objects[cubeIndex]->lengthControlPanel;

            cubeX = currentCube->objects[cubeIndex]->posXControlPanel + cubeW/2;
            cubeY = currentCube->objects[cubeIndex]->posYControlPanel + cubeH/2;
            cubeZ = currentCube->objects[cubeIndex]->posZControlPanel + cubeL/2;

            camEyeXFactor = (cubeX - currentCube->posX)/zoomNumFrames;
            camEyeYFactor = (cubeY - currentCube->posY)/zoomNumFrames;
            camEyeZFactor = (camEyeZ*cubeW/currentCube->width - camEyeZ)/zoomNumFrames
                            + (cubeZ - currentCube->posZ)/zoomNumFrames;

            camTargetXFactor = (cubeX - currentCube->posX)/zoomNumFrames;
            camTargetYFactor = (cubeY - currentCube->posY)/zoomNumFrames;
            camTargetZFactor = (cubeZ - currentCube->posZ)/zoomNumFrames;
        }

    }
}

/**
 * Reset zoom state values.
 */
void VimusGUIEditor::resetZoom()
{
    camEyeX = 0.0;
    camEyeY = 0.0;
    camEyeZ = CAM_EYE_Z;
    camTargetX = 0.0;
    camTargetY = 0.0;
    camTargetZ = 0.0;

    camEyeXTrans = 0.0;
    camEyeYTrans = 0.0;
    camEyeZTrans = 0.0;
    camTargetXTrans = 0.0;
    camTargetYTrans = 0.0;
    camTargetZTrans = 0.0;

    camEyeXFactor = 0.0;
    camEyeYFactor = 0.0;
    camEyeZFactor = 0.0;
    camTargetXFactor = 0.0;
    camTargetYFactor = 0.0;
    camTargetZFactor = 0.0;

    zoomCurrentFrame = 0;
    zoomNumFrames = 10;

    scaleX = 1.0;
    scaleY = 1.0;
    scaleZ = 1.0;

    zoomObjRef = 0;
}

void VimusGUIEditor::showFps()
{
    fpsFlag = true;
}

void VimusGUIEditor::hideFps()
{
    fpsFlag = false;
}

void VimusGUIEditor::resetEditorState()
{
    isCtrlKeyPressed = false;
    isShiftKeyPressed = false;
    isMouseRButtonPressed = false;
    isMouseLButtonPressed = false;
}

void VimusGUIEditor::setEditorState(int newState)
{
    switch (newState)
    {
    case EDITOR_READY:
        editorState = EDITOR_READY;
        connectingPin = NULL;
        glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
        break;
    case EDITOR_MOVING_SELECTED:
        editorState = EDITOR_MOVING_SELECTED;
        glutSetCursor(GLUT_CURSOR_INFO);
        break;
    case EDITOR_CONNECTING:
        editorState = EDITOR_CONNECTING;
        break;
    case EDITOR_COPYING_SELECTED:
        editorState = EDITOR_COPYING_SELECTED;
        break;
    case EDITOR_PUTTING_NEW_OBJECCT:
        editorState = EDITOR_PUTTING_NEW_OBJECCT;
        break;
    case EDITOR_SETTING_SELECTION_RECT:
        editorState = EDITOR_SETTING_SELECTION_RECT;
        break;
    case EDITOR_WAITING_SECOND_CLICK:
        editorState = EDITOR_WAITING_SECOND_CLICK;
        break;
    case EDITOR_RESIZING_OBJECT:
        editorState = EDITOR_RESIZING_OBJECT;
        break;
    }
}

void VimusGUIEditor::updateEditorState()
{
}

void VimusGUIEditor::unselectAll()
{
    if (!isShiftKeyPressed)
    {
        for (int i=0; i<currentCube->numObjects; i++)
        {
            currentCube->objects[i]->isSelected = false;
            for (int j=0; j<currentCube->objects[i]->numOutputs; j++)
            {
                for (int k=0; k<currentCube->objects[i]->outputPins[j]->numConnections; k++)
                {
					currentCube->objects[i]->outputPins[j]->selOutConnections[k] = false;
                }
            }
        }
    }
}

void VimusGUIEditor::selectObject(int index)
{
    unselectAll();
	if (!currentCube->objects[index]->isSelected)
		currentCube->objects[index]->isSelected = true;
	else
		currentCube->objects[index]->isSelected = false;
    setEditorState(EDITOR_MOVING_SELECTED);
}

void VimusGUIEditor::pickOutputConnectPin(int objIndex, int pinIndex)
{
    connectingPin = currentCube->objects[objIndex]->
        getOutputByIndex(pinIndex);
    setEditorState(EDITOR_CONNECTING);
}

void VimusGUIEditor::pickInputConnectPin(int objIndex, int pinIndex)
{
	this->connectPins (	this->connectingPin->parentIndex,
						this->connectingPin->indexOnParent,
						objIndex,
						pinIndex);

	setEditorState(EDITOR_READY);
}

/**
 * Connect objects pins.
 */
bool VimusGUIEditor::connectPins(int srcObj, int srcPin, int dstObj, int dstPin)
{
	bool ret = false;
	if (this->machine->connectPins (srcObj, srcPin, dstObj, dstPin))
	{
		this->currentCube->objects[srcObj]->outputPins[srcPin]->
			connect(this->currentCube->objects[dstObj]->inputPins[dstPin]);
		ret = true;
	}
	return ret;

}


void VimusGUIEditor::doPick (int x, int y)
{
    pickFlag = true;
    pickPosX = x;
    pickPosY = y;
    if (GLUT_ACTIVE_SHIFT == glutGetModifiers())
        isShiftKeyPressed = true;
}

void VimusGUIEditor::removeSelectedConnections()
{
    for (int i=0; i<currentCube->numObjects; i++)
    {
        for (int j=0; j<currentCube->objects[i]->numOutputs; j++)
        {
			for (int k=currentCube->objects[i]->outputPins[j]->numConnections-1; k>=0; k--)
			{
				if (currentCube->objects[i]->outputPins[j]->selOutConnections[k])
				{
					disconnectPins(	i,
									j,
									//TODO: can't keep parent index.
									currentCube->objects[i]->
										outputPins[j]->connections[k]->parentIndex,
									currentCube->objects[i]->
										outputPins[j]->connections[k]->indexOnParent);
				}
			}
        }
    }
}

/**
 * Disconnect objects pins.
 */
void VimusGUIEditor::disconnectPins(int srcObj, int outPin, int dstObj, int inPin)
{
	this->machine->disconnectPins(srcObj, outPin, dstObj, inPin);

	this->disconnectGUIPins (srcObj, outPin, dstObj, inPin);
}

/**
 * Disconnect only GUI objects pins.
 */
void VimusGUIEditor::disconnectGUIPins(int srcObj, int outPin, int dstObj, int inPin)
{
    currentCube->objects[srcObj]->outputPins[outPin]->
        disconnect(currentCube->objects[dstObj]->inputPins[inPin]);
}

void VimusGUIEditor::removeSelectedObjects()
{
    int i=0;
    while ((currentCube->numObjects > 0) && (i < currentCube->numObjects))
    {
        if (currentCube->objects[i]->isSelected)
        {
			this->removeObject(i);
        }
        else
        {
            i++;
        }
    }
}

void VimusGUIEditor::removeSelected()
{
    removeSelectedConnections();
    removeSelectedObjects();
}

/**
 * Creates a new object.
 */
VimusGUIObject * VimusGUIEditor::createObject(const string& label, float posX, float posY, float posZ)
{
    //TODO: check if it has a machine.

    VimusGUIObject * obj = this->machine->createObject(label, posX, posY, posZ);
    if (obj)
    {
        this->currentCube->addObject(obj);
    }
    return obj;
}


/**
 * Removes an object by index.
 */
void VimusGUIEditor::removeObject(int i)
{
	if (i < this->currentCube->numObjects)
	{
		this->machine->removeObject(i);
		this->removeGUIObject(i);
	}
}

void VimusGUIEditor::removeGUIObject(int i)
{
	int k;
	for (int j=0; j<currentCube->objects[i]->numOutputs; j++)
	{
		k = currentCube->objects[i]->outputPins[j]->numConnections - 1;
		while (currentCube->objects[i]->outputPins[j]->numConnections>0)
		{
			disconnectGUIPins(	i,
								j,
								currentCube->objects[i]->
									outputPins[j]->connections[k]->
										parentIndex,
								currentCube->objects[i]->
									outputPins[j]->connections[k]->
										indexOnParent);
			//TODO: catch exception.
			k--;
		}
	}
	for (int j=0; j<currentCube->objects[i]->numInputs; j++)
	{
		k = currentCube->objects[i]->inputPins[j]->numConnections - 1;
		while (currentCube->objects[i]->inputPins[j]->numConnections>0)
		{
			disconnectGUIPins(	currentCube->objects[i]->
									inputPins[j]->connections[k]->
										parentIndex,
								currentCube->objects[i]->
									inputPins[j]->connections[k]->
										indexOnParent,
								i,
								j);
			//TODO: catch exception.
			k--;
		}
	}
	currentCube->removeObject(i);
}

/**
 * Add a machine reference to VimusUserInterface
 */
void VimusGUIEditor :: addMachine (VimusMachine * machine)
{
	if (machine)
	{
		this->machine = machine;
	}
}

/**
 * Set currentCube by index.
 */
void VimusGUIEditor :: setCurrentCube (int objIndex)
{
	if (objIndex >= 0 && objIndex < this->currentCube->numObjects)
	{
		if (currentCube->objects[objIndex]->isCube)
		{
			this->machine->setCurrentAbsObj(objIndex);
			currentCube = (VimusGUICube *) currentCube->objects[objIndex];
		}
	}
}

/**
 * Set currentCube by son's name.
 */
/*
void VimusGUIEditor :: setCurrentCube (const string& objName)
{
	bool found = false;
	int c = 0;
	while (found == false && c < this->currentCube->numObjects)
	{
		if (!objName.compare(this->currentCube->objects[c]->label))
		{
			setCurrentCube(c);
			found = true;
		}
		c++;
	}
}
*/

/**
 * Set currentCube to be the current cube's parent cube.
 */
void VimusGUIEditor :: setCurrentCubeToParent ()
{
	if (this->currentCube->parentCube)
	{
		this->machine->setCurrentAbsObjToParent();
		this->currentCube = (VimusGUICube *) this->currentCube->parentCube;
	}
}


/**
 * Creates a patch for test.
 */
void VimusGUIEditor :: createTestPatch()
{
/* VJ App -> Resolume

    this->createObject("abs videos", -0.5, 0.3, 0.0);
    this->currentCube->objects[0]->isFixedOnParent = true;
    this->currentCube->objects[0]->setPosControlPanel(-0.9, -0.9, 0.0);
    this->currentCube->objects[0]->setSizeControlPanel(1.4, 0.7);

    this->setCurrentCube(0);

    this->createObject("inlet", -0.5, 0.7, 0.0);
    this->createObject("outlet", -0.5, -0.8, 0.0);
    this->createObject("outlet",  0.0, -0.8, 0.0);
    this->createObject("outlet",  0.5, -0.8, 0.0);

    this->setCurrentCubeToParent();

    VimusGUIObject * objArrayVJapp[VimusGUICube::MAX_OBJ];

    VimusGUIObject * objArrayVideos[VimusGUICube::MAX_OBJ];
    objArrayVideos[0] = new VimusGUINormalObject("inlet", -0.5, 0.7, 0.0, 0, 1);
    objArrayVideos[1] = new VimusGUINormalObject("outlet", -0.5, -0.8, 0.0, 1, 0);
    objArrayVideos[2] = new VimusGUINormalObject("outlet",  0.0, -0.8, 0.0, 1, 0);
    objArrayVideos[3] = new VimusGUINormalObject("outlet",  0.5, -0.8, 0.0, 1, 0);

    VimusGUIObject * objQArray[VimusGUICube::MAX_OBJ];
    objQArray[0] = new VimusGUINormalObject("inlet", -0.5, 0.7, 0.0, 0, 1);
    objArrayVideos[4] = new VimusGUICube("Q ", -0.5, 0.3, 0.0, objQArray, 0, true);
    objArrayVideos[4]->setPosControlPanel(-0.9, 0.0, 0.0);
    objArrayVideos[4]->setSizeControlPanel(0.3, 0.7);

    VimusGUIObject * objWArray[VimusGUICube::MAX_OBJ];
    objWArray[0] = new VimusGUINormalObject("inlet", -0.5, 0.7, 0.0, 0, 1);
    objArrayVideos[5] = new VimusGUICube("W ", -0.2, 0.3, 0.0, objWArray, 0, true);
    objArrayVideos[5]->setPosControlPanel(-0.5, 0.0, 0.0);
    objArrayVideos[5]->setSizeControlPanel(0.3, 0.7);

    VimusGUIObject * objEArray[VimusGUICube::MAX_OBJ];
    objEArray[0] = new VimusGUINormalObject("inlet", -0.5, 0.7, 0.0, 0, 1);
    objArrayVideos[6] = new VimusGUICube("E ", 0.1, 0.3, 0.0, objEArray, 0, true);
    objArrayVideos[6]->setPosControlPanel(-0.1, 0.0, 0.0);
    objArrayVideos[6]->setSizeControlPanel(0.3, 0.7);

    VimusGUIObject * objRArray[VimusGUICube::MAX_OBJ];
    objRArray[0] = new VimusGUINormalObject("inlet", -0.5, 0.7, 0.0, 0, 1);
    objArrayVideos[7] = new VimusGUICube("R ", 0.4, 0.3, 0.0, objRArray, 0, true);
    objArrayVideos[7]->setPosControlPanel(0.4, 0.0, 0.0);
    objArrayVideos[7]->setSizeControlPanel(0.3, 0.7);

    VimusGUIObject * objArrayLayers[VimusGUICube::MAX_OBJ];
    objArrayLayers[0] = new VimusGUINormalObject("inlet", -0.5, 0.7, 0.0, 0, 1);
    objArrayLayers[1] = new VimusGUINormalObject("inlet",  0.0, 0.7, 0.0, 0, 1);
    objArrayLayers[2] = new VimusGUINormalObject("inlet",  0.5,  0.7, 0.0, 0, 1);
    objArrayLayers[3] = new VimusGUINormalObject("outlet", -0.5, -0.8, 0.0, 1, 0);
    objArrayLayers[4] = new VimusGUINormalObject("outlet",  0.0, -0.8, 0.0, 1, 0);
    objArrayLayers[5] = new VimusGUINormalObject("outlet",  0.5, -0.8, 0.0, 1, 0);
    objArrayVJapp[1] = new VimusGUICube("layers", -0.3, -0.15, 0.0, objArrayLayers, 6, true);
    objArrayVJapp[1]->setPosControlPanel(-0.9, 0.0, 0.0);
    objArrayVJapp[1]->setSizeControlPanel(0.8, 0.7);

    VimusGUIObject * objArrayPreview[VimusGUICube::MAX_OBJ];
    objArrayPreview[0] = new VimusGUINormalObject("inlet", -0.5, 0.7, 0.0, 0, 1);
    objArrayVJapp[2] = new VimusGUICube("preview", -0.1, -0.6, 0.0, objArrayPreview, 1, true);
    objArrayVJapp[2]->setPosControlPanel(0.0, 0.0, 0.0);
    objArrayVJapp[2]->setSizeControlPanel(0.7, 0.7);

    mainCube = new VimusGUICube("VJ app", 0.0, 0.0, 0.0, objArrayVJapp, 3, true);

	currentCube = mainCube;
    mainCube->maximize();
	mainCube->setCurrentSide(VimusGUIObject::SIDE_CONTROLPANEL);

//    VimusGUINormalObject * vazio = new VimusGUINormalObject(0.2, -1.0, 0.0);
//    mainCube->addObject(vazio);

/*
    this->connectPins(  0, objArrayVJapp[0]->getOutputByIndex(0)->indexOnParent,
                        1, objArrayVJapp[1]->getInputByIndex(0)->indexOnParent);

/*
    this->connectPins(  0, objArrayVJapp[0]->getOutputByIndex(1)->indexOnParent,
                        1, objArrayVJapp[1]->getInputByIndex(1)->indexOnParent);

    this->connectPins(  0, objArrayVJapp[0]->getOutputByIndex(2)->indexOnParent,
                        1, objArrayVJapp[1]->getInputByIndex(2)->indexOnParent);

    this->connectPins(  1, objArrayVJapp[1]->getOutputByIndex(0)->indexOnParent,
                        2, objArrayVJapp[2]->getInputByIndex(0)->indexOnParent);

    this->connectPins(  1, objArrayVJapp[1]->getOutputByIndex(1)->indexOnParent,
                        0, objArrayVJapp[0]->getInputByIndex(0)->indexOnParent);
*/

/*
    VimusGUIObject* videocap1 =	this->createObject("videocap", -0.5, 0.6, 0.0);

    VimusGUIObject* videocap2 =	this->createObject("videocap", 0.5, 0.6, 0.0);

    VimusGUIObject* threshold1 = this->createObject("pixel_cvthreshold", -0.5, 0.0, 0.0);

    VimusGUIObject* threshold2 = this->createObject("pixel_cvthreshold", 0.5, 0.0, 0.0);

	VimusGUIObject* vitalino = this->createObject("vitalino", -0.3, -0.3, 0.0);

	VimusGUIObject* preview1 = this->createObject("video", -0.1, -0.6, 0.0);

	VimusGUIObject* preview2 = this->createObject("video", -0.1, -0.6, 0.0);

	this->connectPins(  videocap1->indexOnParent,
                        videocap1->getOutputByIndex(0)->indexOnParent,
                        threshold1->indexOnParent,
                        threshold1->getInputByIndex(0)->indexOnParent);

	this->connectPins(  videocap2->indexOnParent,
                        videocap2->getOutputByIndex(0)->indexOnParent,
                        threshold2->indexOnParent,
                        threshold2->getInputByIndex(0)->indexOnParent);

	this->connectPins(  threshold1->indexOnParent,
                        threshold1->getOutputByIndex(0)->indexOnParent,
                        vitalino->indexOnParent,
                        threshold1->getInputByIndex(0)->indexOnParent);

	this->connectPins(  threshold2->indexOnParent,
                        threshold2->getOutputByIndex(0)->indexOnParent,
                        vitalino->indexOnParent,
                        vitalino->getInputByIndex(1)->indexOnParent);

	this->connectPins(  threshold1->indexOnParent,
                        threshold1->getOutputByIndex(0)->indexOnParent,
                        preview1->indexOnParent,
                        preview1->getInputByIndex(0)->indexOnParent);

	this->connectPins(  threshold2->indexOnParent,
                        threshold2->getOutputByIndex(0)->indexOnParent,
                        preview2->indexOnParent,
                        preview2->getInputByIndex(0)->indexOnParent);
//*/

/*
    this->createObject("pixelAdd", -0.3, 0.0, 0.0);

	this->createObject("audiocap", 0.5, 0.3, 0.0);

	this->createObject("abs AVeffect", -0.3, -0.15, 0.0);

	this->setCurrentCube (2);

	this->createObject("inlet", -0.5, 0.7, 0.0);
	this->createObject("inlet", 0.0, 0.7, 0.0);
	this->createObject("outlet", -0.5, -0.8, 0.0);

	this->setCurrentCubeToParent();

    if (DEBUG_MODE)
        cout << "\nCreated a test patch.";
*/
}
