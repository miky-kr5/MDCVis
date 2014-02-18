/*------------------------------------------------------------------------------
; File:          Settings.cpp
; Description:   Implementation of the application settings class.
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

#include <cstdlib>
#include <cassert>
#include <cstring>
#include <cctype>
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>

#include "Settings.h"

using std::ifstream;
using std::ofstream;
using std::cout;
using std::cerr;
using std::endl;
using core::stringw;

const string DEF_SETTINGS = "<?xml version=\"1.0\"?>\n"
                            "<mdcvis>\n"
                            "  <video>\n"
                            "    <setting name=\"driver\"        value=\"OpenGL\">\n"
                            "    <setting name=\"fullscreen\"    value=\"1\">\n"
                            "    <setting name=\"antialiasing\"  value=\"2\">\n"
                            "    <setting name=\"vsync\"         value=\"1\">\n"
                            "    <setting name=\"resolution\"    value=\"800x600\">\n"
                            "  </video>\n"
                            "  <controls>\n"
                            "    <key name = \"forward\"  value=\"w\">\n"
                            "    <key name = \"backward\" value=\"s\">\n"
                            "    <key name = \"strafe_l\" value=\"a\">\n"
                            "    <key name = \"strafe_r\" value=\"d\">\n"
                            "  </controls>\n"
                            "  <audio>\n"
                            "    <!-- Unused -->\n"
                            "    <setting name = \"volume\" value=\"1.0\">\n"
                            "  </audio>\n"
                            "</mdcvis>\n";

mdcSettings * mdcSettings::instance = NULL;

mdcSettings * mdcSettings::getInstance() {
	if ( instance == NULL ) {
		instance = new mdcSettings();
	}

	instance->refs++;

	return instance;
}

void mdcSettings::freeInstance() {
	if ( instance != NULL ) {
		instance->refs--;

		if ( instance->refs == 0 ) {
			delete instance;
		}
	}
}

mdcSettings::mdcSettings(): refs( 0 ), changed(false) {
	screenDimensions = new core::dimension2d<u32>();

	char * userHome = getenv( "HOME" );
	if ( userHome != NULL ) {
		settingsPath = userHome;

#if defined( _WIN32 ) || defined( __MINGW32__ )
		settingsPath += "\\.mdcvis\\";
#elif defined( __linux__ )
		settingsPath += "/.mdcvis/";
#else
#error "Not a GNU/Linux or Windows platform."
#endif

		// Check if the settings directory exists.
		// It may not exist if this is the first time the app has been executed.
		if ( !settingsDirExists() ) {
			string command;
			int success;

			command = "mkdir " + settingsPath;
			success = system( command.c_str() );

			if ( success != 0 ) {
				canUseSettings = false;
			} else {
				canUseSettings = true;
			}
		} else {
			canUseSettings = true;
		}

		// Check if the settings file exists.
		// It may not exist if this is the first time the app has been executed.
		if ( !settingsFileExists() ) {
			createSettingsFile();
		}

		loadSettingsFile();
	}
}

mdcSettings::~mdcSettings() {
	delete screenDimensions;
}

/* Methods*/

void mdcSettings::createSettingsFile() {
	if ( canUseSettings ) {
		string settingsFile = settingsPath + "settings.xml";

		ofstream ofs( settingsFile.c_str(), ofstream::out | ofstream::trunc );
		ofs << DEF_SETTINGS;
		ofs.close();
	}
}

