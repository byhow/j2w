#include <stdio.h>
int j; // file scope of j begins

void f(int i) { // block scope of i begins
    int j = 1; // block scope of j begins; hides file-scope j
    i++; // i refers to the function parameter
    for (int i = 0; i < 2; i++) { // block scope of loop-local i begins
      int j = 2;
      printf("%d\n", j);
    }
    printf("%d\n", j);
}

void g(int j);
