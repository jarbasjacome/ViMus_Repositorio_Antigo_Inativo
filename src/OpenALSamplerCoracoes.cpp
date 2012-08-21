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

#include "OpenALSamplerCoracoes.h"

OpenALSamplerCoracoes::OpenALSamplerCoracoes()
{

	ALenum error;

	// Inicializar OpenAL

	alutInit(0, NULL);
	// Clear Error Code (so we can catch any new errors)
	if ((error = alGetError()) != AL_NO_ERROR)
		this->displayOpenALError("alutInit : ", error);

	// Gerar buffers
	alGenBuffers(NUM_BUFFERS, buffers);
	if ((error = alGetError()) != AL_NO_ERROR)
		printf("\nalGenBuffers : %d", error);

	// Carregar arquivos e armazená-los nos buffers
	if (!this->loadWaveBuffer("/dados/data/coracoes_a_mil/coracoes.wav", buffers[0]))
		alDeleteBuffers(NUM_BUFFERS, buffers);

	// Generate the sources
	alGenSources(NUM_SOURCES, source);
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		this->displayOpenALError("alGenSources : ", error);
	}

	for (int i=0; i<NUM_SOURCES; i++)
	{
		alSourcei(source[i], AL_BUFFER, buffers[i]);
		if ((error = alGetError()) != AL_NO_ERROR)
			this->displayOpenALError("alSourcei : ", error);
	}

}

OpenALSamplerCoracoes::~OpenALSamplerCoracoes()
{
	alDeleteSources(NUM_SOURCES, source);
	alDeleteBuffers(NUM_BUFFERS, buffers);

	alutExit();
}

void OpenALSamplerCoracoes::displayOpenALError(char* _string, ALenum error)
{
	printf("\n");
	switch (error)
	{
	case AL_INVALID_NAME:
		printf("%s Invalid Name", _string);
		break;
	case AL_INVALID_ENUM:
		printf("%s Invalid Enum", _string);
		break;
	case AL_INVALID_VALUE:
		printf("%s Invalid Value", _string);
		break;
	case AL_INVALID_OPERATION:
		printf("%s Invalid Operation", _string);
		break;
	case AL_OUT_OF_MEMORY:
		printf("%s Out Of Memory", _string);
		break;
	default:
		printf("*** ERROR *** Unknown error case in displayOpenALError\n");
		break;
	};
}

int	OpenALSamplerCoracoes::loadWaveBuffer (char* _fileName, ALuint _buffer)
{
	int			error;
	ALenum		format;
	ALsizei		size;
	ALsizei		freq;
	ALboolean	loop;
	ALvoid*		data;

	FILE *File=NULL;
	File=fopen(_fileName,"r");
	if (File)
	{
		// Carregar arquivo
		alutLoadWAVFile((ALbyte*) _fileName, &format, &data, &size, &freq, &loop);
		if ((error = alGetError()) != AL_NO_ERROR)
		{
			displayOpenALError("alutLoadWAVFile : ", error);
			return 0;
		}
	}
	else
	{
		printf("\n%s - Arquivo inexistente ou em uso.", _fileName);
		return 0;
	}


	// Carregar dados do arquivo no buffer.
	alBufferData(_buffer,format,data,size,freq);
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		displayOpenALError("alBufferData :", error);
		return 0;
	}

	// Liberar memoria usada para carregar arquivo.
	alutUnloadWAV(format,data,size,freq);
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		displayOpenALError("alutUnloadWAV :", error);
		return 0;
	}

	return 1;
}

void OpenALSamplerCoracoes::playSample (int sNumber)
{
	alSourcePlay(source[sNumber]);
}

void OpenALSamplerCoracoes::stopSample (int sNumber)
{
	alSourceStop(source[sNumber]);
}

void OpenALSamplerCoracoes::setSamplePitch (int sNumber, float pitch)
{
	alSourcef(source[sNumber],AL_PITCH,pitch);
}

void OpenALSamplerCoracoes::setPlaybackPos (int sNumber, float sec)
{
    alSourcef(source[sNumber],AL_SEC_OFFSET,sec);
}

void OpenALSamplerCoracoes::setGain(int sNumber, float gain)
{
    alSourcef(source[sNumber],AL_GAIN,gain);
}
ALfloat OpenALSamplerCoracoes::getSecondOffset(int sNumber)
{
    ALfloat pos = 0;
    alGetSourcef(source[sNumber],AL_SEC_OFFSET,&pos);
    return pos;
}