void mdcSettings::loadSettingsFile() {
	const stringw settingTag         ( L"setting" );
	const stringw keyTag             ( L"key" );
	// Section names
	const stringw videoTag           ( L"video" );
	const stringw audioTag           ( L"audio" );
	const stringw controlsTag        ( L"controls" );
	// Video tags
	const stringw driverName         ( L"driver" );
    const stringw fullScreenName     ( L"fullscreen" );
    const stringw antialiasingName   ( L"antialiasing" );
    const stringw vSyncName          ( L"vsync" );
    const stringw resolutionName     ( L"resolution" );
	// Controls tags
	const stringw forwardName        ( L"forward" );
	const stringw backwardName       ( L"backward" );
	const stringw strafeRightName    ( L"strafe_r" );
	const stringw strafeLeftName     ( L"strafe_l" );
	// Audio tags.
	const stringw volumeName         ( L"volume" );
	// Helper strings
	const stringw sTrue              ( L"1" );
	const stringw nullDriver         ( L"Null" );
	const stringw softDriver         ( L"Software" );
	const stringw burnDriver         ( L"Burnings" );
	const stringw dx8Driver          ( L"DirectX8" );
	const stringw dx9Driver          ( L"DirectX9" );
	const stringw oglDriver          ( L"OpenGL" );

	stringw                          key;

	if ( canUseSettings ) {
		stringw currentSection;
		core::stringw settingsFile =  settingsPath.c_str();
		settingsFile               += L"settings.xml";

		// Create an irrLicht XML file reader.
		irr::IrrlichtDevice * nullDevice = irr::createDevice(irr::video::EDT_NULL);
		if ( !nullDevice ){
			// If we could not get a null device then disable settings file I/O
			// and set default settings.
			cerr << "Failed to get an irrLicht  null device." << endl;

			canUseSettings = false;
			antialiasing = 0;
			fullScreen = false;
			screenDimensions->Width = 800;
			screenDimensions->Height = 600;
			vSync = false;
			driver = video::EDT_OPENGL;

			return;
		}

		io::IXMLReader* xml = nullDevice->getFileSystem()->createXMLReader(settingsFile);
		if ( !xml ){
			// If we could not get an XML reader, ditto.
			cerr << "Failed to create an irrLicht XML file reader." << endl;

			canUseSettings = false;
			antialiasing = 0;
			fullScreen = false;
			screenDimensions->Width = 800;
			screenDimensions->Height = 600;
			vSync = false;
			driver = video::EDT_OPENGL;

			return;
		}

		// Read the settings from the XML file.
		while( xml->read() ) {
			switch( xml->getNodeType() ) {
				case irr::io::EXN_ELEMENT: {

					if ( currentSection.empty() ) {

						if ( videoTag.equals_ignore_case( xml->getNodeName() ) ) {
							currentSection = videoTag;
						} else if ( audioTag.equals_ignore_case( xml->getNodeName() ) ) {
							currentSection = audioTag;
						} else if ( controlsTag.equals_ignore_case( xml->getNodeName() ) ) {
							currentSection = controlsTag;
						}

					} else if ( currentSection.equals_ignore_case( videoTag ) && settingTag.equals_ignore_case( xml->getNodeName() ) ) {

						key = xml->getAttributeValueSafe( L"name" );

						if ( !key.empty() ) {
							if ( key.equals_ignore_case( fullScreenName ) ) {
								fullScreen = sTrue.equals_ignore_case( xml->getAttributeValueSafe( L"value" ) );

							} else if ( key.equals_ignore_case( vSyncName ) ) {
								vSync = sTrue.equals_ignore_case( xml->getAttributeValueSafe( L"value" ) );

							} else if ( key.equals_ignore_case( driverName ) ) {
								stringw driverType = xml->getAttributeValueSafe( L"value" );

								if ( nullDriver.equals_ignore_case( driverType ) ) {
									driver = video::EDT_NULL;

								} else if ( softDriver.equals_ignore_case( driverType ) ) {
									driver = video::EDT_SOFTWARE;

								} else if ( burnDriver.equals_ignore_case( driverType ) ) {
									driver = video::EDT_BURNINGSVIDEO;

								} else if ( dx8Driver.equals_ignore_case( driverType ) ) {
									driver = video::EDT_DIRECT3D8;

								} else if ( dx9Driver.equals_ignore_case( driverType ) ) {
									driver = video::EDT_DIRECT3D9;

								} else if ( oglDriver.equals_ignore_case( driverType ) ) {
									driver = video::EDT_OPENGL;
								}

							}else if ( key.equals_ignore_case( antialiasingName ) ) {
        						core::stringc s = xml->getAttributeValueSafe( L"value" );
        						antialiasing = core::strtol10(s.c_str());

							}else if ( key.equals_ignore_case( resolutionName ) ) {
								int i;
								char * sp;
        						core::stringc s = xml->getAttributeValueSafe( L"value" );

        						screenDimensions->Width = core::strtol10(s.c_str());

        						sp = const_cast< char * >( s.c_str() );
        						for ( i = 0; sp[ i ]; ){
        							if( sp[ i++ ] == 'x' ) break;
        						}

        						screenDimensions->Height = core::strtol10( & ( s.c_str()[ i ] ) );
							}
						}

					} else if ( currentSection.equals_ignore_case( controlsTag ) && keyTag.equals_ignore_case( xml->getNodeName() ) ) {

						key = xml->getAttributeValueSafe( L"name" );

						if ( !key.empty() ) {
							if ( key.equals_ignore_case( forwardName ) ) {
								core::stringc f_key = xml->getAttributeValueSafe( L"value" );
								forward.Action = EKA_MOVE_FORWARD;
								setKeyMapKey( f_key[ 0 ], forward );

							} else if ( key.equals_ignore_case( backwardName ) ) {
								core::stringc b_key = xml->getAttributeValueSafe( L"value" );
								backward.Action = EKA_MOVE_BACKWARD;
								setKeyMapKey( b_key[ 0 ], backward );

							} else if ( key.equals_ignore_case( strafeLeftName ) ) {
								core::stringc s_key = xml->getAttributeValueSafe( L"value" );
								s_left.Action = EKA_STRAFE_LEFT;
								setKeyMapKey( s_key[ 0 ], s_left );

							} else if ( key.equals_ignore_case( strafeRightName ) ) {
								core::stringc s_key = xml->getAttributeValueSafe( L"value" );
								s_right.Action = EKA_STRAFE_RIGHT;
								setKeyMapKey( s_key[ 0 ], s_right );

							}
						}

					} else if ( currentSection.equals_ignore_case( audioTag ) && settingTag.equals_ignore_case( xml->getNodeName() ) ) {
						// UNUSED.
					}
				}
				break;

                case irr::io::EXN_ELEMENT_END:
                    currentSection=L"";
					break;

                default:
                	// Ignore.
                	break;
            }
        }

        xml->drop();
        nullDevice->drop();
	}
}

