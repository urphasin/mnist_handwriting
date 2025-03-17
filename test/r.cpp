#include "iostream"
#include "square.h"
#include "cube.h"

using namespace std;

int main() {

    int data[10];

    FILE *file;

    file = fopen("file.bin", "rb");//

    if(!file) {
        printf("Error opening file.\n");
        return 1;
    }
    fread(data, sizeof(data), 1, file);

    fclose(file);

    for (int i = 0; i < 10; i++) {
        printf("data[%d] : %d\n", i, data[i]);
    }

    return 0;
}