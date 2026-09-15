#include <stdio.h>
#include <unistd.h>

int main(void) {
    while (1) {
        for (int count = 1; count <= 5; count++) {
            printf("%d\n", count);
            fflush(stdout);
            sleep(1);
        }
    }
}
