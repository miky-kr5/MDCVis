/*------------------------------------------------------------------------------
; File:          SettingsCtrl.cpp
; Description:   Implementation of the settings controller class.
; Author:        Miguel Angel Astor, sonofgrendel@gmail.com
; Date created:  2/18/2014
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

#include "SettingsCtrl.h"
#include "SettingsDlg.h"
#include "Application.h"

#include <iostream>

mdcSettingsCtrl::mdcSettingsCtrl( mdcApplication * app ): dialog(NULL) {
	this->app = app;
	settings = mdcSettingsMdl::getInstance();
}

void mdcSettingsCtrl::setDialog( mdcSettingsDlg * dialog ) {
	this->dialog = dialog;
}

void mdcSettingsCtrl::invalidateDialog() {
	this->dialog = NULL;
}

bool mdcSettingsCtrl::OnEvent( const irr::SEvent& event ) {
	irr::s32 id;

	if ( event.EventType == irr::EET_GUI_EVENT ) {

		if ( event.GUIEvent.EventType == irr::gui::EGET_BUTTON_CLICKED ) {
			id = event.GUIEvent.Caller->getID();

			if ( id == BTN_SAVE ) {

				if ( dialog != NULL ) {
					unsigned int w, h;

					settings->setFullScreen( dialog->isFullScreenChecked() );
					settings->setVSyncEnabled( dialog->isVSyncChecked() );
					settings->setAntialiasingFactor( dialog->getSelectedAAFactor() );

					w  = dialog->getSelectedScreenWidth();
					h  = dialog->getSelectedScreenHeight();
					settings->setScreenDimensions(new irr::core::dimension2d< irr::u32 >( w, h ) );

					settings->setForwardKey( dialog->getKeyMap( irr::EKA_MOVE_FORWARD ) );
					settings->setBackwardKey( dialog->getKeyMap( irr::EKA_MOVE_BACKWARD ) );
					settings->setStrafeLeftKey( dialog->getKeyMap( irr::EKA_STRAFE_LEFT ) );
					settings->setStrafeRightKey( dialog->getKeyMap( irr::EKA_STRAFE_RIGHT ) );

					settings->saveSettings();

					dialog->closeWindow();
					invalidateDialog();

					app->onSettingsDialogHidden();
				}

			}else if ( id == BTN_CANCEL ) {

				if ( dialog != NULL ) {
					dialog->closeWindow();
					invalidateDialog();

					app->onSettingsDialogHidden();
				}
			}
		} else if( event.GUIEvent.EventType == irr::gui::EGET_ELEMENT_CLOSED ) {
			if ( dialog != NULL ) {
				dialog->closeWindow();
				invalidateDialog();

				app->onSettingsDialogHidden();
			}
		}
	}

	return false;
}
