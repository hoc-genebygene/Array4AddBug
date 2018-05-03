#include <array>
#include <iostream>

template <class Type1, class Type2>
inline void array4_add_inplace(std::array<Type1, 4> & a, std::array<Type2, 4> b) {
    for(int i = 0; i < 4; i++) {
        a[i] += b[i];
    }

//  Fixes bug:
//    a[0] += b[0];
//    a[1] += b[1];
//    a[2] += b[2];
//    a[3] += a[3];
}

using ArrayType = std::array<uint8_t, 4>;

static ArrayType static_a{3,1,0,0};
static ArrayType static_b{4,0,0,0};
static ArrayType static_c{4,0,0,0};
static ArrayType static_d{4,0,0,0};

std::array<uint8_t, 4> getOccCountOfRegion(uint32_t region) {
    ArrayType a = static_a;
    ArrayType b = static_b;
    const ArrayType & c = static_c;
    const ArrayType & d = static_d;

    array4_add_inplace(a, c);
    array4_add_inplace(b, d);
    array4_add_inplace(a, b);
    return a;
}

int main() {
    auto occ_count = getOccCountOfRegion(1);

    std::cout << (int)occ_count[0] << " " << (int)occ_count[1] << " " << (int)occ_count[2] << " " << (int)occ_count[3] << std::endl;

    // Correct results: 15 1 0 0
    // Incorrect results: 8 1 0 0
}
