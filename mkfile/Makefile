CFLAGS= -O3 -Wall -g -I. -I ./flann/ -I ./tdb/
DIR_SRC = .
SRC1 = $(wildcard $(DIR_SRC)/*.c)
SRC2 = $(wildcard $(DIR_SRC)/*.cpp)
OBJ1 = $(patsubst %.c, $(DIR_SRC)/%.o, $(notdir $(SRC1)))
OBJ2 = $(patsubst %.cpp,$(DIR_SRC)/%.o, $(notdir $(SRC2)))

all:
	@echo "OBJ1 = $(OBJ1) "
	@echo "obj2 = $(OBJ2) "
	make $(OBJ1)
	make $(OBJ2)
$(OBJ1):$(SRC1)
	gcc $(CFLAGS) -c $(patsubst %.o, $(DIR_SRC)/%.c, $(notdir $@)) -o $@
$(OBJ2):$(SRC2)
	g++ $(CFLAGS) -c $(patsubst %.o, $(DIR_SRC)/%.cpp, $(notdir $@)) -o $@

.PHONY: clean
clean:
	rm $(OBJ1) $(OBJ2)
