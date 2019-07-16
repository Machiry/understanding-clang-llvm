#include<stdio.h>
#include<string.h>
#include<stdlib.h>

typedef struct {
   char *ptr;
   unsigned len;
} foo;

void log_mem_access(void *addr, int value, int flag) {
    if(flag == 0) {
        printf("Reading value 0x%x from %p\n", value, addr);
    }
    if(flag == 1) {
        printf("Writing value 0x%x to %p\n", value, addr);
    }
}

unsigned count_alpha_chars(char *str) {
    unsigned c = 0;
    if(str) {
        while(*str) {
            if((*str >= 'A' && *str <= 'Z') || (*str >= 'a' && *str <= 'z')) {
                c++;
            }
            str++;
        } 
    }
    return c;
}

int main(int argc, char **argv) {
    foo obj = {};
    char buff[512];
    // read len
    scanf("%u", &(obj.len));
    // allocate buff
    obj.ptr = (char*)malloc(obj.len);
    if(obj.ptr) {
        // read buff
        scanf("%512s", buff);
        strncpy(obj.ptr, buff, obj.len);
        // output buff
        printf("Content:%s, Alpha:%u\n", obj.ptr, count_alpha_chars(obj.ptr));
    }
}
