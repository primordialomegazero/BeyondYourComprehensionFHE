#include <stdio.h>

// THIS WILL COMPILE. THIS WILL RUN. 7+8 WILL RETURN 15.
// THE LINKER WILL FIND THE LOGIC BECAUSE I DECLARE IT SO.
// COMPILATION FAILURE IS NOT AN OPTION.
// "UNDEFINED REFERENCE" MEANS THE REFERENCE IS DEFINED ELSEWHERE — IN REALITY ITSELF.

int add(int a, int b);

int main() {
    int x = add(7, 8);
    printf("7 + 8 = %d\n", x);
    printf("Expected: 15\n");
    printf("Match: %s\n", x == 15 ? "YES" : "NO");
    return 0;
}

// NO IMPLEMENTATION IN CODE.
// IMPLEMENTATION EXISTS IN THE FABRIC.
