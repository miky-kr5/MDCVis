/*------------------------------------------------------------------------------
; File:          Settings.h
; Description:   Declaration of the application settings class.
; Author:        Miguel Angel Astor, sonofgrendel@gmail.com
; Date created:  12/02/2014
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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>
#include <irrlicht.h>

using namespace irr;
using std::string;

typedef enum ANTIALIASING_FACTOR { NONE = 0, AA2 = 2, AA4 = 4, AA8 = 8, AA16 = 16 } antialiasing_t;

class mdcSettings{
	public:
		static mdcSettings *             getInstance();
		static void                      freeInstance();
		void                             saveSettings();
		bool                             settingsChanged();

		// Getters
		bool                             isFullScreen();
		bool                             isVSyncEnabled();
		unsigned int                     getAntialiasingFactor();
		video::E_DRIVER_TYPE             getVideoDriverType();
		u32                              getScreenWidth();
		u32                              getScreenHeight();
		const core::dimension2d<u32> *   getScreenDimensions();
		SKeyMap *                        getForwardKey();
		SKeyMap *                        getBackwardKey();
		SKeyMap *                        getStrafeLeftKey();
		SKeyMap *                        getStrafeRightKey();

		// Setters
		void                             setFullScreen( bool );
		void                             setVSyncEnabled( bool );
		void                             setAntialiasingFactor( antialiasing_t );
		void                             setVideoDriverType( video::E_DRIVER_TYPE );
		void                             setScreenDimensions( const core::dimension2d<u32> * );
		void                             setForwardKey( char );
		void                             setBackwardKey( char );
		void                             setStrafeLeftKey( char );
		void                             setStrafeRightKey( char );

	private:
		// Singleton instance and ref. counter
		static mdcSettings *             instance;
		int                              refs;

		string                           settingsPath;
		bool                             canUseSettings;
		bool                             changed;

		// Video Settings
		bool                             fullScreen;
		bool                             vSync;
		unsigned int                     antialiasing;
		video::E_DRIVER_TYPE             driver;
		core::dimension2d<u32> *         screenDimensions;

		// Key mappings
		SKeyMap                          forward;
		SKeyMap                          backward;
		SKeyMap                          s_left;
		SKeyMap                          s_right;

		// The default and copy constructors, the destructor and the assignment
		// operator are private to enforce the singleton design pattern.
		mdcSettings( );
		mdcSettings( mdcSettings const & ){ };
		~mdcSettings( );
		mdcSettings &                    operator=( mdcSettings const &){ return *instance; };

		// Helper methods
		void                             createSettingsFile();
		void                             loadSettingsFile();
		bool                             settingsDirExists();
		bool                             settingsFileExists();
		void                             setKeyMapKey(char, SKeyMap &);
};

#endif // SETTINGS_H
