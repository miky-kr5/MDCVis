/*------------------------------------------------------------------------------
; File:          application.h
; Description:   Declaration of the base application class.
; Author:        Miguel Angel Astor, sonofgrendel@gmail.com
; Date created:  9/17/2013
; Date modified: 12/13/2013
;
; Copyright (C) 2013 Fundacion Museos Nacionales
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
#ifndef APPLICATION_H
#define APPLICATION_H

#include <irrlicht.h>

using namespace irr;

/*------------------------------------------------------------------------------
; The core class for the program.
; Implements the main loop and the different scene builders.
;-----------------------------------------------------------------------------*/
class Application : public IEventReceiver{
    public:
        Application();                        // Default constructor.
        ~Application();                       // Default destructor.
        void run();                           // Main loop.
        void buildScene();                    // Basic scene builder.
        bool OnEvent( const SEvent& event );  // Event handler.

    private:
        // Irrlicht engine objects.
        IrrlichtDevice         * device;
        video::IVideoDriver    * driver;
        scene::ISceneManager   * smgr;
        gui::IGUIEnvironment   * guienv;

        int                      lastFPS;
};

#endif // APPLICATION_H
