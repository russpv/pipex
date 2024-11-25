SHELL := /bin/bash
TARGET = pipex
OUTPUT = executable

# Colors
YELLOW = \033[33m
GREEN = \033[32m
RESET = \033[0m
BOLD = \033[1m

# Dirs
OBJDIR = obj
OBJEXT = o
SRCEXT = c
DEPEXT = d
INCDIR = inc
TARGETDIR = bin
SRCDIR = src
RESDIR = res

# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
EXT_CFLAGS = -DEXTENDEDFUNC
LDFLAGS = -L$(LIB_DIR) -lft
LDFLAGS_SO = -L$(LIB_DIR) -lft -Wl,-rpath,$(LIB_DIR)

INC	= -I$(INCDIR)
INCDEP = -I$(INCDIR)

LIB_CPDIR = .
LIB_DIR = libft
LIB_NAME = libft.a
LIB_NAME_SO = libft.so
LIB_PATH = $(LIB_DIR)/$(LIB_NAME)
LIB_PATH_SO = $(LIB_DIR)/$(LIB_NAME_SO)

# ----------------------------------------------------------------
#  DO NOT EDIT BELOW
# ----------------------------------------------------------------

# BONUS_SOURCES = $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
# BONUS_OBJECTS = $(addprefix $(OBJDIR), $(BONUS_SOURCES:.c=.o))
SOURCES = $(shell find $(SRCDIR) -type f -name "*.$(SRCEXT)")
OBJECTS = $(patsubst $(SRCDIR)/%,$(OBJDIR)/%,$(SOURCES:.$(SRCEXT)=.$(OBJEXT)))

# Default Make
all: resources $(TARGET)

# Copy resources from dir to target dir
resources: directories
	@if [ -d "$(RESDIR)" ] && [ "$(shell ls -A $(RESDIR))" ]; then \
        cp $(RESDIR)/* $(TARGETDIR)/; \
	else \
        echo "Warning: $(RESDIR) directory is empty or doesn't exist."; \
	fi

#Make the dirs
directories:
	@mkdir -p $(TARGETDIR)
	@mkdir -p $(OBJDIR)

# Pull in dependencies for *existing* .o files
-include $(OBJECTS:.$(OBJEXT)=.$(DEPEXT))

# Link the executable
$(TARGET): $(OBJECTS) $(LIB_PATH)
	@echo "Linking with object files: $(OBJECTS)"
	$(CC) -o $(TARGETDIR)/$(TARGET) $^ $(LDFLAGS)
	chmod +x $(TARGETDIR)/$(TARGET)
	@cp $(TARGETDIR)/$(TARGET) .

#Compile
$(OBJDIR)/%.$(OBJEXT): $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<
	@$(CC) $(CFLAGS) $(INCDEP) -MM $(SRCDIR)/$*.$(SRCEXT) > $(OBJDIR)/$*.$(DEPEXT)
	@cp -f $(OBJDIR)/$*.$(DEPEXT) $(OBJDIR)/$*.$(DEPEXT).tmp
	@sed -e 's|.*:|$(OBJDIR)/$*.$(OBJEXT):|' < $(OBJDIR)/$*.$(DEPEXT).tmp > $(OBJDIR)/$*.$(DEPEXT)
	@sed -e 's/.*://' -e 's/\\$$//' < $(OBJDIR)/$*.$(DEPEXT).tmp | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $(OBJDIR)/$*.$(DEPEXT)
	@rm -f $(OBJDIR)/$*.$(DEPEXT).tmp
	
#@echo "$(GREEN)$(BOLD)SUCCESS$(RESET)"
#@echo "$(YELLOW)Created: $(words $(OBJECTS) ) object file(s)$(RESET)"
#@echo "$(YELLOW)Created: $(NAME)$(RESET)"

bonus: .bonus_made

.bonus_made: 
	@echo "Creating $(NAME) $(OUTPUT) with extended functionality..."
	$(MAKE) CFLAGS="$(CFLAGS) $(EXT_CFLAGS)"
	-@touch .bonus_made
#	@echo "$(GREEN)$(BOLD)SUCCESS$(RESET)"
#	@echo "$(YELLOW)Created: $(words $(OBJECTS) $(BONUS_OBJECTS)) object file(s)$(RESET)"
#	@echo "$(YELLOW)Created: $(NAME)$(RESET)"
#
#%(OBJDIR)/%.o: %.c | $(OBJDIR) # -DDEBUGMODE
#	$(CC) -c $(CFLAGS) $< -o $@ #Initial compilation of .c files


# make lib
$(LIB_PATH):
	@$(MAKE) -C $(LIB_DIR)
	@echo "Copying $(LIB_NAME) to ../"
	@cp $(LIB_PATH) .

# clean only objects
clean:
	@$(RM) -rf $(OBJDIR)
	@$(MAKE) -C $(LIB_DIR) clean

# Clean+, objects and binaries
cleaner: clean
	@$(RM) -rf $(TARGETDIR)
	@$(RM) -f $(TARGET) # copied to root for lame evaluators
	rm -f $(LIB_NAME) # don't delete so!
	-@rm -f .bonus_made
# @echo "$(GREEN)$(BOLD)SUCCESS$(RESET)"
# @echo "$(YELLOW) Deleted: $(words $(OBJECTS) $(BONUS_OBJECTS)) object file(s)$(RESET)"

# Full clean
fclean: cleaner
	rm -f $(LIB_PATH)
	rm -f $(LIB_NAME)
	rm -f $(LIB_NAME_SO)
	@$(MAKE) -C $(LIB_DIR) fclean
	@rm -f $(LIB_DIR)/error.txt
# @echo "$(GREEN)SUCCESS$(RESET)"
# @echo "$(YELLOW) Deleted $(words $(NAME)) object files(s)$(RESET)"
# @echo "$(YELLOW) Deleted: $(NAME)"

re: fclean all

so:
	$(CC) -fPIC $(CFLAGS) -c $(SOURCES) $(BONUS_SOURCES)
	$(CC) -nostartfiles -shared -o pipex.so $(OBJECTS) $(BONUS_OBJECTS)

.PHONY: all bonus clean cleaner fclean re resources so
