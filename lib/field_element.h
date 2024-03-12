#ifndef FINITFIELDSHW_FIELD_ELEMENT_H
#define FINITFIELDSHW_FIELD_ELEMENT_H

#include "stdint.h"
#include "finite_field.h"
#include "stdbool.h"

struct FieldElement {
    uint64_t coeff_size;
    uint8_t *coefficients; // little - endian
    FiniteField field;
};
typedef struct FieldElement *FieldElement;

FieldElement Add(FieldElement lhs, FieldElement rhs);

FieldElement Sub(FieldElement lhs, FieldElement rhs);

FieldElement Mult(FieldElement lhs, FieldElement rhs);

FieldElement Inv(FieldElement elem); // a^(-1) = a^(p^n-2)

FieldElement Neg(FieldElement elem);

FieldElement DivMod(FieldElement lhs, FieldElement rhs, FieldElement res); // - mult на обратный

FieldElement GetIdentity(FiniteField f);

FieldElement GetZero(FiniteField f);

FieldElement GetFromArray(FiniteField f, uint8_t const *array, uint64_t array_size);

FieldElement Copy(FieldElement elem);

bool InSameField(FieldElement lhs, FieldElement rhs);

void FreeElement(FieldElement elem);


#endif //FINITFIELDSHW_FIELD_ELEMENT_H
