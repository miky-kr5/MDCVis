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
#include <cassert>

#include "Application.hpp"
#include "SettingsDlg.hpp"

static const char * DB_FILENAME = "exhibits/mdc.db";

/*------------------------------------------------------------------------------
; Application::Application()
;
; Application constructor. Creates a new application, setting up some default
; parameters for the irrLicht Engine.
;-----------------------------------------------------------------------------*/
mdcApplication::mdcApplication(){
	int w, h;

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
	device->setResizable( false );

	// Get pointers to the engine objects to avoid calling the getters at a later time.
	driver = device->getVideoDriver();
	smgr   = device->getSceneManager();
	guienv = device->getGUIEnvironment();

	// Create the loading screen.
	w = settings->getScreenWidth();
	h = settings->getScreenHeight();
	loadingScreen = guienv->addImage( core::rect<s32>( 0, 0, w, h ), NULL, 0xABCD );
	if ( w == 640 && h == 480 ) {
		loadingScreen->setImage( driver->getTexture( "gfx/loading640x480.png" ) );
	} else if( w == 800 && h == 600 ) {
		loadingScreen->setImage( driver->getTexture( "gfx/loading800x600.png" ) );
	} else if( w == 1024 && h == 768 ) {
		loadingScreen->setImage( driver->getTexture( "gfx/loading1024x768.png" ) );
	} else if( w == 1280 && h == 800 ) {
		loadingScreen->setImage( driver->getTexture( "gfx/loading1280x800.png" ) );
	} else if( w == 1280 && h == 1024 ) {
		loadingScreen->setImage( driver->getTexture( "gfx/loading1280x1024.png" ) );
	} else {
		// The screen resolution is unrecognized.
		device->closeDevice();
		assert( 0 );
	}

	// Set up the gui font and create the dialog listeners.
	gui::IGUISkin* skin = guienv->getSkin();
	gui::IGUIFont* font = guienv->getFont("font/fontcourier.bmp");
	if ( font )
		skin->setFont(font);
	settingsCtrl = new mdcSettingsCtrl( this );
	dlgVisible = false;

	// Set scene to NULL so that it will be loaded after the first render.
	scene = NULL;
	exDlg = NULL;

	lastFPS = -1;
	nodeSelected = false;
	selectedNodeId = 0;
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

	mdcSettingsMdl::freeInstance();
	mdcExhibitMdl::freeInstance();
	delete settingsCtrl;
	delete scene;
}

void mdcApplication::loadScene(){
	int                       nEx;
	vec3_t                    r, s, t;
	float                     ra;
	core::stringc             path;
	int                  *    ids;
	char                 *    modelPath;
	scene::IAnimatedMesh *    mesh;
	scene::ISceneNode    *    node;

	const SKeyMap        *    f  = settings->getForwardKey();
	const SKeyMap        *    b  = settings->getBackwardKey();
	const SKeyMap        *    sl = settings->getStrafeLeftKey();
	const SKeyMap        *    sr = settings->getStrafeRightKey();

	scene = new mdcScene( device );

	scene->changeCameraKeyMaps( *f, *b, *sl, *sr );

	exhibits = mdcExhibitMdl::getInstance();

	if ( !exhibits->setDatabaseFile( DB_FILENAME ) ) {
		std::cerr << "The exhibits database could not be opened." << std::endl;
	}

	device->getFileSystem()->addFileArchive( "exhibits/exhibits.zip" );

	nEx = exhibits->getNumOfExhibits();
	ids = ( int * )malloc( sizeof( int ) * nEx );
	exhibits->getFirstNExhibitIds( ids, nEx );

	for ( int i = 0; i < nEx; i++ ) {

		exhibits->getRotationById( r, ids[ i ] );
		exhibits->getTranslationById( t, ids[ i ] );
		exhibits->getScalingById( s, ids[ i ] );
		ra = exhibits->getRotationAmountById( ids [ i ] );

		exhibits->getExhibitModelPathById( &modelPath, ids[ i ] );

		if ( modelPath != NULL ) {
			path = "exhibits/";
			path += modelPath;

			mesh = smgr->getMesh( path.c_str() );
			node = smgr->addOctreeSceneNode( mesh->getMesh( 0 ), NULL, ids[ i ], 1024 );
			if ( node != NULL ) {
				node->setMaterialFlag( video::EMF_LIGHTING, false );
				node->setMaterialFlag( video::EMF_NORMALIZE_NORMALS, true );
				node->setMaterialType( video::EMT_SOLID );

				r.x = 0;
				r.y = r.y * ra;
				r.z = 0;

				node->setRotation( core::vector3df( r.x, r.y, r.z ) );
				node->setScale( core::vector3df( s.x, s.y, s.z ) );
				node->setPosition( core::vector3df( t.x, t.y, t.z ) );

				scene->addMeshToCollisionDetection( mesh, node );
			}
		}

		free( modelPath );
	}

	free( ids );

	collMan = smgr->getSceneCollisionManager();
}