void mdcSettings::saveSettings() {
	if ( canUseSettings ) {
		string settingsFile = settingsPath + "settings.xml";

		ofstream ofs( settingsFile.c_str(), ofstream::out | ofstream::trunc );

		ofs << "<?xml version=\"1.0\"?>\n<mdcvis>\n  <video>\n";

		switch(driver) {
			case video::EDT_NULL:
				ofs << "    <setting name=\"driver\"        value=\"Null\">\n";
				break;

			case video::EDT_SOFTWARE:
				ofs << "    <setting name=\"driver\"        value=\"Software\">\n";
				break;

			case video::EDT_BURNINGSVIDEO:
				ofs << "    <setting name=\"driver\"        value=\"Burnings\">\n";
				break;

			case video::EDT_DIRECT3D8:
				ofs << "    <setting name=\"driver\"        value=\"DirectX8\">\n";
				break;

			case video::EDT_DIRECT3D9:
				ofs << "    <setting name=\"driver\"        value=\"DirectX9\">\n";
				break;

			case video::EDT_OPENGL:
				ofs << "    <setting name=\"driver\"        value=\"OpenGL\">\n";
				break;

			case video::EDT_COUNT:
				// Driver should never be EDT_COUNT.
				assert( !( driver == video::EDT_COUNT ) );
				break;
		}

		ofs << "    <setting name=\"fullscreen\"    value=\"" << ( fullScreen ? 1 : 0 ) << "\">\n";
		ofs << "    <setting name=\"antialiasing\"  value=\"" << antialiasing << "\">\n";
		ofs << "    <setting name=\"vsync\"         value=\"" << ( vSync ? 1 : 0 ) << "\">\n";
		ofs << "    <setting name=\"resolution\"    value=\"" << screenDimensions->Width << "x"
			<< screenDimensions->Height << "\">\n";

		ofs << "  </video>\n"
			"  <controls>\n"
			"    <key name = \"forward\"  value=\"w\">\n"
			"    <key name = \"backward\" value=\"s\">\n"
			"    <key name = \"strafe_l\" value=\"a\">\n"
			"    <key name = \"strafe_r\" value=\"d\">\n"
			"  </controls>\n"
			"  <audio>\n"
			"    <!-- Unused -->\n"
			"    <setting name = \"volume\" value=\"1.0\">\n"
			"  </audio>\n"
			"</mdcvis>\n";

		ofs.close();
	}
}

bool mdcSettings::settingsDirExists() {
	struct stat info;

	stat( settingsPath.c_str(), &info );

	if ( ( info.st_mode & S_IFDIR ) == S_IFDIR ) {
		return true;
	} else {
		return false;
	}
}

bool mdcSettings::settingsFileExists() {
	string settingsFile = settingsPath + "settings.xml";
	ifstream ifs( settingsFile.c_str() );

	if ( ifs.good() ) {
		ifs.close();
		return true;
	} else {
		ifs.close();
		return false;
	}
}

