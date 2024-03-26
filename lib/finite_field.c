#include "finite_field.h"

FiniteField CreateF_p(uint8_t p) {
    FiniteField field = (FiniteField) malloc(sizeof(struct FiniteField));
    if (field == NULL) return NULL;
    field->p = p;
    int irreducible[] = {1, 0};
    field->pol = PolynomFromArray(irreducible, 2, p);
    if (field->pol == NULL) {
        free(field);
        return NULL;
    }
    return field;
}

//given polynom is big-endian, stored as little-endian
static uint8_t mod(int lhs, uint8_t p) {
    if (lhs < 0) return p - ((-lhs) % p);
    return lhs % p;
}

FiniteField CreateF_q(uint8_t p, uint8_t deg_polynom, int const *polynom) {
    FiniteField field = (FiniteField) malloc(sizeof(struct FiniteField));
    if (field == NULL) return NULL;
    field->pol = PolynomFromArray(polynom, deg_polynom + 1, p);
    if (field->pol == NULL) {
        free(field);
        return NULL;
    }
    field->p = p;
    return field;
}

bool AreEqualFields(FiniteField lhs, FiniteField rhs) {
    if (lhs == rhs) {
        return true;
    }
    if (lhs->p != rhs->p) {
        return false;
    }
    if (lhs->pol == NULL || rhs->pol == NULL) return lhs->pol == rhs->pol && lhs->p == rhs->p;
    return AreEqualPolynom(lhs->pol, rhs->pol);
}

void FreeField(FiniteField f) {
    if (f != NULL) {
        FreePolynom(f->pol);
        free(f);
    }
}
