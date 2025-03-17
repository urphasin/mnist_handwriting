#include "iostream"
#include "square.h"
#include "cube.h"

using namespace std;

int main() {

    int data[] = {  1000000000,
                    1000000001,
                    1000000002,
                    1000000003,
                    1000000004,
                    1000000005,
                    1000000006,
                    1000000007,
                    1000000008,
                    1000000009};

    FILE *file;

    file = fopen("file.bin", "wb");//

    if(!file) {
        printf("Error opening file.\n");
        return 1;
    }
    fwrite(data, sizeof(data), 1, file);

    fclose(file);
    return 0;
}