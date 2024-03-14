#include "finite_field.h"
#include "stdio.h"

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

FiniteField CreateF_q(uint8_t p, uint64_t deg_polynom, int const *polynom) {
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
//FiniteField CreateF_q(uint8_t p, uint64_t deg_polynom, int const *polynom) {
//    FiniteField field = (FiniteField) malloc(sizeof(struct FiniteField));
//    if (field != NULL) {
//        field->pol->coeff_size = deg_polynom + 1;
//        uint8_t *pol = (uint8_t *) malloc(sizeof(uint8_t) * (field->pol->coeff_size));
//        if (pol == NULL) {
//            free(field);
//            return NULL;
//        }
//        for (uint64_t i = deg_polynom, ind = 0; i > 0; i--, ind++) {
//            pol[ind] = polynom[i] % p;
//        }
//        pol[deg_polynom] = polynom[0] % p;
//        field->pol->coefficients = pol;
//        field->p = p;
//        return field;
//    }
//    return NULL;
//}

bool AreEqualFields(FiniteField lhs, FiniteField rhs) {
    if (lhs == rhs) {
        return true;
    }
    if (lhs->p != rhs->p) {
        return false;
    }
    return AreEqual(lhs->pol, rhs->pol);
}

void FreeField(FiniteField f) {
    if (f != NULL) {
        FreePolynom(f->pol);
        free(f);
    }
}
