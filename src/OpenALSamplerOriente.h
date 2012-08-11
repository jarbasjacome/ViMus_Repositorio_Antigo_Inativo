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

#ifndef _OPENALSAMPLERORIENTE_H_
#define _OPENALSAMPLERORIENTE_H_

#include <stdlib.h>
#include <stdio.h>

#include <al.h>
#include <alc.h>
#include <alut.h>

/**
 * OpenALSamplerOriente class do sampling control.
 *
 * @author Jarbas Jácome
 * Based on Lee Winder OpenAL Tutorial.
 * http://www.gamedev.net/reference/articles/article2008.asp
 *
 */
class OpenALSamplerOriente
{
	public:

		OpenALSamplerOriente();
		virtual ~OpenALSamplerOriente();

		void playSample (int sNumber);
		void stopSample (int sNumber);

		void setSamplePitch (int sNumber, float pitch);

		void setPlaybackPos (int sNumber, float sec);

		void setGain(int sNumber, float gain);

		ALfloat getSecondOffset(int sNumber);

	private:

		static const int NUM_BUFFERS = 2;
		static const int NUM_SOURCES = 2;

		ALuint buffers[NUM_BUFFERS];
		ALuint source[NUM_SOURCES];

		int	loadWaveBuffer (char* _fileName, ALuint _buffer);
		void displayOpenALError (char* _string, ALenum error);

};

#endif //_OPENALSAMPLERORIENTE_H_
