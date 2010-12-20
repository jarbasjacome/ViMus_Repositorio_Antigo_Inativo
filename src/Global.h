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

#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <stdlib.h>

//SOH PRA DEBUGAR, DEPOIS TIRA...
#include <stdio.h>

// Macro for releasing a COM object
#define RELEASE(x) { if ((x)!=NULL) (x)->Release();  (x)=NULL; }
#define SAFE_RELEASE(p)   { if(p) { (p)->Release(); (p)=NULL; } }
// Garbage Colletion Defines
#define SAFE_DELETE(p)   { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }

#endif //_GLOBAL_H_
