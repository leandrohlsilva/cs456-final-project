EXES = final

MYCFLAGS= -Wall
MYLDFLAGS= -lGL -lglut -lGLU

all: $(EXES)

%.o: %.c
	gcc $(CPPFLAGS) ${MYCFLAGS} $< -o $@

%: %.c
	gcc $(CPPFLAGS) ${MYCFLAGS} ${MYLDFLAGS} $^ -o $@

clean:
	rm -f ${EXES} *.o 

lab3: final.c offreader.c utils.c reader.c glsl_setup.c sgi_texture.c
