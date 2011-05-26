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

#include "VimusMachineLanternaMagica.h"

using namespace std;
using namespace cv;

/**
 * VimusMachineLanternaMagica default constructor.
 */
VimusMachineLanternaMagica::VimusMachineLanternaMagica()
{
    VimusMachineObject::initObject();

	this->label = string ("lanterna");
	this->addInputPin(VimusMachinePin::TYPE_VIDEO);
	this->addInputPin(VimusMachinePin::TYPE_CONTROL);

	this->addOutputPin(VimusMachinePin::TYPE_VIDEO);

	this->ppNullFrame = new unsigned char*;
	(*this->ppNullFrame) = new unsigned char[VIDEO_WIDTH*VIDEO_HEIGHT*3];
	for (int i=0; i<VIDEO_WIDTH*VIDEO_HEIGHT*3; i++)
	{
		(*this->ppNullFrame)[i] = 0;
	}

	// do not assign another value to it.
	this->ppOutputData = new unsigned char*;
	(*this->ppOutputData) = new unsigned char[VIDEO_WIDTH*VIDEO_HEIGHT*3];
	for (int i=0; i<VIDEO_WIDTH*VIDEO_HEIGHT*3; i++)
	{
		(*this->ppOutputData)[i] = 0;
	}

	// set video output pin
	((VimusMachinePinVideo *)this->outputPins[0])->ppFrame = this->ppOutputData;

	// do not assign another value to it.
	this->ppInputData = new unsigned char*;
	(*this->ppInputData) = new unsigned char[VIDEO_WIDTH*VIDEO_HEIGHT*3];
	for (int i=0; i<VIDEO_WIDTH*VIDEO_HEIGHT*3; i++)
	{
		(*this->ppInputData)[i] = 0;
	}

	// set video input pin
	//((VimusMachinePinVideo *)this->inputPins[0])->ppFrame = this->ppInputData;

    if (DEBUG_MODE)
        cout << "\nVimusMachineLanternaMagica constructed.";

//    // win width divided by win height
//    float winXratio = (float) glutGet(GLUT_WINDOW_WIDTH)/
//                                (float) glutGet(GLUT_WINDOW_HEIGHT);
//    // TODO: plug
//    winXratio = 1.3333f;
//
//    mapaData[i] = mapaResized[i].data;
//

    this->capturedFrame = new unsigned char[VIDEO_WIDTH*VIDEO_HEIGHT*3];

	for (int i=0; i<VIDEO_WIDTH*VIDEO_HEIGHT; i++)
	{
        this->capturedFrame[i*3+1] = 255;
        this->capturedFrame[i*3] = 0;
        this->capturedFrame[i*3+2] = 0;
	}

	Mat red(VIDEO_WIDTH, VIDEO_HEIGHT, CV_8UC3, capturedFrame,0);

	this->frame = red.clone();
	this->frameDest = red.clone();

    glGenTextures(1, &texName);

    glBindTexture(GL_TEXTURE_2D, texName);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, VIDEO_WIDTH, VIDEO_HEIGHT, 0, GL_BGR,
                                GL_UNSIGNED_BYTE, this->capturedFrame);

    try
    {
        video[0] = VideoCapture("/dados/jabahpureza/video_samples/soul_of_the_funky_drummers_raw_i420_noaudio.avi");
        video[1] = VideoCapture("/dados/jabahpureza/video_samples/bootsy_01_raw_i420_noaudio.avi");
        video[2] = VideoCapture("/dados/jabahpureza/video_samples/cold_sweat_01_raw_i420_noaudio.avi");
        video[3] = VideoCapture("/dados/jabahpureza/video_samples/cold_sweat_02_raw_i420_noaudio.avi");
        video[4] = VideoCapture("/dados/jabahpureza/video_samples/do_it_to_death_01_raw_i420_noaudio.avi");
        video[5] = VideoCapture("/dados/jabahpureza/video_samples/funky_drummer_01_raw_i420_noaudio.avi");
        video[6] = VideoCapture("/dados/jabahpureza/video_samples/give_it_up_or_turnit_a_loose_01_raw_i420_noaudio.avi");
        video[7] = VideoCapture("/dados/jabahpureza/video_samples/give_it_up_or_turnit_a_loose_02_raw_i420_noaudio.avi");
        video[8] = VideoCapture("/dados/jabahpureza/video_samples/i_dont_want_nobody_to_give_me_nothing_01_raw_i420_noaudio.avi");
        video[9] = VideoCapture("/dados/jabahpureza/video_samples/i_dont_want_nobody_to_give_me_nothing_02_raw_i420_noaudio.avi");
        video[10] = VideoCapture("/dados/jabahpureza/video_samples/i_dont_want_nobody_to_give_me_nothing_03_raw_i420_noaudio.avi");
        video[11] = VideoCapture("/dados/jabahpureza/video_samples/i_feel_good_01_raw_i420_noaudio.avi");
        video[12] = VideoCapture("/dados/jabahpureza/video_samples/i_feel_good_02_raw_i420_noaudio.avi");
        video[13] = VideoCapture("/dados/jabahpureza/video_samples/i_got_a_feeling_01_raw_i420_noaudio.avi");
        video[14] = VideoCapture("/dados/jabahpureza/video_samples/licking_stick_01_raw_i420_noaudio.avi");
        video[15] = VideoCapture("/dados/jabahpureza/video_samples/licking_stick_02_raw_i420_noaudio.avi");
        video[16] = VideoCapture("/dados/jabahpureza/video_samples/make_it_funky_01_raw_i420_noaudio.avi");
        video[17] = VideoCapture("/dados/jabahpureza/video_samples/mother_popcorn_01_raw_i420_noaudio.avi");
        video[18] = VideoCapture("/dados/jabahpureza/video_samples/papa_dont_take_no_mess_01_raw_i420_noaudio.avi");
        video[19] = VideoCapture("/dados/jabahpureza/video_samples/sex_machine_01_raw_i420_noaudio.avi");
        video[20] = VideoCapture("/dados/jabahpureza/video_samples/super_bad_01_raw_i420_noaudio.avi");
        video[21] = VideoCapture("/dados/jabahpureza/video_samples/thank_you_raw_i420_noaudio.avi");
    }
    catch ( ... )
    {
        cout << "erro opencv videocapture";
    }

    currVideo = 3;

    timePast = 0;
    currFrame = 0;

    this->audioSampler = new OpenALSampler();

    pastTimeMSecs = 0;
    sampleStartTimeMSecs = 0;

    schedMeasureChange=-1; //-1 means no measure scheduled
    schedGrooveChange=-1;

    currMeasure = 0;
    repeatMode = this->REPEAT_MODE_GROOVE;

    for (int i=0; i<NUM_VIDEOS; i++)
    {
        for (int j=0; j<20; j++)
        {
            this->timeStamps[i][j] = -1;
        }
    }