void mdcSettings::setKeyMapKey( char c, SKeyMap & key ) {
	char l_c = tolower( c );

	switch ( l_c ) {
		case 'a': key.KeyCode = KEY_KEY_A; break;
		case 'b': key.KeyCode = KEY_KEY_B; break;
		case 'c': key.KeyCode = KEY_KEY_C; break;
		case 'd': key.KeyCode = KEY_KEY_D; break;
		case 'e': key.KeyCode = KEY_KEY_E; break;
		case 'f': key.KeyCode = KEY_KEY_F; break;
		case 'g': key.KeyCode = KEY_KEY_G; break;
		case 'h': key.KeyCode = KEY_KEY_H; break;
		case 'i': key.KeyCode = KEY_KEY_I; break;
		case 'j': key.KeyCode = KEY_KEY_J; break;
		case 'k': key.KeyCode = KEY_KEY_K; break;
		case 'l': key.KeyCode = KEY_KEY_L; break;
		case 'm': key.KeyCode = KEY_KEY_M; break;
		case 'n': key.KeyCode = KEY_KEY_N; break;
		case 'o': key.KeyCode = KEY_KEY_O; break;
		case 'p': key.KeyCode = KEY_KEY_P; break;
		case 'q': key.KeyCode = KEY_KEY_Q; break;
		case 'r': key.KeyCode = KEY_KEY_R; break;
		case 's': key.KeyCode = KEY_KEY_S; break;
		case 't': key.KeyCode = KEY_KEY_T; break;
		case 'u': key.KeyCode = KEY_KEY_U; break;
		case 'v': key.KeyCode = KEY_KEY_V; break;
		case 'w': key.KeyCode = KEY_KEY_W; break;
		case 'x': key.KeyCode = KEY_KEY_X; break;
		case 'y': key.KeyCode = KEY_KEY_Y; break;
		case 'z': key.KeyCode = KEY_KEY_Z; break;
		case '0': key.KeyCode = KEY_KEY_0; break;
		case '1': key.KeyCode = KEY_KEY_1; break;
		case '2': key.KeyCode = KEY_KEY_2; break;
		case '3': key.KeyCode = KEY_KEY_3; break;
		case '4': key.KeyCode = KEY_KEY_4; break;
		case '5': key.KeyCode = KEY_KEY_5; break;
		case '6': key.KeyCode = KEY_KEY_6; break;
		case '7': key.KeyCode = KEY_KEY_7; break;
		case '8': key.KeyCode = KEY_KEY_8; break;
		case '9': key.KeyCode = KEY_KEY_9; break;
		case '^': key.KeyCode = KEY_UP;    break;
		case '<': key.KeyCode = KEY_LEFT;  break;
		case '>': key.KeyCode = KEY_RIGHT; break;
		case ',': key.KeyCode = KEY_DOWN;  break;
		default : assert( 0 );             break;
	}
}

/* Getters */
bool mdcSettings::settingsChanged(){
	return changed;
}

bool mdcSettings::isFullScreen() {
	return fullScreen;
}

bool mdcSettings::isVSyncEnabled() {
	return vSync;
}

unsigned int mdcSettings::getAntialiasingFactor() {
	return antialiasing;
}

video::E_DRIVER_TYPE mdcSettings::getVideoDriverType() {
	return driver;
}

u32 mdcSettings::getScreenWidth() {
	return screenDimensions->Width;
}

u32 mdcSettings::getScreenHeight() {
	return screenDimensions->Height;
}

const core::dimension2d<u32> * mdcSettings::getScreenDimensions() {
	return const_cast< const core::dimension2d<u32> * > ( screenDimensions );
}

SKeyMap * mdcSettings::getForwardKey() {
	return &forward;
}

SKeyMap * mdcSettings::getBackwardKey() {
	return &backward;
}

SKeyMap * mdcSettings::getStrafeLeftKey() {
	return &s_left;
}

SKeyMap * mdcSettings::getStrafeRightKey() {
	return &s_right;
}


/* Setters */
void mdcSettings::setFullScreen( bool isFullScreen) {
	changed = true;
	fullScreen = isFullScreen;
}

void mdcSettings::setVSyncEnabled( bool isVSyncEnabled ) {
	changed = true;
	vSync = isVSyncEnabled;
}

void mdcSettings::setAntialiasingFactor( antialiasing_t factor ) {
	changed = true;
	antialiasing = factor;
}

void mdcSettings::setVideoDriverType( video::E_DRIVER_TYPE newDriverType ) {
	changed = true;
	driver = newDriverType;
}

void mdcSettings::setScreenDimensions( const core::dimension2d<u32> * newDimensions ) {
	changed = true;
	screenDimensions = const_cast< core::dimension2d<u32> * >( newDimensions );
}

void mdcSettings::setForwardKey( char key ) {
	setKeyMapKey( key, forward );
}

void mdcSettings::setBackwardKey( char key ) {
	setKeyMapKey( key, backward );
}

void mdcSettings::setStrafeLeftKey( char key ) {
	setKeyMapKey( key, s_left );
}

void mdcSettings::setStrafeRightKey( char key ) {
	setKeyMapKey( key, s_right );
}