/*------------------------------------------------------------------------------
; Application::run()
;
; Main loop of the application. It renders the demo scene and calculates the
; fps count.
;-----------------------------------------------------------------------------*/
void mdcApplication::run(){
	int                                fps;
	core::stringw                      str;
	core::line3d<f32>                  ray;
	scene::ICameraSceneNode *          camera;
	scene::ISceneNode       *          selectedSceneNode;
	core::vector3df                    intersection;
    core::triangle3df                  hitTriangle;

	// Render everything until the user requests the application to close itself.
	if( device != NULL ){
		if( device->isWindowActive() && scene == NULL){
			driver->beginScene( true, true, video::SColor( 255, 97, 220, 220 ) );
			guienv->drawAll();
			driver->endScene();
			loadScene();
			loadingScreen->remove();
		}

		while( device->run() ) {
			if( device->isWindowActive() ) {
				driver->beginScene( true, true, video::SColor( 255, 97, 220, 220 ) );
				smgr->drawAll();
				guienv->drawAll();
				driver->endScene();

				fps = driver->getFPS();

				if( !settings->isFullScreen() && lastFPS != fps ) {
					str = L"Museo de Ciencias :: [";
					str += driver->getName();
					str += "] FPS:";
					str += fps;
					device->setWindowCaption( str.c_str() );
					lastFPS = fps;
				}

				if ( scene == NULL ) {
					loadScene();
					loadingScreen->remove();
					camera = scene->getCamera();
				} else {
					ray.start = camera->getPosition();
        			ray.end = ray.start + ( camera->getTarget() - ray.start ).normalize() * 300.0f;

        			selectedSceneNode = collMan->getSceneNodeAndCollisionPointFromRay(
																						ray,
																						intersection,
																						hitTriangle,
																						0,
																						0
																					 );

        			if ( selectedSceneNode != NULL && !dlgVisible ) {
						selectedNodeId = selectedSceneNode->getID();
						device->getCursorControl()->setActiveIcon( gui::ECI_HAND );
						device->getCursorControl()->setVisible( true );

        			} else {
        				if ( !dlgVisible ) {
							selectedNodeId = 0;
							device->getCursorControl()->setVisible( false );
        				}
        			}
				}
			} else device->yield();
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
			if ( !dlgVisible ) {
				stopMovement();
				dlgVisible = true;
				device->getCursorControl()->setActiveIcon( gui::ECI_NORMAL );
				device->getCursorControl()->setVisible( true );
				settingsCtrl->setDialog( new mdcSettingsDlg( guienv ) );
				device->setEventReceiver( settingsCtrl );
				return true;
			}
		}
	} else if (event.EventType == EET_MOUSE_INPUT_EVENT) {
		if ( event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN || event.MouseInput.Event == EMIE_RMOUSE_PRESSED_DOWN ) {
			if ( !dlgVisible && selectedNodeId > 0 ) {
				stopMovement();
				dlgVisible = true;
				device->getCursorControl()->setActiveIcon( gui::ECI_NORMAL );
				device->getCursorControl()->setVisible( true );
				exDlg = new mdcExhibitDlg( guienv, driver, selectedNodeId );
				return true;
			}
		}
	} else if( event.EventType == irr::EET_GUI_EVENT ) {
		if( event.GUIEvent.EventType == irr::gui::EGET_ELEMENT_CLOSED ) {
			if ( exDlg != NULL ) {
				int w = settings->getScreenWidth();
				int h = settings->getScreenHeight();
				exDlg->closeWindow();
				delete exDlg;
				exDlg = NULL;
				device->getCursorControl()->setVisible( false );
				dlgVisible = false;
				guienv->setFocus( 0 );
				device->getCursorControl()->setPosition( core::vector2d<s32>( w / 2, h / 2 ) );
				return true;
			}
		}
	}
	return false;
}

void mdcApplication::onSettingsDialogHidden() {
	int w = settings->getScreenWidth();
	int h = settings->getScreenHeight();
	dlgVisible = false;

	const SKeyMap * f  = settings->getForwardKey();
	const SKeyMap * b  = settings->getBackwardKey();
	const SKeyMap * sl = settings->getStrafeLeftKey();
	const SKeyMap * sr = settings->getStrafeRightKey();

	scene->changeCameraKeyMaps( *f, *b, *sl, *sr );

	device->getCursorControl()->setVisible( false );
	device->setEventReceiver( this );

	guienv->setFocus( 0 );
	device->getCursorControl()->setPosition( core::vector2d<s32>( w / 2, h / 2 ) );
}

void mdcApplication::stopMovement() {
	SEvent fwEvt;
	SEvent::SKeyInput fKey;
	fwEvt.EventType = EET_KEY_INPUT_EVENT;
	fKey.Key = settings->getForwardKey()->KeyCode;
	fKey.PressedDown = false;
	fwEvt.KeyInput = fKey;
	device->postEventFromUser ( fwEvt );

	SEvent bkEvt;
	SEvent::SKeyInput bKey;
	bkEvt.EventType = EET_KEY_INPUT_EVENT;
	bKey.Key = settings->getBackwardKey()->KeyCode;
	bKey.PressedDown = false;
	bkEvt.KeyInput = bKey;
	device->postEventFromUser ( bkEvt );

	SEvent slEvt;
	SEvent::SKeyInput slKey;
	slEvt.EventType = EET_KEY_INPUT_EVENT;
	slKey.Key = settings->getStrafeLeftKey()->KeyCode;
	slKey.PressedDown = false;
	slEvt.KeyInput = slKey;
	device->postEventFromUser ( slEvt );

	SEvent srEvt;
	SEvent::SKeyInput srKey;
	srEvt.EventType = EET_KEY_INPUT_EVENT;
	srKey.Key = settings->getForwardKey()->KeyCode;
	srKey.PressedDown = false;
	srEvt.KeyInput = srKey;
	device->postEventFromUser ( srEvt );
}
