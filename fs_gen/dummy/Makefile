
SRCs=dummy_fs_gen.o dummy_fs.o dummy_streaming.o
TARGET=dummy_fs
LDFLAGS=
CFLAGS=-c -Wall

all: $(SRCs) $(TARGET)

$(TARGET) : $(SRCs)
	$(CC) $(LDFLAGS) $(SRCs) -o $@

*.o:
	$(CC) $(CFLAGS) $< -O $@

clean:
	rm -rf *.o $(TARGET)

