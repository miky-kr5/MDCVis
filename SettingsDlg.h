/*------------------------------------------------------------------------------
; File:          SettingsDlg.h
; Description:   Declaration of the settings configuration dialog class.
; Author:        Miguel Angel Astor, sonofgrendel@gmail.com
; Date created:  18/02/2014
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

#ifndef SETTINGSDLG_H
#define SETTINGSDLG_H

#include <irrlicht.h>

#include "definitions.h"
#include "SettingsMdl.h"

using namespace irr;

enum ELEMENT_IDS {
	AA_COMB = 0x1000,
	RES_COMB,
	FS_CHECK,
	V_SYNC_CHECK,
	BTN_SAVE,
	BTN_CANCEL
};

class mdcSettingsDlg {
	public:
		mdcSettingsDlg( gui::IGUIEnvironment * );

		void                   closeWindow()             const;
		bool                   isFullScreenChecked()     const;
		bool                   isVSyncChecked()          const;
		aaFactor_t             getSelectedAAFactor()     const;
		unsigned int           getSelectedScreenWidth()  const;
		unsigned int           getSelectedScreenHeight() const;
		char                   getKeyMap( EKEY_ACTION )  const;

	private:
		mdcSettingsMdl    *    model;
		gui::IGUIWindow   *    windowSettings;
		gui::IGUIButton   *    buttonSave;
		gui::IGUIButton   *    buttonExit;
		gui::IGUICheckBox *    checkboxFullscreen;
		gui::IGUICheckBox *    checkboxVSync;
		gui::IGUIComboBox *    aaBox;
		gui::IGUIComboBox *    resBox;
		gui::IGUIComboBox *    frwBox;
		gui::IGUIComboBox *    bckBox;
		gui::IGUIComboBox *    stlBox;
		gui::IGUIComboBox *    strBox;
};

#endif // SETTINGSDLG_H
