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

#include "OpenALCapture.h"

OpenALCapture::OpenALCapture()
{
    const ALchar *devicelist = alcGetString (NULL, ALC_CAPTURE_DEVICE_SPECIFIER);

    if (devicelist)
    {
        std::cout << "\nAvailable Capture Devices are:-\n";
        int countDevs = 0;
        while (*devicelist )
        {
            std::cout << "\n" << countDevs << "-" << devicelist;
            countDevs++;
            devicelist += strlen(devicelist) + 1;
        }
    }

    devicelist = alcGetString (NULL, ALC_CAPTURE_DEVICE_SPECIFIER);

    if (devicelist)
    {
        int countDevs = 0;
        while (*devicelist && countDevs < DEVICE)
        {
            devicelist += strlen(devicelist) + 1;
            countDevs++;
        }
        std::cout << "\n\nSelected device: ";
        std::cout << "\n" << countDevs << "-" << devicelist;
    }

    this->capDevice = alcCaptureOpenDevice(devicelist, SAMPLE_RATE, AL_FORMAT_STEREO16, BUFFER_SIZE);

    for (int i=0; i<BUFFER_SIZE; i++)
    {
        this->buffer[i] = 0;
        this->tempBuffer[i] = 0;
    }

    this->startCapture();
}

OpenALCapture::~OpenALCapture()
{
    alcCaptureCloseDevice(this->capDevice);

}

void OpenALCapture::startCapture ()
{
    alcCaptureStart(this->capDevice);
    //this->error(alcGetError(this->capDevice));
}

void OpenALCapture::stopCapture ()
{
    alcCaptureStop(this->capDevice);
    //this->error(alcGetError(this->capDevice));
}

void OpenALCapture::grabSamples()
{
    alcGetIntegerv(this->capDevice, ALC_CAPTURE_SAMPLES, (ALCsizei)sizeof(ALint), &numAvailSamples);
    if (numAvailSamples >= this->BUFFER_SIZE)
    {
        alcCaptureSamples(this->capDevice, &(this->tempBuffer), numAvailSamples);
        //this->error(alcGetError(this->capDevice));
        this->softWave();
    }
}

void OpenALCapture::softWave()
{
    int c=OpenALCapture::BUFFER_SIZE/2;
    int zarathustra=0;
    while (c>0)
    {
        if (this->tempBuffer[c] > this->tempBuffer[zarathustra])
            zarathustra = c;
        c--;
    }

    int antiZara = BUFFER_SIZE-1-zarathustra;
    for (int i=0; i<antiZara; i++)
    {
        this->buffer[i] = ((float) this->tempBuffer[i+zarathustra]) / 4294967296.0f;
    }
    for (int i=antiZara; i<BUFFER_SIZE-1; i++)
    {
        this->buffer[i] = ((float) this->tempBuffer[i-(antiZara)]) / 4294967296.0f;
    }
}

float OpenALCapture::getSample(int index)
{
    return this->buffer[index];
}


void OpenALCapture::error(ALCenum error)
{
    switch (error)
    {
        case ALC_NO_ERROR:
            std::cout << "\n\n\n\n\n\n\n\n\nALC_NO_ERROR";
            break;
        case ALC_INVALID_DEVICE:
            std::cout << "\n\n\n\n\n\n\n\n\nALC_INVALID_DEVICE";
            break;
        case ALC_INVALID_CONTEXT:
            std::cout << "\n\n\n\n\n\n\n\n\nALC_INVALID_CONTEXT";
            break;
        case ALC_INVALID_ENUM:
            std::cout << "\n\n\n\n\n\n\n\n\nALC_INVALID_ENUM";
            break;
        case ALC_INVALID_VALUE:
            std::cout << "\n\n\n\n\n\n\n\n\nALC_INVALID_VALUE";
            break;
        case ALC_OUT_OF_MEMORY:
            std::cout << "\n\n\n\n\n\n\n\n\nALC_OUT_OF_MEMORY";
            break;
        default:
            std::cout << "\n\n\n\n\n\n\n\n\nCODIGO DE ERRO INVALIDO!";
            break;
    }
}
