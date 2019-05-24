all:
	gcc src/bof.c -o ./bof/share/bof -no-pie -fno-stack-protector
	gcc src/orw.c -o ./orw/share/orw -no-pie -fno-stack-protector -z execstack -lseccomp -s
	gcc src/hello.c -o ./hello/share/hello -no-pie -s
	gcc src/shellcode-2019.c -o ./shellcode-2019/share/shellcode-2019 -Wl,-z,relro,-z,now -lseccomp -s
	gcc src/secure-bof.c -o ./secure-bof/share/secure-bof -Wl,-z,relro,-z,now -no-pie -fno-stack-protector -s
	gcc src/box.c -o ./box/share/box -s
	#gcc src/printable.c -o ./printable/share/printable -no-pie -Wl,-z,relro,-z,now -s
	#gcc src/uaf.c -o ./uaf/share/uaf -no-pie -Wl,-z,relro,-z,now -s
	#gcc src/tcache.c -o ./tcache/share/tcache -no-pie -Wl,-z,relro,-z,now -s
	#gcc src/baby_heap.c -o ./baby_heap/share/baby_heap -Wl,-z,relro,-z,now -s