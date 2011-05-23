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

#ifndef _VIMUSGUIOBJECT_H_
#define _VIMUSGUIOBJECT_H_

#include "Configuration.h"

#include "VimusGUIPin.h"

#include <stdlib.h>
#include <string>

#include <GL/glut.h>

/**
 * VimusGUIObject is a generic object
 *
 * @author Jarbas Jácome
 * @author Mavix
 */
class VimusGUIObject
{
    public:

		/**
         * Vimus object types.
         */
        static const int OBJECT_TYPE_NORMAL			= 0;
        static const int OBJECT_TYPE_INTERFACE		= 1;
        static const int OBJECT_TYPE_CUBE			= 2;
		static const int OBJECT_TYPE_COMMENT		= 3;

		/**
		 * Vimus cube object sides.
		 */
		static const int SIDE_CONTROLPANEL    = 0;
        static const int SIDE_INPUT           = 1;
        static const int SIDE_OUTPUT          = 2;
        static const int SIDE_PATCH           = 3;

        /**
         * VimusGUIObject default constructor.
         */
        VimusGUIObject();

        /**
         * VimusGUIObject destructor.
         */
        virtual ~VimusGUIObject();

		/**
         * Set up input and output Pins.
         */
        void setPins(int numInputs, int numOutputs);

        /**
         * Set parent.
         */
        void setParent(VimusGUIObject * parent, int index,
							   int parentCubeCurrentSide);

        /**
         * Draw VimusGUIObject.
         */
        virtual void update() = 0;

        /**
         * Draw VimusGUIObject.
         */
        virtual void draw(int renderMode) = 0;

        /**
         * Draw VimusGUIObject side.
         */
		//TODO: it doesnt makes sense for non cube objects.
        virtual void drawSide(int renderMode, int side) = 0;

        /**
         * Turn minimized3D on.
         */
        void enableMinimized3D();

        /**
         * Turn minimized3D off.
         */
        void disableMinimized3D();

        /**
         * Move to new position.
         */
        //void moveTo(float posX, float posY, float posZ);

        /**
         * Translate.
         */
        virtual void translate(float x, float y, float z) = 0;

        /**
         * Get input VimusGUIPin by index.
         */
        VimusGUIPin * getInputByIndex(int index);

        /**
         * Get output VimusGUIPin by index.
         */
        VimusGUIPin * getOutputByIndex(int index);

        /**
         * Connect an output to another object input.
         */
//        void connect(VimusGUIPin * output, VimusGUIPin * input);

        /**
         * Renders a string.
         */
        void renderBitmapString(float x, float y, float z, void *font, const char *string);

        /**
         * Renders a stroke font string.
         */
        void renderStrokeFontString(float x, float y, float z, void *font, const char *string);

        /**
         * Compute string width.
         */
        float bitmapStringWidth(void *font, const char *string);

        /**
         * Compute stroke font string width.
         */
        float strokeFontStringWidth(void *font, const char *string);

        /**
         * Draw minimized VimusGUIVideoPreview in 2D.
         */
        void drawGraphOnParent2D();

        /**
         * Draw minimized VimusGUIVideoPreview in 3D.
         */
        void drawGraphOnParent3D();

        void setPosControlPanel(float x, float y, float z);
        void setSizeControlPanel(float w, float h);

        float posXControlPanel;
        float posYControlPanel;
        float posZControlPanel;
        float widthControlPanel;
        float heightControlPanel;
        float lengthControlPanel;

        /**
         * VimusGUIObject fixed on parent cube control panel.
         */
        bool isFixedOnParent;

        /**
         * Parent VimusGUICube.
         */
        VimusGUIObject * parentCube;

        /**
         * Parent VimusGUICube current side.
         */
        int parentCubeCurrentSide;

        /**
         * Index of this object in parent objects array.
         */
        int indexOnParent;

        /**
         * True if this object is a VimusGUICube.
         */
        bool isCube;

        /**
         * True if this object is a VimusGUIVideoPreview.
         */
        bool isInterfaceObject;

        /**
         * True is this object is selected.
         */
        bool isSelected;

        /**
         * VimusGUIObject label text.
         */
        char* label;

        /**
         * Number of VimusGUIObject inputs.
         */
        int numInputs;

        /**
         * Number of VimusGUIObject outputs.
         */
        int numOutputs;

        const static int MAX_PINS = 30;

        VimusGUIPin * inputPins[MAX_PINS];
        VimusGUIPin * outputPins[MAX_PINS];

		/**
         * VimusGUIObject X position.
         */
        float posX;

        /**
         * VimusGUIObject Y position.
         */
        float posY;

        /**
         * VimusGUIObject Z position.
         */
        float posZ;

        /**
         * VimusGUIObject width.
         */
        float width;

        /**
         * VimusGUIObject height.
         */
        float height;

        /**
         * VimusGUIObject length.
         */
        float length;

        /**
         * VimusGUIObject X scale.
         */
        float scaleX;

        /**
         * VimusGUIObject Y scale.
         */
        float scaleY;

        /**
         * VimusGUIObject Z scale.
         */
        float scaleZ;

        /**
         * If true, VimusGUIObject will be drawn in 3D when minimized.
         */
        bool isMinimized3D;

		void setLabel(const std::string& label);

		void addInputPin();

		void addOutputPin();

		virtual void keyBoardFunc(unsigned char key, int x, int y) = 0;

		virtual void specialKeyBoardFunc(int key, int x, int y) = 0;

    private:

};

#endif //_VIMUSGUIOBJECT_H_
