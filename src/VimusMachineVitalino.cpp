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

#include "VimusMachineVitalino.h"

using namespace std;

/**
 * VimusMachineVitalino default constructor.
 */
VimusMachineVitalino::VimusMachineVitalino()
{
    VimusMachineObject::initObject();

	this->label = string ("vitalino");
	this->addInputPin(VimusMachinePin::TYPE_VIDEO);
	this->addInputPin(VimusMachinePin::TYPE_VIDEO);

	this->addOutputPin(VimusMachinePin::TYPE_VIDEO);

	this->ppNullFrame = new unsigned char*;
	(*this->ppNullFrame) = new unsigned char[VIDEO_WIDTH*VIDEO_HEIGHT*3];
	for (int i=0; i<VIDEO_WIDTH*VIDEO_HEIGHT*3; i++)
	{
		(*this->ppNullFrame)[i] = 255;
	}

	// do not assign another valor to it.
	this->ppOutputData = new unsigned char*;
	(*this->ppOutputData) = new unsigned char[VIDEO_WIDTH*VIDEO_HEIGHT*3];
	for (int i=0; i<VIDEO_WIDTH*VIDEO_HEIGHT*3; i++)
	{
		(*this->ppOutputData)[i] = 255;
	}

	// set video output pin
	((VimusMachinePinVideo *)this->outputPins[0])->ppFrame = this->ppOutputData;

	// do not assign another valor to it.
	this->ppInputData[0] = new unsigned char*;
	(*this->ppInputData[0]) = new unsigned char[VIDEO_WIDTH*VIDEO_HEIGHT*3];
	for (int i=0; i<VIDEO_WIDTH*VIDEO_HEIGHT*3; i++)
	{
		(*this->ppInputData[0])[i] = 255;
	}

	// do not assign another valor to it.
	this->ppInputData[1] = new unsigned char*;
	(*this->ppInputData[1]) = new unsigned char[VIDEO_WIDTH*VIDEO_HEIGHT*3];
	for (int i=0; i<VIDEO_WIDTH*VIDEO_HEIGHT*3; i++)
	{
		(*this->ppInputData[1])[i] = 255;
	}

	// set video input pin
	//((VimusMachinePinVideo *)this->inputPins[0])->ppFrame = this->ppInputData;

	this->pontos2 = new int[VIDEO_WIDTH*VIDEO_HEIGHT*2];
	for (int i=0;i<VIDEO_WIDTH*VIDEO_HEIGHT*2;i++)
	{
		this->pontos2[i]=0;
	}
	this->numPontos2 = 0;

	for (int i=0; i<BARRO_WIDTH; i++)
	{
	    for (int j=0; j< BARRO_HEIGHT; j++)
	    {
	        for (int l=0; l < BARRO_DEPTH; l++)
	        {
	            this->barro[i][j][l][0] = true;
	            this->barro[i][j][l][1] = false;
	        }
	    }
    }

    this->numVoxels = BARRO_WIDTH*BARRO_HEIGHT*BARRO_DEPTH;

    light_ambient[0] = 0.0f;
    light_ambient[1] = 0.0f;
    light_ambient[2] = 0.0f;
    light_ambient[3] = 1.0f;

    light_diffuse[0] = 1.0f;
    light_diffuse[1] = 1.0f;
    light_diffuse[2] = 1.0f;
    light_diffuse[3] = 1.0f;

    light_specular[0] = 1.0f;
    light_specular[1] = 1.0f;
    light_specular[2] = 1.0f;
    light_specular[3] = 1.0f;

    light_position[0] = 2.0f;
    light_position[1] = 5.0f;
    light_position[2] = 5.0f;
    light_position[3] = 0.0f;

    mat_ambient[0] =  0.7f;
    mat_ambient[1] =  0.7f;
    mat_ambient[2] =  0.7f;
    mat_ambient[3] =  1.0f;

    mat_diffuse[0] = 0.8f;
    mat_diffuse[1] = 0.8f;
    mat_diffuse[2] = 0.8f;
    mat_diffuse[3] = 1.0f;

    mat_specular[0] = 1.0f;
    mat_specular[1] = 1.0f;
    mat_specular[2] = 1.0f;
    mat_specular[3] = 1.0f;

    high_shininess[0] = 100.0f;

    glClearColor(1,1,1,1);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    this->rotInc = 0;

    if (DEBUG_MODE)
        cout << "\nVimusMachineVitalino constructed.";
}

/**
 * VimusMachineVitalino destructor.
 */
VimusMachineVitalino::~VimusMachineVitalino()
{
}

/**
 * Update VimusMachineVitalino.
 */
