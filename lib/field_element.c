#include "field_element.h"
#include "stdlib.h"

#define MAX(a, b) (((a)>(b))?(a):(b))

static FieldElement init(FiniteField *f, uint8_t n) {
    FieldElement element = (FieldElement) malloc(sizeof(struct FieldElement));
    if (element != NULL) {
        element->field = f;
        element->coeff_size = 1;
        element->coefficients = (uint8_t *) (malloc(sizeof(uint8_t) * n));
        if (element->coefficients == NULL) {
            free(element);
            return NULL;
        }
    }
    return element;
}

FieldElement GetIdentity(FiniteField *f) {
    FieldElement element = init(f, 1);
    if (element != NULL) {
        element->coefficients[0] = 1;
    }
    return element;
}

FieldElement GetZero(FiniteField *f) {
    FieldElement element = init(f, 1);
    if (element != NULL) {
        element->coefficients[0] = 0;
    }
    return element;
}

//TODO: check whether n > f->max_n is correct
//polynom coefficients are stored as BigEndian
FieldElement GetFromArray(FiniteField *f, uint8_t const *polynom, uint64_t n) {
    if (n > f->max_n) return NULL;
    FieldElement element = init(f, n);
    if (element != NULL) {
        for (uint64_t i = n - 1, ind = 0; i > 0; i--, ind++) {
            element->coefficients[ind] = polynom[i] % element->field->p;
        }
        element->coefficients[n - 1] = polynom[0] % element->field->p;
    }
    return element;
}

bool InSameField(FieldElement lhs, FieldElement rhs) {
    if (lhs->field == rhs->field) {
        return true;
    }
    if (lhs->field->p != rhs->field->p || lhs->field->max_n != rhs->field->max_n) {
        return false;
    }
    for (int i = 0; i < lhs->field->max_n; i++) {
        if (lhs->field->polynom[i] != rhs->field->polynom[i]) {
            return false;
        }
    }
    return true;
}


//return NUll if error occurred
FieldElement Copy(FieldElement elem) {
    FieldElement res = init(elem->field, elem->coeff_size);
    if (res != NULL) {
        for (size_t i = 0; i < elem->coeff_size; i++) {
            res->coefficients[i] = elem->coefficients[i];
        }
    }
    return res;
}

void FreeElement(FieldElement elem) {
    free(elem->coefficients);
    free(elem);
}

static uint8_t get_ith_coeff(FieldElement elem, size_t i) {
    return i < elem->coeff_size ? elem->coefficients[i] : 0;
}

//returns NULL if ERROR occurred
FieldElement Add(FieldElement lhs, FieldElement rhs) {
    /* если поля различаются только многочленом по которому факторизуем то они изоморфны и
     * можно было бы считать что в одном поле но тогда непонятно как проводить операции */
    if (!InSameField(lhs, rhs)) {
        return NULL;
    }
    FieldElement res = init(lhs->field, MAX(lhs->coeff_size, rhs->coeff_size));
    if (res == NULL) return NULL;
    for (size_t i = 0; i < res->coeff_size; i++) {
        res->coefficients[i] = (get_ith_coeff(lhs, i) + get_ith_coeff(rhs, i)) % lhs->field->p;
    }
    uint8_t ind = res->coeff_size - 1;
    while (res->coefficients[ind] == 0) {
        ind--;
    }
    res->coefficients = (uint8_t *) realloc(res->coefficients, ind + 1);
    if (res->coefficients == NULL) {
        free(res);
        return NULL;
    }
    return res;
}

static uint8_t inv_coeff(uint8_t p, uint8_t coeff) {
    return (p - coeff) % p;
}

FieldElement Neg(FieldElement elem) {
    FieldElement res = Copy(elem);
    if (res != NULL) {
        for (size_t i = 0; i < res->coeff_size; i++) {
            res->coefficients[i] = inv_coeff(res->field->p, res->coefficients[i]);
        }
    }
    return res;
}

FieldElement Sub(FieldElement lhs, FieldElement rhs) {
    FieldElement tmp = Neg(rhs);
    if (tmp != NULL) {
        FieldElement res = Add(lhs, tmp);
        FreeElement(tmp);
        return res;
    }
    return NULL;
}
