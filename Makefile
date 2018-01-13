CC = gcc
CFLAGS = -std=gnu99 -Wall -Wno-comment -c

ODIR = obj
SDIR = src
IDIR = include
INCLUDES = -I./ev3dev-c/source/ev3/  -I$(IDIR)/
LIBS = -lm -lev3dev-c -lbluetooth -lpthread
SRCS = $(wildcard $(SDIR)/*.c)
OBJS = $(patsubst $(SDIR)/%.c,$(ODIR)/%.o,$(SRCS))
APP = InSight

$(ODIR)/%.o: $(SDIR)/%.c $(IDIR)/%.h
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $<

$(APP): $(OBJS)
	$(CC) $^ $(LIBS) -o $@

clean: $(APP) $(OBJS)
	rm  $^
