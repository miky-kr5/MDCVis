/*------------------------------------------------------------------------------
; File:          Scene.hpp
; Description:   Declaration of the 3D scene loader class.
; Author:        Miguel Angel Astor, sonofgrendel@gmail.com
; Date created:  2/13/2014
;
; Copyright (C) 2014 Fundacion Museos Nacionales
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

#ifndef SCENE_H
#define SCENE_H

#include <irrlicht.h>

#include "definitions.hpp"

using namespace irr;
using core::stringw;

class mdcScene {
	public:
		mdcScene( IrrlichtDevice * );
		~mdcScene();

		void changeCameraKeyMaps( SKeyMap, SKeyMap, SKeyMap, SKeyMap )                 const;
		void addMeshToCollisionDetection( scene::IAnimatedMesh *, scene::ISceneNode *) const;
		scene::ICameraSceneNode * getCamera();

	private:
		scene::ICameraSceneNode                    *     camera;
		scene::ISceneNodeAnimatorCameraFPS         *     animator;
		scene::IMetaTriangleSelector               *     metaSelector;
		scene::ISceneNodeAnimatorCollisionResponse *     collider;
		scene::ISceneManager                       *     smgr;
};

#endif // SCENE_H
