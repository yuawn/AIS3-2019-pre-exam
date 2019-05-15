all:
	gcc src/bof.c -o ./bof/share/bof -no-pie -fno-stack-protector
	gcc src/orw.c -o ./orw/share/orw -no-pie -fno-stack-protector -z execstack -lseccomp
	gcc src/hello.c -o ./hello/share/hello -no-pie
	#gcc src/fmt-1.c -o ./fmt-1/share/fmt-1 -no-pie -Wl,-z,relro,-z,now
	#gcc src/fmt-3.c -o ./fmt-3/share/fmt-3 -no-pie
	#gcc src/printable.c -o ./printable/share/printable -no-pie -Wl,-z,relro,-z,now -s
	#gcc src/uaf.c -o ./uaf/share/uaf -no-pie -Wl,-z,relro,-z,now -s
	#gcc src/tcache.c -o ./tcache/share/tcache -no-pie -Wl,-z,relro,-z,now -s
	#gcc src/baby_heap.c -o ./baby_heap/share/baby_heap -Wl,-z,relro,-z,now -s