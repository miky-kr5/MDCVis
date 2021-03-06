/*------------------------------------------------------------------------------
; File:          SettingsDlg.hpp
; Description:   Declaration of the settings configuration dialog class.
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

#ifndef SETTINGSDLG_H
#define SETTINGSDLG_H

#include <irrlicht.h>

#include "definitions.hpp"
#include "SettingsMdl.hpp"

using namespace irr;

enum SETNG_GUI_ELEMENT_IDS {
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
		static int             w;
		static int             h;

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
