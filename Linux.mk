COMPILER = g++
FLAGS = -Wall
INCLUDE = -I./include -I/usr/X11R6/include
LIBDIRS = -L./lib -L/usr/x11R6/lib -L./lib/Linux 
LIBS = -lIrrlicht -lGL -lXxf86vm -lXext -lX11 -lXcursor -lsqlite3 -lpthread -ldl
TARGET = bin/MDCVis
