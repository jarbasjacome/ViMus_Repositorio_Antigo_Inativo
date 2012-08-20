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

#include "VimusMachineEditor.h"

using namespace std;

/**
 * VimusMachineEditor default constructor.
 */
VimusMachineEditor :: VimusMachineEditor (VideoCaptureOpenCV* vidCap, MyFreenectDevice* kin)
{
    this->videoCap = vidCap;
    this->kinect = kin;

	this->rootAbsObj = new VimusMachineAbstractionObject ("root");

	this->currentAbsObj = this->rootAbsObj;

	this->numOpenGLObjs = 0;
	for (int i=0; i<MAX_GL_OBJ; i++)
	{
		this->opengGLObjects[i] = NULL;
	}

    if (DEBUG_MODE)
        cout << "\nVimusMachineEditor constructed.";
}

/**
* VimusMachineEditor destructor.
*/
VimusMachineEditor :: ~VimusMachineEditor()
{
    if (opengGLObjects)
    {
        for (int i=0; i<MAX_GL_OBJ; i++)
        {
            if (opengGLObjects[i])
            {
                delete opengGLObjects[i];
                opengGLObjects[i] = NULL;
            }
        }
    }
}

/**
 * Creates a new machine object.
 */
VimusGUIObject * VimusMachineEditor :: createObject(const string& label, float posX,
												float posY, float posZ)
{
	VimusGUIObject * guiObj = NULL;

	VimusMachineObject * macObj = NULL;

	this->parseMsg (label);

	if (this->msgTokens.size() > 0)
	{
		if (!this->msgTokens[0].compare("abs"))
		{
			macObj = new VimusMachineAbstractionObject(label);
			guiObj = new VimusGUICube(label, posX, posY, posZ, false);
		}
		else if (!this->msgTokens[0].compare("video"))
		{
			macObj = new VimusMachineVideoPreview();
			guiObj = new VimusGUIVideoPreview(
							posX,
							posY,
							posZ,
							((VimusMachineVideoPreview *)
										macObj)->getCurrentFramePointer()
						);
		}
		else if (!this->msgTokens[0].compare("inlet"))
		{
			macObj = new VimusMachineNormalObject(label, 0, 1);
			guiObj = new VimusGUINormalObject(label, posX, posY, posZ, 0, 1);
		}
		else if (!this->msgTokens[0].compare("outlet"))
		{
			macObj = new VimusMachineNormalObject(label, 1, 0);
			guiObj = new VimusGUINormalObject(label, posX, posY, posZ, 1, 0);

		}
		else if (!this->msgTokens[0].compare("videocap"))
		{
		    //Create VimusMachineVideoCapture passing video capture manager
			macObj = new VimusMachineVideoCapture(this->videoCap);
			guiObj = new VimusGUINormalObject(label, posX, posY, posZ, 0, 1);
		}
		else if (!this->msgTokens[0].compare("audiocap"))
		{
			macObj = new VimusMachineNormalObject(label, 0, 1);
			guiObj = new VimusGUINormalObject(label, posX, posY, posZ, 0, 1);
		}
		else if (!this->msgTokens[0].compare("pixelAdd"))
		{
			macObj = new VimusMachinePixelAdd();
			guiObj = new VimusGUINormalObject(label, posX, posY, posZ, 2, 1);
		}
		else if (!this->msgTokens[0].compare("pixelCVThreshold"))
		{
			macObj = new VimusMachineCVThreshold();
			guiObj = new VimusGUINormalObject(label, posX, posY, posZ, 2, 1);
		}
		else if (!this->msgTokens[0].compare("pixelCVBlob"))
		{
		    VimusMachineCVBlobDetection * blob = new VimusMachineCVBlobDetection();
			macObj = blob;
			guiObj = new VimusGUIOpenGLObject(label, posX, posY, posZ, 2, 1, blob);

            this->addOpenGLObject(blob);
		}
		else if (!this->msgTokens[0].compare("mangue"))
		{
		    VimusMachineMangue * mangue = new VimusMachineMangue();
			macObj = mangue;
			guiObj = new VimusGUIOpenGLObject(label, posX, posY, posZ, 2, 1, mangue);

            this->addOpenGLObject(mangue);
		}
		else if (!this->msgTokens[0].compare("vitalino"))
		{
		    VimusMachineVitalino * vitalino = new VimusMachineVitalino();
			macObj = vitalino;
			guiObj = new VimusGUIOpenGLObject(label, posX, posY, posZ, 2, 1, vitalino);

			this->addOpenGLObject(vitalino);
		}
		else if (!this->msgTokens[0].compare("lanterna"))
		{
		    VimusMachineLanternaMagica * lanterna = new VimusMachineLanternaMagica();
			macObj = lanterna;
			guiObj = new VimusGUIOpenGLObject(label, posX, posY, posZ, 2, 1, lanterna);

			this->addOpenGLObject(lanterna);
		}
		else if (!this->msgTokens[0].compare("oriente"))
		{
		    VimusMachineOriente * oriente = new VimusMachineOriente(this->kinect);
			macObj = oriente;
			guiObj = new VimusGUIOpenGLObject(label, posX, posY, posZ, 1, 1, oriente);

			this->addOpenGLObject(oriente);
		} else if (!this->msgTokens[0].compare("coracoes"))
		{
		    VimusMachineCoracoes * coracoes = new VimusMachineCoracoes(this->kinect);
			macObj = coracoes;
			guiObj = new VimusGUIOpenGLObject(label, posX, posY, posZ, 1, 1, coracoes);

			this->addOpenGLObject(coracoes);
		}
        if (macObj)
			this->currentAbsObj->addObject(macObj);
	}

	return guiObj;
}

