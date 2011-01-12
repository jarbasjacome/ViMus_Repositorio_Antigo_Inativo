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

#ifndef _VIMUSMACHINEABSTRACTIONOBJECT_H_
#define _VIMUSMACHINEABSTRACTIONOBJECT_H_

#include "VimusMachineNormalObject.h"
#include "VimusMachineVideoPreview.h"

#include <iostream>
#include <string>

/**
 * VimusMachineAbstractionObject represents an object which contains
 * a patch, with one or more objects connected.
 *
 * @author Jarbas Jácome
 */
class VimusMachineAbstractionObject : public VimusMachineNormalObject
{
    public:

        const static int MAX_OBJ = 100;

        /**
         * VimusMachineAbstractionObject patch objects array.
         */
        VimusMachineObject* objects[MAX_OBJ];

        /**
         * Number of objects.
         */
        int numObjects;

		/**
         * VimusMachineAbstractionObject constructor.
         */
        VimusMachineAbstractionObject(const std::string& label);

        /**
         * VimusMachineAbstractionObject destructor.
         */
        ~VimusMachineAbstractionObject();

		/**
		 * Set
		 */
		void setLabel();

        /**
         * Draw VimusMachineAbstractionObject.
         */
        void update();

        /**
         * Add an object to VimusMachineAbstractionObject patch.
         */
        void addObject(VimusMachineObject * object);

        /**
         * Remove an object from VimusMachineAbstractionObject patch.
         */
        void removeObject(int index);

		/**
		 * Do some action before and/or after connect a pin to
		 * another objects pin.
		 */
		bool connectOutput (int outPin, VimusMachineObject * dstObj, int inPin);

		/**
		 * Do some action before and/or after be connected by another
		 * objects pin.
		 */
		bool connectInput (VimusMachineObject * srcObj, int outPin, int inPin);

		/**
		 * Do some action before and/or after disconnect an output pin from
		 * another objects pin.
		 */
		void disconnectOutput (int outPin, VimusMachineObject * dstObj, int inPin);

		/**
		 * Do some action before and/or after disconnect an input pin from
		 * another objects pin.
		 */
		void disconnectInput (VimusMachineObject * srcObj, int outPin, int inPin);

    private:

        /**
         * Temporary Pin objects to draw lines.
         */
        VimusMachinePin * tempOutputPin;
        VimusMachinePin * tempInputPin;

        /**
         * Update patch.
         */
        void updatePatch();

};

#endif //_VIMUSMACHINEABSTRACTIONOBJECT_H_
