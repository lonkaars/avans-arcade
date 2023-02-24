CC = gcc
LD = gcc
RM = rm -f
TARGET = main
LFLAGS += -lSDL2 -lm
OBJS += main.o

all: $(TARGET)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

$(TARGET): $(OBJS)
	$(LD) $^ $(LFLAGS) -o $@

clean:
	$(RM) $(TARGET) $(OBJS)

