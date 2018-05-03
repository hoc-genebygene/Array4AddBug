#include <array>
#include <iostream>

template <class Type1, class Type2>
inline void array4_add_inplace(std::array<Type1, 4> & a, std::array<Type2, 4> b) {
    for(int i = 0; i < 4; i++) {
        a[i] += b[i];
    }
}

template <int n>
static constexpr int pow2_constexpr() {
    static_assert(n >= 0, "n must be positive");
    return 1<< n;
}

//using the property that log2(x) == log2(x / 2) + 1, we can write a log2 function:
template <int n>
static constexpr int log2_floor_constexpr() {
    //static_assert(n >= 1, "n must be greater than 0");
    return n <= 1 ? 0 : 1 + log2_floor_constexpr<(n >> 1)>();
}

template <int n>
static constexpr int log2_ceil_constexpr() {
    static_assert(n >= 1, "n must be greater than 0");
    return
    n == 1 ? 0 :
    (pow2_constexpr<log2_floor_constexpr<n>()>() != n ? log2_floor_constexpr<n>() + 1 : log2_floor_constexpr<n>());
}

static const constexpr int NUMBER_OF_BASES = 4;

struct Reference {
    static const constexpr int COUNT_LOOKUP_SIZE = 256;
    static std::array<std::array<uint8_t, 4>,COUNT_LOOKUP_SIZE> countLookup;

    static const constexpr int NUMBER_OF_BITS_PER_BASE_COUNT_U32 = 32 / NUMBER_OF_BASES;
    static const constexpr int NUMBER_OF_BITS_PER_BASE_COUNT_U32_MASK = pow2_constexpr<NUMBER_OF_BITS_PER_BASE_COUNT_U32>() - 1;

    static std::array<uint8_t, 4> getOccCountOfRegion(uint32_t region) {
        std::array<uint8_t, 4> a = countLookup[region & NUMBER_OF_BITS_PER_BASE_COUNT_U32_MASK];
        std::array<uint8_t, 4> b = countLookup[(region >> NUMBER_OF_BITS_PER_BASE_COUNT_U32) & NUMBER_OF_BITS_PER_BASE_COUNT_U32_MASK];
        const std::array<uint8_t, 4> & c = countLookup[(region >> 2*NUMBER_OF_BITS_PER_BASE_COUNT_U32) & NUMBER_OF_BITS_PER_BASE_COUNT_U32_MASK];
        const std::array<uint8_t, 4> & d = countLookup[region >> 3*NUMBER_OF_BITS_PER_BASE_COUNT_U32];

        array4_add_inplace(a, c);
        array4_add_inplace(b, d);
        array4_add_inplace(a, b);
        return a;
    }
};

const constexpr int NUMBER_OF_BITS_PER_BASE = log2_ceil_constexpr<NUMBER_OF_BASES>();
const constexpr int BASE_MASK = pow2_constexpr<NUMBER_OF_BITS_PER_BASE>() - 1;

std::array<std::array<uint8_t, 4>,Reference::COUNT_LOOKUP_SIZE> makeCountLookupArray() {
    std::array<std::array<uint8_t, 4>,Reference::COUNT_LOOKUP_SIZE> countLookup;

    // Initialize the count lookup
    const constexpr int numberOfBitsInIndex = log2_ceil_constexpr<Reference::COUNT_LOOKUP_SIZE>();
    for (int i = 0; i < Reference::COUNT_LOOKUP_SIZE; ++i) {
        std::array<uint8_t, 4> allBaseCount = {{0}};
        for (int base = 0; base < NUMBER_OF_BASES; ++base) {
            uint32_t baseCount = 0;
            for (int j = 0; j < numberOfBitsInIndex; j += NUMBER_OF_BITS_PER_BASE) {
                baseCount += (((i >> j) & BASE_MASK) == base);
            }
            // Shift the count into its proper position
            allBaseCount[base] |= baseCount;
        }
        countLookup[i] = allBaseCount;
    }

    return countLookup;
}

std::array<std::array<uint8_t, 4>,Reference::COUNT_LOOKUP_SIZE> Reference::countLookup = makeCountLookupArray();

int main() {
    auto occ_count = Reference::getOccCountOfRegion(1);

    std::cout << (int)occ_count[0] << " " << (int)occ_count[1] << " " << (int)occ_count[2] << " " << (int)occ_count[3] << std::endl;
}
