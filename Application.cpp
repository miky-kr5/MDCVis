/*------------------------------------------------------------------------------
; File:          application.cpp
; Description:   Implementation of the base application class.
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
#include <cstdlib>

#include "Application.h"

/*------------------------------------------------------------------------------
; Application::Application()
;
; Application constructor. Creates a new application, setting up some default
; parameters for the irrLicht Engine.
;-----------------------------------------------------------------------------*/
Application::Application(){
    core::dimension2d<u32> deskres;
    // Obtain the screen resolution.
    device = createDevice( video::EDT_NULL );
    deskres = device->getVideoModeList()->getDesktopResolution();
    device -> drop();
    // Setup the application with the following parameters.
    device = irr::createDevice( video::EDT_OPENGL,                     // OpenGL rendering.
                                core::dimension2d<u32>( 1024, 768 ),   // Screen size.
                                32,                                    // 32 bits per pixel.
                                true,                                 // Fullscreen status.
                                false,                                 // Stencil buffer.
                                true,                                  // V-sync status.
                                this );                                // Register this own class as its event listener.

    if( device == NULL )
        return;

    device->setWindowCaption( L"Museo de Ciencias :: Demostracion 1" );
    device->getCursorControl()->setVisible( false );

    // Get pointers to the engine objects to avoid calling the getters at a later time.
    driver = device->getVideoDriver();
    smgr = device->getSceneManager();
    guienv = device->getGUIEnvironment();

    lastFPS = -1;
}

/*------------------------------------------------------------------------------
; Application::~Application()
;
; Application destructor. Releases all the objects used by the application.
;-----------------------------------------------------------------------------*/
Application::~Application(){
    // Delete all objects created by the engine.
    if(device != NULL)
        device->drop();
}

