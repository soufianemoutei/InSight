CC = gcc
CFLAGS = -std=gnu99 -Wall -Wno-comment -c

ODIR = obj
SDIR = src
INCLUDES = -I./ev3dev-c/source/ev3/  -Iinclude/
LIBS = -lm -lev3dev-c -lbluetooth -lpthread
SRCS = $(wildcard $(SDIR)/*.c)
OBJS = $(patsubst $(SDIR)/%.c,$(ODIR)/%.o,$(SRCS))
APP = InSight

$(ODIR)/%.o: $(SDIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $<

$(APP): $(OBJS)
	$(CC) $^ $(LIBS) -o $@

clean: $(APP) $(OBJS)
	rm  $^
