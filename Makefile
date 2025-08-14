CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LIBS = -lncurses -lm
SRCDIR = assets
SOURCES = main.c $(SRCDIR)/fluid.c $(SRCDIR)/grid.c $(SRCDIR)/input.c $(SRCDIR)/renderer.c $(SRCDIR)/math_utils.c 
TARGET = fluid_sim

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(LIBS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)

.PHONY: clean
