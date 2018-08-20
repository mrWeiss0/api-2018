CC=gcc
CFLAGS=-g -std=c11 -Wall -Wextra

NAME=ndtm

BUILD_DIR=./build
CONCAT_DIR=./concat
CONCAT=concat
CONCAT_DEP=concat.d
CONCAT_FILE=$(CONCAT_DIR)/$(NAME)-$(CONCAT)

SRC=$(wildcard *.c)
OBJ=$(SRC:%.c=$(BUILD_DIR)/%.o)
DEP=$(OBJ:%.o=%.d)

$(NAME): $(OBJ)
	$(CC) -o $@ $^

# Dependencies
$(BUILD_DIR)/%.d: %.c
	@mkdir -p $(@D)
	@$(CC) -MM -MF $@ $<

-include $(DEP)

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

# Create single file for submit
$(CONCAT): $(CONCAT_FILE)

$(CONCAT_FILE): $(CONCAT_FILE).c
	$(CC) $(CFLAGS) $< -o $@

-include $(CONCAT_DEP)

# Concatenate all the sources, headers first,
# excluding their include directives
$(CONCAT_FILE).c: $(SRC)
	@mkdir -p $(@D)
	cat $(filter %.h, $^) $(filter %.c, $^) | grep -v '#include "' > $@

clean:
	rm -rf $(BUILD_DIR) $(NAME) $(CONCAT_DIR)

.PHONY: clean $(CONCAT)
