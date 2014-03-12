/*------------------------------------------------------------------------------
; File:          SettingsDlg.cpp
; Description:   Implementation of the settings configuration dialog class.
; Author:        Miguel Angel Astor, sonofgrendel@gmail.com
; Date created:  2/18/2014
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

#include "SettingsDlg.hpp"

using core::stringw;

#if defined( SPA )
static const stringw WIN_TITLE = L"Configuración";
static const stringw VIDEO_SET = L"Configuración del video";
static const stringw F_SCREEN  = L"Pantalla completa";
static const stringw V_SYNC    = L"Sincronización vertical";
static const stringw AA_FAC    = L"Factor de antialiasing";
static const stringw SCREEN_S  = L"Resolución de pantalla";
static const stringw KEY_SET   = L"Configuración de los controles";
static const stringw KEYF_BTN  = L"Avanzar";
static const stringw KEYB_BTN  = L"Retroceder";
static const stringw KEYSL_BTN = L"Moverse a la izquierda";
static const stringw KEYSR_BTN = L"Moverse a la derecha";
static const stringw SAVE_BTN  = L"Guardar";
static const stringw CNCL_BTN  = L"Cancelar";
static const stringw UP_ARROW  = L"Flecha arriba";
static const stringw DW_ARROW  = L"Flecha abajo";
static const stringw LF_ARROW  = L"Flecha izquierda";
static const stringw RT_ARROW  = L"Flecha derecha";
static const stringw VID_NOTE  = L"NOTA: Cambios al video necesitan reinicio";
#elif defined( ENG )
static const stringw WIN_TITLE = L"Settings";
static const stringw VIDEO_SET = L"Video configuration";
static const stringw F_SCREEN  = L"Fullscreen";
static const stringw V_SYNC    = L"Vertical sync";
static const stringw AA_FAC    = L"Antialiasing factor";
static const stringw SCREEN_S  = L"Screen resolution";
static const stringw KEY_SET   = L"Control configuration";
static const stringw KEYF_BTN  = L"Forward";
static const stringw KEYB_BTN  = L"Backward";
static const stringw KEYSL_BTN = L"Strafe left";
static const stringw KEYSR_BTN = L"Strafe right";
static const stringw SAVE_BTN  = L"Save";
static const stringw CNCL_BTN  = L"Cancel";
static const stringw UP_ARROW  = L"Up arrow";
static const stringw DW_ARROW  = L"Down arrow";
static const stringw LF_ARROW  = L"Left arrow";
static const stringw RT_ARROW  = L"Right arrow";
static const stringw VID_NOTE  = L"NOTA: Changes to video need restart";
#else
#error "No language defined."
#endif

#define getCorrectKeymap() ( action == EKA_MOVE_FORWARD ? settings->getForwardKey() : \
							( action == EKA_MOVE_BACKWARD ? settings->getBackwardKey() : \
							 ( action == EKA_STRAFE_LEFT ? settings->getStrafeLeftKey() : \
							  ( action == EKA_STRAFE_RIGHT ? settings->getStrafeRightKey() : NULL ) ) ) )

#define getCorrectComboForAction() ( action == EKA_MOVE_FORWARD ? frwBox : \
							        ( action == EKA_MOVE_BACKWARD ? bckBox : \
							         ( action == EKA_STRAFE_LEFT ? stlBox : \
							          ( action == EKA_STRAFE_RIGHT ? strBox : NULL ) ) ) )

static void setKeyMapComboBoxItems( gui::IGUIComboBox * );
static void setKeyMapComboBoxSelected( gui::IGUIComboBox *, mdcSettingsMdl *, enum EKEY_ACTION );

int mdcSettingsDlg::w = -1;
int mdcSettingsDlg::h = -1;

mdcSettingsDlg::mdcSettingsDlg( gui::IGUIEnvironment * env) {
	model = mdcSettingsMdl::getInstance();

	if ( w == -1 && h == -1) {
		w = model->getScreenWidth() / 2;
		h = model->getScreenHeight() / 2;
	}

    for ( s32 i=0; i < gui::EGDC_COUNT ; ++i ) {
        video::SColor col = env->getSkin()->getColor( ( gui::EGUI_DEFAULT_COLOR )i );
        col.setAlpha( 255 );
        env->getSkin()->setColor( ( irr::gui::EGUI_DEFAULT_COLOR )i, col );
    }

    windowSettings = env->addWindow( core::rect<s32>( w - 200, h - 200, w + 200, h + 200 ), true, WIN_TITLE.c_str() );
	windowSettings->setDraggable( false );

    env->addStaticText ( VIDEO_SET.c_str() , core::rect< s32 >( 10, 25, 200, 45), false, true, windowSettings );

    checkboxFullscreen = env->addCheckBox(
            model->isFullScreen(),
            core::rect<s32>( 20, 50, 220, 70 ), windowSettings, FS_CHECK,
            F_SCREEN.c_str() );

	checkboxVSync = env->addCheckBox(
            model->isVSyncEnabled(),
            core::rect<s32>( 20, 75, 300, 95 ), windowSettings, V_SYNC_CHECK,
            V_SYNC.c_str() );

	env->addStaticText ( AA_FAC.c_str() , core::rect< s32 >( 95, 103, 300, 123), false, true, windowSettings );

	aaBox = env->addComboBox( core::rect<s32>( 20, 100, 90, 120 ), windowSettings, AA_COMB );
    aaBox->addItem( L"x0" );
    aaBox->addItem( L"x2" );
    aaBox->addItem( L"x4" );
    aaBox->addItem( L"x8" );
    aaBox->addItem( L"x16" );

	switch( model->getAntialiasingFactor() ) {
		default:
		case 0:
			aaBox->setSelected( 0 );
			break;
		case 2:
			aaBox->setSelected( 1 );
			break;
		case 4:
			aaBox->setSelected( 2 );
			break;
		case 8:
			aaBox->setSelected( 3 );
			break;
		case 16:
			aaBox->setSelected( 4 );
			break;
	}

	env->addStaticText ( SCREEN_S.c_str() , core::rect< s32 >( 135, 128, 335, 148), false, true, windowSettings );

	resBox = env->addComboBox( core::rect<s32>( 20, 125, 130, 145 ), windowSettings, RES_COMB );
    resBox->addItem( L"640x480" );
    resBox->addItem( L"800x600" );
    resBox->addItem( L"1024x768" );
    resBox->addItem( L"1280x800" );
    resBox->addItem( L"1280x1024" );

	if ( model->getScreenWidth() == 640 && model->getScreenHeight() == 480 ) {
		resBox->setSelected( 0 );
	} else if( model->getScreenWidth() == 800 && model->getScreenHeight() == 600 ) {
		resBox->setSelected( 1 );
	} else if( model->getScreenWidth() == 1024 && model->getScreenHeight() == 768 ) {
		resBox->setSelected( 2 );
	} else if( model->getScreenWidth() == 1280 && model->getScreenHeight() == 800 ) {
		resBox->setSelected( 3 );
	} else if( model->getScreenWidth() == 1280 && model->getScreenHeight() == 1024 ) {
		resBox->setSelected( 4 );
	}

	env->addStaticText ( KEY_SET.c_str() , core::rect< s32 >( 10, 160, 350, 180), false, true, windowSettings );

	env->addStaticText ( KEYF_BTN.c_str() , core::rect< s32 >( 205, 185, 390, 205), false, true, windowSettings );
	frwBox = env->addComboBox( core::rect<s32>( 20, 185, 200, 205 ), windowSettings, RES_COMB );
	setKeyMapComboBoxItems( frwBox );
	setKeyMapComboBoxSelected( frwBox, model, EKA_MOVE_FORWARD );

	env->addStaticText ( KEYB_BTN.c_str() , core::rect< s32 >( 205, 210, 390, 230), false, true, windowSettings );
	bckBox = env->addComboBox( core::rect<s32>( 20, 210, 200, 230 ), windowSettings, RES_COMB );
	setKeyMapComboBoxItems( bckBox );
	setKeyMapComboBoxSelected( bckBox, model, EKA_MOVE_BACKWARD );

	env->addStaticText ( KEYSL_BTN.c_str() , core::rect< s32 >( 205, 235, 390, 255), false, true, windowSettings );
	stlBox = env->addComboBox( core::rect<s32>( 20, 235, 200, 255 ), windowSettings, RES_COMB );
	setKeyMapComboBoxItems( stlBox );
	setKeyMapComboBoxSelected( stlBox, model, EKA_STRAFE_LEFT );

	env->addStaticText ( KEYSR_BTN.c_str() , core::rect< s32 >( 205, 260, 390, 280), false, true, windowSettings );
	strBox = env->addComboBox( core::rect<s32>( 20, 260, 200, 280 ), windowSettings, RES_COMB );
	setKeyMapComboBoxItems( strBox );
	setKeyMapComboBoxSelected( strBox, model, EKA_STRAFE_RIGHT );

	env->addStaticText ( VID_NOTE.c_str() , core::rect< s32 >( 10, 340, 400, 360), false, true, windowSettings );

    buttonSave = env->addButton(
            core::rect<s32>( 220, 370, 300, 390 ), windowSettings, BTN_SAVE,
            SAVE_BTN.c_str() ) ;

    buttonExit = env->addButton(
            core::rect<s32>( 310, 370, 390, 390 ), windowSettings, BTN_CANCEL,
            CNCL_BTN.c_str() );
}

void mdcSettingsDlg::closeWindow() const {
	windowSettings->remove();
}

bool mdcSettingsDlg::isFullScreenChecked() const {
	return checkboxFullscreen->isChecked();
}

bool mdcSettingsDlg::isVSyncChecked() const {
	return checkboxVSync->isChecked();
}

aaFactor_t mdcSettingsDlg::getSelectedAAFactor() const {
	switch ( aaBox->getSelected() ){
		case 0:
			return NONE;
		case 1:
			return AA2;
		case 2:
			return AA4;
		case 3:
			return AA8;
		case 4:
			return AA16;
	}
	return NONE;
}

unsigned int mdcSettingsDlg::getSelectedScreenWidth()  const {
	switch ( resBox->getSelected() ) {
		case 0:
			return 640;
		case 1:
			return 800;
		case 2:
			return 1024;
		case 3:
		case 4:
			return 1280;
	}
	return 640;
}

unsigned int mdcSettingsDlg::getSelectedScreenHeight() const {
	switch ( resBox->getSelected() ) {
		case 0:
			return 480;
		case 1:
			return 600;
		case 2:
			return 768;
		case 3:
			return 800;
		case 4:
			return 1024;
	}
	return 480;
}

char mdcSettingsDlg::getKeyMap( EKEY_ACTION action) const {
	const gui::IGUIComboBox * combo = getCorrectComboForAction();

	switch ( combo->getSelected() ) {
		case 0:
			return '0';
		case 1:
			return '1';
		case 2:
			return '2';
		case 3:
			return '3';
		case 4:
			return '4';
		case 5:
			return '5';
		case 6:
			return '6';
		case 7:
			return '7';
		case 8:
			return '8';
		case 9:
			return '9';
		case 10:
			return 'a';
		case 11:
			return 'b';
		case 12:
			return 'c';
		case 13:
			return 'd';
		case 14:
			return 'e';
		case 15:
			return 'f';
		case 16:
			return 'g';
		case 17:
			return 'h';
		case 18:
			return 'i';
		case 19:
			return 'j';
		case 20:
			return 'k';
		case 21:
			return 'l';
		case 22:
			return 'm';
		case 23:
			return 'n';
		case 24:
			return 'o';
		case 25:
			return 'p';
		case 26:
			return 'q';
		case 27:
			return 'r';
		case 28:
			return 's';
		case 29:
			return 't';
		case 30:
			return 'u';
		case 31:
			return 'v';
		case 32:
			return 'w';
		case 33:
			return 'x';
		case 34:
			return 'y';
		case 35:
			return 'z';
		case 36:
			return '^';
		case 37:
			return ',';
		case 38:
			return '<';
		case 39:
			return '>';
		default:
			assert( 0 );
			break;
	}
}

static void setKeyMapComboBoxItems( gui::IGUIComboBox * combo) {
	combo->addItem( L"0" );
	combo->addItem( L"1" );
	combo->addItem( L"2" );
	combo->addItem( L"3" );
	combo->addItem( L"4" );
	combo->addItem( L"5" );
	combo->addItem( L"6" );
	combo->addItem( L"7" );
	combo->addItem( L"8" );
	combo->addItem( L"9" );
	combo->addItem( L"a" );
	combo->addItem( L"b" );
	combo->addItem( L"c" );
	combo->addItem( L"d" );
	combo->addItem( L"e" );
	combo->addItem( L"f" );
	combo->addItem( L"g" );
	combo->addItem( L"h" );
	combo->addItem( L"i" );
	combo->addItem( L"j" );
	combo->addItem( L"k" );
	combo->addItem( L"l" );
	combo->addItem( L"m" );
	combo->addItem( L"n" );
	combo->addItem( L"o" );
	combo->addItem( L"p" );
	combo->addItem( L"q" );
	combo->addItem( L"r" );
	combo->addItem( L"s" );
	combo->addItem( L"t" );
	combo->addItem( L"u" );
	combo->addItem( L"v" );
	combo->addItem( L"w" );
	combo->addItem( L"x" );
	combo->addItem( L"y" );
	combo->addItem( L"z" );
	combo->addItem( UP_ARROW.c_str() );
	combo->addItem( DW_ARROW.c_str() );
	combo->addItem( LF_ARROW.c_str() );
	combo->addItem( RT_ARROW.c_str() );
}

static void setKeyMapComboBoxSelected( gui::IGUIComboBox * combo, mdcSettingsMdl * settings, enum EKEY_ACTION action ) {
	const SKeyMap * key = getCorrectKeymap();

	if ( key != NULL ){
		switch ( key->KeyCode ) {
			case KEY_KEY_0:
				combo->setSelected( 0 );
				break;
			case KEY_KEY_1:
				combo->setSelected( 1 );
				break;
			case KEY_KEY_2:
				combo->setSelected( 2 );
				break;
			case KEY_KEY_3:
				combo->setSelected( 3 );
				break;
			case KEY_KEY_4:
				combo->setSelected( 4 );
				break;
			case KEY_KEY_5:
				combo->setSelected( 5 );
				break;
			case KEY_KEY_6:
				combo->setSelected( 6 );
				break;
			case KEY_KEY_7:
				combo->setSelected( 7 );
				break;
			case KEY_KEY_8:
				combo->setSelected( 8 );
				break;
			case KEY_KEY_9:
				combo->setSelected( 9 );
				break;
			case KEY_KEY_A:
				combo->setSelected( 10 );
				break;
			case KEY_KEY_B:
				combo->setSelected( 11 );
				break;
			case KEY_KEY_C:
				combo->setSelected( 12 );
				break;
			case KEY_KEY_D:
				combo->setSelected( 13 );
				break;
			case KEY_KEY_E:
				combo->setSelected( 14 );
				break;
			case KEY_KEY_F:
				combo->setSelected( 15 );
				break;
			case KEY_KEY_G:
				combo->setSelected( 16 );
				break;
			case KEY_KEY_H:
				combo->setSelected( 17 );
				break;
			case KEY_KEY_I:
				combo->setSelected( 18 );
				break;
			case KEY_KEY_J:
				combo->setSelected( 19 );
				break;
			case KEY_KEY_K:
				combo->setSelected( 20 );
				break;
			case KEY_KEY_L:
				combo->setSelected( 21 );
				break;
			case KEY_KEY_M:
				combo->setSelected( 22 );
				break;
			case KEY_KEY_N:
				combo->setSelected( 23 );
				break;
			case KEY_KEY_O:
				combo->setSelected( 24 );
				break;
			case KEY_KEY_P:
				combo->setSelected( 25 );
				break;
			case KEY_KEY_Q:
				combo->setSelected( 26 );
				break;
			case KEY_KEY_R:
				combo->setSelected( 27 );
				break;
			case KEY_KEY_S:
				combo->setSelected( 28 );
				break;
			case KEY_KEY_T:
				combo->setSelected( 29 );
				break;
			case KEY_KEY_U:
				combo->setSelected( 30 );
				break;
			case KEY_KEY_V:
				combo->setSelected( 31 );
				break;
			case KEY_KEY_W:
				combo->setSelected( 32 );
				break;
			case KEY_KEY_X:
				combo->setSelected( 33 );
				break;
			case KEY_KEY_Y:
				combo->setSelected( 34 );
				break;
			case KEY_KEY_Z:
				combo->setSelected( 35 );
				break;
			case KEY_UP:
				combo->setSelected( 36 );
				break;
			case KEY_DOWN:
				combo->setSelected( 37 );
				break;
			case KEY_LEFT:
				combo->setSelected( 38 );
				break;
			case KEY_RIGHT:
				combo->setSelected( 39 );
				break;
			default:
				assert( 0 );
				break;
		}
	}
}
