/*------------------------------------------------------------------------------
; File:          SettingsCtrl.h
; Description:   Declaration of the settings controller class.
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

#ifndef SETTINGSCTRL_H
#define SETTINGSCTRL_H

#include <irrlicht.h>

#include "definitions.h"
#include "SettingsMdl.h"
#include "SettingsDlg.h"

class mdcSettingsCtrl : public irr::IEventReceiver {
	public:
		mdcSettingsCtrl( mdcApplication * );

		virtual bool        OnEvent( const irr::SEvent& );
		void                setDialog( mdcSettingsDlg * );
		void                invalidateDialog();

	private:
		mdcApplication *    app;
		mdcSettingsMdl *    settings;
		mdcSettingsDlg *    dialog;
};

#endif // SETTINGSCTRL_H
