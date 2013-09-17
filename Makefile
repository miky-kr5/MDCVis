TARGET = MDCVis
SOURCES = main.cpp
OBJECTS = main.o
CPPFLAGS = -I../include -I/usr/X11R6/include
ifeq ($(HOSTTYPE), x86_64)
LIBSELECT=64
endif
LDFLAGS = -L/usr/X11R6/lib$(LIBSELECT) -L../lib/Linux -lIrrlicht -lGL -lXxf86vm -lXext -lX11 -lXcursor

all: Release

Release: CXXFLAGS = -O3 -ffast-math
Release: BUILDPATH = ./obj/Release
Release: DESTPATH = ./bin/Release/$(TARGET)
Debug: DESTPATH = ./bin/Debug/$(TARGET)
Debug: BUILDPATH = ./obj/Debug
Debug: CXXFLAGS = -g -Wall
Release Debug: $(OBJECTS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(BUILDPATH)/$(OBJECTS) -o $(DESTPATH) $(LDFLAGS)

$(OBJECTS): $(SOURCES)
	$(CXX) $(CPPFLAGS) -c $< -o $(BUILDPATH)/$@

clean:
	$(RM) ./bin/Release/$(TARGET) ./bin/Debug/$(TARGET) ./obj/Release/*.o ./obj/Debug/*.o

