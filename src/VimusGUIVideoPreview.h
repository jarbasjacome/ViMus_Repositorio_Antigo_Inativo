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

#ifndef _VIMUSGUIVIDEOPREVIEW_H_
#define _VIMUSGUIVIDEOPREVIEW_H_

#include"Configuration.h"
#include "VimusGUIObject.h"

#include <stdlib.h>
#include <GL/glext.h>
#include <GL/glut.h>

#include <iostream>

/**
 * VimusGUIVideoPreview is an interface object for display videos
 * and images.
 *
 * @author Jarbas Jácome
 * @author Mavix
 */
class VimusGUIVideoPreview : public VimusGUIObject
{
    public:

        /**
         * VimusGUIVideoPreview default constructor.
         */
        VimusGUIVideoPreview();

        /**
         * VimusGUIVideoPreview constructor.
         */
        VimusGUIVideoPreview(float posX, float posY, float posZ,
								unsigned char ** ppCurrentFrame);

        /**
         * VimusGUIVideoPreview constructor.
         */
        VimusGUIVideoPreview(float posX, float posY, float posZ,
                        float width, float height, float length,
                        int numInputs, int numOutputs, bool isFixedOnParent,
                        int parentCubeCurrentSide);

        /**
         * VimusGUIVideoPreview destructor.
         */
        ~VimusGUIVideoPreview();

        /**
         * Update VimusGUIVideoPreview.
         */
        void update();

        /**
         * Draw VimusGUIVideoPreview.
         */
        void draw(int renderMode);

        /**
         * Draw VimusGUIObject side.
         */
        void drawSide(int renderMode, int side);

		/**
		 * Draw currrent video frame.
		 */
		void drawFrame(	float posX, float posY, float posZ,
						float width, float height);

        /**
         * Translate.
         */
        void translate(float x, float y, float z);

   		void keyBoardFunc(unsigned char key, int x, int y);

		void specialKeyBoardFunc(int key, int x, int y);

    private:

		unsigned char ** ppCurrentFrame;

        GLuint texName;
};

#endif //_VIMUSGUIVIDEOPREVIEW_H_
