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

#ifndef _VIMUSMACHINEEDITOR_H_
#define _VIMUSMACHINEEDITOR_H_

#include "VimusMachineObjects.h"
#include "VimusGUIObjects.h"
#include "VideoCaptureOpenCV.h"
#include "Freenect.h"

#include <string>
#include <sstream>
#include <vector>

/**
 * VimusMachineEditor is module responsible for patch edition.
 *
 * @author Jarbas Jácome
 */
class VimusMachineEditor
{
    public:

		/**
         * VimusMachineEditor default constructor.
         */
        VimusMachineEditor(VideoCaptureOpenCV* vidCap, MyFreenectDevice* kin);

		/**
         * VimusMachineEditor default destructor.
         */
        ~VimusMachineEditor();

		/**
		 * Creates a new machine object.
		 */
		VimusGUIObject * createObject(const std::string& label, float posX,
												float posY, float posZ);
		/**
		 * Removes an object by index.
		 */
		void removeObject(int index);

		/**
		 * Connect objects pins.
		 */
		bool connectPins(int srcObj, int outPin, int dstObj, int inPin);

		/**
		 * Disconnect objects pins.
		 */
		void disconnectPins(int srcObj, int outPin, int dstObj, int inPin);

		/**
		 * Set currentAbsObj by index.
		 */
		void setCurrentAbsObj (int index);

		/**
		 * Set currentAbsObj by son's name.
		 */
		void setCurrentAbsObj (const std::string& objName);

		/**
		 * Set currentAbsObj to be the current cube's parent cube.
		 */
		void setCurrentAbsObjToParent ();

		/**
		 * Get root abstraction object.
		 */
		VimusMachineAbstractionObject * getRootAbsObj();

        /**
         * Draws all OpenGL objects
         */
        void drawOpenGLObjects();


    private:

		/**
		 * This is the root abstraction object of current machine objects tree.
		 * This object contains all current instatiated machine objects.
		 */
		VimusMachineAbstractionObject * rootAbsObj;

		/**
		 * This is the pointer used by VimusMachineEditor to navegate
		 * through machine objects tree.
		 */
		VimusMachineAbstractionObject * currentAbsObj;

        static const int MAX_GL_OBJ = 1000;

        /**
         * OpenGL machine objects array.
         */
        VimusMachineOpenGLObject* opengGLObjects[MAX_GL_OBJ];

        /**
         * Number of OpenGL machine objects
         */
        int numOpenGLObjs;

        /**
         * Adds an OpenGL machine object
         *
         * @return index of the added object in the machine editors array
         */
        void addOpenGLObject(VimusMachineOpenGLObject* obj);

        /**
         * Remove an OpenGL machine object
         */
        void removeOpenGLObject (int index);

		/**
		 * Tokens of message (words separeted by " ").
		 */
		std::vector<std::string> msgTokens;

		/**
		 * Parse message text to tokens.
		 */
		void parseMsg (const std::string& msg);

		/**
		 * Reference to Video Capture Manager.
		 */
		 VideoCaptureOpenCV* videoCap;

        MyFreenectDevice* kinect;

};
#endif //_VIMUSMACHINEEDITOR_H_
