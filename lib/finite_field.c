#include "finite_field.h"

inline FiniteField CreateF_p(uint8_t p) {
    uint8_t *pol = (uint8_t *) malloc(sizeof(uint8_t));
    *pol = p;
    return (FiniteField) {p, 1, pol};
}

//given polynom is big-endian, stored as little-endian
inline FiniteField CreateF_q(uint8_t p, uint64_t n, uint8_t const *polynom) {
    uint8_t *pol = (uint8_t *) malloc(sizeof(uint8_t) * n);
    for (uint64_t i = n - 1, ind = 0; i > 0; i--, ind++) {
        pol[ind] = polynom[i] % p;
    }
    pol[n - 1] = polynom[0] % p;
    return (FiniteField) {p, n, pol};
}