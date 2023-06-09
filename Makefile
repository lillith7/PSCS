# $@ = target file
# $< = first dependency
# $^ = all dependencies

# detect all .o files based on their .c source
C_SOURCES = $(wildcard kernel/*.c drivers/*.c cpu/*.c)
HEADERS = $(wildcard kernel/*.h  drivers/*.h cpu/*.h)
OBJ_FILES = ${C_SOURCES:.c=.o kernel/task_switcher.o cpu/interrupt.o}

# First rule is the one executed when no parameters are fed to the Makefile
all: run

# Notice how dependencies are built as needed
kernel.bin: boot/kernel_entry.o ${OBJ_FILES}
	x86_64-elf-ld -m elf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary

os-image.bin: boot/mbr.bin kernel.bin
	cat $^ > $@

run: os-image.bin
	qemu-system-i386 -d int -netdev user,id=net0 -device ne2k_pci,netdev=net0 -m 64M -audiodev coreaudio,id=audio0 -machine pcspk-audiodev=audio0 -fda $<

echo: os-image.bin
	xxd $<

# only for debug
kernel.elf: boot/kernel_entry.o ${OBJ_FILES}
	x86_64-elf-ld -m 64M elf_i386 -o $@ -Ttext 0x1000 $^

debug: os-image.bin kernel.elf
	qemu-system-i386 -d int -netdev user,id=net0 -device ne2k_pci,netdev=net0 -net nic -net user,hostfwd=tcp::5555-:22,dhcpstart=192.168.1.2 -m 16M -s -S -fda os-image.bin -d guest_errors,int &
	i386-elf-gdb -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

%.o: %.c ${HEADERS}
	x86_64-elf-gcc -g -m32 -ffreestanding -c $< -o $@ # -g for debugging

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -o $@

%.dis: %.bin
	ndisasm -b 32 $< > $@

clean:
	$(RM) *.bin *.o *.dis *.elf
	$(RM) kernel/*.o
	$(RM) boot/*.o boot/*.bin
	$(RM) drivers/*.o
	$(RM) cpu/*.o
