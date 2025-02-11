OBJS = Spirograph.o View\View.o Controller\Controller.o Model\Model.o
INCLUDES = -I../include
LIBS = -L../lib
LDFLAGS = -lglad -lglfw3 
CFLAGS = -g -std=c++11
PROGRAM = Spirograph

ifeq ($(OS),Windows_NT)     # is Windows_NT on XP, 2000, 7, Vista, 10...
    LDFLAGS += -lopengl32 -lgdi32
    PROGRAM := $(addsuffix .exe,$(PROGRAM))
    COMPILER = g++
    RM = cmd /C del
else ifeq ($(shell uname -s),Darwin)     # is MACOSX
    LDFLAGS += -framework Cocoa -framework OpenGL -framework IOKit
    COMPILER = clang++
    RM = rm -f
endif

Spirograph: clean $(OBJS)
	$(COMPILER) -o $(PROGRAM) $(OBJS) $(LIBS) $(LDFLAGS)

Spirograph.o: Spirograph.cpp
	$(COMPILER) $(INCLUDES) $(CFLAGS) -c Spirograph.cpp

View\View.o: View\View.cpp View\View.h
	$(COMPILER) $(INCLUDES) $(CFLAGS) -c View\View.cpp -o View\View.o

Controller\Controller.o: Controller\Controller.cpp Controller\Controller.h
	$(COMPILER) $(INCLUDES) $(CFLAGS) -c Controller\Controller.cpp -o Controller\Controller.o

Model\Model.o: Model\Model.cpp Model\Model.h
	$(COMPILER) $(INCLUDES) $(CFLAGS) -c Model\Model.cpp -o Model\Model.o

clean: 
	$(RM) "Spirograph.o" "View\View.o" "Controller\Controller.o" "Model\Model.o" "$(PROGRAM)"