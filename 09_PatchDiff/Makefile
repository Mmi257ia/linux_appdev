LABYRINTH_BASE = labyrinth
PATCHES_DIR = patches
TRASH = *.o

.PHONY: all run clean

all: $(LABYRINTH_BASE) $(LABYRINTH_BASE)1 $(LABYRINTH_BASE)2 $(LABYRINTH_BASE)3 run

$(LABYRINTH_BASE)1.c: $(LABYRINTH_BASE).c $(PATCHES_DIR)/1.patch
	patch $< -o $@ < $(word 2, $^)

$(LABYRINTH_BASE)2.c: $(LABYRINTH_BASE)1.c $(PATCHES_DIR)/2.patch
	patch $< -o $@ < $(word 2, $^)

$(LABYRINTH_BASE)3.c: $(LABYRINTH_BASE)2.c $(PATCHES_DIR)/3.patch
	patch $< -o $@ < $(word 2, $^)

run: $(LABYRINTHS)
	./$(LABYRINTH_BASE)
	./$(LABYRINTH_BASE)1 10
	./$(LABYRINTH_BASE)2 ' X' 5
	./$(LABYRINTH_BASE)3 42 ' X' 5
	./$(LABYRINTH_BASE)3 4242 ' X' 5
	./$(LABYRINTH_BASE)3 42 ' X' 5

clean:
	rm -f $(TRASH) $(LABYRINTH_BASE) $(LABYRINTH_BASE)? $(LABYRINTH_BASE)?.c
