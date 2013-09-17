/**-----------------------------------------------------------------------------
; File:          application.cpp
; Description:   Implementation of the base application class.
; Authot:        Miguel Angel Astor, sonofgrendel@gmail.com
; Date created:  9/17/2013
; Date modified: 9/17/2013
;
; Copyright (C) 20013 Fundacion Museos Nacionales
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
#include "application.h"

Application::Application(){
    // Setup the application with the following
    device = irr::createDevice(video::EDT_OPENGL, // OpenGL rendering.
                               core::dimension2d<u32>(800, 600), // 800x600 screen.
                               16,    // 16 bits per pixel.
                               false, // No fullscreen.
                               false, // No stencil buffer.
                               false, // No vertical sync.
                               this); // Register this own class as event listener.
    device->setWindowCaption(L"Museo de Ciencias :: Demostracion 1");

    // Get pointers to the engine objects to avoid calling the getters at a later time.
    driver = device->getVideoDriver();
    smgr = device->getSceneManager();
    guienv = device->getGUIEnvironment();
}

Application::~Application(){
    // Delete all objects created by the engine.
    device->drop();
}

void Application::buildScene(){
    // Add a label to the window.
    guienv->addStaticText(L"Hello World! This is the Irrlicht OpenGL renderer!",
                          core::rect<int>(10,10,200,22),
                          true);

    // Load a mesh and set it's materials.
    scene::IAnimatedMesh* mesh = smgr->getMesh("media/sydney.md2");
    scene::IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode( mesh );
    if(node){
        node->setMaterialFlag(video::EMF_LIGHTING, false);
        node->setFrameLoop(0, 310);
        node->setMaterialTexture( 0, driver->getTexture("media/sydney.bmp") );
    }

    // Add the mesh to the scene.
    smgr->addCameraSceneNode(0, core::vector3df(0,30,-40), core::vector3df(0,5,0));
}

void Application::run(){
    // Render everything until the user requests the application to close itself.
    while(device->run()){
        driver->beginScene(true, true, video::SColor(0,200,200,200));
        smgr->drawAll();
        guienv->drawAll();
        driver->endScene();
    }
}

bool Application::OnEvent(const SEvent& event){
    // If the user pressed the ESC key then shutdown the engine.
    if(event.EventType == irr::EET_KEY_INPUT_EVENT){
        if(event.KeyInput.Key == irr::KEY_ESCAPE && event.KeyInput.PressedDown){
            device->closeDevice();
            return true;
        }
    }
    return false;
}
