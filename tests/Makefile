CXX_FLAGS = -ggdb -pedantic-errors -Wall -Weffc++ -Wextra # -Wconversion
FILES = $(wildcard *.cpp)
DESTDIR = ../build

all: $(basename ${FILES})

%: %.cpp
	g++ ${CXX_FLAGS} -o ${DESTDIR}/$@ $<
	-./${DESTDIR}/$@ --reporter compact --success

clean: 
	cd ${DESTDIR} && rm $(basename ${FILES})
