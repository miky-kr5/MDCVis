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

#include <iostream>
#include <cstdlib>

#include "Application.h"
#include "SettingsDlg.h"

const char * DB_FILENAME = "exhibits/mdc.db";

/*------------------------------------------------------------------------------
; Application::Application()
;
; Application constructor. Creates a new application, setting up some default
; parameters for the irrLicht Engine.
;-----------------------------------------------------------------------------*/
mdcApplication::mdcApplication(){
	settings = mdcSettingsMdl::getInstance();

	SIrrlichtCreationParameters params = SIrrlichtCreationParameters();

	params.AntiAlias  = (u8) settings->getAntialiasingFactor();
	params.DriverType = settings->getVideoDriverType();
	params.WindowSize = core::dimension2d<u32>( settings->getScreenWidth(), settings->getScreenHeight() );
	params.Fullscreen = settings->isFullScreen();
	params.Vsync      = settings->isVSyncEnabled();

	params.EventReceiver = this;

	device = createDeviceEx(params);

	if( device == NULL ) return;

	device->setWindowCaption( L"Museo de Ciencias :: " );
	device->getCursorControl()->setVisible( false );

	// Get pointers to the engine objects to avoid calling the getters at a later time.
	driver = device->getVideoDriver();
	smgr   = device->getSceneManager();
	guienv = device->getGUIEnvironment();

	scene = new mdcScene( device );

	const SKeyMap * f  = settings->getForwardKey();
	const SKeyMap * b  = settings->getBackwardKey();
	const SKeyMap * sl = settings->getStrafeLeftKey();
	const SKeyMap * sr = settings->getStrafeRightKey();

	scene->changeCameraKeyMaps( *f, *b, *sl, *sr );

	gui::IGUISkin* skin = guienv->getSkin();
	gui::IGUIFont* font = guienv->getFont("font/fontcourier.bmp");
	if ( font )
		skin->setFont(font);

	settingsCtrl = new mdcSettingsCtrl( this );

	settingsVisible = false;

	exhibits = mdcExhibitMdl::getInstance();
	if ( !exhibits->setDatabaseFile( DB_FILENAME ) ) {
		std::cerr << "The exhibits database could not be opened." << std::endl;
	}

	lastFPS = -1;
}

/*------------------------------------------------------------------------------
; Application::~Application()
;
; Application destructor. Releases all the objects used by the application.
;-----------------------------------------------------------------------------*/
mdcApplication::~mdcApplication(){
	if(device != NULL)
		device->drop();

	if ( settings->settingsChanged() ) settings->saveSettings();

	settings->freeInstance();
	exhibits->freeInstance();
	delete settingsCtrl;
	delete scene;
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
				guienv->drawAll();
				driver->endScene();

				fps = driver->getFPS();

				if( !settings->isFullScreen() && lastFPS != fps ){
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
bool mdcApplication::OnEvent( const SEvent& event ) {
	if ( event.EventType == irr::EET_KEY_INPUT_EVENT ) {
		if ( event.KeyInput.Key == irr::KEY_ESCAPE && event.KeyInput.PressedDown ) {
			device->closeDevice();
			return true;

		} else if ( event.KeyInput.Key == irr::KEY_F1 && event.KeyInput.PressedDown ) {
			if ( !settingsVisible ) {
				settingsVisible = true;
				device->getCursorControl()->setVisible( true );
				settingsCtrl->setDialog( new mdcSettingsDlg( guienv ) );
				device->setEventReceiver( settingsCtrl );
			}
			return true;
		}
	}
	return false;
}

void mdcApplication::onSettingsDialogHidden() {
	settingsVisible = false;

	const SKeyMap * f  = settings->getForwardKey();
	const SKeyMap * b  = settings->getBackwardKey();
	const SKeyMap * sl = settings->getStrafeLeftKey();
	const SKeyMap * sr = settings->getStrafeRightKey();

	scene->changeCameraKeyMaps( *f, *b, *sl, *sr );

	device->getCursorControl()->setVisible( false );
	device->setEventReceiver( this );
}
