EXE := deck

.PHONY: all
all: $(EXE)

$(EXE): src/*.c
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: clean
clean:
	$(RM) $(EXE)
