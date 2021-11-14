

#------- MEPHISTO MAKEFILE ------#


FLAGS = -ansi -std=c99 -Wall -g -DDEBUG
DEBUG = -DDEBUG -g
AR = ar rc
RANLIB = ranlib
CC = gcc -c
SRC = src
BIN = bin

GTKINC	= `PKG_CONFIG_PATH=/usr/share/pkgconfig pkg-config --cflags gtk+-2.0` -Wl,--export-dynamic
GTKLIBS	= `PKG_CONFIG_PATH=/usr/share/pkgconfig pkg-config --libs gtk+-2.0` -Wl,--export-dynamic

INC	= $(GTKINC)
LIBFLAG	= -L./$(SRC) -lGamePackage  
LIBS	= $(SRC)/libGamePackage.a 


OPTS	= -g


Makefile:  $(BIN)/ClientMephisto $(BIN)/Server
all: $(BIN)/ClientMephisto $(BIN)/Server
tar: Chat_Beta_src.tar.gz
usertar: Chat_Beta.tar.gz
testSocket:
	@echo "To run the examples, first start the server in one terminal,"
	@echo "then start one (or multiple) client(s) in another."
	@echo
	@echo "For example:"
	@echo "bin/Server 14222"
	@echo "bin/ClientMephisto crystalcove.eecs.uci.edu 14222"

#----- Object Files -----#

$(SRC)/FileIO.o: $(SRC)/FileIO.h $(SRC)/FileIO.c $(SRC)/ClientConst.h
	$(CC) $(SRC)/FileIO.c -o $(SRC)/FileIO.o $(FLAGS)

$(SRC)/Checks.o: $(SRC)/Checks.h $(SRC)/Checks.c $(SRC)/ClientConst.h 
	$(CC) -lm $(SRC)/Checks.c -o $(SRC)/Checks.o $(FLAGS)

$(SRC)/GameFunctions.o: $(SRC)/GameFunctions.h $(SRC)/GameFunctions.c $(SRC)/ClientConst.h  
	$(CC) $(SRC)/GameFunctions.c -o $(SRC)/GameFunctions.o $(FLAGS)

$(SRC)/MovePiece.o: $(SRC)/MovePiece.h $(SRC)/MovePiece.c $(SRC)/ClientConst.h
	$(CC) $(SRC)/MovePiece.c -o $(SRC)/MovePiece.o $(FLAGS)
# add more here #

$(SRC)/ClientMephisto.o: $(SRC)/ClientMephisto.c $(SRC)/ClientConst.h $(SRC)/MovePiece.h $(SRC)/Checks.h $(SRC)/FileIO.h
	$(CC) -rdynamic $(SRC)/ClientMephisto.c $(INC) $(OPTS) -o $(SRC)/ClientMephisto.o $(FLAGS)

#----- Socket Communication Files -----#

$(SRC)/ServerUtil.o: $(SRC)/ServerUtil.c $(SRC)/ServerConst.h  $(SRC)/ServerUtil.h $(SRC)/ListUtil.h
	$(CC) $(SRC)/ServerUtil.c $(FLAGS) -o $(SRC)/ServerUtil.o

$(SRC)/ServerFunctions.o: $(SRC)/ServerFunctions.c $(SRC)/ServerConst.h  $(SRC)/ServerFunctions.h $(SRC)/ListUtil.h 
	$(CC) $(SRC)/ServerFunctions.c $(FLAGS) -o $(SRC)/ServerFunctions.o

$(SRC)/ListUtil.o: $(SRC)/ListUtil.h $(SRC)/ListUtil.c $(SRC)/ServerConst.h  $(SRC)/ServerUtil.h 
	$(CC) $(SRC)/ListUtil.c $(FLAGS) -o $(SRC)/ListUtil.o

$(SRC)/ClientUtil.o: $(SRC)/ClientUtil.c $(SRC)/ClientConst.h  $(SRC)/ClientUtil.h
	$(CC) $(SRC)/ClientUtil.c $(FLAGS) -o $(SRC)/ClientUtil.o

$(SRC)/Client.o: $(SRC)/Client.c $(SRC)/ClientUtil.h $(SRC)/ClientConst.h $(SRC)/MovePiece.h $(SRC)/Checks.h	
	$(CC) $(SRC)/Client.c $(FLAGS) -o $(SRC)/Client.o

$(SRC)/Server.o: $(SRC)/Server.c $(SRC)/ServerConst.h  $(SRC)/ServerUtil.h $(SRC)/ListUtil.h $(SRC)/ServerFunctions.h
	$(CC) $(SRC)/Server.c $(FLAGS) -o $(SRC)/Server.o


#----- Library Files -----#


$(SRC)/libGamePackage.a: $(SRC)/FileIO.o $(SRC)/GameFunctions.o
	$(AR) $(SRC)/libGamePackage.a $(SRC)/FileIO.o  $(SRC)/GameFunctions.o
	$(RANLIB) $(SRC)/libGamePackage.a

$(SRC)/libAI.a: $(SRC)/AI.o $(SRC)/AIUtilities.o
	$(AR) $(SRC)/libAI.a $(SRC)/AI.o $(SRC)/AIUtilities.o
	$(RANLIB) $(SRC)/libAI.a


#----- Executable -----#

$(BIN)/ClientMephisto: $(SRC)/ClientMephisto.o $(SRC)/Checks.o $(SRC)/ClientUtil.o $(SRC)/MovePiece.o $(LIBS)
	gcc -rdynamic $(SRC)/ClientMephisto.o $(SRC)/Checks.o $(SRC)/ClientUtil.o $(SRC)/MovePiece.o $(LIBFLAG) $(OPTS) -o $(BIN)/ClientMephisto -lm $(GTKLIBS) $(FLAGS)

$(BIN)/Server: $(SRC)/Server.o $(SRC)/ServerUtil.o $(SRC)/ListUtil.o $(SRC)/ServerFunctions.o
	gcc $(SRC)/Server.o $(SRC)/ListUtil.o  $(SRC)/ServerUtil.o $(SRC)/ServerFunctions.o -o $(BIN)/Server $(FLAGS)

$(BIN)/Client: $(SRC)/Client.o $(SRC)/ClientUtil.o $(SRC)/Checks.o $(SRC)/MovePiece.o 	
	gcc $(SRC)/Client.o $(SRC)/ClientUtil.o $(SRC)/Checks.o $(SRC)/MovePiece.o  -o $(BIN)/Client $(FLAGS)


#----- Tar Balls -----#

Chat_Beta_src.tar.gz: Makefile INSTALL README COPYRIGHT doc/ bin/ src/
	tar -czvf Chat_Beta_src.tar.gz . --exclude='./.git' --exclude='./.gitignore' --exclude='./src/*.o' --exclude='./bin/*' --exclude='./src/*.a' --exclude='./*.gz'

Chat_Beta.tar.gz: INSTALL README COPYRIGHT doc/ bin/
	tar -czvf Chat_Beta.tar.gz . --exclude='./.git' --exclude='./.gitignore' --exclude='./src' --exclude='./bin/Client' --exclude='Makefile' --exclude='./*.gz' --exclude='./doc/P2_SoftwareSpec.pdf'


#----- Misc -----#

clean: 
	rm -f $(SRC)/*.o $(BIN)/* $(SRC)/*.a *.gz

	  
#* EOF *#
