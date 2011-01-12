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

#ifndef _MACHINE_H_
#define _MACHINE_H_

#include "VimusGUIObjects.h"

#include <string>

/**
 * Machine is the machine generic class.
 *
 * @author Jarbas Jácome
 */
class Machine
{
    public:

		/**
         * Machine default constructor.
         */
        Machine();

		/**
         * Machine default destructor.
         */
        ~Machine();

		/**
		 * Start VimusMachine.
		 */
		virtual void start() = 0; //TODO: check if it is already started, stop, resume.

		/**
		 * Updates machine state.
		 * Executes DSP Duty cicle.
		 */
		virtual void update() = 0;

		/**
		 * Creates a new machine object.
		 */
		virtual VimusGUIObject * createObject(const string& label, float posX,
												float posY, float posZ) = 0;

		/**
		 * Removes an object by index.
		 */
		virtual void removeObject(int index) = 0;

		/**
		 * Connect objects pins.
		 */
		virtual bool connectPins(int srcObj, int outPin, int dstObj, int inPin) = 0;

		/**
		 * Disconnect objects pins.
		 */
		virtual void disconnectPins(int srcObj, int outPin, int dstObj, int inPin) = 0;

		/**
		 * Set currentAbsObj by index.
		 */
		virtual void setCurrentAbsObj(int index) = 0;

		/**
		 * Set currentAbsObj by son's name.
		 */
		virtual void setCurrentAbsObj(const string& objName) = 0;

		/**
		 * Set currentAbsObj to be the current cube's parent cube.
		 */
		virtual void setCurrentAbsObjToParent() = 0;

    private:

};

#endif //_MACHINE_H_
