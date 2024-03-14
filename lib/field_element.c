#include "field_element.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

#define MAX(a, b) (((a)>(b))?(a):(b))

static FieldElement init(FiniteField f) {
    FieldElement element = (FieldElement) malloc(sizeof(struct FieldElement));
    if (element != NULL) {
        element->field = f;
    }
    return element;
}

static bool descend(FieldElement elem) {
    Polynom dummy = elem->pol;
    elem->pol = ModPolynom(elem->pol, elem->field->pol, elem->field->p);
    FreePolynom(dummy);
    if (elem->pol == NULL) {
        free(elem);
        return false;
    }
    return true;
}

uint64_t GetDeg(FieldElement element) {
    return PolynomDeg(element->pol);
}

FieldElement GetIdentity(FiniteField f) {
    FieldElement element = init(f);
    if (element == NULL) return NULL;
    element->pol = IdentityPolynom();
    if (element->pol == NULL) {
        free(element);
        return NULL;
    }
    return element;
}

FieldElement GetZero(FiniteField f) {
    FieldElement element = init(f);
    if (element == NULL) return NULL;
    element->pol = ZeroPolynom();
    if (element->pol == NULL) {
        free(element);
        return NULL;
    }

    return element;
}

//array coefficients are stored as BigEndian
//array_size is the size of array
FieldElement GetFromArray(FiniteField f, int const *array, uint64_t array_size) {
    FieldElement element = init(f);
    if (element == NULL) return NULL;
    element->pol = PolynomFromArray(array, array_size, f->p);
    if (element->pol == NULL) {
        free(element);
        return NULL;
    }
    if (!descend(element)) {
        return NULL;
    }
    return element;
    return element;
}

bool InSameField(FieldElement lhs, FieldElement rhs) {
    return AreEqualFields(lhs->field, rhs->field);
}

//return NUll if error occurred
FieldElement Copy(FieldElement elem) {
    FieldElement res = init(elem->field);
    if (res != NULL) {
        res->pol = CopyPolynom(elem->pol);
    }
    return res;
}

void FreeElement(FieldElement elem) {
    if (elem != NULL) {
        FreePolynom(elem->pol);
    }
    free(elem);
}

//returns NULL if ERROR occurred
FieldElement Add(FieldElement lhs, FieldElement rhs) {
    if (!InSameField(lhs, rhs)) {
        return NULL;
    }
    FieldElement res = init(lhs->field);
    if (res == NULL) return NULL;
    res->pol = AddPolynom(lhs->pol, rhs->pol, lhs->field->p);
    if (res->pol == NULL) {
        free(res);
        return NULL;
    }
    return res;
}

FieldElement Mult(FieldElement lhs, FieldElement rhs) {
    if (!InSameField(lhs, rhs)) {
        return NULL;
    }
    FieldElement res = init(lhs->field);
    if (res == NULL) return NULL;
    res->pol = MultPolynom(lhs->pol, rhs->pol, lhs->field->p);
    if (res->pol == NULL) {
        free(res);
        return NULL;
    }
    if (!descend(res)) {
        return NULL;
    }
    return res;
}

//p > 0
FieldElement pow(FieldElement elem, unsigned int p) {
    FieldElement res = GetIdentity(elem->field);
    FieldElement value = Copy(elem);
    unsigned int tmp = p;
    while (tmp > 0) {
        if (p % 2 == 1) {
            FieldElement dummy = res;
            res = Mult(res, value);
            FreeElement(dummy);
            if (res == NULL) return NULL;
        }
        FieldElement dummy = value;
        value = Mult(value, value);
        FreeElement(dummy);
        if (value == NULL) return NULL;
        tmp /= 2;
    }
    FreeElement(value);
    return res;
}

static int fast_pow(int val, int pow) {
    int result = 1;
    while (pow > 0) {
        if (pow % 2 == 0) {
            result *= val;
        }
        val *= val;
        pow /= 2;
    }
    return result;
}

FieldElement Inv(FieldElement element) {
    return pow(element, fast_pow(element->field->p, PolynomDeg(element->field->pol)) - 2);
}

FieldElement Pow(FieldElement elem, int p) {
    if (p < 0) {
        FieldElement tmp = Inv(elem);
        FieldElement res = pow(tmp, -p);
        FreeElement(tmp);
        return res;
    }
    return pow(elem, p);
}

FieldElement Division(FieldElement lhs, FieldElement rhs) {
    FieldElement tmp = Inv(rhs);
    FieldElement res = Mult(lhs, tmp);
    FreeElement(tmp);
    return res;
}

//returns NULL if error occurred
FieldElement Neg(FieldElement elem) {
    FieldElement res = init(elem->field);
    if (res == NULL) return NULL;
    res->pol = NegPolynom(elem->pol, elem->field->p);
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