void VimusMachineVitalino::update()
{
//	for (int i=0; i<VIDEO_WIDTH*VIDEO_HEIGHT*3; i++)
//	{
////	    if ((*this->ppInputData[0])[i] < (*this->massa1)[i])
////            (*this->massa1)[i] = (*this->ppInputData[0])[i];
////        if ((*this->ppInputData[1])[i] < (*this->massa2)[i])
////            (*this->massa2)[i] = (*this->ppInputData[1])[i];
//        (*this->massa1)[i] = (*this->ppInputData[0])[i];
//        (*this->massa2)[i] = (*this->ppInputData[1])[i];
//    }

    this->numPontos2 = 0;
	for (int j=0; j<VIDEO_HEIGHT; j++)
	{
	    for (int i=0; i<VIDEO_WIDTH; i++)
	    {
            if ((*this->ppInputData[0])[(j*VIDEO_HEIGHT+i)*3] == 0)
            {
                this->pontos2[this->numPontos2*2] = i;
                this->pontos2[this->numPontos2*2+1] = j;
                this->numPontos2++;
            }
        }
	}

    int corSensivel = 0;

	for (int i=0; i<this->numPontos2; i++)
	{
		for (int j=0; j<VIDEO_HEIGHT; j++)
		{
		    if ((*this->ppInputData[1])[(j*VIDEO_HEIGHT + this->pontos2[i*2])*3] == corSensivel)
		    {
		        this->barro [    this->pontos2[i*2]/4     ]
                            [    this->pontos2[i*2+1]/4   ]
                            [           j/4               ][0] = false;
                this->barro [    this->pontos2[i*2]/4     ]
                            [    this->pontos2[i*2+1]/4   ]
                            [           j/4               ][1] = true;
		    }
		}
	}

}

/**
 * Draw one voxel.
 */
void VimusMachineVitalino::drawVoxel(int razao, float adjustPos, float x, float y, float z, float color)
{
    glPushMatrix();
    glTranslatef(((float)x)/razao + adjustPos, -(((float)y)/razao + adjustPos), z/(float)razao + adjustPos);
    glColor3f(color,color,color);
    //glColor3f(1,1,1);
    glutSolidCube(0.018);
    glPopMatrix();
}

/**
 * Update VimusMachineVitalino.
 */
void VimusMachineVitalino::draw(int rendermode)
{
}

/**
 * Update VimusMachineVitalino.
 */
