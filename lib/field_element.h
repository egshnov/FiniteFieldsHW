#ifndef FINITFIELDSHW_FIELD_ELEMENT_H
#define FINITFIELDSHW_FIELD_ELEMENT_H

#include <stdint.h>
#include "finite_field.h"
#include <stdbool.h>
#include "polynom.h"

//TODO: change size to size_t ?
struct FieldElement {
    Polynom pol;// little - endian
    FiniteField field;
};
typedef struct FieldElement *FieldElement;
#define ERROR 1
#define SUCCESS 0

FieldElement Add(FieldElement lhs, FieldElement rhs);

FieldElement Sub(FieldElement lhs, FieldElement rhs);

FieldElement Mult(FieldElement lhs, FieldElement rhs);

FieldElement Pow(FieldElement elem, int deg);

FieldElement Inv(FieldElement elem); // a^(-1) = a^(p^n-2)

FieldElement Neg(FieldElement elem);

FieldElement Division(FieldElement lhs, FieldElement rhs);

FieldElement GetIdentity(FiniteField f);

FieldElement GetZero(FiniteField f);

FieldElement GetFromArray(FiniteField f, int const *array, uint64_t array_size);

FieldElement Copy(FieldElement elem);

bool InSameField(FieldElement lhs, FieldElement rhs);

bool AreEqual(FieldElement lhs, FieldElement rhs);

bool IsZero(FieldElement elem);

uint64_t GetDeg(FieldElement elem);

void FreeElement(FieldElement elem);


#endif //FINITFIELDSHW_FIELD_ELEMENT_H
