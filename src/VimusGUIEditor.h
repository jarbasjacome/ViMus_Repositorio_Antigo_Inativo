/**
 * Copyright 2007-2011 Jarbas J�come
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

#ifndef _VIMUSGUIEDITOR_H_
#define _VIMUSGUIEDITOR_H_

#include "Configuration.h"
#include "VimusGUIObjects.h"
#include "VimusGUIPin.h"

//#include "Machine.h"
#include "VimusMachine.h"

#include <string>
#include <sstream>
#include <time.h>
#include <GL/glut.h>

/**
 * Editor is responsible for Vimus patch construct and navigation.
 *
 * @author Jarbas J�come
 * @author Mavix
 */
class VimusGUIEditor
{
    public:

        VimusGUIEditor();

        ~VimusGUIEditor();

		/**
		 * Add a machine reference to VimusGUIEditor
		 */
		void addMachine (VimusMachine * machine);

        void mouseFunc(int button, int state, int x, int y);

		void motionFunc(int x, int y);

        void passiveMotionFunc(int x, int y);

        void keyBoardFunc(unsigned char key, int x, int y);

		void specialKeyBoardFunc(int key, int x, int y);

		void displayFunc();

        void update();

		void draw();

		/**
		 * Creates a patch for test.
		 */
		void createTestPatch();

		/**
		 * Creates a new object.
		 */
		VimusGUIObject* createObject(const string& label, float posX, float posY, float posZ);

		/**
		 * Removes an object by index.
		 */
		void removeObject(int index);

		/**
		 * Connect objects pins.
		 */
		bool connectPins(int srcObj, int srcPin, int dstObj, int dstPin);

		/**
		 * Disconnect objects pins.
		 */
		void disconnectPins(int srcObj, int srcPin, int dstObj, int dstPin);

    private:

        /**
         * VimusGUIEditor zoom states constants.
         */
        static const int ZOOM_STOPPED   = 0;
        static const int ZOOM_IN        = 1;
        static const int ZOOM_OUT       = 2;
        static const int ZOOM_FREE      = 3;

        VimusGUICube * mainCube;

        VimusGUICube * currentCube;

	VimusMachine * machine;

        /**
         * Holds current mouse cursor X position.
         */
        int lastMouseX;

        /**
         * Holds current mouse cursor Y position.
         */
        int lastMouseY;

        /**
         * VimusGUICube zoom state.
         */
        int zoomState;

        /**
         * Set zoom state.
         */
        void setZoomState(int newState);

        /**
         * Zoom in to an target cube.
         */
        void zoomInTo (int indexCube);

        void zoomOutFrom ( int indexCube);

        void zoomOut ();

        /**
         * Compute zoom values.
         */
        void computeZoomValues (int cubeIndex);

        float scaleX;
        float scaleY;
        float scaleZ;

        /**
         * Upate VimusGUICube rotationstate.
         */
        void updateZoomState();

        /**
         * Reset zoom state values.
         */
        void resetZoom();

        float camEyeX, camEyeY, camEyeZ, camTargetX, camTargetY, camTargetZ;
        float camEyeXTrans, camEyeYTrans, camEyeZTrans;
        float camTargetXTrans, camTargetYTrans, camTargetZTrans;

        float camEyeXFactor, camEyeYFactor, camEyeZFactor;
        float camTargetXFactor, camTargetYFactor, camTargetZFactor;

        int zoomCurrentFrame, zoomNumFrames;

        int zoomObjRef;

        std::string fpsS;
        bool fpsFlag;

//        clock_t lastFrameTime;
//        clock_t currentFrameTime;
//        float fpsArray[100];
//        float fpsAcum;
//        float fps;

        float fps;
        clock_t lastTime;
        clock_t currTime;
        float timePast;
        float fpsAcum;

        void updateFps();
        void drawFps();

        void showFps();
        void hideFps();

        /**
         * VimusGUIEditor zoom states constants.
         */
        static const int EDITOR_READY                   = 0;
        static const int EDITOR_WAITING_SECOND_CLICK    = 1;
        static const int EDITOR_MOVING_SELECTED         = 2;
        static const int EDITOR_CONNECTING              = 3;
        static const int EDITOR_COPYING_SELECTED        = 4;
        static const int EDITOR_PUTTING_NEW_OBJECCT     = 5;
        static const int EDITOR_SETTING_SELECTION_RECT  = 6;
        static const int EDITOR_RESIZING_OBJECT         = 7;

        bool pickFlag;
        int pickPosX;
        int pickPosY;
        void processHits (GLint hits, GLuint buffer[]);
        void doPick(int x, int y);

        int editorState;
        bool isEditMode;
        bool isCtrlKeyPressed;
        bool isShiftKeyPressed;
        bool isMouseRButtonPressed;
        bool isMouseLButtonPressed;

        static const int BUFFER = 64;
        GLuint selectBuf[BUFFER];
        int hits;
        int viewport[4];
        GLdouble modelview[16];
        GLdouble projection[16];
        GLfloat winX, winY, winZ;
        GLdouble posCursorX, posCursorY, posCursorZ;
        GLdouble posCursorXTemp, posCursorYTemp, posCursorZTemp;
        void screenCoordToSceneCoord(int screenX, int screenY, GLdouble *sceneX,
                                          GLdouble *sceneY, GLdouble *sceneZ);

        void resetEditorState();

        void setEditorState(int state);
        void updateEditorState();

        void unselectAll();

        void selectObject(int index);

        void pickOutputConnectPin(int objIndex, int pinIndex);
        void pickInputConnectPin(int objIndex, int pinIndex);

        void removeSelected();
        void removeSelectedConnections();
        void removeSelectedObjects();

        VimusGUIPin * connectingPin;

		/**
		 * Set currentCube by index.
		 */
		void setCurrentCube (int index);

		/**
		 * Set currentCube by son's name.
		 */
//		void setCurrentCube (const string& objName);

		/**
		 * Set currentCube to be the current cube's parent cube.
		 */
		void setCurrentCubeToParent ();

		/**
		 * Disconnect only GUI objects pins.
		 */
		void disconnectGUIPins(int srcObj, int srcPin, int dstObj, int dstPin);

		/**
		 * Removes a GUI object by index.
		 */
		void removeGUIObject(int index);

};
#endif //_VIMUSEDITOR_H_
