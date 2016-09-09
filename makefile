# Project: gofish
# Makefile created by Dev-C++ 4.9.9.2
# Modified by jwebb

CPP  = g++
RES  = 
OBJ  = kmeans.o $(RES)
LINKOBJ  = kmeans.o $(RES)
BIN  = kmeans
CXXFLAGS = $(CXXINCS)  
CFLAGS = $(INCS)  
RM = rm -f


all: kmeans

clean: 
	${RM} $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CPP) $(LINKOBJ) -o kmeans $(LIBS)

card.o: card.cpp
	$(CPP) -c kmeans.cpp -o kmeans.o $(CXXFLAGS)