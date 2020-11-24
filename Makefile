
CUDA_VER?= 10.2
CXX:= g++

SRCS:= sobel.cpp

INCS:= $(wildcard *.h)
LIB:=sobel

OBJS:= $(SRCS:.cpp=.o)
PKGS:= opencv4

CFLAGS+=$(shell pkg-config --cflags $(PKGS))
LIBS+=$(shell pkg-config --libs $(PKGS))

all: $(LIB)

%.o: %.cpp $(INCS) Makefile
	@echo $(CFLAGS)
	$(CXX) -c -ggdb -o $@ $(CFLAGS) $<

$(LIB): $(OBJS) $(DEP) Makefile
	@echo $(CFLAGS)
	$(CXX) -o $@ $(OBJS) $(LIBS)

install: $(LIB)
	cp -rv $(LIB) /home/system/

clean:
	rm -rf $(OBJS) $(LIB)
