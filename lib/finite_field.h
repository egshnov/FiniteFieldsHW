#ifndef FINITFIELDSHW_FINITE_FIELD_H
#define FINITFIELDSHW_FINITE_FIELD_H

#include "stdint.h"
#include "stdlib.h"

//TODO: Make FiniteField a pointer?
typedef struct {
    uint8_t p;
    uint64_t max_n;
    uint8_t *polynom; //irreducible operations are performed using modulo polynom
} FiniteField;


#endif //FINITFIELDSHW_FINITE_FIELD_H
