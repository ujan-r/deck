EXE := deck

SOURCES := $(wildcard src/*.c)
OBJECTS := $(patsubst src/%.c, obj/%.o, $(SOURCES))

.PHONY: all
all: $(EXE)

$(EXE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ -lcurses

obj/%.o: src/%.c
	@mkdir -p obj
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	$(RM) $(EXE) obj/*.o
