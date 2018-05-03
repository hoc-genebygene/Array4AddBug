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

void print_array(ArrayType arr) {
    std::cout << (int)arr[0] << " " << (int)arr[1] << " " << (int)arr[2] << " " << (int)arr[3] << std::endl;
}

std::array<uint8_t, 4> add_some_stuff() {
    ArrayType a = static_a;
    ArrayType b = static_b;
    ArrayType c = static_c;
    ArrayType d = static_d;

    // Also fixes bug...
    // ArrayType a{3,1,0,0};
    // ArrayType b{4,0,0,0};
    // ArrayType c{4,0,0,0};
    // ArrayType d{4,0,0,0};


    print_array(a);
    print_array(b);
    print_array(c);
    print_array(d);

    array4_add_inplace(a, c);
    std::cout << std::endl;
    print_array(a);
    
    array4_add_inplace(b, d);
    std::cout << std::endl;
    print_array(b);

    array4_add_inplace(a, b);
    print_array(a);

    return a;
}

int main() {
    auto count = add_some_stuff();

    print_array(count);

    // Correct results: 15 1 0 0
    // Incorrect results: 8 1 0 0
}
