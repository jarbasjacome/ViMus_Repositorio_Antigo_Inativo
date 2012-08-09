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

#include "VimusGUIGLUT.h"

using namespace std;

/**
 * Vimus User Interface Singleton object, to make it possible to
 * call non-static member functions from GLUT callback
 * function like keyDoardFunc, displayFunc.
 */
VimusGUIGLUT * VimusGUIGLUT::vimusUIPtr = new VimusGUIGLUT();

/**
 * Constructor
 */
VimusGUIGLUT :: VimusGUIGLUT()
{
    //this->editor = new VimusGUIEditor();
}

/**
 * Destructor
 */
VimusGUIGLUT :: ~VimusGUIGLUT()
{
	if (glutGetWindow()) {
		glutDestroyWindow(glutGetWindow());
	}
}

/**
 * This method is called by keyMouseStaticFunc when
 * mouse button is pressed or released.
 * When a user presses and releases mouse buttons in the window,
 * each press and each release generates a mouse callback.
 * This function can call non-static function members.
 *
 * @param int button is GLUT_LEFT_BUTTON,  GLUT_MIDDLE_BUTTON, or  GLUT_RIGHT_BUTTON.
 * @param int state is GLUT_UP or GLUT_DOWN.
 * @param int x cursor position.
 * @param int y cursor position.
 */
void VimusGUIGLUT ::mouseFunc(int button, int state, int x, int y)
{
    vimusUIPtr->editor->mouseFunc(button, state, x, y);
}

/**
 * This method is called by keyMotionStaticFunc when the mouse moves within
 * the window while one or more mouse buttons are pressed.
 *
 * This function can call non-static function members.
 *
 * @param int x cursor position.
 * @param int y cursor position.
 */
void VimusGUIGLUT::motionFunc(int x, int y)
{
    vimusUIPtr->editor->motionFunc(x, y);
}

/**
 * This method is called by keyPassiveMotionStaticFunc when the mouse moves
 * within the window while no mouse buttons are pressed.
 *
 * This function can call non-static function members.
 *
 * @param int x cursor position.
 * @param int y cursor position.
 */
void VimusGUIGLUT ::passiveMotionFunc(int x, int y)
{
    vimusUIPtr->editor->passiveMotionFunc(x, y);
}

/**
 * This method is called by keyBoardStaticFunc when a
 * key is pressed.
 * This function can call non-static function members.
 *
 * @param key unsigned char corresponding to key pressed
 * @param x cursor position. Type: int
 * @param y cursor position. Type: int
 */
void VimusGUIGLUT ::keyBoardFunc(unsigned char key,
													  int x, int y)
{
    switch (key)
	{
		case 27:
			finish();
			break;
    }
    vimusUIPtr->editor->keyBoardFunc(key, x, y);
}

/**
 * This method is called by GLUT when a special key is pressed.
 * This function can call non-static function members.
 *
 * @param key int corresponding to key pressed
 * @param x cursor position. Type: int
 * @param y cursor position. Type: int
 */
void VimusGUIGLUT ::specialKeyBoardFunc(int key, int x, int y)
{
	switch (key)
	{
        case GLUT_KEY_F11:
            if (glutGameModeGet(GLUT_GAME_MODE_ACTIVE))
            {
                glutLeaveGameMode();

                //initialize window size.
                glutInitWindowSize (WINDOW_WIDTH, WINDOW_HEIGHT);

                //initialize window position.
                glutInitWindowPosition (WINDOW_POSX, WINDOW_POSY);

                //create new OpenGL window.
                glutCreateWindow (WINDOW_NAME);

                glutSetCursor(GLUT_CURSOR_LEFT_ARROW);

                vimusUIPtr->registerCallbackFunctions();

            }
            else
            {
                glutDestroyWindow(glutGetWindow());
                std::ostringstream resolutionStream;
                std::string resolutionString;
                resolutionStream << glutGet(GLUT_SCREEN_WIDTH)
                                 << "x"
                                 << glutGet(GLUT_SCREEN_HEIGHT)
                                 << ":32@60";
                resolutionString = resolutionStream.str();
                char* resChar = new char[256];
                resolutionString.copy(resChar, resolutionString.size(), 0);
                resChar[resolutionString.size()]='\0';

                glutGameModeString( resChar );

                // start fullscreen game mode
                glutEnterGameMode();

                //show cursor at fullscreen mode.
                glutSetCursor(GLUT_CURSOR_LEFT_ARROW);

                vimusUIPtr->registerCallbackFunctions();
            }
            break;
	}
    vimusUIPtr->editor->specialKeyBoardFunc(key, x, y);
}