//    int bootsy[] =    { 2980,    5400,   7806,	10232,	12621,	15046,	17471,
//                        19860,   22298,	24675,	27064,	29549,	31963,	34388,
//                        36777,   39202,	41664,	44065,	46478,	48903   };
    int bootsy[] =    { 2980,    7806,	12621,	17471,
                        22298,	27064,	31963,  36777,
                        41664,	46478,	51328};
    int coldsweat01[] = {50, 3902,   7771,   11605,  15423};
    int coldsweat02[] = {6,	3976,	7907,	11822,  15690};
    int doittodeath[] = {1118,	5395,	9671,	14022,	18323,	22723};//,	27123};
    int funkydrummer[]= { 	0,	4482,	8941,	13424,	17858,	22292};//	26726};
    int giveitup01[]=   {3215,	11355,	19555};
    int giveitup02[]=   {8427 ,	16528,	24687,	32873,	41035,	49082,	57073,  65071,  73001};
    int i_dont_want_nobody01[] = {  2748,	11183,	19637};
    int i_dont_want_nobody02[] = {	0,      8443,	16809,	25226};
    int i_dont_want_nobody03[] = {  0,      16802,	33639};
    int i_feel_good_01[] = { 6907,	10532,	14149,	17785};
    int i_feel_good_02[] = { 0,	7130,	14153 };
    int i_got_a_feeling_01[]	= { 0,	7613,	15227,	22862};
    int licking_stick_01[] = { 12767,	22264,	31684,	41216};
    int licking_stick_02[] = { 0,	10668,	20146};
    int make_it_funky_01[] = { 3840,	13321,	25703,	35200};
    int mother_popcorn_01[] = { 51,	8465,	16777,	25095};
    int papa_dont_take_no_mess[]	= { 0,	4662,	9549,	13907,	18494};
    int sex_machine_01[] = { 2654,	4752,	13364,	22065 };
    int super_bad_01[] = { 69,	7179,	14129,	21228,	28362,	31889,	39083,	46176};

    for (int i=0; i<11; i++)
    {
        if (i<11) this->timeStamps[1][i] = bootsy[i];
        if (i<5) this->timeStamps[2][i] = coldsweat01[i];
        if (i<5) this->timeStamps[3][i] = coldsweat02[i];
        if (i<6) this->timeStamps[4][i] = doittodeath[i];
        if (i<6) this->timeStamps[5][i] = funkydrummer[i];
        if (i<3) this->timeStamps[6][i] = giveitup01[i];
        if (i<9) this->timeStamps[7][i] = giveitup02[i];
        if (i<3) this->timeStamps[8][i] = i_dont_want_nobody01[i];
        if (i<4) this->timeStamps[9][i] = i_dont_want_nobody02[i];
        if (i<3) this->timeStamps[10][i] = i_dont_want_nobody03[i];
        if (i<4) this->timeStamps[11][i] = i_feel_good_01[i];
        if (i<3) this->timeStamps[12][i] = i_feel_good_02[i];
        if (i<4) this->timeStamps[13][i] = i_got_a_feeling_01[i];
        if (i<4) this->timeStamps[14][i] = licking_stick_01[i];
        if (i<3) this->timeStamps[15][i] = licking_stick_02[i];
        if (i<4) this->timeStamps[16][i] = make_it_funky_01[i];
        if (i<4) this->timeStamps[17][i] = mother_popcorn_01[i];
        if (i<5) this->timeStamps[18][i] = papa_dont_take_no_mess[i];
        if (i<4) this->timeStamps[19][i] = sex_machine_01[i];
        if (i<8) this->timeStamps[20][i] = super_bad_01[i];
    }

    videoPlaying = true;

    this->audioSampler->playSample(currVideo);
    boost::xtime_get(&(this->startSysTime2), boost::TIME_UTC);
}

