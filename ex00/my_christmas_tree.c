#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void head(int spaces);
void tree(int spaces, int parts, int* ornaments);
char random_ball(void);
int random_color(void);
int* positions(int height, int ornaments);

int main(int argc, char** argv) {
    if (argc < 3) {
        printf("Usage: %s <height> <ornaments>\n", argv[0]);
        return 127;
    }
    int height = atoi(argv[1]);
    if (height < 5) {
        printf("Height must be at least 5\n");
        return 1;
    }
    int ornaments = atoi(argv[2]);
    int parts = 2 * (height - 1) + 1;
    int* ornament_positions = positions(height, ornaments);
    if (ornament_positions == NULL) {
        printf("Too many ornaments for this height\n");
        return 1;
    }
    tree(0, parts, ornament_positions);
    free(ornament_positions);
}

void head(int spaces) {
    for (int i = 0; i < spaces - 2; i++) {
        putchar(' ');
    }
    printf("\\   /\n");
    for (int i = 0; i < spaces - 4; i++) {
        putchar(' ');
    }
    printf("--> \x1B[33mX\x1B[0m <--\n");
}

void tree(int spaces, int parts, int* ornaments) {
    if (parts == 1) {
        head(spaces);
        return;
    }
    tree(spaces + 1, parts - 2, ornaments);
    int place_ornament = ornaments[spaces];
    int* position = place_ornament ? (int*) calloc(place_ornament, sizeof(int)) : NULL;
    if (place_ornament && position == NULL)
        return;
    for (int i = 0; i < place_ornament; i++) {
        position[i] = (rand() % (parts - 2) + 1);
    }
    if (spaces)
        printf("%*s", spaces, " ");
    printf("/\x1B[32m");
    for (int i = 1; i < parts - 1; i++) {
        int placed = 0;
        for (int j = 0; j < place_ornament; j++) {
            if (position[j] == i) {
                printf("\x1B[%im%c\x1B[32m", random_color(), random_ball());
                placed = 1;
                break;
            }
        }
        if (!placed)
            putchar('+');
    }
    printf("\x1B[0m%c", '\\');
    putchar('\n');
    if (position)
        free(position);
}

int* positions(int height, int ornaments) {
    int* arr = (int*) calloc(height, sizeof(int));
    if (arr == NULL)
        return NULL;
    srand((size_t) arr ^ time(0));
    for (int i = 0; i < height && ornaments > 0; i++) {
        do {
            int is_placed = rand() % 2;
            if (!is_placed)
                break;
            arr[i] = arr[i] + 1;
            ornaments--;
        } while (ornaments > 0);
    }
    // add any remaining ones first-fit
    for (int i = 0; i < height && ornaments > 0; i++) {
        if (arr[i] == 0) {
            arr[i] = 1;
            ornaments--;
        }
    }
    return arr;
}

char random_ball(void) {
    int rng = rand() % 3;
    switch (rng) {
        case 0:
            return '@';
        case 1:
            return '0';
        default:
            return 'o';
    }
}

int random_color(void) {
    int rng = rand() % 3;
    switch (rng) {
        case 0:
            return 34;
        case 1:
            return 33;
        default:
            return 37;
    }
}