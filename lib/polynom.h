#ifndef FINITEFIELDSHW_POLYNOM_H
#define FINITEFIELDSHW_POLYNOM_H

#include <stdint.h>
#include <stdbool.h>

//polynom with coefficents from F_p
struct Polynom {
    uint64_t coeff_size;
    uint8_t *coefficients; // little - endian
};
typedef struct Polynom *Polynom;

Polynom AddPolynom(Polynom lhs, Polynom rhs, uint8_t p);

Polynom SubPolynom(Polynom lhs, Polynom rhs, uint8_t p);

Polynom MultPolynom(Polynom lhs, Polynom rhs, uint8_t p);

Polynom NegPolynom(Polynom elem, uint8_t p);

Polynom ModPolynom(Polynom lhs, Polynom rhs, uint8_t p);

Polynom IdentityPolynom();

Polynom ZeroPolynom();

Polynom PolynomFromArray(int const *array, uint64_t array_size, uint8_t p);

Polynom CopyPolynom(Polynom elem);

uint64_t PolynomDeg(Polynom elem);

bool AreEqualPolynoms(Polynom lhs, Polynom rhs);

bool IsZeroPolynom(Polynom pol);

bool IsIdentityPolynom(Polynom pol);

void FreePolynom(Polynom elem);

#endif //FINITEFIELDSHW_POLYNOM_H
