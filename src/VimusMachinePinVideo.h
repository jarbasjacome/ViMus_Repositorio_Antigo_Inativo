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

#ifndef _VIMUSMACHINEPINVIDEO_H_
#define _VIMUSMACHINEPINVIDEO_H_

#include <stdlib.h>

#include "VimusMachinePin.h"

/**
 * VimusMachinePinVideo represents a video Pin.
 *
 * @author Jarbas Jácome
 */
class VimusMachinePinVideo : public VimusMachinePin
{
    public:

        /**
         * VimusMachinePinVideo constructor.
         */
        VimusMachinePinVideo (bool input, int index);

        /**
         * VimusMachinePinVideo destructor.
         */
        ~VimusMachinePinVideo();

		unsigned char ** ppFrame;

    private:


};

#endif //_VIMUSMACHINEPINVIDEO_H_
