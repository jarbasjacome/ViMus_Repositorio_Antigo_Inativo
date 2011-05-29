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

#include "OpenALSampler.h"

OpenALSampler::OpenALSampler()
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
	if (!this->loadWaveBuffer("/dados/jabahpureza/audio_samples/sample_papa_brass01.wav", buffers[0]))
		alDeleteBuffers(NUM_BUFFERS, buffers);
	if (!this->loadWaveBuffer("/dados/jabahpureza/audio_samples/sample_papa_brass02.wav", buffers[1]))
		alDeleteBuffers(NUM_BUFFERS, buffers);
	if (!this->loadWaveBuffer("/dados/jabahpureza/audio_samples/super_bad_01.wav", buffers[2]))
		alDeleteBuffers(NUM_BUFFERS, buffers);
	if (!this->loadWaveBuffer("/dados/jabahpureza/audio_samples/cold_sweat_01.wav", buffers[3]))
		alDeleteBuffers(NUM_BUFFERS, buffers);
	if (!this->loadWaveBuffer("/dados/jabahpureza/audio_samples/cold_sweat_02.wav", buffers[4]))
		alDeleteBuffers(NUM_BUFFERS, buffers);
	if (!this->loadWaveBuffer("/dados/jabahpureza/audio_samples/do_it_to_death_01.wav", buffers[5]))
		alDeleteBuffers(NUM_BUFFERS, buffers);
	if (!this->loadWaveBuffer("/dados/jabahpureza/audio_samples/funky_drummer_01.wav", buffers[6]))
		alDeleteBuffers(NUM_BUFFERS, buffers);
	if (!this->loadWaveBuffer("/dados/jabahpureza/audio_samples/give_it_up_or_turnit_a_loose_01.wav", buffers[7]))
		alDeleteBuffers(NUM_BUFFERS, buffers);
	if (!this->loadWaveBuffer("/dados/jabahpureza/audio_samples/give_it_up_or_turnit_a_loose_02.wav", buffers[8]))
		alDeleteBuffers(NUM_BUFFERS, buffers);
	if (!this->loadWaveBuffer("/dados/jabahpureza/audio_samples/i_dont_want_nobody_to_give_me_nothing_01.wav", buffers[9]))
		alDeleteBuffers(NUM_BUFFERS, buffers);
	if (!this->loadWaveBuffer("/dados/jabahpureza/audio_samples/i_dont_want_nobody_to_give_me_nothing_02.wav", buffers[10]))
		alDeleteBuffers(NUM_BUFFERS, buffers);
	if (!this->loadWaveBuffer("/dados/jabahpureza/audio_samples/i_dont_want_nobody_to_give_me_nothing_03.wav", buffers[11]))
		alDeleteBuffers(NUM_BUFFERS, buffers);
	if (!this->loadWaveBuffer("/dados/jabahpureza/audio_samples/i_got_a_feeling_01.wav", buffers[12]))
		alDeleteBuffers(NUM_BUFFERS, buffers);
	if (!this->loadWaveBuffer("/dados/jabahpureza/audio_samples/licking_stick_01.wav", buffers[13]))
		alDeleteBuffers(NUM_BUFFERS, buffers);
	if (!this->loadWaveBuffer("/dados/jabahpureza/audio_samples/licking_stick_02.wav", buffers[14]))
		alDeleteBuffers(NUM_BUFFERS, buffers);
	if (!this->loadWaveBuffer("/dados/jabahpureza/audio_samples/make_it_funky_01.wav", buffers[15]))
		alDeleteBuffers(NUM_BUFFERS, buffers);
	if (!this->loadWaveBuffer("/dados/jabahpureza/audio_samples/mother_popcorn_01.wav", buffers[16]))
		alDeleteBuffers(NUM_BUFFERS, buffers);
	if (!this->loadWaveBuffer("/dados/jabahpureza/audio_samples/papa_dont_take_no_mess_01.wav", buffers[17]))
		alDeleteBuffers(NUM_BUFFERS, buffers);
	if (!this->loadWaveBuffer("/dados/jabahpureza/audio_samples/sex_machine_01.wav", buffers[18]))
		alDeleteBuffers(NUM_BUFFERS, buffers);
	if (!this->loadWaveBuffer("/dados/jabahpureza/audio_samples/thank_you.wav", buffers[19]))
		alDeleteBuffers(NUM_BUFFERS, buffers);
	if (!this->loadWaveBuffer("/dados/jabahpureza/audio_samples/bootsy_01.wav", buffers[20]))
		alDeleteBuffers(NUM_BUFFERS, buffers);
	if (!this->loadWaveBuffer("/dados/jabahpureza/audio_samples/sample_ataque_give_it_up_or_turnit_a_loose.wav", buffers[21]))
		alDeleteBuffers(NUM_BUFFERS, buffers);
	if (!this->loadWaveBuffer("/dados/jabahpureza/audio_samples/sample_clap_your_hand.wav", buffers[22]))
		alDeleteBuffers(NUM_BUFFERS, buffers);
	if (!this->loadWaveBuffer("/dados/jabahpureza/audio_samples/sample_clap_your_hand_com_batera.wav", buffers[23]))
		alDeleteBuffers(NUM_BUFFERS, buffers);
	if (!this->loadWaveBuffer("/dados/jabahpureza/audio_samples/sample_papa_fala.wav", buffers[24]))
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

	alSourcef(source[0],AL_GAIN,0.2f);
	alSourcef(source[1],AL_GAIN,0.2f);
	alSourcef(source[3],AL_GAIN,0.3f);
	alSourcef(source[20],AL_GAIN,0.2f);
}

OpenALSampler::~OpenALSampler()
{
	alDeleteSources(NUM_SOURCES, source);
	alDeleteBuffers(NUM_BUFFERS, buffers);

	alutExit();
}

void OpenALSampler::displayOpenALError(char* _string, ALenum error)
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

int	OpenALSampler::loadWaveBuffer (char* _fileName, ALuint _buffer)
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

void OpenALSampler::playSample (int sNumber)
{
	alSourcePlay(source[sNumber]);
}

void OpenALSampler::stopSample (int sNumber)
{
	alSourceStop(source[sNumber]);
}

void OpenALSampler::setSamplePitch (int sNumber, float pitch)
{
	alSourcef(source[sNumber],AL_PITCH,pitch);
}

void OpenALSampler::setPlaybackPos (int sNumber, float sec)
{
    alSourcef(source[sNumber],AL_SEC_OFFSET,sec);
}
