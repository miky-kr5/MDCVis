/*------------------------------------------------------------------------------
; File:          ExhibitDlg.hpp
; Description:   Declaration of the exhibit dialog class.
; Author:        Miguel Angel Astor, sonofgrendel@gmail.com
; Date created:  2/21/2014
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

#ifndef EXHIBITDLG_H
#define EXHIBITDLG_H

#include <irrlicht.h>

#include "definitions.hpp"
#include "ExhibitMdl.hpp"

enum EXDLG_GUI_ELEMENT_IDS {
	EXDLG_WIN = 0x1000
};

class mdcExhibitDlg {
	public:
	mdcExhibitDlg( irr::gui::IGUIEnvironment *, video::IVideoDriver *, int );
	~mdcExhibitDlg();

	void                          closeWindow() const;

	private:
		static int                w;
		static int                h;
		mdcExhibitMdl        *    model;
		irr::gui::IGUIWindow *    win;
};

#endif // EXHIBITDLG_H