/**
 * VimusMachineLanternaMagica destructor.
 */
VimusMachineLanternaMagica::~VimusMachineLanternaMagica()
{
    delete(this->audioSampler);
}

/**
 * Update VimusMachineLanternaMagica.
 */
void VimusMachineLanternaMagica::update()
{

    try
    {
        boost::xtime_get(&(this->currSysTime2), boost::TIME_UTC);
        this->pastTimeMSecs = (this->currSysTime2.nsec - this->startSysTime2.nsec) / 1000000.0f;
        this->pastTimeMSecs += (this->currSysTime2.sec - this->startSysTime2.sec)*1000;

        this->pastTimeMSecs += this->sampleStartTimeMSecs;

        if (this->pastTimeMSecs > this->timeStamps[currVideo][currMeasure+1] &&
            this->timeStamps[currVideo][currMeasure+1] > -1)
        {
            currMeasure++;
            cout << "\n\n\n\n\n\n\n passou compasso " << currMeasure;
            if (this->repeatMode == this->REPEAT_MODE_MEASURE)
            {
                if (this->schedMeasureChange > -1)
                {
                    currMeasure = schedMeasureChange;
                    schedMeasureChange = -1;
                }
                else
                {
                    currMeasure--;
                }
                this->sampleStartTimeMSecs = this->timeStamps[currVideo][currMeasure];
                this->video[currVideo].set(CV_CAP_PROP_POS_MSEC, this->sampleStartTimeMSecs);
                boost::xtime_get(&(this->startSysTime2), boost::TIME_UTC);
                this->audioSampler->setPlaybackPos(currVideo, this->sampleStartTimeMSecs/1000.0f);
            }
            // verifica se está no repeat_mode_groove e se o compasso atual é o último
            else if (this->repeatMode == this->REPEAT_MODE_GROOVE && this->timeStamps[currVideo][currMeasure+1] == -1)
            {
                if (this->schedGrooveChange > -1)
                {
                    this->audioSampler->stopSample(currVideo);
                    this->currVideo=this->schedGrooveChange;
                    this->schedGrooveChange = -1;
                    this->audioSampler->playSample(this->currVideo);
                }
                currMeasure = 0;
                this->sampleStartTimeMSecs = this->timeStamps[currVideo][currMeasure];
                this->video[currVideo].set(CV_CAP_PROP_POS_MSEC, this->sampleStartTimeMSecs);
                boost::xtime_get(&(this->startSysTime2), boost::TIME_UTC);
                this->audioSampler->setPlaybackPos(currVideo, this->sampleStartTimeMSecs/1000.0f);
            }
            else
            {
                this->video[currVideo].set(CV_CAP_PROP_POS_MSEC, this->pastTimeMSecs);
            }
        }
        else
        {
            this->video[currVideo].set(CV_CAP_PROP_POS_MSEC, this->pastTimeMSecs);
        }

        if (video[currVideo].grab() && videoPlaying)
        {
            video[currVideo].retrieve(this->frame, 0);
            this->capturedFrame = (unsigned char *) this->frame.data;
        }
        else
        {
            videoPlaying = false;
//            this->currMeasure=0;
//            this->sampleStartTimeMSecs = 0;
//            this->video[currVideo].set(CV_CAP_PROP_POS_MSEC, 0);
//            currFrame = 0;
//            boost::xtime_get(&(this->startSysTime2), boost::TIME_UTC);
//            this->audioSampler->playSample(currVideo);
        }
    }
    catch ( ... )
    {
        cout << "\nOpenCV retrieve error!\n";
    }

}

