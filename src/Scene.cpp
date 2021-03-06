/*------------------------------------------------------------------------------
; File:          Scene.cpp
; Description:   Implementation of the 3D scene loader class.
; Author:        Miguel Angel Astor, sonofgrendel@gmail.com
; Date created:  2/13/2014
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

#include <cassert>

#include "Scene.hpp"

#define FAR_UNITS             50000.0f
#define CAMERA_ROTATE_SPEED   100.0f
#define MOVEMENT_SPEED        0.5f
#define TRIANGLE_LIMIT        1024
#define MIN_POLYGONS          128

mdcScene::mdcScene( IrrlichtDevice * device ) {
	// Section names
	const stringw sceneTag         ( L"scene" );
	const stringw skyTag           ( L"skybox" );
	const stringw camTag           ( L"camera" );
	// Data tags
	const stringw modelTag         ( L"model" );
	const stringw sideTag          ( L"side" );
	const stringw vectTag          ( L"vector" );
	const stringw sTrue            ( L"1" );
	// Skybox sides;
	const stringw top              ( L"top" );
	const stringw bottom           ( L"bottom" );
	const stringw front            ( L"front" );
	const stringw back             ( L"back" );
	const stringw left             ( L"left" );
	const stringw right            ( L"right" );
	// Camera data
	const stringw camStart         ( L"start" );
	const stringw lookAt           ( L"look_at" );

	// Scene model and collision data.
	SKeyMap                                   keyMap[4];
	core::list< scene::ISceneNodeAnimator * > camAnimators;
	scene::IAnimatedMesh *                    mesh;
	scene::ISceneNode *                       node;
	scene::ITriangleSelector *                mapSelector;

	// Camera positions.
	float sx, sy, sz, lx, ly, lz;

	// Possible scene object flags.
	bool solid;
	bool alpha;
	bool visible;

	// XML reading helper strings.
	stringw currentSection;
	stringw key;
	stringw topTex;
	stringw bottomTex;
	stringw frontTex;
	stringw backTex;
	stringw leftTex;
	stringw rightTex;

	// Pointers to the relevant irrLicht managers.
	video::IVideoDriver *     driver = device->getVideoDriver();
	io::IXMLReader      *     xml    = device->getFileSystem()->createXMLReader("scene.xml");
	smgr = device->getSceneManager();

	// Setup the keyboard controls.
	keyMap[0].Action = EKA_MOVE_FORWARD;
	keyMap[0].KeyCode = KEY_KEY_W;

	keyMap[1].Action = EKA_MOVE_BACKWARD;
	keyMap[1].KeyCode = KEY_KEY_S;

	keyMap[2].Action = EKA_STRAFE_LEFT;
	keyMap[2].KeyCode = KEY_KEY_A;

	keyMap[3].Action = EKA_STRAFE_RIGHT;
	keyMap[3].KeyCode = KEY_KEY_D;


	// Load the scene file into the virtual filesystem.
	device->getFileSystem()->addFileArchive( "mdc.zip" );

	// Get the XML reader.
	xml = device->getFileSystem()->createXMLReader( "scene.xml" );

	// Disable mipmap generation.
	driver->setTextureCreationFlag( video::ETCF_CREATE_MIP_MAPS, false );

	// Create the metaSelector used for collision detection.
	metaSelector = smgr->createMetaTriangleSelector();

	// Read the scene file.
	while( xml->read() ) {
		switch( xml->getNodeType() ) {
			case irr::io::EXN_ELEMENT: {
				// If there is no current XML section then set it.
				if ( currentSection.empty() ) {

					if ( sceneTag.equals_ignore_case( xml->getNodeName() ) ) {
						currentSection = sceneTag;

					} else if ( skyTag.equals_ignore_case( xml->getNodeName() ) ) {
						currentSection = skyTag;

					} else if ( camTag.equals_ignore_case( xml->getNodeName() ) ){
						currentSection = camTag;
					}

				} else if ( currentSection.equals_ignore_case( sceneTag ) && modelTag.equals_ignore_case( xml->getNodeName() ) ) {
					// If we are in the scene section the load the corresponding model and
					// add it to the scene manager.

					key = xml->getAttributeValueSafe( L"name" );

					if ( !key.empty() ) {

						// Read the model attributes.
						solid   = sTrue.equals_ignore_case( xml->getAttributeValueSafe( L"solid" ) );
						visible = sTrue.equals_ignore_case( xml->getAttributeValueSafe( L"visible" ) );
						alpha   = sTrue.equals_ignore_case( xml->getAttributeValueSafe( L"alpha" ) );

						// Read the mesh and add it to the scene graph.
						mesh = smgr->getMesh( key );
						node = smgr->addOctreeSceneNode( mesh->getMesh( 0 ), NULL, -1, TRIANGLE_LIMIT );

						if( node != NULL ){
							// All models ignore lighting and normalize normals for future shader use.
							node->setMaterialFlag( video::EMF_LIGHTING, false );
							node->setMaterialFlag( video::EMF_NORMALIZE_NORMALS, true );

							// If the model uses a texture with alpha channel the load it
							if ( alpha ){
								node->setMaterialType( video::EMT_TRANSPARENT_ALPHA_CHANNEL );
								node->setMaterialFlag( video::EMF_BACK_FACE_CULLING, false );
							}else{
								// Else ignore the alpha channel.
								node->setMaterialType( video::EMT_SOLID );
								node->setMaterialFlag( video::EMF_BACK_FACE_CULLING, true );
							}

							// Just set the visibility.
							if ( !visible )
								node->setVisible(false);

							// If the model is solid then add it to the collision selectors.
							if ( solid ) {
								mapSelector = smgr->createOctreeTriangleSelector( mesh->getMesh(0), node, MIN_POLYGONS );
								metaSelector->addTriangleSelector( mapSelector );
								mapSelector->drop();
							}
						}

					}

				} else if ( currentSection.equals_ignore_case( skyTag ) && sideTag.equals_ignore_case( xml->getNodeName() ) ) {

					// If we are in the skybox section then load all skybox textures.
					key = xml->getAttributeValueSafe( L"name" );

					if ( !key.empty() ) {
						if ( key.equals_ignore_case( top ) ) {
							topTex = xml->getAttributeValueSafe( L"texture" );

						} else if ( key.equals_ignore_case( bottom ) ) {
							bottomTex = xml->getAttributeValueSafe( L"texture" );

						} else if ( key.equals_ignore_case( front ) ) {
							frontTex = xml->getAttributeValueSafe( L"texture" );

						} else if ( key.equals_ignore_case( back ) ) {
							backTex = xml->getAttributeValueSafe( L"texture" );

						} else if ( key.equals_ignore_case( left ) ) {
							leftTex = xml->getAttributeValueSafe( L"texture" );

						} else if ( key.equals_ignore_case( right ) ) {
							rightTex = xml->getAttributeValueSafe( L"texture" );
						}
					}
				} else if ( currentSection.equals_ignore_case( camTag ) && vectTag.equals_ignore_case( xml->getNodeName() ) ){
					// If we are in the camera section then load start and look at positions.
					key = xml->getAttributeValueSafe( L"name" );

					if ( !key.empty() ) {
						if ( key.equals_ignore_case( camStart ) ) {
							core::stringc x = xml->getAttributeValueSafe( L"x" );
							core::stringc y = xml->getAttributeValueSafe( L"y" );
							core::stringc z = xml->getAttributeValueSafe( L"z" );

							sx = core::fast_atof( x.c_str() );
							sy = core::fast_atof( y.c_str() );
							sz = core::fast_atof( z.c_str() );

						} else if ( key.equals_ignore_case( lookAt ) ){
							core::stringc x = xml->getAttributeValueSafe( L"x" );
							core::stringc y = xml->getAttributeValueSafe( L"y" );
							core::stringc z = xml->getAttributeValueSafe( L"z" );

							lx = core::fast_atof( x.c_str() );
							ly = core::fast_atof( y.c_str() );
							lz = core::fast_atof( z.c_str() );
						}
					}
				}
			}
			break;

			case irr::io::EXN_ELEMENT_END:
				// At the end of the section mark current section as empty so it can grab the next section.
				currentSection=L"";
				break;

			default:
				break;
		}
	}

	// Create the skybox.
	smgr->addSkyBoxSceneNode( driver->getTexture( topTex ),
							  driver->getTexture( bottomTex ),
							  driver->getTexture( leftTex ),
							  driver->getTexture( rightTex ),
							  driver->getTexture( frontTex ),
							  driver->getTexture( backTex ) );

	// Create the camera.
	camera = smgr->addCameraSceneNodeFPS( NULL, CAMERA_ROTATE_SPEED, MOVEMENT_SPEED, -1, keyMap, 4 );
	camera->setPosition( core::vector3df( sx, sy, sz ) );
	camera->setTarget( core::vector3df( lx, ly, lz ) );
	camera->setFarValue( FAR_UNITS );

	// Get the camera animator to disable vertical movement.
	// This animator will allow to change key mappings in other parts of the program.
	camAnimators = camera->getAnimators();
	core::list<scene::ISceneNodeAnimator*>::ConstIterator i = camAnimators.begin();
	for( ; i != camAnimators.end(); i++ ){
		scene::ISceneNodeAnimator * tempAnimator = *i;
		if( tempAnimator->getType() == scene::ESNAT_CAMERA_FPS ){
			( animator = static_cast< scene::ISceneNodeAnimatorCameraFPS * >( tempAnimator ) )->setVerticalMovement( false );
		}
	}

	// Create the collision detector and add it to the camera.
	collider = smgr->createCollisionResponseAnimator( metaSelector,
													  camera,
													  core::vector3df( 25, 50, 25 ),
													  core::vector3df( 0, -20.f, 0 ),
													  core::vector3df( 0, 60, 0 ),
													  0.05f );
	camera->addAnimator( collider );


	xml->drop();

	// Reenable mipmap creation.
	driver->setTextureCreationFlag( video::ETCF_CREATE_MIP_MAPS, true );
}

mdcScene::~mdcScene(){
	metaSelector->drop();
	collider->drop();
}

void mdcScene::addMeshToCollisionDetection( scene::IAnimatedMesh * mesh, scene::ISceneNode * node ) const {
	scene::ITriangleSelector * mapSelector;

	mapSelector = smgr->createOctreeTriangleSelector( mesh->getMesh(0), node, MIN_POLYGONS );
	metaSelector->addTriangleSelector( mapSelector );

	node->setTriangleSelector( mapSelector );

	mapSelector->drop();
}

scene::ICameraSceneNode * mdcScene::getCamera() {
	return camera;
}

void mdcScene::changeCameraKeyMaps( SKeyMap forward, SKeyMap backward, SKeyMap strafeL, SKeyMap strafeR ) const {
	SKeyMap keyMap[ 4 ];

	keyMap[ 0 ].Action  = EKA_MOVE_FORWARD;
	keyMap[ 0 ].KeyCode = forward.KeyCode;

	keyMap[ 1 ].Action  = EKA_MOVE_BACKWARD;
	keyMap[ 1 ].KeyCode = backward.KeyCode;

	keyMap[ 2 ].Action  = EKA_STRAFE_LEFT;
	keyMap[ 2 ].KeyCode = strafeL.KeyCode;

	keyMap[ 3 ].Action  = EKA_STRAFE_RIGHT;
	keyMap[ 3 ].KeyCode = strafeR.KeyCode;

	animator->setKeyMap(keyMap, 4);
}
