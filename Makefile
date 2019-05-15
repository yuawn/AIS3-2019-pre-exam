all:
	gcc src/bof.c -o ./bof/share/bof -no-pie -fno-stack-protector
	gcc src/orw.c -o ./orw/share/orw -no-pie -fno-stack-protector -z execstack -lseccomp -s
	gcc src/hello.c -o ./hello/share/hello -no-pie -s
	gcc src/shellcode-2019.c -o ./shellcode-2019/share/shellcode-2019 -Wl,-z,relro,-z,now -lseccomp -s
	#gcc src/fmt-3.c -o ./fmt-3/share/fmt-3 -no-pie
	#gcc src/printable.c -o ./printable/share/printable -no-pie -Wl,-z,relro,-z,now -s
	#gcc src/uaf.c -o ./uaf/share/uaf -no-pie -Wl,-z,relro,-z,now -s
	#gcc src/tcache.c -o ./tcache/share/tcache -no-pie -Wl,-z,relro,-z,now -s
	#gcc src/baby_heap.c -o ./baby_heap/share/baby_heap -Wl,-z,relro,-z,now -s