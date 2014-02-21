/*------------------------------------------------------------------------------
; File:          ExhibitMdl.h
; Description:   Declaration of the exhibit model class.
; Author:        Miguel Angel Astor, sonofgrendel@gmail.com
; Date created:  2/19/2014
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
#ifndef EXHIBITMDL_H
#define EXHIBITMDL_H

#include <string>

#include "definitions.h"

#define BAD_VALUE -1

using std::string;

class mdcExhibitMdl{
	public:
		static mdcExhibitMdl *    getInstance();
		static void               freeInstance();

		// Database management.
		bool                      setDatabaseFile( const char * );
		bool                      releaseDatabase();

		// Different methods to get row ids.
		int                       getNumOfExhibits();
		int                       getFirstNExhibitIds( int *, int );
		int                       getExhibitsIdsRange( int *, int, int );

		// Text getters.
		void                      getExhibitTitleById( char**, int );
		void                      getExhibitDescriptionById( char**, int );
		void                      getExhibitModelPathById( char**, int );
		void                      getExhibitPhotoPathById( char**, int );

		// Geometric transformation getters.
		void                      getTranslationById( vec3_t &, int );
		void                      getRotationById( vec3_t &, int );
		float                     getRotationAmountById( int );
		void                      getScalingById( vec3_t &, int );

	private:
		static mdcExhibitMdl *    instance;
		sqlite3              *    db;

		int                       refs;
		bool                      dbUsable;

		// Private for the singleton pattern.
		mdcExhibitMdl();
		mdcExhibitMdl( mdcExhibitMdl const & ) { };
		~mdcExhibitMdl();
		mdcExhibitMdl & operator=( mdcExhibitMdl const & ) { return *instance; }
};

#endif
