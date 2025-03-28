FILES = ./build/kernel.asm.o ./build/kernel.o ./build/idt/idt.asm.o ./build/idt/idt.o ./build/gdt/gdt.o ./build/gdt/gdt.asm.o ./build/memory/memory.o ./build/io/io.asm.o ./build/task/tss.asm.o ./build/task/task.o ./build/task/task.asm.o ./build/task/process.o ./build/memory/heap/heap.o ./build/memory/heap/kheap.o ./build/memory/paging/paging.o ./build/memory/paging/paging.asm.o ./build/disk/disk.o ./build/fs/pparser.o ./build/fs/file.o ./build/fs/fat/fat16.o ./build/string/string.o ./build/disk/streamer.o
INCLUDES = -I./src
FLAGS = -g -ffreestanding -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-function -fno-builtin -Werror -Wno-unused-label -Wno-cpp -Wno-unused-parameter -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc

all: ./bin/boot.bin ./bin/kernel.bin user_programs
	rm -rf ./bin/os.bin
	dd if=./bin/boot.bin >> ./bin/os.bin
	dd if=./bin/kernel.bin >> ./bin/os.bin
	dd if=/dev/zero bs=1048576 count=16 >> ./bin/os.bin
	sudo mkdir -p /mnt/d
	sudo mount -t vfat ./bin/os.bin /mnt/d
	# Copy a file over
	sudo cp ./hello.txt /mnt/d
	sudo cp ./programs/blank/blank.bin /mnt/d
	sudo umount /mnt/d

./bin/kernel.bin: $(FILES)
	i686-elf-ld -g -relocatable $(FILES) -o ./build/kernelfull.o
	i686-elf-gcc $(FLAGS) -T ./src/linker.ld -o ./bin/kernel.bin -ffreestanding -O0 -nostdlib ./build/kernelfull.o

./bin/boot.bin: ./src/boot/boot.asm
	mkdir -p bin
	nasm -f bin ./src/boot/boot.asm -o ./bin/boot.bin

./build/kernel.asm.o: ./src/kernel.asm
	mkdir -p build
	nasm -f elf -g ./src/kernel.asm -o ./build/kernel.asm.o

./build/kernel.o: ./src/kernel.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/kernel.c -o ./build/kernel.o

./build/idt/idt.asm.o: ./src/idt/idt.asm
	mkdir -p build/idt
	nasm -f elf -g ./src/idt/idt.asm -o ./build/idt/idt.asm.o

./build/idt/idt.o: ./src/idt/idt.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -I./src/idt -std=gnu99 -c ./src/idt/idt.c -o ./build/idt/idt.o

./build/gdt/gdt.asm.o: ./src/gdt/gdt.asm
	nasm -f elf -g ./src/gdt/gdt.asm -o ./build/gdt/gdt.asm.o

./build/gdt/gdt.o: ./src/gdt/gdt.c
	mkdir -p build/gdt
	i686-elf-gcc $(INCLUDES) $(FLAGS) -I./src/gdt -std=gnu99 -c ./src/gdt/gdt.c -o ./build/gdt/gdt.o

./build/memory/memory.o: ./src/memory/memory.c
	mkdir -p build/memory
	i686-elf-gcc $(INCLUDES) $(FLAGS) -I./src/memory -std=gnu99 -c ./src/memory/memory.c -o ./build/memory/memory.o

./build/io/io.asm.o: ./src/io/io.asm
	mkdir -p build/io
	nasm -f elf -g ./src/io/io.asm -o ./build/io/io.asm.o

./build/task/task.asm.o: ./src/task/task.asm
	mkdir -p build/task
	nasm -f elf -g ./src/task/task.asm -o ./build/task/task.asm.o

./build/task/tss.asm.o: ./src/task/tss.asm
	mkdir -p build/task
	nasm -f elf -g ./src/task/tss.asm -o ./build/task/tss.asm.o

./build/task/process.o: ./src/task/process.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -I./src/task -std=gnu99 -c ./src/task/process.c -o ./build/task/process.o

./build/task/task.o: ./src/task/task.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -I./src/task -std=gnu99 -c ./src/task/task.c -o ./build/task/task.o


./build/memory/heap/heap.o: ./src/memory/heap/heap.c
	mkdir -p build/memory/heap
	i686-elf-gcc $(INCLUDES) $(FLAGS) -I./src/memory/heap -std=gnu99 -c ./src/memory/heap/heap.c -o ./build/memory/heap/heap.o

./build/memory/heap/kheap.o: ./src/memory/heap/kheap.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -I./src/memory/heap -std=gnu99 -c ./src/memory/heap/kheap.c -o ./build/memory/heap/kheap.o


./build/memory/paging/paging.o: ./src/memory/paging/paging.c
	mkdir -p build/memory/paging
	i686-elf-gcc $(INCLUDES) $(FLAGS) -I./src/memory/paging -std=gnu99 -c ./src/memory/paging/paging.c -o ./build/memory/paging/paging.o

./build/memory/paging/paging.asm.o: ./src/memory/paging/paging.asm
	nasm -f elf -g ./src/memory/paging/paging.asm -o ./build/memory/paging/paging.asm.o

./build/disk/disk.o: ./src/disk/disk.c
	mkdir -p build/disk
	i686-elf-gcc $(INCLUDES) $(FLAGS) -I./src/disk -std=gnu99 -c ./src/disk/disk.c -o ./build/disk/disk.o

./build/disk/streamer.o: ./src/disk/streamer.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -I./src/disk -std=gnu99 -c ./src/disk/streamer.c -o ./build/disk/streamer.o

./build/fs/pparser.o: ./src/fs/pparser.c
	mkdir -p build/fs
	i686-elf-gcc $(INCLUDES) $(FLAGS) -I./src/fs -std=gnu99 -c ./src/fs/pparser.c -o ./build/fs/pparser.o


./build/fs/file.o: ./src/fs/file.c
	mkdir -p build/fs
	i686-elf-gcc $(INCLUDES) $(FLAGS) -I./src/fs -std=gnu99 -c ./src/fs/file.c -o ./build/fs/file.o

./build/fs/fat/fat16.o: ./src/fs/fat/fat16.c
	mkdir -p build/fs/fat
	i686-elf-gcc $(INCLUDES) $(FLAGS) -I./src/fs -I./src/fat -std=gnu99 -c ./src/fs/fat/fat16.c -o ./build/fs/fat/fat16.o


./build/string/string.o: ./src/string/string.c
	mkdir -p build/string
	i686-elf-gcc $(INCLUDES) $(FLAGS) -I./src/string -std=gnu99 -c ./src/string/string.c -o ./build/string/string.o

user_programs:
	cd ./programs/blank && $(MAKE) all

user_programs_clean:
	cd ./programs/blank && $(MAKE) clean

clean: user_programs_clean
	rm -rf build
	rm -rf bin