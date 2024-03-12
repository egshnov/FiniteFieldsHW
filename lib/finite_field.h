#ifndef FINITFIELDSHW_FINITE_FIELD_H
#define FINITFIELDSHW_FINITE_FIELD_H

#include "stdint.h"
#include "stdlib.h"
#include "stdbool.h"

struct FiniteField {
    uint8_t p;
    uint64_t polynom_size; //polynom size
    uint8_t *polynom; //irreducible, mult and divmod operations are performed modulo polynom
};
typedef struct FiniteField *FiniteField;

FiniteField CreateF_p(uint8_t p);

// deg_polynom - polynom deg, stored deg_polynom+1
FiniteField CreateF_q(uint8_t p, uint64_t deg_polynom, int const *polynom);

bool AreEqualFields(FiniteField lhs, FiniteField rhs);

void FreeField(FiniteField f);

#endif //FINITFIELDSHW_FINITE_FIELD_H
