EXE := deck

.PHONY: all
all: $(EXE)

$(EXE): src/*.c
	$(CC) $(CFLAGS) -o $@ $^ -lcurses

.PHONY: clean
clean:
	$(RM) $(EXE)