/*------------------------------------------------------------------------------
; Application::buildScene()
;
; Loads all media files and constructs the demo scene. Also sets up the camera
; and some controls.
;-----------------------------------------------------------------------------*/
void Application::buildScene(){
    scene::IAnimatedMesh**                          meshes;
    scene::ISceneNode**                             nodes;
    SKeyMap                                        keyMap[9];
    scene::ICameraSceneNode*                       camera;
    core::list<scene::ISceneNodeAnimator*>         camAnimators;
    scene::ITriangleSelector*                      mapSelector;
    scene::IMetaTriangleSelector*                  metaSelector;
    scene::ISceneNodeAnimatorCollisionResponse*    collider;

    meshes = (scene::IAnimatedMesh**)malloc( 9 * sizeof(scene::IAnimatedMesh*) );
    nodes = (scene::ISceneNode**)malloc( 9 * sizeof(scene::ISceneNode*) );

    if( device != NULL ){
        // Load a mesh and set it's materials.
        meshes[0] = smgr->getMesh( "media/gfx/TestModel1/partes/balcones.obj" );
        nodes[0] = smgr->addOctreeSceneNode( meshes[0]->getMesh( 0 ), 0, -1, 1024 );
        if( nodes[0] ){
            nodes[0]->setMaterialType( video::EMT_SOLID );
            nodes[0]->setMaterialFlag( video::EMF_LIGHTING, true );
            nodes[0]->setMaterialFlag( video::EMF_NORMALIZE_NORMALS, true );
            //node->setScale( core::vector3df( 186, 186, 186 ) );
        }

        meshes[1] = smgr->getMesh( "media/gfx/TestModel1/partes/columnas.obj" );
        nodes[1] = smgr->addOctreeSceneNode( meshes[1]->getMesh( 0 ), 0, -1, 1024 );
        if( nodes[1] ){
            nodes[1]->setMaterialType( video::EMT_SOLID );
            nodes[1]->setMaterialFlag( video::EMF_LIGHTING, true );
            nodes[1]->setMaterialFlag( video::EMF_NORMALIZE_NORMALS, true );
            //node->setScale( core::vector3df( 186, 186, 186 ) );
        }

        meshes[2] = smgr->getMesh( "media/gfx/TestModel1/partes/escaleras.obj" );
        nodes[2] = smgr->addOctreeSceneNode( meshes[2]->getMesh( 0 ), 0, -1, 1024 );
        if( nodes[2] ){
            nodes[2]->setMaterialType( video::EMT_SOLID );
            nodes[2]->setMaterialFlag( video::EMF_LIGHTING, true );
            nodes[2]->setMaterialFlag( video::EMF_NORMALIZE_NORMALS, true );
            //node->setScale( core::vector3df( 186, 186, 186 ) );
        }

        meshes[3] = smgr->getMesh( "media/gfx/TestModel1/partes/marquesinas1.obj" );
        nodes[3] = smgr->addOctreeSceneNode( meshes[3]->getMesh( 0 ), 0, -1, 1024 );
        if( nodes[3] ){
            nodes[3]->setMaterialType( video::EMT_SOLID );
            nodes[3]->setMaterialFlag( video::EMF_LIGHTING, true );
            nodes[3]->setMaterialFlag( video::EMF_NORMALIZE_NORMALS, true );
            //node->setScale( core::vector3df( 186, 186, 186 ) );
        }

        meshes[4] = smgr->getMesh( "media/gfx/TestModel1/partes/marquesinas2.obj" );
        nodes[4] = smgr->addOctreeSceneNode( meshes[4]->getMesh( 0 ), 0, -1, 1024 );
        if( nodes[4] ){
            nodes[4]->setMaterialType( video::EMT_SOLID );
            nodes[4]->setMaterialFlag( video::EMF_LIGHTING, true );
            nodes[4]->setMaterialFlag( video::EMF_NORMALIZE_NORMALS, true );
            //node->setScale( core::vector3df( 186, 186, 186 ) );
        }

        meshes[5] = smgr->getMesh( "media/gfx/TestModel1/partes/paredes.obj" );
        nodes[5] = smgr->addOctreeSceneNode( meshes[5]->getMesh( 0 ), 0, -1, 1024 );
        if( nodes[5] ){
            nodes[5]->setMaterialType( video::EMT_SOLID );
            nodes[5]->setMaterialFlag( video::EMF_LIGHTING, true );
            nodes[5]->setMaterialFlag( video::EMF_NORMALIZE_NORMALS, true );
            //node->setScale( core::vector3df( 186, 186, 186 ) );
        }

        meshes[6] = smgr->getMesh( "media/gfx/TestModel1/partes/reja.obj" );
        nodes[6] = smgr->addOctreeSceneNode( meshes[6]->getMesh( 0 ), 0, -1, 1024 );
        if( nodes[6] ){
            nodes[6]->setMaterialType( video::EMT_SOLID );
            nodes[6]->setMaterialFlag( video::EMF_LIGHTING, true );
            nodes[6]->setMaterialFlag( video::EMF_NORMALIZE_NORMALS, true );
            //node->setScale( core::vector3df( 186, 186, 186 ) );
        }

        meshes[7] = smgr->getMesh( "media/gfx/TestModel1/partes/suelo.obj" );
        nodes[7] = smgr->addOctreeSceneNode( meshes[7]->getMesh( 0 ), 0, -1, 1024 );
        if( nodes[7] ){
            nodes[7]->setMaterialType( video::EMT_SOLID );
            nodes[7]->setMaterialFlag( video::EMF_LIGHTING, true );
            nodes[7]->setMaterialFlag( video::EMF_NORMALIZE_NORMALS, true );
            //node->setScale( core::vector3df( 186, 186, 186 ) );
        }

        meshes[8] = smgr->getMesh( "media/gfx/TestModel1/partes/techo.obj" );
        nodes[8] = smgr->addOctreeSceneNode( meshes[8]->getMesh( 0 ), 0, -1, 1024 );
        if( nodes[8] ){
            nodes[8]->setMaterialType( video::EMT_SOLID );
            nodes[8]->setMaterialFlag( video::EMF_LIGHTING, true );
            nodes[8]->setMaterialFlag( video::EMF_NORMALIZE_NORMALS, true );
            //node->setScale( core::vector3df( 186, 186, 186 ) );
        }

        // Setup some lights.
        //smgr->addLightSceneNode( 0, core::vector3df( 0, 0, -300 ), video::SColorf( 1.0f, 1.0f, 0.8f, 0.05f ), 1600.0f );
        smgr->addLightSceneNode( 0, core::vector3df( 0, 700, 0 ), video::SColorf( 1.0f, 1.0f, 1.0f, 0.0f ), 400.0f );
        //smgr->addLightSceneNode( 0, core::vector3df( 0, 300, 300 ), video::SColorf( 1.0f, 1.0f, 1.0f, 0.0f ), 400.0f );

        // Setup the keyboard controls.
        keyMap[0].Action = EKA_MOVE_FORWARD;
        keyMap[0].KeyCode = KEY_UP;
        keyMap[1].Action = EKA_MOVE_FORWARD;
        keyMap[1].KeyCode = KEY_KEY_W;

        keyMap[2].Action = EKA_MOVE_BACKWARD;
        keyMap[2].KeyCode = KEY_DOWN;
        keyMap[3].Action = EKA_MOVE_BACKWARD;
        keyMap[3].KeyCode = KEY_KEY_S;

        keyMap[4].Action = EKA_STRAFE_LEFT;
        keyMap[4].KeyCode = KEY_LEFT;
        keyMap[5].Action = EKA_STRAFE_LEFT;
        keyMap[5].KeyCode = KEY_KEY_A;

        keyMap[6].Action = EKA_STRAFE_RIGHT;
        keyMap[6].KeyCode = KEY_RIGHT;
        keyMap[7].Action = EKA_STRAFE_RIGHT;
        keyMap[7].KeyCode = KEY_KEY_D;

        keyMap[8].Action = EKA_JUMP_UP;
        keyMap[8].KeyCode = KEY_SPACE;

        // Setup the camera.
        camera = smgr->addCameraSceneNodeFPS( 0, 100.0f, 0.5f, -1, keyMap, 8 );
        camera->setPosition( core::vector3df( 0, 700, 0 ) );
        camera->setTarget( core::vector3df( 0, 700, 1 ) );
        camera->setFarValue( 50000.0f );

        camAnimators = camera->getAnimators();
        core::list<scene::ISceneNodeAnimator*>::ConstIterator i = camAnimators.begin();
        for( ; i != camAnimators.end(); i++ ){
            scene::ISceneNodeAnimator *animator = *i;
            if( animator->getType() == scene::ESNAT_CAMERA_FPS ){
                ( static_cast<scene::ISceneNodeAnimatorCameraFPS*>( animator ) )->setVerticalMovement( false );
            }
        }

        // Enable collisions.
        metaSelector = smgr->createMetaTriangleSelector();
        for(int i = 0; i < 9; i++){
            mapSelector = smgr->createOctreeTriangleSelector( meshes[i]->getMesh(0), nodes[i], 128 );
            metaSelector->addTriangleSelector( mapSelector );
        }

        collider = smgr->createCollisionResponseAnimator( metaSelector,
                                                          camera,
                                                          core::vector3df( 25, 50, 25 ),
                                                          core::vector3df( 0, -10.f, 0 ),
                                                          core::vector3df( 0, 45, 0 ),
                                                          0.005f );
        camera->addAnimator( collider );
        collider->drop();

        // Create sky box
        driver->setTextureCreationFlag( video::ETCF_CREATE_MIP_MAPS, false );
        smgr->addSkyBoxSceneNode( driver->getTexture( "./media/gfx/skybox/top.png" ),
                                  driver->getTexture( "./media/gfx/skybox/bottom.png" ),
                                  driver->getTexture( "./media/gfx/skybox/sides.png" ),
                                  driver->getTexture( "./media/gfx/skybox/sides.png" ),
                                  driver->getTexture( "./media/gfx/skybox/sides.png" ),
                                  driver->getTexture( "./media/gfx/skybox/sides.png" ) );
        driver->setTextureCreationFlag( video::ETCF_CREATE_MIP_MAPS, true );
    }
}

/*------------------------------------------------------------------------------
; Application::run()
;
; Main loop of the application. It renders the demo scene and calculates the
; fps count.
;-----------------------------------------------------------------------------*/
void Application::run(){
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
                    str = L"Museo de Ciencias :: Demostracion 1 [";
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
bool Application::OnEvent( const SEvent& event ){
    if( event.EventType == irr::EET_KEY_INPUT_EVENT ){
        if( event.KeyInput.Key == irr::KEY_ESCAPE && event.KeyInput.PressedDown ){
            device->closeDevice();
            return true;
        }
    }
    return false;
}
