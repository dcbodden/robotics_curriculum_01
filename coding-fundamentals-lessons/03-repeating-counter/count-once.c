#include <stdio.h>
#include <unistd.h>

int main(void) {
    for (int count = 1; count <= 5; count++) {
        printf("%d\n", count);
        fflush(stdout);
        sleep(1);
    }

    return 0;
}
