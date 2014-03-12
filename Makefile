LINTARGET = bin/mdcvis
WINTARGET = bin/MDCVis.exe
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

ifeq ($(shell uname), Linux)
install:
	mkdir -p /opt/mdcvis
	cp $(TARGET) data/* /opt/mdcvis -rf
	cp LICENSE CREDITS.md README.md /opt/mdcvis
	echo > /usr/share/applications/mdc.desktop
	echo "[Desktop Entry]" >> /usr/share/applications/mdc.desktop
	echo "Type=Application" >> /usr/share/applications/mdc.desktop
	echo "Comment=Una aplicación para visitar el Museo de Ciencias de Caracas" >> /usr/share/applications/mdc.desktop
	echo "Version=1.0" >> /usr/share/applications/mdc.desktop
	echo "Name=Museo de Ciencias" >> /usr/share/applications/mdc.desktop
	echo "Exec=/opt/mdcvis/MDCVis %U" >> /usr/share/applications/mdc.desktop
	echo "Path=/opt/mdcvis" >> /usr/share/applications/mdc.desktop
	echo "StartupNotify=true" >> /usr/share/applications/mdc.desktop
	echo "Terminal=false" >> /usr/share/applications/mdc.desktop
	echo "Icon=/opt/mdcvis/mdcicon.png" >> /usr/share/applications/mdc.desktop
	echo "Categories=Education;Science;" >> /usr/share/applications/mdc.desktop
	echo >> /usr/share/applications/mdc.desktop
	chmod a+x /usr/share/applications/mdc.desktop

uninstall:
	$(RM) -rf /opt/mdcvis /usr/share/applications/mdc.desktop
endif

clean:
	$(RM) $(LINTARGET) $(WINTARGET) src/*.o mdcvis.res -r
