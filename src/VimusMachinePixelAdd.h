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

#ifndef _VIMUSMACHINEPIXELADD_H_
#define _VIMUSMACHINEPIXELADD_H_

#include "VimusMachineObject.h"

/**
 * VimusMachinePixelAdd represents an interface object
 * to play videos.
 *
 * @author Jarbas Jácome
 */
class VimusMachinePixelAdd : public VimusMachineObject
{
    public:

        /**
         * VimusMachinePixelAdd default constructor.
         */
        VimusMachinePixelAdd();

        /**
         * VimusMachinePixelAdd constructor.
         */
        VimusMachinePixelAdd(int numInputs, int numOutputs);

        /**
         * VimusMachinePixelAdd destructor.
         */
        ~VimusMachinePixelAdd();

        /**
         * Update VimusMachinePixelAdd.
         */
        void update();

		/**
		 * Get pointer to currentFrame buffer pointer.
		 * This is necessary for GUI have a direct access to
		 * machine frame buffer.
		 */
		unsigned char** getCurrentFramePointer();

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

		unsigned char** ppInputData;

		unsigned char** ppOutputData;

		unsigned char** ppNullFrame;

};

#endif //_VIMUSMACHINEPIXELADD_H_
