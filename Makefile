VERSION = 1.1-1
LINTARGET = bin/mdcvis
WINTARGET = bin/MDCVis.exe
APPDATA = data/exhibits data/font data/gfx data/mdc.zip data/mdcicon.png LICENSE CREDITS.md README.md
LINSETUP = mdcvis.deb
WINSETUP = MDCVis_setup.exe
COMPILER = g++
FLAGS = -Wall
INCLUDE = -I./include
OBJECTS = src/Application.o src/ExhibitDlg.o src/ExhibitMdl.o src/main.o src/Scene.o src/SettingsCtrl.o src/SettingsDlg.o src/SettingsMdl.o

all: FLAGS += -O3
all: INCLUDE += -I/usr/X11R6/include
all: LIBDIRS = -L/usr/x11R6/lib -L./lib/Linux 
all: LIBS = -lIrrlicht -lGL -lXxf86vm -lXext -lX11 -lXcursor -lsqlite3 -lpthread -ldl
all: $(LINTARGET)

debug: FLAGS += -g
debug: INCLUDE += -I/usr/X11R6/include
debug: LIBDIRS = -L/usr/x11R6/lib -L./lib/Linux
debug: LIBS = -lIrrlicht -lGL -lXxf86vm -lXext -lX11 -lXcursor -lsqlite3 -lpthread -ldl
debug: $(LINTARGET)

windows: FLAGS += -O3
windows: LIBDIRS = -L./lib/Windows
windows: LIBS = -lIrrlicht -lopengl32 -lsqlite3 -lpthreadGC2 -ldl -static-libgcc -static-libstdc++
windows: $(WINTARGET)	

debug-windows: FLAGS += -g
debug-windows: LIBDIRS = -L./lib/Windows
debug-windows: LIBS = -lIrrlicht -lopengl32 -lsqlite3 -lpthreadGC2 -ldl -static-libgcc -static-libstdc++
debug-windows: $(WINTARGET)

$(LINTARGET): $(OBJECTS)
	$(COMPILER) -o $(LINTARGET) $(OBJECTS) $(FLAGS) $(INCLUDE) $(LIBDIRS) $(LIBS)

$(WINTARGET): $(OBJECTS) mdcvis.res
	$(COMPILER) -o $(WINTARGET) $(OBJECTS) mdcvis.res $(FLAGS) $(INCLUDE) $(LIBDIRS) $(LIBS)

mdcvis.res: mdcvis.rc
	windres mdcvis.rc -O coff -o mdcvis.res

src/Application.o: src/Application.cpp src/Application.hpp src/definitions.hpp
	$(COMPILER) -o $@ -c $< $(FLAGS) $(INCLUDE)

src/ExhibitDlg.o: src/ExhibitDlg.cpp src/ExhibitDlg.hpp src/definitions.hpp
	$(COMPILER) -o $@ -c $< $(FLAGS) $(INCLUDE)

src/ExhibitMdl.o: src/ExhibitMdl.cpp src/ExhibitMdl.hpp src/definitions.hpp
	$(COMPILER) -o $@ -c $< $(FLAGS) $(INCLUDE)

src/main.o: src/main.cpp
	$(COMPILER) -o $@ -c $< $(FLAGS) $(INCLUDE)

src/Scene.o: src/Scene.cpp src/Scene.hpp src/definitions.hpp
	$(COMPILER) -o $@ -c $< $(FLAGS) $(INCLUDE)

src/SettingsCtrl.o: src/SettingsCtrl.cpp src/SettingsCtrl.hpp src/definitions.hpp
	$(COMPILER) -o $@ -c $< $(FLAGS) $(INCLUDE)

src/SettingsDlg.o: src/SettingsDlg.cpp src/SettingsDlg.hpp src/definitions.hpp
	$(COMPILER) -o $@ -c $< $(FLAGS) $(INCLUDE)

src/SettingsMdl.o: src/SettingsMdl.cpp src/SettingsMdl.hpp src/definitions.hpp
	$(COMPILER) -o $@ -c $< $(FLAGS) $(INCLUDE)

