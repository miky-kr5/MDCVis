#ifndef EXHIBITDLG_H
#define EXHIBITDLG_H

#include "definitions.h"

enum ELEMENT_IDS {
	AA_COMB = 0x1000
};

class mdcExhibitDlg {
	public:
	mdcExhibitDlg( int );
	~mdcExhibitDlg();

	void                   closeWindow()             const;

	private:
};

#endif // EXHIBITDLG_H
