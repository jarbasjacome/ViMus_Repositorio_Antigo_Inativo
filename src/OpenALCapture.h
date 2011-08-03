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

#ifndef _OPENALCAPTURE_H_
#define _OPENALCAPTURE_H_

#include <stdlib.h>

#include <al.h>
#include <alc.h>
#include <alut.h>

#include <iostream>
#include <stdio.h>
#include <string.h>

/**
 * OpenALCapture class to audio captue management.
 *
 * @author Jarbas Jácome
 * Based on Lee Winder OpenAL Tutorial.
 * http://www.gamedev.net/reference/articles/article2008.asp
 *
 * @date 05.27.2011
 *
 */
class OpenALCapture
{
	public:

		OpenALCapture();
		virtual ~OpenALCapture();

		void startCapture();
		void stopCapture();
		void grabSamples();
        float getSample(int index);
        void softWave();
        float getSoftAmp();

        static const int BUFFER_SIZE = 1024;
        static const int SAMPLE_RATE = 44100;

	private:

        static const int DEVICE = 4;
        ALCdevice* capDevice;

        float buffer[BUFFER_SIZE];
        ALint tempBuffer[BUFFER_SIZE];

        ALint numAvailSamples;

        static const int SOFT_AMP_BUFFER_SIZE = 3;
        float softAmp;
        float softAmpArray[SOFT_AMP_BUFFER_SIZE];
        float soma;

        void error(ALCenum error);
};

#endif //_OPENALCAPTURE_H_
