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

    this->capturedFrame = new unsigned char[VIDEO_SAMPLE_WIDTH*VIDEO_SAMPLE_HEIGHT*3];

    this->distorcedFrame = new unsigned char[VIDEO_SAMPLE_WIDTH*VIDEO_SAMPLE_HEIGHT*3];

	for (int i=0; i<VIDEO_SAMPLE_WIDTH*VIDEO_SAMPLE_HEIGHT; i++)
	{
        this->capturedFrame[i*3+0] = 255;
        this->capturedFrame[i*3+1] = 0;
        this->capturedFrame[i*3+2] = 0;

        this->distorcedFrame[i*3] = 0;      //B
        this->distorcedFrame[i*3+1] = 0;    //G
        this->distorcedFrame[i*3+2] = 255;  //R
	}

	Mat red(VIDEO_SAMPLE_WIDTH, VIDEO_SAMPLE_HEIGHT, CV_8UC3, capturedFrame,0);

	this->frame = red.clone();
	this->frameDest = red.clone();

    glGenTextures(1, &texName);

    glBindTexture(GL_TEXTURE_2D, texName);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, VIDEO_SAMPLE_WIDTH, VIDEO_SAMPLE_HEIGHT, 0, GL_BGR,
                                GL_UNSIGNED_BYTE, this->distorcedFrame);
    try
    {
        video[0] = VideoCapture("/dados/data/jabahpureza/video_samples/dear_friends_raw_i420_noaudio.avi");
        video[1] = VideoCapture("/dados/data/jabahpureza/video_samples/soul_of_the_funky_drummers_raw_i420_noaudio.avi");
        video[2] = VideoCapture("/dados/data/jabahpureza/video_samples/cold_sweat_01_raw_i420_noaudio.avi");
        video[3] = VideoCapture("/dados/data/jabahpureza/video_samples/funky_drummer_01_raw_i420_noaudio.avi");
        video[4] = VideoCapture("/dados/data/jabahpureza/video_samples/do_it_to_death_01_raw_i420_noaudio.avi");
        video[5] = VideoCapture("/dados/data/jabahpureza/video_samples/make_it_funky_01_raw_i420_noaudio.avi");
        video[6] = VideoCapture("/dados/data/jabahpureza/video_samples/tony_1.avi");
        video[7] = VideoCapture("/dados/data/jabahpureza/video_samples/tony_2.avi");
        video[8] = VideoCapture("/dados/data/jabahpureza/video_samples/tony_3.avi");
        video[9] = VideoCapture("/dados/data/jabahpureza/video_samples/tony_4.avi");
        video[10] = VideoCapture("/dados/data/jabahpureza/video_samples/tony_5.avi");
        video[11] = VideoCapture("/dados/data/jabahpureza/video_samples/tony_6.avi");
        video[12] = VideoCapture("/dados/data/jabahpureza/video_samples/tony_7.avi");
        video[13] = VideoCapture("/dados/data/jabahpureza/video_samples/tony_8.avi");
    }
    catch ( ... )
    {
        cout << "erro opencv videocapture";
    }

    timePast = 0;
    currFrame = 0;

    this->audioSampler = new OpenALSampler();

    setCurrVideo(0);

    pastTimeMSecs = 0;
    sampleStartTimeMSecs = 0;

    schedMeasureChange=-1; //-1 means no measure scheduled
    schedGrooveChange=-1;

    currMeasure = 0;
    repeatMode = this->REPEAT_MODE_OFF;

    for (int i=0; i<NUM_VIDEOS; i++)
    {
        for (int j=0; j<20; j++)
        {
            this->timeStamps[i][j] = -1;
        }
    }

    int tony_1[] = { 2566,   12472,  22284,  32151,  42071};
    int tony_2[] = {13192,   23529,  33733,  43846,  53871};
    int tony_3[] = { 9796,   19899,  29971,  40054,  50311,  57911};
    int tony_4[] = { 4712,   14708,  24635,  34656,  44661,  54630};
    int tony_5[] = { 2566,   12472,  22284,  32151,  42071};
    int tony_6[] = { 7636,   18455,  28995,  39490,  49958};
    int tony_7[] = { 3557,   14512,  25289};
    int tony_8[] = { 5281,   15262,  25317,  35393,  45468,  55581,  65757};

    int coldsweat01[] = {50, 3902,   7771,   11605,  15400};
    int doittodeath[] = {1118,	5395,	9671,	14022,	18323,	22723};//,	27123};
    int funkydrummer[]= {43,	4515,	8967,	13440,	17893,	22314};//{0,	4482,	8941,	13424,	17858,	22292};//	26726};
    int make_it_funky_01[] = {3834,	13344,	25723,	35200};

    for (int i=0; i<11; i++)
    {
        if (i<5) this->timeStamps[2][i] = coldsweat01[i];
        if (i<6) this->timeStamps[3][i] = funkydrummer[i];
        if (i<6) this->timeStamps[4][i] = doittodeath[i];
        if (i<4) this->timeStamps[5][i] = make_it_funky_01[i];
        if (i<5) this->timeStamps[6][i] = tony_1[i];
        if (i<5) this->timeStamps[7][i] = tony_2[i];
        if (i<6) this->timeStamps[8][i] = tony_3[i];
        if (i<6) this->timeStamps[9][i] = tony_4[i];
        if (i<5) this->timeStamps[10][i] = tony_5[i];
        if (i<5) this->timeStamps[11][i] = tony_6[i];
        if (i<3) this->timeStamps[12][i] = tony_7[i];
        if (i<7) this->timeStamps[13][i] = tony_8[i];
    }

    videoPlaying = false;

    currPitch = 1.0;

    this->audioCapture = new OpenALCapture();

