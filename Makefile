OBJECTS = src/Application.o src/ExhibitDlg.o src/ExhibitMdl.o src/main.o src/Scene.o src/SettingsCtrl.o src/SettingsDlg.o src/SettingsMdl.o

ifeq ($(shell uname), Linux)
	include Linux.mk
else
	ifeq ($(shell uname -o), Msys)
		include Windows.mk
	endif
endif


all: FLAGS += -O3
all: $(OBJECTS)
	mkdir -p bin/
	cd src/
	$(COMPILER) -o $(TARGET) $(OBJECTS) $(FLAGS) $(INCLUDE) $(LIBDIRS) $(LIBS)
	cd ../

debug: FLAGS += -g
debug: $(OBJECTS)
	mkdir -p bin/
	cd src/
	$(COMPILER) -o $(TARGET) $(OBJECTS) $(FLAGS) $(INCLUDE) $(LIBDIRS) $(LIBS)
	cd ../

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
	$(RM) bin/ src/*.o -r
