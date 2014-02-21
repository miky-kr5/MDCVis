#ifndef EXHIBITDLG_H
#define EXHIBITDLG_H

#include <irrlicht.h>

#include "definitions.h"
#include "ExhibitMdl.h"

enum ELEMENT_IDS {
	AA_COMB = 0x1000
};

class mdcExhibitDlg {
	public:
	mdcExhibitDlg( gui::IGUIEnvironment *, int );
	void                     closeWindow() const;

	private:
		gui::IGUIWindow *    windowSettings;
};

#endif // EXHIBITDLG_H
