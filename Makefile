exec = a.out
sources := $(wildcard src/*.c)
sources += $(wildcard GL/src/*.c)
objects = $(sources:.c=.o)
flags = -g -Wall -I../coelum/GL/include `pkg-config --cflags --libs coelum` -lmaze


$(exec): $(objects)
	gcc $(objects) $(flags) -o $(exec)


%.o: %.c include/%.h
	gcc -c $(flags) $< -o $@

clean:
	-rm *.out
	-rm *.o
	-rm src/*.o
