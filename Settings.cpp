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
	nullDevice = irr::createDevice(irr::video::EDT_NULL);

	char * userHome = getenv( "HOME" );
	if ( userHome != NULL ) {
		settingsPath = userHome;
		settingsPath += "/.mdcvis/";

		// Check if the settings directory exists.
		// It may not exists if this is the first time the app has been executed.
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
		// It may not exists if this is the first time the app has been executed.
		if ( !settingsFileExists() ) {
			createSettingsFile();
		}

		loadSettingsFile();

		nullDevice->drop();
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
	const stringw sFalse             ( L"0" );
	const stringw nullDriver         ( L"Null" );
	const stringw softDriver         ( L"Software" );
	const stringw burnDriver         ( L"Burnings" );
	const stringw dx8Driver          ( L"DirectX8" );
	const stringw dx9Driver          ( L"DirectX9" );
	const stringw oglDriver          ( L"OpenGL" );;

	if ( canUseSettings ) {
		stringw currentSection;
		core::stringw settingsFile = settingsPath.c_str();
		settingsFile += L"settings.xml";

		if ( !nullDevice ) return;

		irr::io::IXMLReader* xml = nullDevice->getFileSystem()->createXMLReader(settingsFile);
		if ( !xml ) return;

		//while there is more to read
		while( xml->read() ) {
			switch( xml->getNodeType() ) {
				case irr::io::EXN_ELEMENT: {
					// Set the current XML section we are parsing.
					if ( currentSection.empty() ) {

						if ( videoTag.equals_ignore_case( xml->getNodeName() ) ) {
							currentSection = videoTag;
						} else if ( audioTag.equals_ignore_case( xml->getNodeName() ) ) {
							currentSection = audioTag;
						} else if ( controlsTag.equals_ignore_case( xml->getNodeName() ) ) {
							currentSection = controlsTag;
						}

					} else if ( currentSection.equals_ignore_case( videoTag )
									&& settingTag.equals_ignore_case( xml->getNodeName() ) ) {
						// If we are parsing a setting in the video section.
						stringw key = xml->getAttributeValueSafe(L"name");

						if ( !key.empty() ) {
							if ( key.equals_ignore_case( fullScreenName ) ) {
								fullScreen = sTrue.equals_ignore_case( xml->getAttributeValueSafe(L"value") );

							} else if ( key.equals_ignore_case( vSyncName ) ) {
								vSync = sTrue.equals_ignore_case( xml->getAttributeValueSafe(L"value") );

							} else if ( key.equals_ignore_case( driverName ) ) {
								stringw driverType = xml->getAttributeValueSafe(L"value");

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
        						core::stringc s = xml->getAttributeValueSafe(L"value");
        						antialiasing = core::strtol10(s.c_str());

							}else if ( key.equals_ignore_case( resolutionName ) ) {
								int i;
								char * sp;
        						core::stringc s = xml->getAttributeValueSafe(L"value");

        						screenDimensions->Width = core::strtol10(s.c_str());

        						sp = const_cast< char * >( s.c_str() );
        						for ( i = 0; sp[i]; ){
        							if( sp[i++] == 'x' ) break;
        						}

        						screenDimensions->Height = core::strtol10( & ( s.c_str()[i] ) );
							}
						}

					} else if ( currentSection.equals_ignore_case( controlsTag )
									&& settingTag.equals_ignore_case( xml->getNodeName() ) ) {

					} else if ( currentSection.equals_ignore_case( audioTag )
									&& settingTag.equals_ignore_case( xml->getNodeName() ) ) {
					}
				}
				break;

                case irr::io::EXN_ELEMENT_END:
                    currentSection=L"";
					break;

                default:
                	break;
            }
        }

        xml->drop();
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
				assert(driver == video::EDT_COUNT);
				break;
		}

		ofs << "    <setting name=\"fullscreen\"    value=\"" << (fullScreen ? 1 : 0) << "\">\n";
		ofs << "    <setting name=\"antialiasing\"  value=\"" << antialiasing << "\">\n";
		ofs << "    <setting name=\"vsync\"         value=\"" << (vSync ? 1 : 0) << "\">\n";
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
