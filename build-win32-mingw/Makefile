CPP  = g++.exe
CC   = gcc.exe
WINDRES = windres.exe
RES  = Diamonds_private.res
OBJ  = DiamondsApp.o Constants.o SDLPanel.o SDL_util.o Ball.o Field.o HiScore.o Globals.o DiamondsFrame.o SDLSound.o Levelset.o Settings.o $(RES)
LINKOBJ  = DiamondsApp.o Constants.o SDLPanel.o SDL_util.o Ball.o Field.o HiScore.o Globals.o DiamondsFrame.o SDLSound.o Levelset.o Settings.o $(RES)
LIBS =  -L"lib" -mwindows -lmingw32 -lSDLmain -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf -lwxmsw28_core -lwxbase28 -lcomdlg32 -luser32 -lgdi32 -lole32 -lwsock32 -lcomctl32 -lctl3d32 -lgcc -lstdc++ -lshell32 -loleaut32 -ladvapi32 -luuid -lwinspool  -s 
INCS =  -I"include" 
CXXINCS =  -I"C:/MinGW/include/SDL" -I"C:/MinGW/include/wxWidgets"  -I"include" 
BIN  = diamonds.exe
CXXFLAGS = $(CXXINCS)   -Wall
CFLAGS = $(INCS)   -Wall
RM = rm -f

.PHONY: all all-before all-after clean clean-custom

all: all-before diamonds.exe all-after


clean: clean-custom
	${RM} $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CPP) $(LINKOBJ) -o "diamonds.exe" $(LIBS)

DiamondsApp.o: DiamondsApp.cpp
	$(CPP) -c DiamondsApp.cpp -o DiamondsApp.o $(CXXFLAGS)

Constants.o: Constants.cpp
	$(CPP) -c Constants.cpp -o Constants.o $(CXXFLAGS)

SDLPanel.o: SDLPanel.cpp
	$(CPP) -c SDLPanel.cpp -o SDLPanel.o $(CXXFLAGS)

SDL_util.o: SDL_util.cpp
	$(CPP) -c SDL_util.cpp -o SDL_util.o $(CXXFLAGS)

Ball.o: Ball.cpp
	$(CPP) -c Ball.cpp -o Ball.o $(CXXFLAGS)

Field.o: Field.cpp
	$(CPP) -c Field.cpp -o Field.o $(CXXFLAGS)

HiScore.o: HiScore.cpp
	$(CPP) -c HiScore.cpp -o HiScore.o $(CXXFLAGS)

Globals.o: Globals.cpp
	$(CPP) -c Globals.cpp -o Globals.o $(CXXFLAGS)

DiamondsFrame.o: DiamondsFrame.cpp
	$(CPP) -c DiamondsFrame.cpp -o DiamondsFrame.o $(CXXFLAGS)

SDLSound.o: SDLSound.cpp
	$(CPP) -c SDLSound.cpp -o SDLSound.o $(CXXFLAGS)

Levelset.o: Levelset.cpp
	$(CPP) -c Levelset.cpp -o Levelset.o $(CXXFLAGS)

Settings.o: Settings.cpp
	$(CPP) -c Settings.cpp -o Settings.o $(CXXFLAGS)

Diamonds_private.res: Diamonds_private.rc 
	$(WINDRES) -i Diamonds_private.rc --input-format=rc -o Diamonds_private.res -O coff 