//    this->audioSampler->playSample(currVideo);
//    boost::xtime_get(&(this->startSysTime2), boost::TIME_UTC);

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
    this->audioCapture->grabSamples();
    try
    {
//        boost::xtime_get(&(this->currSysTime2), boost::TIME_UTC);
//        this->pastTimeMSecs = (this->currSysTime2.nsec - this->startSysTime2.nsec) / 1000000.0f;
//        this->pastTimeMSecs += (this->currSysTime2.sec - this->startSysTime2.sec)*1000;

//        this->pastTimeMSecs += this->sampleStartTimeMSecs;

        //se sampler de audio ainda está tocando
        //fazer atualizações no vídeo de acordo com o tempo do áudio
        //o tempo do audio controla o video, por isso a mudança de pitch
        //altera o tempo do vídeo para manter a sincronia.
        if (this->audioSampler->getSecondOffset(currVideo) > 0) {

            this->pastTimeMSecs = this->audioSampler->getSecondOffset(currVideo)*1000;

            if (this->pastTimeMSecs > this->timeStamps[currVideo][currMeasure+1] &&
                this->timeStamps[currVideo][currMeasure+1] > -1)
            {
                currMeasure++;
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
                        this->setCurrVideo(this->schedGrooveChange);
                        this->schedGrooveChange = -1;
                        this->audioSampler->playSample(this->currVideo);
                        currPitch = 1.0;
                        this->audioSampler->setSamplePitch(currVideo, currPitch);
                    }
                    currMeasure = 0;
                    this->sampleStartTimeMSecs = this->timeStamps[currVideo][currMeasure];
                    this->video[currVideo].set(CV_CAP_PROP_POS_MSEC, this->sampleStartTimeMSecs);
                    boost::xtime_get(&(this->startSysTime2), boost::TIME_UTC);
                    this->audioSampler->setPlaybackPos(currVideo, this->sampleStartTimeMSecs/1000.0f);
                }
                else
                {
                    //this->video[currVideo].set(CV_CAP_PROP_POS_MSEC, this->pastTimeMSecs);
                    this->video[currVideo].set(CV_CAP_PROP_POS_MSEC, this->audioSampler->getSecondOffset(currVideo)*1000);
                }
            }
            else
            {
                //this->video[currVideo].set(CV_CAP_PROP_POS_MSEC, this->pastTimeMSecs);
                this->video[currVideo].set(CV_CAP_PROP_POS_MSEC, this->audioSampler->getSecondOffset(currVideo)*1000);
            }
        }

        if (video[currVideo].grab() && videoPlaying)
        {
            video[currVideo].retrieve(this->frame, 0);

            //TODO: if same size of vide dont need to do this!
//            resize(this->frame, this->frameDest, Size(VIDEO_SAMPLE_WIDTH, VIDEO_SAMPLE_HEIGHT),0,0, INTER_LINEAR);
//            this->capturedFrame = (unsigned char *) this->frameDest.data;

            this->capturedFrame = (unsigned char *) this->frame.data;
        }
        else
        {
            videoPlaying = false;
        }

        switch (this->videoEffect)
        {
            case VIDEO_EFFECT_OFF:
                break;
            case VIDEO_EFFECT_RED:
                for (int i=0; i<VIDEO_SAMPLE_HEIGHT; i++)
                {
                    for (int j=0; j<VIDEO_SAMPLE_WIDTH;j++)
                    {
                        this->distorcedFrame[(i*VIDEO_SAMPLE_WIDTH+j)*3] = this->capturedFrame[(i*VIDEO_SAMPLE_WIDTH+j)*3]+this->audioCapture->getSample(i);
                        this->distorcedFrame[(i*VIDEO_SAMPLE_WIDTH+j)*3+1] = this->capturedFrame[(i*VIDEO_SAMPLE_WIDTH+j)*3+1]+this->audioCapture->getSample(i);
                        this->distorcedFrame[(i*VIDEO_SAMPLE_WIDTH+j)*3+2] = this->capturedFrame[(i*VIDEO_SAMPLE_WIDTH+j)*3+2]+this->audioCapture->getSample(i)*255;
                    }
                }
                break;
//            case VIDEO_EFFECT_BRIGHTNESS:
//                float vol;
//                vol = this->audioCapture->getSoftAmp()*8;
//                if (vol > 1.0f) vol = 1.0f;
//                for (int i=0; i<VIDEO_SAMPLE_HEIGHT; i++)
//                {
//                    for (int j=0; j<VIDEO_SAMPLE_WIDTH;j++)
//                    {
//                        this->distorcedFrame[(i*VIDEO_SAMPLE_WIDTH+j)*3] = this->capturedFrame[(i*VIDEO_SAMPLE_WIDTH+j)*3]*vol;
//                        this->distorcedFrame[(i*VIDEO_SAMPLE_WIDTH+j)*3+1] = this->capturedFrame[(i*VIDEO_SAMPLE_WIDTH+j)*3+1]*vol;
//                        this->distorcedFrame[(i*VIDEO_SAMPLE_WIDTH+j)*3+2] = this->capturedFrame[(i*VIDEO_SAMPLE_WIDTH+j)*3+2]*vol;
//                    }
//                }
//                this->audioSampler->setGain(currVideo, vol);
//                break;
//            case VIDEO_EFFECT_BRIGHTNESS_INV:
//                float vol2;
//                vol2 = 1.0f - this->audioCapture->getSoftAmp()*4;
//                if (vol2 < 0.0f) vol2 = 0.0f;
//                for (int i=0; i<VIDEO_SAMPLE_HEIGHT; i++)
//                {
//                    for (int j=0; j<VIDEO_SAMPLE_WIDTH;j++)
//                    {
//                        this->distorcedFrame[(i*VIDEO_SAMPLE_WIDTH+j)*3] = this->capturedFrame[(i*VIDEO_SAMPLE_WIDTH+j)*3]*vol2;
//                        this->distorcedFrame[(i*VIDEO_SAMPLE_WIDTH+j)*3+1] = this->capturedFrame[(i*VIDEO_SAMPLE_WIDTH+j)*3+1]*vol2;
//                        this->distorcedFrame[(i*VIDEO_SAMPLE_WIDTH+j)*3+2] = this->capturedFrame[(i*VIDEO_SAMPLE_WIDTH+j)*3+2]*vol2;
//                    }
//                }
//                this->audioSampler->setGain(currVideo, vol2);
//                break;
            case VIDEO_EFFECT_CONTRAST_GAIN:
                float vol;
                vol = this->audioCapture->getSoftAmp();
                this->changeContrast(0, 0.7-vol*0.7);
                vol = 1.0 - vol*8;
                if (vol < 0) vol = 0;
                this->audioSampler->setGain(currVideo, vol);
                break;
            case VIDEO_EFFECT_CONTRAST_GAIN_INV:
                float vol2;
                vol2 = this->audioCapture->getSoftAmp();
                this->changeContrast(0, 0.7-vol2*0.7);
                this->audioSampler->setGain(currVideo, vol2*8);
                break;
            case VIDEO_EFFECT_CONTRAST_TEMPO:
                float vol3;
                vol3 = this->audioCapture->getSoftAmp();
                this->changeContrast(0, 0.7-vol3*0.7);
                vol3 = 1.0 - vol3*8;
                if (vol3 < 0) vol3 = 0;
                this->audioSampler->setSamplePitch(currVideo, vol3);
                break;
            case VIDEO_EFFECT_CONTRAST_TEMPO_INV:
                float vol4;
                vol4 = this->audioCapture->getSoftAmp();
                this->changeContrast(0, 0.7-vol4*0.7);
                this->audioSampler->setSamplePitch(currVideo, vol4*8);
                break;
            case VIDEO_EFFECT_WAVE:
                int zara;
                int antizara;
                for (int i=0; i<VIDEO_SAMPLE_HEIGHT; i++)
                {
                    zara = this->audioCapture->getSample(i)*VIDEO_SAMPLE_WIDTH*0.1;
                    antizara = VIDEO_SAMPLE_WIDTH - 1 - zara;
                    for (int j=0; j<antizara;j++)
                    {
                        this->distorcedFrame[(i*VIDEO_SAMPLE_WIDTH+j)*3] = this->capturedFrame[(i*VIDEO_SAMPLE_WIDTH+j+zara)*3];
                        this->distorcedFrame[(i*VIDEO_SAMPLE_WIDTH+j)*3+1] = this->capturedFrame[(i*VIDEO_SAMPLE_WIDTH+j+zara)*3+1];
                        this->distorcedFrame[(i*VIDEO_SAMPLE_WIDTH+j)*3+2] = this->capturedFrame[(i*VIDEO_SAMPLE_WIDTH+j+zara)*3+2];
                    }
                    for (int j=antizara; j<VIDEO_SAMPLE_WIDTH;j++)
                    {
                        this->distorcedFrame[(i*VIDEO_SAMPLE_WIDTH+j)*3] = this->capturedFrame[(i*VIDEO_SAMPLE_WIDTH+j-antizara)*3];
                        this->distorcedFrame[(i*VIDEO_SAMPLE_WIDTH+j)*3+1] = this->capturedFrame[(i*VIDEO_SAMPLE_WIDTH+j-antizara)*3+1];
                        this->distorcedFrame[(i*VIDEO_SAMPLE_WIDTH+j)*3+2] = this->capturedFrame[(i*VIDEO_SAMPLE_WIDTH+j-antizara)*3+2];
                    }
                }
                break;
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

    glClearColor (0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glScalef(1,1.333f,1); //TODO: INVERT SCREEN DISTORTION

    if (this->videoPlaying)
    {
        glDisable(GL_BLEND);
        glEnable(GL_TEXTURE_2D);

        glColor4f (1.0f, 1.0f, 1.0f, 1.0f);

        glBindTexture(GL_TEXTURE_2D, texName);

        if (this->videoEffect == VIDEO_EFFECT_OFF)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, VIDEO_SAMPLE_WIDTH, VIDEO_SAMPLE_HEIGHT, 0, GL_BGR,
                                    GL_UNSIGNED_BYTE, this->capturedFrame);
        }
        else
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, VIDEO_SAMPLE_WIDTH, VIDEO_SAMPLE_HEIGHT, 0, GL_BGR,
                                    GL_UNSIGNED_BYTE, this->distorcedFrame);
        }

        glPushMatrix();

        glScalef(1,0.8,1); //TODO: INVERT VIDEO TO SQUARE DISTORTION

        glBegin(GL_QUADS);
            glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, 1.0f, 0.0f);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 0.0f);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,-1.0f, 0.0f);
            glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,-1.0f, 0.0f);
        glEnd();

        glPopMatrix();

    }

