/*------------------------------------------------------------------------------
; File:          definitions.h
; Description:   Defines every class and struct in the project.
; Author:        Miguel Angel Astor, sonofgrendel@gmail.com
; Date created:  2/18/2014
;
; Copyright (C) 2014 Museo de Ciencias de Caracas
;
; This program is free software: you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation, either version 3 of the License, or
; (at your option) any later version.
;
; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License for more details.
;
; You should have received a copy of the GNU General Public License
; along with this program.  If not, see <http://www.gnu.org/licenses/>.
;-----------------------------------------------------------------------------*/

#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <irrlicht.h>

#define SPA

#ifndef SPA
#define ENG
#endif

class mdcApplication;
class mdcSettingsMdl;
class mdcSettingsDlg;
class mdcSettingsCtrl;
class mdcScene;
class mdcExhibitMdl;
class mdcExhibitDlg;

typedef struct sqlite3 sqlite3;
typedef struct VEC_3 vec3_t;

#endif // DEFINITIONS_H
