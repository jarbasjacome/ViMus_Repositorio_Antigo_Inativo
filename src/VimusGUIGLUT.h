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
 * This class is a Vimus User Interface implemented
 * using GLUT.
 *
 * @author Jarbas Jácome
 */

#include "Configuration.h"

#include "VimusGUIEditor.h"

#include "VimusMachine.h"

#include <stdlib.h>
#include <string>
#include <sstream>

#include <GL/glut.h>
#include <boost/thread/xtime.hpp>



class VimusGUIGLUT
{

	public:

		VimusGUIGLUT();
		~VimusGUIGLUT();

		void start();
		static void finish();

        /**
         * GLUT init.
         */
        void initGLUT(int argc, char** argv);

		/**
		 * Add a machine reference to VimusUserInterface
		 */
		void addMachine (VimusMachine * machine);

		/**
		 * Creates a test patch.
		 */
		void createTestPatch ();

	private:

        bool isFullscreen;

        boost::xtime lastSysTime;
        boost::xtime currSysTime;
        double pastTime;

		VimusMachine * machine;

        VimusGUIEditor * editor;

		// used to make possible to call non-static member functions from
		// GLUT callback functions (as keyBoardFunc, or displayFunc)
		static VimusGUIGLUT * vimusUIPtr;

		void registerCallbackFunctions();

        void mouseFunc(int button, int state, int x, int y);
		inline static void mouseStaticFunc(int button, int state, int x, int y)
		{	vimusUIPtr->mouseFunc(button, state, x, y);	};

		void motionFunc(int x, int y);
		inline static void motionStaticFunc(int x, int y)
		{	vimusUIPtr->motionFunc(x, y);	};

        void passiveMotionFunc(int x, int y);
		inline static void passiveMotionStaticFunc(int x, int y)
		{	vimusUIPtr->passiveMotionFunc(x, y);	};

        void keyBoardFunc(unsigned char key, int x, int y);
		inline static void keyBoardStaticFunc(unsigned char key, int x, int y)
		{	vimusUIPtr->keyBoardFunc(key, x, y);	};

		void specialKeyBoardFunc(int key, int x, int y);
		inline static void specialKeyBoardStaticFunc(int key, int x, int y)
		{	vimusUIPtr->specialKeyBoardFunc(key, x, y);	};

		void displayFunc();
		inline static void displayStaticFunc()
		{	vimusUIPtr->displayFunc();	};

		void idleFunc();
		inline static void idleStaticFunc()
		{	vimusUIPtr->idleFunc();	};

};
