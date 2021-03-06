/*------------------------------------------------------------------------------
; File:          main.cpp
; Description:   Application entry point.
; Author:        Miguel Angel Astor, sonofgrendel@gmail.com
; Date created:  9/17/2013
;
; Copyright (C) 2013 Museo de Ciencias de Caracas
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

#include <cstdlib>

#include "Application.hpp"

int main( int argc, char** argv ) {
	mdcApplication *app = new mdcApplication();
	app->run();
	delete app;

	return EXIT_SUCCESS;
}