/**
 * When GLUT determines that the normal plane for the window needs
 * to be redisplayed, the display callback for the window is called.
 * This method is called by displayStaticFunc.
 * This function can call non-static function member.
 */
void VimusGUIGLUT:: displayFunc()
{
    vimusUIPtr->editor->displayFunc();
}

/**
 * This method is called by displayStaticFunc
 * This function can call non-static function member.
 */
void VimusGUIGLUT:: idleFunc()
{
    vimusUIPtr->editor->update();

    vimusUIPtr->editor->draw();

    boost::xtime_get(&(vimusUIPtr->currSysTime), boost::TIME_UTC);
    vimusUIPtr->pastTime = vimusUIPtr->currSysTime.nsec - vimusUIPtr->lastSysTime.nsec;
    if (vimusUIPtr->pastTime < 10000000 && vimusUIPtr->pastTime > 0)
    {
        vimusUIPtr->currSysTime.nsec += 10000000 - vimusUIPtr->pastTime;
        boost::thread::sleep(vimusUIPtr->currSysTime);
    }
    boost::xtime_get(&(vimusUIPtr->lastSysTime), boost::TIME_UTC);

}

/**
 * Starts the GLUT window main loop.
 */
void VimusGUIGLUT :: start()
{
    vimusUIPtr->pastTime = 0;
    boost::xtime_get(&(vimusUIPtr->lastSysTime), boost::TIME_UTC);

    //start GLUT main loop.
	glutMainLoop();

    if (DEBUG_MODE)
        cout << "\nVimusGUIGLUT started.";
}

/**
 * Finishes Vimus.
 */
void VimusGUIGLUT :: finish()
{
    vimusUIPtr->machine->stop();
	if (glutGetWindow()) {
		glutDestroyWindow(glutGetWindow());
	}
}

/**
 * Add a machine reference to VimusUserInterface
 */
void VimusGUIGLUT :: addMachine (VimusMachine * machine)
{
	if (machine)
	{
		vimusUIPtr->editor->addMachine(machine);
		vimusUIPtr->machine = machine;
	}
}

/**
 * Creates a patch for test.
 */
void VimusGUIGLUT :: createTestPatch()
{
	vimusUIPtr->editor->createTestPatch();
}

/**
 * GLUT init.
 */
void VimusGUIGLUT :: initGLUT(int argc, char** argv)
{
	//initialize the GLUT library and negotiate a session with the window system.
	glutInit(&argc, argv);

    //the  initial display mode is used when creating top-level windows, subwindows,
    //and overlays to determine the OpenGL display mode for the to-be-created window
    //or overlay.glutLeaveGameMode
	//glutInitDisplayMode (GLUT_DOUBLE);// | GLUT_ACCUM); //VITALINO
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    //if fullscreen on set fullscreen.
	if (FULLSCREEN) {

	    vimusUIPtr->isFullscreen = false;

        std::ostringstream resolutionStream;
        std::string resolutionString;
        resolutionStream << glutGet(GLUT_SCREEN_WIDTH)
                         << "x"
                         << glutGet(GLUT_SCREEN_HEIGHT)
                         << ":32@60";
        resolutionString = resolutionStream.str();
        char* resChar = new char[256];
        resolutionString.copy(resChar, resolutionString.size(), 0);
        resChar[resolutionString.size()]='\0';

        glutGameModeString(resChar);

        // start fullscreen game mode
        glutEnterGameMode();

        //show cursor at fullscreen mode.
		glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
	}
	else
	{
	    vimusUIPtr->isFullscreen = false;

        //initialize window size.
        glutInitWindowSize (WINDOW_WIDTH, WINDOW_HEIGHT);

        //initialize window position.
        glutInitWindowPosition (WINDOW_POSX, WINDOW_POSY);

        //create new OpenGL window.
        glutCreateWindow (WINDOW_NAME);
	}

    vimusUIPtr->editor = new VimusGUIEditor();

    vimusUIPtr->registerCallbackFunctions();

}

/**
 * Register all callback functions.
 * Must be called always after window creation.
 */
void VimusGUIGLUT::registerCallbackFunctions ()
{
    glutDisplayFunc(this->displayStaticFunc);
    glutMouseFunc(this->mouseStaticFunc);
    glutMotionFunc(this->motionStaticFunc);
    glutPassiveMotionFunc(this->passiveMotionStaticFunc);
	glutKeyboardFunc(this->keyBoardStaticFunc);
	glutSpecialFunc(this->specialKeyBoardStaticFunc);
	glutIdleFunc(this->idleStaticFunc);
}
