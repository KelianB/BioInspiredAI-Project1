EXECUTABLE=run.exe
 
CC=g++
LDFLAGS=
 
src = $(wildcard src/*.cpp) $(wildcard src/*/*.cpp)
obj = $(src:.cpp=.o)
 
all: myprog
 
myprog: $(obj)
	$(CC) -o $(EXECUTABLE) $^ $(LDFLAGS)
 
.PHONY: clean
clean:
	del $(obj) $(EXECUTABLE)