/**
 * Draw VimusMachineLanternaMagica.
 */
void VimusMachineLanternaMagica::draw()
{
    glPushMatrix();

    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();

    gluLookAt (0, 0, 10.0f, 0, 0, 10.0f - 5.0f, 0.0, 1.0, 0.0);

	glDisable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);

    //glColor4f (1.0f, 0.0f, 0.0f, 1.0f);
    glColor4f (1.0f, 1.0f, 1.0f, 1.0f);

    glBindTexture(GL_TEXTURE_2D, texName);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, VIDEO_WIDTH, VIDEO_HEIGHT, 0, GL_BGR,
                                GL_UNSIGNED_BYTE, this->capturedFrame);

    glScalef(0.8,1,1);

    glBegin(GL_QUADS);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,-1.0f, 0.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,-1.0f, 0.0f);
    glEnd();

	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

    glPopMatrix();
}


/**
 * Draw VimusMachineLanternaMagica.
 */
void VimusMachineLanternaMagica::draw(int rendermode)
{
}

/**
 * Get pointer to currentFrame buffer pointer.
 * This is necessary for GUI have a direct access to
 * machine frame buffer.
 */
unsigned char** VimusMachineLanternaMagica::getCurrentFramePointer()
{
	return this->ppOutputData;
}

/**
 * Do some action before and/or after connect a pin to
 * another objects pin.
 */
bool VimusMachineLanternaMagica::connectOutput (int outPin, VimusMachineObject * dstObj, int inPin)
{
	return dstObj->connectInput(this, outPin, inPin);
}

/**
 * Do some action before and/or after be connected by another
 * objects pin.
 */
bool VimusMachineLanternaMagica::connectInput (VimusMachineObject * srcObj, int outPin, int inPin)
{
	bool ret = false;
	if (srcObj->outputPins[outPin]->connect(this->inputPins[inPin]))
	{
		((VimusMachinePinVideo *) this->inputPins[inPin])->ppFrame =
			((VimusMachinePinVideo *) srcObj->outputPins[outPin])->ppFrame;
		this->ppInputData = ((VimusMachinePinVideo *) this->inputPins[inPin])->ppFrame;
		ret = true;
	}
	return ret;
}

/**
 * Do some action before and/or after disconnect an output pin from
 * another objects pin.
 */
void VimusMachineLanternaMagica::disconnectOutput (int outPin, VimusMachineObject * dstObj, int inPin)
{
	dstObj->disconnectInput(this, outPin, inPin);
}

/**
 * Do some action before and/or after disconnect an input pin from
 * another objects pin.
 */
