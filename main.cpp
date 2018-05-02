#include <array>
#include <iostream>

using ArrayType = std::array<uint64_t, 4>;

void array4_add_inplace(ArrayType & a, ArrayType b) {
    for(int i = 0; i < 4; i++) {
        a[i] += b[i];
    }
}

int main() {
    ArrayType a {4,0,0,0};
    ArrayType b {4,0,0,0};
    ArrayType c {4,0,0,0};
    ArrayType d {3,1,0,0};

    array4_add_inplace(a, c);
    array4_add_inplace(b, d);
    array4_add_inplace(a, b);

    std::cout << a[0] << " " << a[1] << " " << a[2] << " " << a[3] << std::endl;
}
