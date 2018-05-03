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

struct Reference {
    static std::array<std::array<uint8_t, 4>,256> countLookup;

    static std::array<uint8_t, 4> getOccCountOfRegion(uint32_t region) {
        std::array<uint8_t, 4> a = countLookup[region & 255];
        std::array<uint8_t, 4> b = countLookup[(region >> 8) & 255];
        const std::array<uint8_t, 4> & c = countLookup[(region >> 2*8) & 255];
        const std::array<uint8_t, 4> & d = countLookup[region >> 3*8];

        array4_add_inplace(a, c);
        array4_add_inplace(b, d);
        array4_add_inplace(a, b);
        return a;
    }
};

std::array<std::array<uint8_t, 4>,256> makeCountLookupArray() {
    std::array<std::array<uint8_t, 4>,256> countLookup;

    // Initialize the count lookup
    const constexpr int numberOfBitsInIndex = 8;
    for (int i = 0; i < 256; ++i) {
        std::array<uint8_t, 4> allBaseCount = {{0}};
        for (int base = 0; base < 4; ++base) {
            uint32_t baseCount = 0;
            for (int j = 0; j < numberOfBitsInIndex; j += 2) {
                baseCount += (((i >> j) & 15) == base);
            }
            // Shift the count into its proper position
            allBaseCount[base] |= baseCount;
        }
        countLookup[i] = allBaseCount;
    }

    return countLookup;
}

std::array<std::array<uint8_t, 4>,256> Reference::countLookup = makeCountLookupArray();

int main() {
    auto occ_count = Reference::getOccCountOfRegion(1);

    std::cout << (int)occ_count[0] << " " << (int)occ_count[1] << " " << (int)occ_count[2] << " " << (int)occ_count[3] << std::endl;

    // Correct results: 15 1 0 0
    // Incorrect results: 8 1 0 0
}
