/*------------------------------------------------------------------------------
; File:          ExhibitDlg.cpp
; Description:   Implementation of the exhibit dialog class.
; Author:        Miguel Angel Astor, sonofgrendel@gmail.com
; Date created:  2/21/2014
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

#include <cstdlib>

#include "SettingsMdl.hpp"
#include "ExhibitDlg.hpp"

#define WIN_W 600
#define WIN_H 400

using namespace::irr;
using core::stringw;

#if defined( SPA )
static const stringw DEF_TITLE = L"Exhibición no válida";
#elif defined( ENG )
static const stringw DEF_TITLE = L"Invalid exhibition";
#else
#error "No language defined."
#endif

static const stringw DEF_DESCR =
								L"Maecenas gravida velit mi, eget tincidunt nibh viverra in. Vestibulum facilisis"
								"molestie urna. Quisque scelerisque porta leo at congue. Sed pellentesque, augue"
								"vitae tincidunt tempor, tellus enim elementum est, et dapibus lectus neque vel"
								"dolor. Aliquam erat volutpat. Quisque eu nibh augue. Nulla facilisi. Fusce sed"
								"massa vitae est ultrices faucibus id in purus. Vivamus eget enim non elit"
								"porttitor venenatis at lobortis nisl. Morbi magna mauris, pretium quis egestas"
								"nec, dictum id quam. Mauris egestas porttitor lectus in laoreet. Sed tincidunt"
								"orci risus, ut convallis tellus volutpat nec. Mauris facilisis risus vel luctus"
								"cursus. Ut posuere augue quis adipiscing molestie. Nullam adipiscing, ligula"
								"eget malesuada pretium, est nunc mollis elit, in sagittis mi arcu eget metus.";

int mdcExhibitDlg::w = -1;
int mdcExhibitDlg::h = -1;

mdcExhibitDlg::mdcExhibitDlg( gui::IGUIEnvironment * gui, video::IVideoDriver * driver, int exId ) {
	stringw               title, desc, photo;
	mdcSettingsMdl   *    settings = mdcSettingsMdl::getInstance();
	char             *    exhibitTitle;
	char             *    exhibitDesc;
	char             *    photoPath;
	gui::IGUIImage   *    img;

	if ( w == -1 && h == -1) {
		w = settings->getScreenWidth() / 2;
		h = settings->getScreenHeight() / 2;
	}

	mdcSettingsMdl::freeInstance();

	for ( s32 i=0; i < gui::EGDC_COUNT ; ++i ) {
        video::SColor col = gui->getSkin()->getColor( ( gui::EGUI_DEFAULT_COLOR )i );
        col.setAlpha( 255 );
        gui->getSkin()->setColor( ( gui::EGUI_DEFAULT_COLOR )i, col );
    }

	model = mdcExhibitMdl::getInstance();

	model->getExhibitTitleById( &exhibitTitle, exId );
	model->getExhibitDescriptionById( &exhibitDesc, exId );

	if ( exhibitTitle != NULL ) {
		title = exhibitTitle;
		free( exhibitTitle );
	} else {
		title = DEF_TITLE;
	}

	if ( exhibitDesc != NULL ) {
		desc = exhibitDesc;
		free( exhibitDesc );
	} else {
		desc = DEF_DESCR;
	}

	win = gui->addWindow( core::rect<s32>( w - ( WIN_W / 2 ),
										   h - ( WIN_H / 2 ),
										   w + ( WIN_W / 2 ),
										   h + ( WIN_H / 2 ) ),
						  true,
						  title.c_str() );

	win->setDraggable( false );

	gui->addStaticText ( desc.c_str() , core::rect<s32>( 250, 40, WIN_W - 25, WIN_H - 25 ), true, true, win, -1, true );

	img = gui->addImage( core::rect<s32>( 25, 40, 240, WIN_H - 25 ), win, -1 );

	model->getExhibitPhotoPathById( &photoPath, exId );

	if ( photoPath != NULL ) {
		photo = photoPath;
		free( photoPath );
		img->setImage( driver->getTexture( photo.c_str() ) );
	}
}


mdcExhibitDlg::~mdcExhibitDlg() {
	mdcExhibitMdl::freeInstance();
}

void mdcExhibitDlg::closeWindow() const {
	win->remove();
}
