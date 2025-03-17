#include "iostream"
#include "square.h"
#include "cube.h"

using namespace std;

int main() {

    int N = 3;

    // cout << "The square of " << N << " is " << square(N) << endl;
    // cout << "The cube of " << N << " is " << cube(N) << endl;

    int num = 0x12345678;
    char *ptr = (char*)&num;

    if (*ptr == 0x12) {
        std::cout << "Big-endian\n";
    } else {
        std::cout << "Little-endian\n";
    }

    return 0;
}