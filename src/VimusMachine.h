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

#ifndef _VIMUSMACHINE_H_
#define _VIMUSMACHINE_H_

//#include "Machine.h"

#include "VimusMachineEditor.h"
#include "VideoCaptureOpenCV.h"
#include "Freenect.h"

#include <string>

/**
 * VimusMachine is ViMus own machine implementation.
 *
 * @author Jarbas Jácome
 */
class VimusMachine// : public Machine //TODO: use abstract machine
{
    public:

	/**
         * VimusMachine default constructor.
         */
        VimusMachine();

	/**
         * VimusMachine default destructor.
         */
        ~VimusMachine();

	/**
	 * Start VimusMachine.
	 */
	void start(); //TODO: check if it is already started, stop, resume.

	void stop();

	/**
	 * Updates machine state.
	 * Executes DSP Duty cicle.
	 */
	void update();

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
     * Draws all OpenGL machine objects
     */
    void drawOpenGLObjects();

    private:

		/**
		 * Machine editor
		 */
		VimusMachineEditor* editor;

		VimusMachineAbstractionObject* rootAbsObj;

        VideoCaptureOpenCV* videoCap;

        MyFreenectDevice* kinect;

};

#endif //_VIMUSMACHINE_H_
