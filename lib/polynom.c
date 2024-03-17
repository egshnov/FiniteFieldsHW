#include "polynom.h"
#include <stdlib.h>
#include <stdbool.h>

#define MAX(a, b) (((a)>(b))?(a):(b))

static uint8_t mod(int lhs, uint8_t p) {
    if (lhs < 0) return (p - ((-lhs) % p)) % p;
    return lhs % p;
}

static uint8_t get_ith_coeff(Polynom elem, size_t i) {
    return i < elem->coeff_size ? elem->coefficients[i] : 0;
}

static Polynom init(uint64_t n) {
    Polynom element = (Polynom) malloc(sizeof(struct Polynom));
    if (element != NULL) {
        element->coeff_size = n;
        element->coefficients = (uint8_t *) (malloc(sizeof(uint8_t) * element->coeff_size));
        if (element->coefficients == NULL) {
            free(element);
            return NULL;
        }
    }
    return element;
}

static bool trim_zeroes(Polynom target) {
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

static uint8_t inv_coeff(uint8_t p, uint8_t coeff) {
    return (p - coeff) % p;
}

uint64_t PolynomDeg(Polynom element) {
    return element->coeff_size - 1;
}

Polynom IdentityPolynom() {
    Polynom element = init(1);
    if (element != NULL) {
        element->coefficients[0] = 1;
    }
    return element;
}

Polynom ZeroPolynom() {
    Polynom element = init(1);
    if (element != NULL) {
        element->coefficients[0] = 0;
    }
    return element;
}

Polynom PolynomFromArray(int const *array, uint64_t array_size, uint8_t p) {
    Polynom element = init(array_size);
    if (element != NULL) {
        for (uint64_t i = array_size - 1, ind = 0; i > 0; i--, ind++) {
            element->coefficients[ind] = mod(array[i], p);
        }
        element->coefficients[array_size - 1] = mod(array[0], p);
    }
    if (!trim_zeroes(element)) {
        free(element);
        return false;
    }
    return element;
}

Polynom CopyPolynom(Polynom elem) {
    Polynom res = init(elem->coeff_size);
    if (res != NULL) {
        for (size_t i = 0; i < elem->coeff_size; i++) {
            res->coefficients[i] = elem->coefficients[i];
        }
    }
    return res;
}

void FreePolynom(Polynom elem) {
    if (elem != NULL) {
        free(elem->coefficients);
    }
    free(elem);
}

Polynom AddPolynom(Polynom lhs, Polynom rhs, uint8_t p) {
    Polynom res = init(MAX(lhs->coeff_size, rhs->coeff_size));
    if (res == NULL) return NULL;
    for (size_t i = 0; i < res->coeff_size; i++) {
        res->coefficients[i] = mod((get_ith_coeff(lhs, i) + get_ith_coeff(rhs, i)), p);
    }
    if (!trim_zeroes(res)) {
        free(res);
        return NULL;
    }
    return res;
}

Polynom NegPolynom(Polynom elem, uint8_t p) {
    Polynom res = CopyPolynom(elem);
    if (res != NULL) {
        for (size_t i = 0; i < res->coeff_size; i++) {
            res->coefficients[i] = inv_coeff(p, res->coefficients[i]);
        }
    }
    return res;
}

Polynom SubPolynom(Polynom lhs, Polynom rhs, uint8_t p) {
    Polynom tmp = NegPolynom(rhs, p);
    if (tmp != NULL) {
        Polynom res = AddPolynom(lhs, tmp, p);
        FreePolynom(tmp);
        return res;
    }
    return NULL;
}

bool AreEqualPolynoms(Polynom lhs, Polynom rhs) {
    if (lhs->coeff_size != rhs->coeff_size) {
        return false;
    }
    for (int i = 0; i < lhs->coeff_size; i++) {
        if (lhs->coefficients[i] != rhs->coefficients[i]) {
            return false;
        }
    }
    return true;
}

Polynom MultPolynom(Polynom lhs, Polynom rhs, uint8_t p) {
    Polynom res = init(lhs->coeff_size + rhs->coeff_size - 1);
    if (res == NULL) return NULL;
    for (int i = 0; i < res->coeff_size; i++) {
        res->coefficients[i] = 0;
    }
    for (int i = 0; i < lhs->coeff_size; i++) {
        for (int j = 0; j < rhs->coeff_size; j++) {
            res->coefficients[i + j] += mod(lhs->coefficients[i] * rhs->coefficients[j], p);
            res->coefficients[i + j] = mod(res->coefficients[i + j], p);
        }
    }
    if (!trim_zeroes(res)) {
        return NULL;
    }
    return res;
}

bool IsZeroPolynom(Polynom pol) {
    return pol->coeff_size == 1 && pol->coefficients[0] == 0;
}

bool IsIdentityPolynom(Polynom pol) {
    return pol->coeff_size == 1 && pol->coefficients[0] == 1;
}

Polynom ModPolynom(Polynom lhs, Polynom rhs, uint8_t p) {
    if (IsZeroPolynom(rhs)) return NULL;
    if (PolynomDeg(lhs) < PolynomDeg(rhs)) {
        return CopyPolynom(lhs);
    }
    Polynom remainder = CopyPolynom(lhs);
    if (remainder == NULL) return NULL;
    if (PolynomDeg(remainder) == 0 && PolynomDeg(rhs) == 0 && remainder->coefficients[0] < rhs->coefficients[0]) {
        remainder->coefficients[0] = mod(remainder->coefficients[0], rhs->coefficients[0]);
    } else {
        Polynom quotient = ZeroPolynom();
        if (quotient == NULL) {
            FreePolynom(remainder);
            return NULL;
        }
        while (!IsZeroPolynom(remainder) && PolynomDeg(remainder) >= PolynomDeg(rhs)) {
            uint8_t coeff = mod(
                    remainder->coefficients[remainder->coeff_size - 1] / rhs->coefficients[rhs->coeff_size - 1],
                    p);
            uint64_t deg = PolynomDeg(remainder) - PolynomDeg(rhs);
            int *tmp_coeffs = (int *) (malloc(sizeof(int) * (deg + 1)));
            tmp_coeffs[0] = coeff;
            for (int i = 1; i < deg + 1; i++) {
                tmp_coeffs[i] = 0;
            }
            Polynom tmp = PolynomFromArray(tmp_coeffs, deg + 1, p);
            Polynom dummy = quotient;

            quotient = AddPolynom(quotient, tmp, p);
            FreePolynom(dummy);
            dummy = remainder;
            Polynom mult = MultPolynom(tmp, rhs, p);
            remainder = SubPolynom(remainder, mult, p);

            FreePolynom(dummy);
            FreePolynom(mult);
            FreePolynom(tmp);
            free(tmp_coeffs);
        }
        FreePolynom(quotient);
    }
    return remainder;
}
