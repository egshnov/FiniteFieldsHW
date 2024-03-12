#include "field_element.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

#define MAX(a, b) (((a)>(b))?(a):(b))

static FieldElement init(FiniteField f, uint8_t n) {
    FieldElement element = (FieldElement) malloc(sizeof(struct FieldElement));
    if (element != NULL) {
        element->field = f;
        element->coeff_size = n;
        element->coefficients = (uint8_t *) (malloc(sizeof(uint8_t) * element->coeff_size));
        if (element->coefficients == NULL) {
            free(element);
            return NULL;
        }
    }
    return element;
}

FieldElement GetIdentity(FiniteField f) {
    FieldElement element = init(f, 1);
    if (element != NULL) {
        element->coefficients[0] = 1;
    }
    return element;
}

FieldElement GetZero(FiniteField f) {
    FieldElement element = init(f, 1);
    if (element != NULL) {
        element->coefficients[0] = 0;
    }
    return element;
}

static uint8_t mod(int lhs, uint8_t p) {
    if (lhs < 0) return p - ((-lhs) % p);
    return lhs % p;
}

static bool trim_zeroes(FieldElement target) {
    uint8_t ind = target->coeff_size - 1;
    while (target->coefficients[ind] == 0 && ind > 0) {
        ind--;
    }
    target->coeff_size = ind + 1;
    uint8_t *insurance = target->coefficients;
    target->coefficients = (uint8_t *) realloc(target->coefficients, target->coeff_size * sizeof(uint8_t));
    if (target->coefficients == NULL) {
        free(insurance);
        return false;
    }
    return true;
}

//array coefficients are stored as BigEndian
//array_size is the size of array
FieldElement GetFromArray(FiniteField f, int const *array, uint64_t array_size) {
    if (array_size > f->polynom_size) return NULL;
    FieldElement element = init(f, array_size);
    if (element != NULL) {
        for (uint64_t i = array_size - 1, ind = 0; i > 0; i--, ind++) {
            element->coefficients[ind] = mod(array[i], element->field->p);
        }
        element->coefficients[array_size - 1] = array[0] % element->field->p;
    }
    if (!trim_zeroes(element)) {
        free(element);
        return false;
    }
    return element;
}

bool InSameField(FieldElement lhs, FieldElement rhs) {
    return AreEqualFields(lhs->field, rhs->field);
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
/* если поля различаются только многочленом по которому факторизуем то они изоморфны и
    * можно было бы считать что в одном поле но тогда непонятно как проводить операции */
FieldElement Add(FieldElement lhs, FieldElement rhs) {
    if (!InSameField(lhs, rhs)) {
        return NULL;
    }
    FieldElement res = init(lhs->field, MAX(lhs->coeff_size, rhs->coeff_size));
    if (res == NULL) return NULL;
    for (size_t i = 0; i < res->coeff_size; i++) {
        res->coefficients[i] = (get_ith_coeff(lhs, i) + get_ith_coeff(rhs, i)) % lhs->field->p;
    }
    if (!trim_zeroes(res)) {
        free(res);
        return NULL;
    }
    return res;
}

static uint8_t inv_coeff(uint8_t p, uint8_t coeff) {
    return (p - coeff) % p;
}

//returns NULL if error occurred
FieldElement Neg(FieldElement elem) {
    FieldElement res = Copy(elem);
    if (res != NULL) {
        for (size_t i = 0; i < res->coeff_size; i++) {
            res->coefficients[i] = inv_coeff(res->field->p, res->coefficients[i]);
        }
    }
    return res;
}

//returns NULL if error occurred
FieldElement Sub(FieldElement lhs, FieldElement rhs) {
    FieldElement tmp = Neg(rhs);
    if (tmp != NULL) {
        FieldElement res = Add(lhs, tmp);
        FreeElement(tmp);
        return res;
    }
    return NULL;
}

FieldElement Mult(FieldElement lhs, FieldElement rhs) {
    uint64_t size_l = lhs->coeff_size - 1;
    uint64_t size_r = rhs->coeff_size - 1;
    for (size_t i = 0; i < size_l + size_r; i++) {

    }
}