void VimusMachineLanternaMagica::disconnectInput (VimusMachineObject * srcObj, int outPin, int inPin)
{
	srcObj->outputPins[outPin]->disconnect(this->inputPins[inPin]);
	this->ppInputData = this->ppNullFrame;
}

/**
 * TODO: PUT THIS OUT OF THIS OBJECT!
 * Renders a bitmap string.
 */
void VimusMachineLanternaMagica::renderBitmapString(
		float x,
		float y,
		float z,
		void *font,
		ostringstream * strStream)
{
    string str;
    str = strStream->str();
    glRasterPos3f(x, y, z);
    for (int i=0; i<(int)str.size(); i++)
    {
        glutBitmapCharacter(font, str.at(i));
    }
}

void VimusMachineLanternaMagica::keyBoardFunc(unsigned char key, int x, int y)
{
	switch (key)
	{
        case 'a':
            break;
        case 'j':
            if (this->repeatMode < this->NUM_OF_REPEAT_MODES-1)
            {
                this->repeatMode++;
            }
            else
            {
                this->repeatMode = 0;
            }
            this->schedGrooveChange=-1;
            this->schedMeasureChange=-1;
            break;
        case 13:
            if (this->repeatMode == this->REPEAT_MODE_OFF)
                this->prevTrack();
            else if (this->repeatMode == this->REPEAT_MODE_GROOVE)
            {
                if (this->schedGrooveChange == -1)
                {
                    this->schedGrooveChange = this->currVideo;
                }
                if (this->schedGrooveChange > 0)
                {
                    this->schedGrooveChange--;
                }
                else
                {
                    this->schedGrooveChange = this->NUM_VIDEOS-1;
                }
            }
            else if (this->repeatMode == this->REPEAT_MODE_MEASURE)
            {
                if (this->schedMeasureChange == -1)
                {
                    if (this->currMeasure > 0)
                    {
                        this->schedMeasureChange = this->currMeasure - 1;
                    }
                }
                else
                {
                    if (this->schedMeasureChange > 0 )
                    {
                        this->schedMeasureChange--;
                    }
                }
            }
            break;
        case '5':
            if (this->repeatMode == this->REPEAT_MODE_OFF)
                this->nextTrack();
            else if (this->repeatMode == this->REPEAT_MODE_GROOVE)
            {
                if (this->schedGrooveChange == -1)
                {
                    this->schedGrooveChange = this->currVideo;
                }
                if (this->schedGrooveChange < this->NUM_VIDEOS-1)
                {
                    this->schedGrooveChange++;
                }
                else
                {
                    this->schedGrooveChange = 0;
                }
            }
            else if (this->repeatMode == this->REPEAT_MODE_MEASURE)
            {
                if (this->schedMeasureChange == -1)
                {
                    if (this->timeStamps[currVideo][currMeasure+2] > -1)
                    {
                        this->schedMeasureChange = this->currMeasure + 1;
                    }
                }
                else
                {
                    if (this->timeStamps[currVideo][schedMeasureChange+2] > -1 )
                    {
                        this->schedMeasureChange++;
                    }
                }
            }
            break;
	}
}

void VimusMachineLanternaMagica::specialKeyBoardFunc(int key, int x, int y)
{
	switch (key)
	{
		case GLUT_KEY_F1:
           	break;
		case GLUT_KEY_F2:
			break;
	}
}

void VimusMachineLanternaMagica::prevTrack()
{
    this->audioSampler->stopSample(currVideo);
    if (currVideo > 0)
        currVideo--;
    else
        currVideo = NUM_VIDEOS-1;
    this->playCurrVideo();
}

void VimusMachineLanternaMagica::nextTrack()
{
    this->audioSampler->stopSample(currVideo);
    if (currVideo < NUM_VIDEOS-1)
        currVideo++;
    else
        currVideo = 0;
    this->playCurrVideo();
}

void VimusMachineLanternaMagica::playCurrVideo()
{
    this->schedGrooveChange=-1;
    this->schedMeasureChange=-1;
    this->currMeasure=0;
    this->videoPlaying = true;
    boost::xtime_get(&(this->startSysTime2), boost::TIME_UTC);
    this->sampleStartTimeMSecs = 0;
    this->audioSampler->playSample(currVideo);
}
