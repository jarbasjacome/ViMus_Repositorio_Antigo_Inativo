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

#ifndef _VIMUSMACHINEOBJECT_H_
#define _VIMUSMACHINEOBJECT_H_

#include <stdlib.h>
#include "Configuration.h"

#include "VimusMachinePin.h"
#include "VimusMachinePinVideo.h"
#include "VimusMachinePinControl.h"

#include <string>

/**
 * VimusMachineObject is a generic object
 *
 * @author Jarbas Jácome
 */
class VimusMachineObject
{
    public:

        /**
         * VimusMachineObject default constructor.
         */
        VimusMachineObject();

        /**
         * VimusMachineObject destructor.
         */
        virtual ~VimusMachineObject();

		/**
         * Initializes object default values.
         */
        void initObject();

        /**
         * Set parent.
         */
        void setParent(VimusMachineObject * parent, int index);

		/**
         * Parent abstraction.
         */
        VimusMachineObject * parentAbsObj;

        /**
         * Index of this object in parent objects array.
         */
        int indexOnParent;

        /**
         * True if this object is a AbstractionObject.
         */
        bool isAbstractionObject;

        /**
         * True if this object is an InterfaceObject.
         */
        bool isInterfaceObject;

        /**
         * VimusMachineObject label text.
         */
        std::string label;

        /**
         * Number of VimusMachineObject inputs.
         */
        int numInputs;

        /**
         * Number of VimusMachineObject outputs.
         */
        int numOutputs;

        const static int MAX_PINS = 30;

        VimusMachinePin * inputPins[MAX_PINS];
        VimusMachinePin * outputPins[MAX_PINS];

        /**
         * Update VimusMachineObject.
         */
        virtual void update() = 0;

        /**
         * Get input Pin by index.
         */
        VimusMachinePin * getInputByIndex(int index);

        /**
         * Get output Pin by index.
         */
        VimusMachinePin * getOutputByIndex(int index);

		void addInputPin();

		void addOutputPin();

		void addInputPin(int type);

		void addOutputPin(int type);

		/**
         * Set up input and output Pins.
         */
        void setPins(int numInputs, int numOutputs);

		/**
		 * Do some action before and/or after connect an output pin to
		 * another objects input pin.
		 */
		virtual bool connectOutput (int outPin, VimusMachineObject * dstObj, int inPin) = 0;

		/**
		 * Do some action before and/or after connect an input pin to
		 * another objects output pin.
		 */
		virtual bool connectInput (VimusMachineObject * srcObj, int outPin, int inPin) = 0;

		/**
		 * Do some action before and/or after disconnect an output pin from
		 * another objects pin.
		 */
		virtual void disconnectOutput (int outPin, VimusMachineObject * dstObj, int inPin) = 0;

		/**
		 * Do some action before and/or after disconnect an input pin from
		 * another objects pin.
		 */
		virtual void disconnectInput (VimusMachineObject * srcObj, int outPin, int inPin) = 0;

    private:

};

#endif //_VIMUSMACHINEOBJECT_H_
