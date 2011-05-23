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

#ifndef _VIMUSMACHINEOPENGLOBJECT_H_
#define _VIMUSMACHINEOPENGLOBJECT_H_

#include <stdlib.h>
#include "Configuration.h"

#include "VimusMachineObject.h"

#include <string>

/**
 * VimusMachineOpenGLObject is a generic object
 *
 * @author Jarbas Jácome
 */
class VimusMachineOpenGLObject : public VimusMachineObject
{
    public:

        /**
         * VimusMachineOpenGLObject default constructor.
         */
        VimusMachineOpenGLObject();

        /**
         * VimusMachineOpenGLObject destructor.
         */
        virtual ~VimusMachineOpenGLObject();

        /**
         * Update VimusMachineOpenGLObject.
         */
        virtual void update() = 0;

        /**
         * Update VimusMachineOpenGLObject.
         */
        virtual void draw() = 0;

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

        virtual void keyBoardFunc(unsigned char key, int x, int y) = 0;
        virtual void specialKeyBoardFunc(int key, int x, int y) = 0;

    private:

};

#endif //_VIMUSMACHINEOPENGLOBJECT_H_