//    ostringstream * strStream = new ostringstream;
//    strStream->clear();
//    (*strStream) << "getSample(0) = " << this->audioCapture->getSample(0);
//    this->renderBitmapString(-1.12,0,1,GLUT_BITMAP_HELVETICA_10, strStream);
//
//    ostringstream * strStream2 = new ostringstream;
//    strStream2->clear();
//    (*strStream2) << "getSoftAmp() = " << this->audioCapture->getSoftAmp();
//    this->renderBitmapString(-1.13,-0.04,1,GLUT_BITMAP_HELVETICA_10, strStream2);

    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);

    float posX1 = 0;
    float posX2 = 0;
    float posY1 = 0;
    float posY2 = 0;
    float dist = 0;
    float distTemp = 0.0;
    posX1 = - 1.0;
    float sumX = 4/(float)(OpenALCapture::BUFFER_SIZE/2.0f);
    posX2 = - 1.0 + sumX;

    float radius;
    float tempRadius;
    float angle;
    float posZ = 0.0f;


    glPushMatrix();

    switch (this->waveDrawer)
    {
        case WAVE_DRAWER_OFF:
            break;
        case WAVE_DRAWER_LINES:
            glColor4f(1.0f, 1.0f, 1.0f, this->audioCapture->getSample(0)*4);
            glLineWidth(2.0f);
            glBegin(GL_LINES);
                dist = 0.15;
                for(int i = 0; i < OpenALCapture::BUFFER_SIZE/2.0f - 1; i+=2)
                {
                    posY1 = this->audioCapture->getSample(i);
                    posY2 = this->audioCapture->getSample(i+2);
                    glVertex2f(posX1, posY1);
                    glVertex2f(posX2, posY2);
                    distTemp = 0.0;
                    for (int i=0; i<10; i++)
                    {
                        distTemp += dist;
                        glVertex2f(posX1, posY1 + distTemp);
                        glVertex2f(posX2, posY2 + distTemp);
                        glVertex2f(posX1, posY1 -distTemp);
                        glVertex2f(posX2, posY2 -distTemp);
                    }
                    posX1 += sumX;
                    posX2 += sumX;
                }
            glEnd();
            break;
		case WAVE_DRAWER_LINES_3D:
            glColor4f(1.0f, 1.0f, 1.0f, this->audioCapture->getSample(0)*4);
            glLineWidth(2.0f);
            glBegin(GL_LINES);
                dist = 1.0;
                for(int i = 0; i < OpenALCapture::BUFFER_SIZE/2.0f - 1; i+=2)
                {
                    posY1 = this->audioCapture->getSample(i);
                    posY2 = this->audioCapture->getSample(i+2);
                    glVertex3f(posX1, posY1, 0.0f);
                    glVertex3f(posX2, posY2, 0.0f);
                    distTemp = 0.0;
                    for (int i=0; i<10; i++)
                    {
                        distTemp += dist;
                        glVertex3f(posX1, posY1, distTemp);
                        glVertex3f(posX2, posY2, distTemp);
                        glVertex3f(posX1, posY1, -distTemp);
                        glVertex3f(posX2, posY2, -distTemp);
                    }
                    posX1 += sumX;
                    posX2 += sumX;
                }
            glEnd();
            break;

		case WAVE_DRAWER_CIRCLES:

            glColor4f(1.0f, 1.0f, 1.0f, this->audioCapture->getSoftAmp()*2);

            radius = 0.1f;
            tempRadius = 0.1f;

            for (int j = 0; j < 20; j++)
            {
                glBegin(GL_LINE_LOOP);
                for(int i = 0; i < OpenALCapture::BUFFER_SIZE/2.0f; i++)
                {
                    tempRadius = radius + this->audioCapture->getSample(i);
                    angle = i*2*M_PI/(OpenALCapture::BUFFER_SIZE/2.0f);
                    glVertex2f((sin(angle) * tempRadius), -(cos(angle) * tempRadius));
                }
                glEnd();
                radius += 0.1;
            }

            break;

		case WAVE_DRAWER_CIRCLES_3D:
            glColor4f(1.0f, 1.0f, 1.0f, this->audioCapture->getSoftAmp()*2);

            radius = 0.1f;
            tempRadius = 0.1f;
            dist = 0.5f;
            posZ = 0.0f;

            glLineWidth(2.0f);

            for (int j = 0; j < 20; j++)
            {
                glBegin(GL_LINE_LOOP);
                for(int i = 0; i < OpenALCapture::BUFFER_SIZE/2.0f; i++)
                {
                    tempRadius = radius + this->audioCapture->getSample(i)*0.5;
                    angle = i*2*M_PI/(OpenALCapture::BUFFER_SIZE/2.0f);
                    glVertex3f(sin(angle)*tempRadius, -cos(angle)*tempRadius, posZ);
                }
                posZ += dist;
                glEnd();
            }
            break;

        case WAVE_DRAWER_CIRCLES_FLOWER:
            glColor4f(1.0f, 1.0f, 1.0f, this->audioCapture->getSoftAmp()*2);
            radius = 2.0f;
            tempRadius = 0.1f;
            angle = 0;
            for (int j = 0; j < 20; j++)
            {
                glBegin(GL_LINE_LOOP);
                for(int i = 0; i < OpenALCapture::BUFFER_SIZE/2.0f; i++)
                {
                    tempRadius = this->audioCapture->getSample(i)*radius;
                    angle = i*2*M_PI/(OpenALCapture::BUFFER_SIZE/2.0f);
                    glVertex2f((sin(angle) * tempRadius), -(cos(angle) * tempRadius));
                }
                glEnd();
                radius += 0.1;
            }
            break;
		case WAVE_DRAWER_CIRCLES_FLOWER_3D:
            glColor4f(1.0f, 1.0f, 1.0f, this->audioCapture->getSoftAmp()*2);

            radius = 0.1f;
            tempRadius = 0.1f;
            dist = 1.0f;
            posZ = 0.0f;

            glTranslatef(0.0f,0.0f,4.0f);
            glRotatef(5, 1.0,1.0,1.0);

            for (int j = 0; j < 2; j++)
            {
                glBegin(GL_LINE_LOOP);
                for(int i = 0; i < OpenALCapture::BUFFER_SIZE/2.0f; i++)
                {
                    tempRadius = radius + this->audioCapture->getSample(i);
                    angle = i*2*M_PI/(OpenALCapture::BUFFER_SIZE/2.0f);
                    glVertex3f(sin(angle)*tempRadius, -cos(angle)*tempRadius, posZ);
                    glVertex3f(sin(angle)*tempRadius, -cos(angle)*tempRadius, -posZ);
                }
                radius += 0.1;
                posZ += dist;
                glEnd();
            }
            break;
    }
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