void VimusMachineVitalino::draw()
{

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);  //decrease FPS because do lot of extra calculations
                             //see http://www.opengl.org/resources/features/
                             //KilgardTechniques/oglpitfall/
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    glClearColor (0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    float adjustPos = -0.66f;

    int corSensivel = 0;

    float razao = 48.0f;

    int res = 1;

    int x, y, z;
    x = y = z = 0;

    glPushMatrix();

    this->rotInc += 0.05;
    if (this->rotInc > 1440) this->rotInc = 0;

    glRotatef(this->rotInc/4,1.0,0.0,0.0);
    glRotatef(this->rotInc,0.0,1.0,0.0);

    glDisable(GL_BLEND);
    glLineWidth (1);

    int contaVoxels = 0;

    for (x=0; x<BARRO_WIDTH; x+=res)
    {
        for (y=0; y<BARRO_HEIGHT; y+=res)
        {
            //varredura frente->tras
            z = 0;
            while (z<BARRO_DEPTH)
            {
                if (this->barro[x][y][z][0] == true || this->barro[x][y][z][1] == false)
                {
                    this->drawVoxel(razao, adjustPos, x, y, z, 1.0f);
                    this->barro[x][y][z][1] = true;
                    contaVoxels++;
                    break;
                }
                z+=res;
            }
            //varredura tras->frente
            z = BARRO_DEPTH-1;
            while (z>0)
            {
                if (this->barro[x][y][z][0] == true || this->barro[x][y][z][1] == false)
                {
                    this->drawVoxel(razao, adjustPos, x, y, z, 1.0f);
                    this->barro[x][y][z][1] = true;
                    contaVoxels++;
                    break;
                }
                z-=res;
            }
        }
        for (z=0; z<BARRO_DEPTH; z+=res)
        {
            //varredura superior->inferior
            y = 0;
            while (y<BARRO_HEIGHT)
            {
                if (this->barro[x][y][z][0] == true || this->barro[x][y][z][1] == false)
                {
                    this->drawVoxel(razao, adjustPos, x, y, z, 0.5f);
                    this->barro[x][y][z][1] = true;
                    contaVoxels++;
                    break;
                }
                y+=res;
            }
            //varredura inferior->superior
            y = BARRO_HEIGHT-1;
            while (y>0)
            {
                if (this->barro[x][y][z][0] == true || this->barro[x][y][z][1] == false)
                {
                    this->drawVoxel(razao, adjustPos, x, y, z, 0.5f);
                    this->barro[x][y][z][1] = true;
                    contaVoxels++;
                    break;
                }
                y-=res;
            }
        }
    }

    for (z=0; z<BARRO_WIDTH; z+=res)
    {
        for (y=0; y<BARRO_HEIGHT; y+=res)
        {
            //varredura esquerda->direita
            x = 0;
            while (x<BARRO_DEPTH)
            {
                if (this->barro[x][y][z][0] == true || this->barro[x][y][z][1] == false)
                {
                    this->drawVoxel(razao, adjustPos, x, y, z, 0.3f);
                    this->barro[x][y][z][1] = true;
                    contaVoxels++;
                    break;
                }
                x+=res;
            }
            //varredura direita->esquerda
            x = BARRO_DEPTH-1;
            while (x>0)
            {
                if (this->barro[x][y][z][0] == true || this->barro[x][y][z][1] == false)
                {
                    this->drawVoxel(razao, adjustPos, x, y, z, 0.3f);
                    this->barro[x][y][z][1] = true;
                    contaVoxels++;
                    break;
                }
                x-=res;
            }
        }
    }

    this->numVoxels = contaVoxels;

	if (this->numVoxels < 3000)
	{
        for (int i=0; i<BARRO_WIDTH; i++)
        {
            for (int j=0; j< BARRO_HEIGHT; j++)
            {
                for (int l=0; l < BARRO_DEPTH; l++)
                {
                    this->barro[i][j][l][0] = true;
                    this->barro[i][j][l][1] = false;
                }
            }
        }
        glClearColor(0,0,0,0);

        this->rotInc = 0;

//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//        glFlush();
//        glutSwapBuffers(); //comentado para evitar bug de swap quando aperta
                             //F5 no modo janela. mas não influencia no bug.

        this->numVoxels = BARRO_WIDTH*BARRO_HEIGHT*BARRO_DEPTH;
        boost::xtime_get(&(this->xtimeRecomecar), boost::TIME_UTC);
        this->xtimeRecomecar.nsec += 700000000;
        boost::thread::sleep(this->xtimeRecomecar);
	}

    glPopMatrix();


    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHT0);
    glDisable(GL_NORMALIZE);  //decrease FPS because do lot of extra calculations
                             //see http://www.opengl.org/resources/features/
                             //KilgardTechniques/oglpitfall/
    glDisable(GL_COLOR_MATERIAL);
    glDisable(GL_LIGHTING);

}

/**
 * Get pointer to currentFrame buffer pointer.
 * This is necessary for GUI have a direct access to
 * machine frame buffer.
 */
unsigned char** VimusMachineVitalino::getCurrentFramePointer()
{
	return this->ppOutputData;
}

/**
 * Do some action before and/or after connect a pin to
 * another objects pin.
 */
bool VimusMachineVitalino::connectOutput (int outPin, VimusMachineObject * dstObj, int inPin)
{
	return dstObj->connectInput(this, outPin, inPin);
}

/**
 * Do some action before and/or after be connected by another
 * objects pin.
 */
bool VimusMachineVitalino::connectInput (VimusMachineObject * srcObj, int outPin, int inPin)
{
	bool ret = false;
	if (srcObj->outputPins[outPin]->connect(this->inputPins[inPin]))
	{
		((VimusMachinePinVideo *) this->inputPins[inPin])->ppFrame =
			((VimusMachinePinVideo *) srcObj->outputPins[outPin])->ppFrame;
		this->ppInputData[inPin] = ((VimusMachinePinVideo *) this->inputPins[inPin])->ppFrame;
		ret = true;
	}
	return ret;
}

/**
 * Do some action before and/or after disconnect an output pin from
 * another objects pin.
 */
void VimusMachineVitalino::disconnectOutput (int outPin, VimusMachineObject * dstObj, int inPin)
{
	dstObj->disconnectInput(this, outPin, inPin);
}

/**
 * Do some action before and/or after disconnect an input pin from
 * another objects pin.
 */
void VimusMachineVitalino::disconnectInput (VimusMachineObject * srcObj, int outPin, int inPin)
{
	srcObj->outputPins[outPin]->disconnect(this->inputPins[inPin]);
	this->ppInputData[inPin] = this->ppNullFrame;
}

void VimusMachineVitalino::keyBoardFunc(unsigned char key, int x, int y)
{
}

void VimusMachineVitalino::specialKeyBoardFunc(int key, int x, int y)
{
}
