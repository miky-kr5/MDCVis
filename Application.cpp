/*------------------------------------------------------------------------------
; File:          Application.cpp
; Description:   Implementation of the base application class.
; Author:        Miguel Angel Astor, sonofgrendel@gmail.com
; Date created:  9/17/2013
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

#include <cstdlib>

#include "Application.h"

/*------------------------------------------------------------------------------
; Application::Application()
;
; Application constructor. Creates a new application, setting up some default
; parameters for the irrLicht Engine.
;-----------------------------------------------------------------------------*/
mdcApplication::mdcApplication(){
	settings = mdcSettings::getInstance();

	SIrrlichtCreationParameters params = SIrrlichtCreationParameters();

	params.AntiAlias = (u8) settings->getAntialiasingFactor();
	params.DriverType = settings->getVideoDriverType();
	params.WindowSize = core::dimension2d<u32>( settings->getScreenWidth(), settings->getScreenHeight() );
	params.Fullscreen = settings->isFullScreen();
	params.Vsync = settings->isVSyncEnabled();

	params.EventReceiver = this;

	device = createDeviceEx(params);

	if( device == NULL )
		return;

	device->setWindowCaption( L"Museo de Ciencias :: " );
	device->getCursorControl()->setVisible( false );

	// Get pointers to the engine objects to avoid calling the getters at a later time.
	driver = device->getVideoDriver();
	smgr = device->getSceneManager();
	guienv = device->getGUIEnvironment();

	scene = new mdcScene(device);

	SKeyMap * f = settings->getForwardKey();
	SKeyMap * b = settings->getBackwardKey();
	SKeyMap * sl = settings->getStrafeLeftKey();
	SKeyMap * sr = settings->getStrafeRightKey();

	scene->changeCameraKeyMaps(*f, *b, *sl, *sr);

	lastFPS = -1;
}

/*------------------------------------------------------------------------------
; Application::~Application()
;
; Application destructor. Releases all the objects used by the application.
;-----------------------------------------------------------------------------*/
mdcApplication::~mdcApplication(){
	// Delete all objects created by the engine.
	if(device != NULL)
		device->drop();

	if ( settings->settingsChanged() )
		settings->saveSettings();

	settings->freeInstance();
}

/*------------------------------------------------------------------------------
; Application::run()
;
; Main loop of the application. It renders the demo scene and calculates the
; fps count.
;-----------------------------------------------------------------------------*/
void mdcApplication::run(){
	int fps;
	core::stringw str;

	// Render everything until the user requests the application to close itself.
	if( device != NULL ){
		while( device->run() ){
			if( device->isWindowActive() ){
				driver->beginScene( true, true, video::SColor( 255, 97, 220, 220 ) );
				smgr->drawAll();
				driver->endScene();

				fps = driver->getFPS();

				if( lastFPS != fps ){
					str = L"Museo de Ciencias :: [";
					str += driver->getName();
					str += "] FPS:";
					str += fps;
					device->setWindowCaption( str.c_str() );
					lastFPS = fps;
				}
			}else device->yield();
		}
	}
}

/*------------------------------------------------------------------------------
; Application::OnEvent()
;
; Captures events sent by irrLicht Engine. Right now it only listens for the
; escape key and stops the engine when it is pressed.
;-----------------------------------------------------------------------------*/
bool mdcApplication::OnEvent( const SEvent& event ){
	if( event.EventType == irr::EET_KEY_INPUT_EVENT ){
		if( event.KeyInput.Key == irr::KEY_ESCAPE && event.KeyInput.PressedDown ){
			device->closeDevice();
			return true;
		}
	}
	return false;
}
