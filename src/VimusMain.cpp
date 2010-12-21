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

/**
 * Vimus main function
 */
#include "VimusGUIGLUT.h"

#include "VimusMachine.h"

int main(int argc, char** argv)
{

	VimusGUIGLUT gui; //user interface singleton.

	VimusMachine * vimusMachine = new VimusMachine ();

	vimusMachine->start();

	gui.initGLUT(argc,argv);

    gui.addMachine(vimusMachine);

    gui.createTestPatch();

	gui.start();

	return 0;
}
