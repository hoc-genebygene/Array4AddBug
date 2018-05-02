#include <array>
#include <iostream>

using ArrayType = std::array<uint64_t, 4>;

void array4_add_inplace(ArrayType & a, ArrayType b) {
    for(int i = 0; i < 4; i++) {
        a[i] += b[i];
    }
}

int main() {
    ArrayType a {1,2,3,4};
    ArrayType b {5,6,7,8};

    array4_add_inplace(a, b);

    std::cout << a[0] << " " << a[1] << " " << a[2] << " " << a[3] << std::endl;
}
