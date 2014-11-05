#include <stdio.h>


void forkedFunction(){
    char buffer[] = "I am a forked function\n";
    while(1){
        write(1, buffer, sizeof(buffer));
        sleep(3);
    }
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