/**
 * Renders a stroke font string.
 */
void VimusMachineLanternaMagica::renderStrokeFontString(
		float x,
		float y,
		float z,
		void *font,
		const char *lab,
		float stroke)
{
	const char *c;
    glPushMatrix();
        glLineWidth(stroke);
        glTranslatef(x, y, z);
        glScalef(0.001, 0.0015, 1.0);
        for (c=lab; *c != '\0'; c++)
        {
            glutStrokeCharacter(font, *c);
        }
        glLineWidth(1.0);
	glPopMatrix();
}

void VimusMachineLanternaMagica::keyBoardFunc(unsigned char key, int x, int y)
{
	switch (key)
	{
        case 'a':
            this->audioSampler->setGain(currVideo, 1.0);
            this->audioSampler->setSamplePitch(currVideo, currPitch);
            if (this->currEffect < NUM_EFFECTS-1)
            {
                if (this->currEffect == EFFECT_OFF)
                {
                    this->lastVideoEffect = this->videoEffect;
                }
                this->currEffect++;
            }
            else
            {
                this->currEffect = 0;
            }
            if (this->currEffect == EFFECT_GAIN)
                this->videoEffect = VIDEO_EFFECT_CONTRAST_GAIN;
            else if (this->currEffect == EFFECT_GAIN_INV)
                this->videoEffect = VIDEO_EFFECT_CONTRAST_GAIN_INV;
            else if (this->currEffect == EFFECT_TEMPO)
                this->videoEffect = VIDEO_EFFECT_CONTRAST_TEMPO;
            else if (this->currEffect == EFFECT_TEMPO_INV)
                this->videoEffect = VIDEO_EFFECT_CONTRAST_TEMPO_INV;
            else if (this->currEffect == EFFECT_OFF)
            {
                this->videoEffect = this->lastVideoEffect;
            }
            break;
        case 'w':
            if (this->waveDrawer < NUM_OF_DRAWER_EFFECTS-1)
            {
                this->waveDrawer++;
            }
            else
            {
                this->waveDrawer = 0;
            }
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
//                if (this->schedGrooveChange == -1)
//                {
//                    this->schedGrooveChange = this->currVideo;
//                }
//                if (this->schedGrooveChange > 0)
//                {
//                    this->schedGrooveChange--;
//                }
//                else
//                {
//                    this->schedGrooveChange = this->NUM_VIDEOS-1;
//                }
                currPitch = currPitch - 0.01;
                this->audioSampler->setSamplePitch(currVideo, currPitch);
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
            if (videoPlaying == false && this->currVideo == 0)
            {
                this->videoPlaying = true;
                this->audioSampler->playSample(currVideo);
                currPitch = 1.0;
                this->audioSampler->setSamplePitch(currVideo, currPitch);
                boost::xtime_get(&(this->startSysTime2), boost::TIME_UTC);
            }
            else
            {
                if (this->repeatMode == this->REPEAT_MODE_OFF)
                    this->nextTrack();
                else if (this->repeatMode == this->REPEAT_MODE_GROOVE)
                {
//                    if (this->schedGrooveChange == -1)
//                    {
//                        this->schedGrooveChange = this->currVideo;
//                    }
//                    if (this->schedGrooveChange < this->NUM_VIDEOS-1)
//                    {
//                        this->schedGrooveChange++;
//                    }
//                    else
//                    {
//                        this->schedGrooveChange = 0;
//                    }
                    currPitch = currPitch + 0.01;
                    this->audioSampler->setSamplePitch(currVideo, currPitch);
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
        this->setCurrVideo(currVideo-1);
    else
        this->setCurrVideo(NUM_VIDEOS-1);
    this->playCurrVideo();
}

void VimusMachineLanternaMagica::nextTrack()
{
    this->audioSampler->stopSample(currVideo);
    if (currVideo < NUM_VIDEOS-1)
        this->setCurrVideo(currVideo+1);
    else
    {
        this->setCurrVideo(0);
    }
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
    currPitch = 1.0;
    this->audioSampler->setSamplePitch(currVideo, currPitch);
}

void VimusMachineLanternaMagica::setCurrVideo(int video)
{
    this->currVideo = video;
    this->videoEffect = VIDEO_EFFECT_WAVE;
    this->lastVideoEffect = VIDEO_EFFECT_OFF;

    this->audioSampler->setGain(currVideo, 1.0);

    switch(this->currVideo)
    {
        case 0: //dear_friends.avi");
            this->videoEffect = VIDEO_EFFECT_WAVE;
            this->waveDrawer = WAVE_DRAWER_OFF;
            this->currEffect = EFFECT_OFF;
            break;
        case 1: //soul_of_the_funky_drummers_raw_i420_noaudio.avi");
            this->videoEffect = VIDEO_EFFECT_RED;
            this->waveDrawer = WAVE_DRAWER_CIRCLES_FLOWER_3D;
            this->currEffect = EFFECT_OFF;
            break;
        case 2: //cold_sweat_02_raw_i420_noaudio.avi
            this->videoEffect = VIDEO_EFFECT_OFF;
            this->waveDrawer = WAVE_DRAWER_OFF;
            this->currEffect = EFFECT_OFF;
            break;
        case 3: //cold_sweat_01_raw_i420_noaudio.avi
            this->videoEffect = VIDEO_EFFECT_OFF;
            this->waveDrawer = WAVE_DRAWER_OFF;
            this->currEffect = EFFECT_OFF;
            break;
        case 4: //funky_drummer_01_raw_i420_noaudio.avi
            this->videoEffect = VIDEO_EFFECT_OFF;
            this->waveDrawer = WAVE_DRAWER_OFF;
            this->currEffect = EFFECT_OFF;
            break;
        case 5: //do_it_to_death_01_raw_i420_noaudio.avi
            this->videoEffect = VIDEO_EFFECT_WAVE;
            this->waveDrawer = WAVE_DRAWER_OFF;
            this->currEffect = EFFECT_OFF;
            break;
        case 6: //give_it_up_or_turnit_a_loose_01_raw_i420_noaudio.avi
            this->videoEffect = VIDEO_EFFECT_WAVE;
            this->waveDrawer = WAVE_DRAWER_OFF;
            this->currEffect = EFFECT_OFF;
            break;
        case 7: // i_dont_want_nobody_to_give_me_nothing_01_raw_i420_noaudio
            this->videoEffect = VIDEO_EFFECT_RED;
            this->waveDrawer = WAVE_DRAWER_OFF;
            this->currEffect = EFFECT_OFF;
            break;
        case 8: // i_dont_want_nobody_to_give_me_nothing_03_raw_i420_noaudio
            this->videoEffect = VIDEO_EFFECT_RED;
            this->waveDrawer = WAVE_DRAWER_OFF;
            this->currEffect = EFFECT_OFF;
            break;
        case 9: //super_bad_01_raw_i420_noaudio.avi
            this->videoEffect = VIDEO_EFFECT_RED;
            this->waveDrawer = WAVE_DRAWER_OFF;
            this->currEffect = EFFECT_OFF;
            break;
        case 10: // i_got_a_feeling_01_raw_i420_noaudio
            this->videoEffect = VIDEO_EFFECT_RED;
            this->waveDrawer = WAVE_DRAWER_OFF;
            this->currEffect = EFFECT_OFF;
            break;
        case 11: // make_it_funky_01_raw_i420_noaudio
            this->videoEffect = VIDEO_EFFECT_RED;
            this->waveDrawer = WAVE_DRAWER_OFF;
            this->currEffect = EFFECT_OFF;
            break;
        case 12: // papa_dont_take_no_mess_01_raw_i420_noaudio
            this->videoEffect = VIDEO_EFFECT_RED;
            this->waveDrawer = WAVE_DRAWER_OFF;
            this->currEffect = EFFECT_OFF;
            break;
        case 13: // bootsy_01_raw_i420_noaudio
            this->videoEffect = VIDEO_EFFECT_OFF;
            this->waveDrawer = WAVE_DRAWER_OFF;
            this->currEffect = EFFECT_GAIN_INV;
            break;
    }
}


void VimusMachineLanternaMagica :: changeContrast (float bright, float contrast)
{

	for (int i= 0; i<VIDEO_SAMPLE_WIDTH*VIDEO_SAMPLE_HEIGHT*3; i++)
	{
		if (this->capturedFrame[i] + bright > 255)
			this->distorcedFrame[i] = 255;
		else
			this->distorcedFrame[i] = this->capturedFrame[i] + bright;
	}

	for(int i=0;i<256;i++){
	  if(i<(int)(128.0f+128.0f*tan(contrast))&&i>(int)(128.0f-128.0f*tan(contrast)))
		contrasTransform[i]=(i-128)/tan(contrast)+128;
	  else if(i>=(int)(128.0f+128.0f*tan(contrast)))
		contrasTransform[i]=255;
	  else
		contrasTransform[i]=0;
	}

	for (int i= 0; i<VIDEO_SAMPLE_WIDTH*VIDEO_SAMPLE_HEIGHT*3; i++)
	{
		this->distorcedFrame[i] = contrasTransform[this->capturedFrame[i]];
	}
}
