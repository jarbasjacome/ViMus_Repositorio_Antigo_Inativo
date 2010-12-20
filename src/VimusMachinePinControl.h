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

#ifndef _VIMUSMACHINEPINCONTROL_H_
#define _VIMUSMACHINEPINCONTROL_H_

#include <stdlib.h>

#include "VimusMachinePin.h"

/**
 * VimusMachinePinControl represents a control Pin.
 *
 * @author Jarbas Jácome
 */
class VimusMachinePinControl : public VimusMachinePin
{
    public:

        /**
         * VimusMachinePinControl constructor.
         */
        VimusMachinePinControl (bool input, int index);

        /**
         * VimusMachinePinControl destructor.
         */
        ~VimusMachinePinControl();

        /**
         * Connect to an input pin.
         */
		void connect(VimusMachinePin *pin);

        /**
         * Disconnect a pin.
         */
        void disconnect(VimusMachinePinControl *pin);

        VimusMachinePinControl * connections[MAX_CONNECTIONS];

    private:


};

#endif //_VIMUSMACHINEPINCONTROL_H_
