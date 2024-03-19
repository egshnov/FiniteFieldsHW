#include "finite_field.h"

FiniteField CreateF_p(uint8_t p) {
    FiniteField field = (FiniteField) malloc(sizeof(struct FiniteField));
    if (field != NULL) {
        field->p = p;
        field->pol = (Polynom) malloc(sizeof(struct Polynom));
        if (field->pol == NULL) {
            free(field);
            return NULL;
        }
        field->pol->coeff_size = 1;
        field->pol->coefficients = malloc(sizeof(uint8_t));
        field->pol->coefficients[0] = p;
        return field;
    }
    return NULL;
}

//given polynom is big-endian, stored as little-endian
static uint8_t mod(int lhs, uint8_t p) {
    if (lhs < 0) return p - ((-lhs) % p);
    return lhs % p;
}

FiniteField CreateF_q(uint8_t p, uint8_t deg_polynom, int const *polynom) {
    FiniteField field = (FiniteField) malloc(sizeof(struct FiniteField));
    if (field != NULL) {
        field->pol = PolynomFromArray(polynom, deg_polynom + 1, p);
        if (field->pol == NULL) {
            free(field);
            return NULL;
        }
        field->p = p;
        return field;
    }
    return NULL;
}

bool AreEqualFields(FiniteField lhs, FiniteField rhs) {
    if (lhs == rhs) {
        return true;
    }
    if (lhs->p != rhs->p) {
        return false;
    }
    return AreEqualPolynoms(lhs->pol, rhs->pol);
}

void FreeField(FiniteField f) {
    if (f != NULL) {
        FreePolynom(f->pol);
        free(f);
    }
}