/**
 * Removes an object by index.
 */
void VimusMachineEditor :: removeObject(int i)
{
	int k;
	for (int j=0; j<currentAbsObj->objects[i]->numOutputs; j++)
	{
		k = currentAbsObj->objects[i]->outputPins[j]->numConnections - 1;
		while (currentAbsObj->objects[i]->outputPins[j]->numConnections>0)
		{
			disconnectPins(	i,
							j,
							currentAbsObj->objects[i]->
								outputPins[j]->connections[k]->parentIndex,
							currentAbsObj->objects[i]->
								outputPins[j]->connections[k]->indexOnParent);
			//TODO: catch exception.
			k--;
		}
	}
	for (int j=0; j<currentAbsObj->objects[i]->numInputs; j++)
	{
		k = currentAbsObj->objects[i]->inputPins[j]->numConnections - 1;
		while (currentAbsObj->objects[i]->inputPins[j]->numConnections>0)
		{
			disconnectPins( currentAbsObj->objects[i]->
								inputPins[j]->connections[k]->parentIndex,
							currentAbsObj->objects[i]->
								inputPins[j]->connections[k]->indexOnParent,
							i,
							j);
			//TODO: catch exception.
			k--;
		}
	}
	this->currentAbsObj->removeObject(i);
}

/**
 * Connect objects pins.
 */
bool VimusMachineEditor :: connectPins(int srcObj, int outPin, int dstObj, int inPin)
{
	return this->currentAbsObj->objects[srcObj]->connectOutput(
					outPin, this->currentAbsObj->objects[dstObj], inPin);
}

/**
 * Disconnect objects pins.
 */
void VimusMachineEditor :: disconnectPins(int srcObj, int outPin, int dstObj, int inPin)
{
	this->currentAbsObj->objects[srcObj]->disconnectOutput(
			outPin, this->currentAbsObj->objects[dstObj], inPin);
}

/**
 * Parse message text to tokens.
 */
void VimusMachineEditor :: parseMsg (const string& msg)
{
	msgTokens.clear();

	size_t found;
	int last_pos = -1;
	found=msg.find_first_of(" ");
	while (found!=string::npos)
	{
		msgTokens.push_back(msg.substr(last_pos+1, found-(last_pos+1)));
		last_pos = found;
		found=msg.find_first_of(" ",found+1);
	}
	msgTokens.push_back(msg.substr(last_pos+1, msg.size()-(last_pos+1)));
}

/**
 * Set currentAbsObj by index.
 */
void VimusMachineEditor :: setCurrentAbsObj (int objIndex)
{
	if (objIndex >= 0 && objIndex < this->currentAbsObj->numObjects)
	{
		if (currentAbsObj->objects[objIndex]->isAbstractionObject)
		{

			currentAbsObj = (VimusMachineAbstractionObject *)
											currentAbsObj->objects[objIndex];
		}
	}
}

/**
 * Set currentAbsObj by son's name.
 */
void VimusMachineEditor :: setCurrentAbsObj (const string& objName)
{
	bool found = false;
	int c = 0;
	while (found == false && c < this->currentAbsObj->numObjects)
	{
		if (!objName.compare(this->currentAbsObj->objects[c]->label))
		{
			setCurrentAbsObj(c);
			found = true;
		}
		c++;
	}
}

/**
 * Set currentAbsObj to be the current cube's parent cube.
 */
void VimusMachineEditor :: setCurrentAbsObjToParent ()
{
	if (this->currentAbsObj->parentAbsObj)
		this->currentAbsObj = (VimusMachineAbstractionObject *)
												this->currentAbsObj->parentAbsObj;
}

/**
 * Get root abstraction object.
 */
VimusMachineAbstractionObject * VimusMachineEditor ::getRootAbsObj ()
{
	return this->rootAbsObj;
}

/**
 * Adds an OpenGL machine object
 *
 * @return index of the added object in the machine editors array
 */
void VimusMachineEditor :: addOpenGLObject(VimusMachineOpenGLObject* obj)
{
    if (numOpenGLObjs < MAX_GL_OBJ)
    {
        opengGLObjects[numOpenGLObjs] = obj;
		numOpenGLObjs++;
    }
}

/**
 * Remove an OpenGL machine object
 */
void VimusMachineEditor :: removeOpenGLObject (int index)
{
    //TODO:
}

/**
 * Draws all OpenGL objects
 */
void VimusMachineEditor:: drawOpenGLObjects()
{
    for (int i=0; i<numOpenGLObjs; i++)
    {
        opengGLObjects[i]->draw();
    }
}
