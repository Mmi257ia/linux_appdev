LABYRINTH_BASE = labyrinth
PATCHES_DIR = patches
PATCHES = $(shell ls -1 $(PATCHES_DIR) | grep -Eo '^[^.]*')
LABYRINTHS = $(addprefix $(LABYRINTH_BASE), $(PATCHES))
TRASH = *.o

.PHONY: all run clean

.SECONDARY: $(LABYRINTH_BASE)%.c

all: $(LABYRINTH_BASE) $(LABYRINTHS) run

$(LABYRINTH_BASE)%.c: $(PATCHES_DIR)/%.patch
	patch $(LABYRINTH_BASE).c -o $@ < $<

run: $(LABYRINTHS)
	./$(LABYRINTH_BASE)
	./$(LABYRINTH_BASE)1 10
	./$(LABYRINTH_BASE)2 ' X' 5
	./$(LABYRINTH_BASE)3 42 ' X' 5
	./$(LABYRINTH_BASE)3 4242 ' X' 5
	./$(LABYRINTH_BASE)3 42 ' X' 5

clean:
	rm -f $(TRASH) $(LABYRINTHS) $(LABYRINTH_BASE) $(addsuffix .c, $(LABYRINTHS))
