#include<stdio.h>
int func2() {
    return printf("In func2\n");
}
int func1() {
    return printf("In func1\n");
}

int main() {
    unsigned i;
    scanf("%u", &i);
    func1();
    while(i > 0) {
        func2();
    }
    return 0;
}
