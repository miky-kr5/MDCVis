/**-----------------------------------------------------------------------------
; File:          application.cpp
; Description:   Implementation of the base application class.
; Author:        Miguel Angel Astor, sonofgrendel@gmail.com
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
    core::dimension2d<u32> deskres;
    // Obtain the screen resolution.
    device = createDevice(video::EDT_NULL);
    deskres = device->getVideoModeList()->getDesktopResolution();
    device -> drop();
    // Setup the application with the following parameters.
    device = irr::createDevice(video::EDT_OPENGL, // OpenGL rendering.
                               deskres, // 800x600 screen.
                               32,    // 32 bits per pixel.
                               true,  // Fullscreen.
                               false, // No stencil buffer.
                               true, // No vertical sync.
                               this); // Register this own class as event listener.
    if(device == NULL) return;
    device->setWindowCaption(L"Museo de Ciencias :: Demostracion 1");
    device->getCursorControl()->setVisible(false);

    // Get pointers to the engine objects to avoid calling the getters at a later time.
    driver = device->getVideoDriver();
    smgr = device->getSceneManager();
    guienv = device->getGUIEnvironment();

    lastFPS = -1;
}

Application::~Application(){
    // Delete all objects created by the engine.
    if(device != NULL)
        device->drop();
}

void Application::buildScene(){
    scene::IAnimatedMesh* mesh;
    scene::ISceneNode* node;
    SKeyMap keyMap[9];
    scene::ICameraSceneNode* camera;
    core::list<scene::ISceneNodeAnimator*> camAnimators;
    scene::ITriangleSelector* mapSelector;
    scene::IMetaTriangleSelector* metaSelector;
    scene::ISceneNodeAnimatorCollisionResponse* collider;

    // Add a label to the window.
    if(device != NULL){
        guienv->addStaticText(L"Hello World! This is the Irrlicht OpenGL renderer!",
                              core::rect<int>(10,10,200,22),
                              true);

        // Load a mesh and set it's materials.
        mesh = smgr->getMesh("media/gfx/model/museo_demo.obj");
        node = smgr->addOctreeSceneNode(mesh->getMesh(0), 0, -1, 1024);
        if(node){
            node->setMaterialType(video::EMT_SOLID);
            node->setMaterialFlag(video::EMF_LIGHTING, false);
        }

        // Setup some lights.
        /*smgr->addLightSceneNode(0, core::vector3df(0, 0, -300), video::SColorf(1.0f, 1.0f, 0.8f, 0.05f), 1600.0f);
        smgr->addLightSceneNode(0, core::vector3df(0, 700, 0), video::SColorf(1.0f, 1.0f, 1.0f, 0.0f), 400.0f);
        smgr->addLightSceneNode(0, core::vector3df(0, 300, 300), video::SColorf(1.0f, 1.0f, 1.0f, 0.0f), 400.0f);*/

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
        camera = smgr->addCameraSceneNodeFPS(0, 100.0f, 0.5f, -1, keyMap, 8);
        camera->setPosition(core::vector3df(0, 700, 0));
        camera->setTarget(core::vector3df(0, 700, 1));
        camera->setFarValue(50000.0f);

        camAnimators = camera->getAnimators();
        core::list<scene::ISceneNodeAnimator*>::ConstIterator i = camAnimators.begin();
        for(; i != camAnimators.end(); i++){
            scene::ISceneNodeAnimator *animator = *i;
            if(animator->getType() == scene::ESNAT_CAMERA_FPS){
                (static_cast<scene::ISceneNodeAnimatorCameraFPS*>(animator))->setVerticalMovement(false);
            }
        }

        // Enable collisions.
        mapSelector = smgr->createOctreeTriangleSelector(mesh->getMesh(0), node, 128);
        metaSelector = smgr->createMetaTriangleSelector();
        metaSelector->addTriangleSelector(mapSelector);

        collider = smgr->createCollisionResponseAnimator(metaSelector,
                                                         camera,
                                                         core::vector3df(25,50,25),
                                                         core::vector3df(0, mesh ? -10.f : 0.0f, 0),
                                                         core::vector3df(0,45,0), 0.005f);
        camera->addAnimator(collider);
        collider->drop();

        // Create sky box
        driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);
        smgr->addSkyBoxSceneNode(driver->getTexture("./media/gfx/skybox/blue_sky.jpg"),
                                 driver->getTexture("./media/gfx/skybox/blue_sky.jpg"),
                                 driver->getTexture("./media/gfx/skybox/blue_sky.jpg"),
                                 driver->getTexture("./media/gfx/skybox/blue_sky.jpg"),
                                 driver->getTexture("./media/gfx/skybox/blue_sky.jpg"),
                                 driver->getTexture("./media/gfx/skybox/blue_sky.jpg"));
        driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);
    }
}

void Application::run(){
    int fps;
    core::stringw str;

    // Render everything until the user requests the application to close itself.
    if(device != NULL){
        while(device->run()){
            if(device->isWindowActive()){
                driver->beginScene(true, true, video::SColor(255, 97, 220, 220));
                smgr->drawAll();
                driver->endScene();

                fps = driver->getFPS();

                if(lastFPS != fps){
                    str = L"Museo de Ciencias :: Demostracion 1 [";
                    str += driver->getName();
                    str += "] FPS:";
                    str += fps;
                    device->setWindowCaption(str.c_str());
                    lastFPS = fps;
                }
            }else device->yield();
        }
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
