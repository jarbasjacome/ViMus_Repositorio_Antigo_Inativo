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

#ifndef _VIMUSGUICUBE_H_
#define _VIMUSGUICUBE_H_

#include "Configuration.h"

#include <stdlib.h>
#include <math.h>

#include <GL/glut.h>

#include "VimusGUINormalObject.h"

#include <iostream>

/**
 * VimusGUICube represents a node (unit generator or object)
 * of a VimusPatch.
 *
 * @author Jarbas Jácome
 * @author Mavix
 */
class VimusGUICube : public VimusGUINormalObject
{
    public:

        const static int MAX_OBJ = 100;

        /**
         * VimusGUICube rotation states constants.
         */
        static const int ROTATION_STOPPED        = 0;
        static const int ROTATION_FREE           = 1;
        static const int ROTATION_AUTO           = 2;

        /**
         * VimusGUICube rotation state.
         */
        int rotationState;

        /**
         * VimusGUICube current side.
         */
        int currentSide;

        /**
         * VimusGUICube patch objects array.
         */
        VimusGUIObject* objects[MAX_OBJ];

        /**
         * Number of objects.
         */
        int numObjects;

        /**
         * VimusGUICube patch selected objects array.
         */
        VimusGUIObject* selectedObjects[MAX_OBJ];

        /**
         * Number of selected objects.
         */
        int numSelectedObjects;

        VimusGUIPin * selectedPatchOutputPins[MAX_OBJ];

        int numSelectedPatchOutputPins;

        /**
         * VimusGUICube default constructor.
         */
        VimusGUICube();

        /**
         * VimusGUICube constructor.
         */
        VimusGUICube( const std::string& label, float posX, float posY, float posZ,
						bool isFixedOnParent);

        /**
         * VimusGUICube constructor.
         */
        VimusGUICube (const std::string& label, float posX, float posY, float posZ,
                        VimusGUIObject * objs[MAX_OBJ], int numObjs,
                        bool isFixedOnParent);

        /**
         * VimusGUICube destructor.
         */
        ~VimusGUICube();

        /**
         * Draw VimusGUICube.
         */
        void update();

        /**
         * Draw VimusGUICube.
         */
        void draw(int renderMode);

        /**
         * Draw VimusGUICube side.
         */
        void drawSide(int renderMode, int side);

        /**
        * Adds X axis rotation angle.
        */
        void addAngleX(float angle);

        /**
         * Adds Y axis rotation angle.
         */
        void addAngleY(float angle);

        /**
         * Turn left.
         */
        void turnLeft();

        /**
         * Turn left.
         */
        void turnRight();

        /**
         * Turn left.
         */
        void turnUp();

        /**
         * Turn left.
         */
        void turnDown();

        /**
         * Enable ROTATION_FREE rotation mode.
         */
        void enableFreeRotation();

        /**
         * Disable free rotation mode.
         */
        void disableFreeRotation();

        /**
         * Returns if it is in ROTATION_FREE rotation state.
         */
        bool isFreeRotation();

        /**
         * Maximize VimusGUICube size.
         */
        void maximize();

        /**
         * Minimize VimusGUICube size.
         */
        void minimize();

        /**
         * Restore VimusGUICube size.
         */
        void restore();

        /**
         * Restore VimusGUICube size.
         */
        void restore(float x, float y, float z, float w, float h, float len);

        void restoreToControlPanel();
        void restoreToPatch();

        /**
         * Add an object to VimusGUICube patch.
         */
        void addObject(VimusGUIObject * object);

        /**
         * Remove an object from VimusGUICube patch.
         */
        void removeObject(int index);

        /**
         * Turn patch 3D view mode on.
         */
        void enablePatch3D();

        /**
         * Turn patch 3D view mode off.
         */
        void disablePatch3D();

        void rotateTo(int side);

		/**
         * Set VimusGUICube current visible side.
         */
        void setCurrentSide(int side);

        void keyBoardFunc(unsigned char key, int x, int y);

		void specialKeyBoardFunc(int key, int x, int y);

    private:

        /**
         * VimusGUICube X position at normal view mode.
         */
        float posXNormal;

        /**
         * VimusGUICube Y position at normal view mode.
         */
        float posYNormal;

        /**
         * VimusGUICube Z position at normal view mode.
         */
        float posZNormal;

        /**
         * VimusGUICube X position at minimized view mode.
         */
        float posXMini;

        /**
         * VimusGUICube Y position at minimized view mode.
         */
        float posYMini;

        /**
         * VimusGUICube Z position at minimized view mode.
         */
        float posZMini;

        /**
         * VimusGUICube width at normal view mode.
         */
        float widthNormal;

        /**
         * VimusGUICube height at normal view mode.
         */
        float heightNormal;

        /**
         * VimusGUICube length at normal view mode.
         */
        float lengthNormal;

        int rotNumFrames;

        int rotCurrentFrame;

        /**
         * VimusGUICube X axis rotation angle.
         */
        float rotationAngleX;

        /**
         * VimusGUICube Y axis rotation angle.
         */
        float rotationAngleY;

        /**
         * VimusGUICube Z axis rotation angle.
         */
        float rotationAngleZ;

        /**
         * VimusGUICube X axis rotation angle increment.
         */
        float rotAngleXInc;

        /**
         * VimusGUICube Y axis rotation angle increment.
         */
        float rotAngleYInc;

        /**
         * VimusGUICube Z axis rotation angle increment.
         */
        float rotAngleZInc;

        /**
         * VimusGUICube X axis rotation angle increment factor
         * (increment/number of frames).
         */
        float rotAngleXFactor;

        /**
         * VimusGUICube Y axis rotation angle increment factor
         * (increment/number of frames).
         */
        float rotAngleYFactor;

        /**
         * VimusGUICube Z axis rotation angle increment factor
         * (increment/number of frames).
         */
        float rotAngleZFactor;

        /**
         * VimusGUICube view mode.
         */
        int viewMode;

        static const int VIEW_MINIMIZED    = 0;
        static const int VIEW_NORMAL       = 1;
        static const int VIEW_MAXIMIZED    = 2;

        /**
         * Temporary VimusGUIPin objects to draw lines.
         */
        VimusGUIPin * tempOutputPin;
        VimusGUIPin * tempInputPin;

        /**
         * Set rotation state.
         */
        void setRotationState(int rotState);

        /**
         * Get rotation state.
         */
        int getRotationState();

        /**
         * Draws patch lines.
         */
        void drawLines(int renderMode);

        /**
         * Draw control panel.
         */
        void drawControlPanel ( int renderMode, float x, float y, float z,
                                float w, float h);

        /**
         * Update patch.
         */
        void updatePatch();

        /**
         * Draw patch.
         */
        void drawPatch(int renderMode);

        /**
         * Upate VimusGUICube rotationstate.
         */
        void updateRotationState();

        /**
         * Set position at normal view mode.
         */
        void setPosNormal(float posX, float posY, float posZ);

        /**
         * Set position at minimized view mode.
         */
        void setPosMinimized(float posX, float posY, float posZ);

        void rotateTo(float angleXInc, float angleYInc, float angleZInc);

        void rotationReset();

        void setFixedOnParentSide(int side);

        void translate(float x, float y, float z);

};

#endif //_VIMUSGUICUBE_H_
