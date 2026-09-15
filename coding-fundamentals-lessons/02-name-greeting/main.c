#include <stdio.h>
#include <string.h>

int main(void) {
    char name[50];

    printf("What is your name? ");
    fflush(stdout);

    if (fgets(name, sizeof(name), stdin) == NULL) {
        printf("\nNo name was entered.\n");
        return 1;
    }

    name[strcspn(name, "\n")] = '\0';
    if (name[0] == '\0') {
        printf("Please run the program again and type at least one character.\n");
        return 1;
    }

    printf("Nice to meet you, %s!\n", name);
    return 0;
}
