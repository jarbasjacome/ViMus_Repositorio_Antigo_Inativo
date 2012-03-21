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

/*********************************************
 * Vimus
 *
 * Configurações do sistema.
 * Futuramente isso será uma classe que poderá
 * ler e armazenar em arquivo e ser alterada
 * pela interface gráfica.
 *
 *********************************************/

#ifndef  _CONFIGURATION_H_
#define _CONFIGURATION_H_

//#define __WINDOWS__

#ifdef __WINDOWS__
#include <windows.h>
#endif

#define DEBUG_MODE      false

// Performance settings
#define MAX_FRAME_RATE  60

// Window settings
#define FULLSCREEN		true
#define WINDOW_WIDTH	640
#define WINDOW_HEIGHT	480
#define WINDOW_POSX		100
#define WINDOW_POSY		100
#define WINDOW_NAME		"ViMus"

// Display settings
#define SCREEN_FORMAT			0.8		// SECREEN_FORMAT = height/width
#define IS_FIXED_SCREEN_FORMAT	false	// if false, SCREEN_FORMAT will be
										// ignored and it will be calculated
										// by dividing WindowHeight/WindowWidth

#define SCREEN_PLANE_WIDTH      2.0f
#define SCREEN_PLANE_HEIGHT     2.0f

//ORTHO
#define LEFT		-1.0f
#define RIGHT		 1.0f
#define BOTTOM		-1.0f
#define TOP			 1.0f
#define NEAR       -20.0f
#define FAR         20.0f

#define CAM_EYE_Z       12.5f

#define VIDEO_WIDTH			512
#define VIDEO_HEIGHT		512

#define VIDEO_SAMPLE_WIDTH      720
#define VIDEO_SAMPLE_HEIGHT     576

#define PERSP_FOV       11.25f
#define PERSP_ASPECT     1.0f
#define PERSP_NEAR       1.0f
#define PERSP_FAR       30.0f

#endif //_CONFIGURATION_H_
