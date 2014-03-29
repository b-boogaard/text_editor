TARGET	= editor

CC	= g++
CFLAGS	= -Wall -Iinclude -lcurses
LINKER	= g++ -o
LFLAGS	= -Wall -Iinclude -lcurses

SRCDIR	= src
INCDIR	= include
OBJDIR	= tmp
BINDIR	= bin

SOURCES	:= $(wildcard $(SRCDIR)/*.cpp)
INCLUDES := $(wildcard $(INCDIR)/*.h)
OBJECTS	:= $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
rm	= rm -f


$(BINDIR)/$(TARGET): $(OBJECTS)
	@echo @$(LINKER) $@ $(OBJECTS) $(LFLAGS) 
	@$(LINKER) $@ $(OBJECTS) $(LFLAGS) 
	@echo "Linking complete!"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	@echo @$(CC) $(CFLAGS) -c $< -o $@
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

.PHONEY: clean
clean:
	@echo @$(rm) $(OBJECTS)
	@$(rm) $(OBJECTS)
	@echo "Cleanup complete!"

.PHONEY: remove
remove: clean
	@echo @$(rm) $(BINDIR)/$(TARGET)
	@$(rm) $(BINDIR)/$(TARGET)
	@echo "Executable removed!"
