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
    //TODO: add normalise
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
