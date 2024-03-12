#include "finite_field.h"
#include "stdio.h"

FiniteField CreateF_p(uint8_t p) {
    FiniteField field = (FiniteField) malloc(sizeof(struct FiniteField));
    if (field != NULL) {
        uint8_t *pol = (uint8_t *) malloc(sizeof(uint8_t));
        if (pol == NULL) {
            free(field);
            return NULL;
        }
        *pol = p;
        field->polynom_size = 1;
        field->p = p;
        field->polynom = pol;
        return field;
    }
    return NULL;
}

//given polynom is big-endian, stored as little-endian

FiniteField CreateF_q(uint8_t p, uint64_t deg_polynom, uint8_t const *polynom) {
    FiniteField field = (FiniteField) malloc(sizeof(struct FiniteField));
    if (field != NULL) {
        field->polynom_size = deg_polynom + 1;
        uint8_t *pol = (uint8_t *) malloc(sizeof(uint8_t) * (field->polynom_size));
        if (pol == NULL) {
            free(field);
            return NULL;
        }
        for (uint64_t i = deg_polynom, ind = 0; i > 0; i--, ind++) {
            pol[ind] = polynom[i] % p;
        }
        pol[deg_polynom] = polynom[0] % p;
        field->polynom = pol;
        field->p = p;
        return field;
    }
    return NULL;
}

void PrintField(FiniteField f) {
    printf("p is %d\n", f->p);
    printf("polynom size is %lu\n", f->polynom_size);
    printf("\n");
    for (int i = f->polynom_size - 1; i > 0; i--) {
        printf("%d * x^%d + ", f->polynom[i], i);
    }
    printf("%d\n", f->polynom[0]);


}

bool AreEqualFields(FiniteField lhs, FiniteField rhs) {
    if (lhs == rhs) {
        return true;
    }
    if (lhs->p != rhs->p || lhs->polynom_size != rhs->polynom_size) {
        return false;
    }
    for (int i = 0; i < lhs->polynom_size; i++) {
        if (lhs->polynom[i] != rhs->polynom[i]) {
            return false;
        }
    }
    return true;
}

void FreeField(FiniteField f) {
    free(f->polynom);
    free(f);
}