setup: $(WINSETUP)

$(WINSETUP): $(WINTARGET)
	makensis MDCVis.nsi

ifeq ($(shell uname), Linux)
deb: $(LINSETUP)
	md MDCVis
	md MDCVis exhibits
	copy $(TARGET) data\mdc.zip lib\Windows\Irrlicht.dll MDCVis
	

$(LINSETUP): $(LINTARGET)
	mkdir -p mdcvis_$(VERSION)/opt/mdcvis
	mkdir -p mdcvis_$(VERSION)/usr/share/app-install/desktop
	mkdir -p mdcvis_$(VERSION)/DEBIAN/
	cp $(LINTARGET) $(APPDATA) mdcvis_$(VERSION)/opt/mdcvis -rf
	cp data/museo_ciencias-mdcvis.desktop mdcvis_$(VERSION)/usr/share/app-install/desktop
	echo "Package: mdcvis" > mdcvis_$(VERSION)/DEBIAN/control
	echo "Version:" $(VERSION) >> mdcvis_$(VERSION)/DEBIAN/control
	echo "Section: Education, Science" >> mdcvis_$(VERSION)/DEBIAN/control
	echo "Priority: optional" >> mdcvis_$(VERSION)/DEBIAN/control
	echo "Architecture: i386" >> mdcvis_$(VERSION)/DEBIAN/control
	echo "Installed-Size: 22833" >> mdcvis_$(VERSION)/DEBIAN/control
	echo "Depends: libc6 (>= 2.15), libstdc++6 (>= 4.6.3), libxxf86vm1 (>= 1.1.1)" >> mdcvis_$(VERSION)/DEBIAN/control
	echo "Maintainer: Miguel Angel Astor Romero <sonofgrendel@gmail.com>" >> mdcvis_$(VERSION)/DEBIAN/control
	echo "Description: Visitas virtuales en el Museo de Ciencias de Caracas" >> mdcvis_$(VERSION)/DEBIAN/control
	echo " Esta aplicación permite realizar visitas dentro de una representación">> mdcvis_$(VERSION)/DEBIAN/control
	echo " virtual del edificio histórico del Museo de Ciencias de Caracas." >> mdcvis_$(VERSION)/DEBIAN/control
	echo "#!/bin/sh" > mdcvis_$(VERSION)/DEBIAN/postinst
	echo "" >> mdcvis_$(VERSION)/DEBIAN/postinst
	echo "xdg-desktop-menu install /usr/share/app-install/desktop/museo_ciencias-mdcvis.desktop" >> mdcvis_$(VERSION)/DEBIAN/postinst
	echo "#!/bin/sh" > mdcvis_$(VERSION)/DEBIAN/prerm
	echo "" >> mdcvis_$(VERSION)/DEBIAN/prerm
	echo "xdg-desktop-menu uninstall /usr/share/app-install/desktop/museo_ciencias-mdcvis.desktop" >> mdcvis_$(VERSION)/DEBIAN/prerm
	chmod 755 mdcvis_$(VERSION)/DEBIAN/postinst mdcvis_$(VERSION)/DEBIAN/prerm
	chown -R root mdcvis_$(VERSION)/
	dpkg-deb --build mdcvis_$(VERSION)
	$(RM) -r mdcvis_$(VERSION)

install:
	mkdir -p /opt/mdcvis
	cp $(TARGET) data/* /opt/mdcvis -rf
	cp LICENSE CREDITS.md README.md /opt/mdcvis
	cp data/museo_ciencias-mdcvis.desktop /usr/share/applications/
	chown root /usr/share/applications/mdc.desktop 
	chgrp root /usr/share/applications/mdc.desktop 
	chmod 755 /usr/share/applications/mdc.desktop

uninstall:
	$(RM) -rf /opt/mdcvis /usr/share/applications/mdc.desktop
endif

clean:
	$(RM) $(LINTARGET) $(WINTARGET) $(OBJECTS) mdcvis.res $(LINSETUP) $(WINSETUP) -r
