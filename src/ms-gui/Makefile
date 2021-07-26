LIBS=-lsfml-graphics -lsfml-window -lsfml-system
CXX := g++

all: ms-gui

%.o: %.cpp
	$(CXX) -c $< -o $@

%.o: %.hpp
	$(CXX) -c $< -o $@

ms-gui: main.o
	@echo "** Building the game"
	$(CXX) -o ms-gui main.o $(LIBS)

clean:
	@echo "** Removing object files and executable..."
	rm -f ms-gui *.o

install:
	@echo '** Installing...'
	cp ms-gui /usr/bin/

uninstall:
	@echo '** Uninstalling...'
	$(RM) /usr/bin/ms-gui
 
