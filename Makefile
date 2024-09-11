CC=gcc

TARGET = main 
SRCS = main.c

all: $(TARGET)

$(TARGET): $(SRCS) 
	$(CC) -o $(TARGET) $(SRCS) 

clean:
	rm -f *.bitmap *.bmp main *.o
