BIN_FILE := tinycom 

SRC_FILE := $(wildcard *.c)

OBJ_FILE := $(patsubst %.c, %.o, $(SRC_FILE))

CFLAGS += -Os -Werror -Wall -std=gnu99
LDFLAGS += -L.
INCLUDES := -I.

TARGETS += $(BIN_FILE)

all : $(TARGETS)

%.o : %.c %.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(STATIC_FILE) : $(OBJ_FILE)
	$(AR) -r $@ $<

$(BIN_FILE) : $(OBJ_FILE) 
	$(CC) $(LDFLAGS) $^ -o $@

.PHONY : clean
clean :
	rm -rf $(TARGETS) *.o *.a
