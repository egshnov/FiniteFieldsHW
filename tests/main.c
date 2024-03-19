#include "minunit.h"
#include <finite_fields.h>
#include <string.h>

static uint8_t mod(int lhs, uint8_t p) {
    if (lhs < 0) return (p - ((-lhs) % p)) % p;
    return lhs % p;
}

static int int_fast_pow(int val, int pow) {
    if (val == 0) return 0;
    if (val == 1) return 1;
    int result = 1;
    while (pow > 0) {
        if (pow % 2 == 1) {
            result *= val;
        }
        val *= val;
        pow /= 2;
    }
    return result;
}

static void
operation_test(uint8_t p, uint8_t deg, int *irreducible, int *lhs_pol, uint8_t lhs_size, int *rhs_pol,
               uint8_t rhs_size,
               int *res_pol, uint8_t res_size, FieldElement (*operation)(FieldElement, FieldElement)) {
    FiniteField field = CreateF_q(p, deg, irreducible);
    FieldElement lhs = GetFromArray(field, lhs_pol, lhs_size);
    FieldElement rhs = GetFromArray(field, rhs_pol, rhs_size);
    FieldElement res = operation(lhs, rhs);
    mu_check(res->pol->coeff_size == res_size);
    for (int i = 0; i < res_size; i++) {
        mu_check(res_pol[i] == res->pol->coefficients[res_size - i - 1]);
    }
    FreeElement(lhs);
    FreeElement(rhs);
    FreeElement(res);
    FreeField(field);
}

MU_TEST(zero_identity_different_fields) {
    FiniteField field2 = CreateF_p(2);
    FieldElement ident_2 = GetIdentity(field2);
    FieldElement zero_2 = GetZero(field2);
    mu_check(ident_2->pol->coeff_size == 1);
    mu_check(zero_2->pol->coeff_size == 1);
    mu_check(ident_2->pol->coefficients[0] == 1);
    mu_check(zero_2->pol->coefficients[0] == 0);

    int polynom[] = {1, 0, 0, 0, 2};
    FiniteField field5 = CreateF_q(5, 4, polynom);
    FieldElement ident_5_4 = GetIdentity(field5);
    FieldElement zero_5_4 = GetZero(field5);

    mu_check(ident_5_4->pol->coeff_size == 1);
    mu_check(zero_5_4->pol->coeff_size == 1);
    mu_check(ident_5_4->pol->coefficients[0] == 1);
    mu_check(zero_5_4->pol->coefficients[0] == 0);

    FreeElement(ident_5_4);
    FreeElement(zero_5_4);
    FreeField(field2);
    FreeElement(ident_2);
    FreeElement(zero_2);
    FreeField(field5);

}

MU_TEST(same_field_test) {
    FiniteField GF2 = CreateF_p(2);
    FiniteField GF22 = CreateF_p(2);
    mu_check(AreEqualFields(GF2, GF2));
    mu_check(AreEqualFields(GF2, GF22));

    int polynom[] = {1, -2, 4};
    FiniteField GF5_2 = CreateF_q(5, 2, polynom);
    FiniteField GF5_2_2 = CreateF_q(5, 2, polynom);

    mu_check(AreEqualFields(GF5_2, GF5_2_2));
    mu_check(!AreEqualFields(GF2, GF5_2_2));
    FiniteField GF7 = CreateF_p(7);
    mu_check(!AreEqualFields(GF7, GF2));
    int polynom2[] = {1, -1, 2};
    FiniteField GF5_2_3 = CreateF_q(5, 2, polynom2);
    mu_check(!AreEqualFields(GF5_2, GF5_2_3));
    FreeField(GF5_2_3);
    FreeField(GF7);
    FreeField(GF22);
    FreeField(GF2);
    FreeField(GF5_2);
    FreeField(GF5_2_2);
}

MU_TEST(neg_test) {
    int polynom[] = {1, 0, 0, 0, 2};
    FiniteField field = CreateF_q(5, 4, polynom);
    int target[] = {4, 3, 0, 1, 3};
    FieldElement elem = GetFromArray(field, target, 5);
    mu_check(elem != NULL);
    FieldElement neg_elem = Neg(elem);
    mu_check(neg_elem != NULL);
    FieldElement res = Add(elem, neg_elem);
    mu_check(res->pol->coeff_size == 1);
    mu_check(res->pol->coefficients[0] == 0);

    FreeField(field);
    FreeElement(elem);
    FreeElement(neg_elem);
    FreeElement(res);
}

MU_TEST(copy_test) {
    int polynom[] = {1, 0, 0, 0, 2};
    FiniteField field = CreateF_q(5, 4, polynom);
    int target[] = {4, 3, 0, 1, 3};
    FieldElement elem = GetFromArray(field, target, 5);
    mu_check(elem != NULL);
    FieldElement cpy_elem = Copy(elem);
    mu_check(cpy_elem != NULL);
    FieldElement res = Sub(elem, cpy_elem);
    mu_check(res->pol->coeff_size == 1);
    mu_check(res->pol->coefficients[0] == 0);

    FreeField(field);
    FreeElement(elem);
    FreeElement(cpy_elem);
    FreeElement(res);
}

MU_TEST_SUITE(utils) {
    MU_RUN_TEST(zero_identity_different_fields);
    MU_RUN_TEST(neg_test);
    MU_RUN_TEST(same_field_test);
    MU_RUN_TEST(copy_test);
}

MU_TEST(addition_subtraction_test) {
    {
        int irreducible[] = {1, 1, 2};
        int lhs_pol[] = {1, 1};
        int res_pol[] = {2, 2};
        operation_test(5, 2, irreducible, lhs_pol, 2, lhs_pol, 2, res_pol, 2, &Add);
    }
    {
        int irreducible[] = {1, 1, 2};
        int lhs_pol[] = {1, 1};
        int res_pol[] = {0};
        operation_test(5, 2, irreducible, lhs_pol, 2, lhs_pol, 2, res_pol, 1, &Sub);
    }
    {

        int irreducible[] = {1, 0, 1, 0, 0, 0, 1, 4, 7};    //x^8+x^6+x^2+4*x+7
        int lhs_pol[] = {1, 0, 3, 0, 0, 4};
        int rhs_pol[] = {6, 0, 0, 4, 0, 0, 0};
        int res_pol[] = {6, 1, 0, 7, 0, 0, 4};
        operation_test(11, 8, irreducible, lhs_pol, 6, rhs_pol, 7, res_pol, 7, &Add);
    }
    {

        int irreducible[] = {1, 0, 1, 0, 0, 0, 1, 4, 7};    //x^8+x^6+x^2+4*x+7
        int lhs_pol[] = {1, 0, 3, 0, 0, 4};
        int res_pol[] = {0};
        operation_test(11, 8, irreducible, lhs_pol, 6, lhs_pol, 6, res_pol, 1, &Sub);
    }
    {
        int irreducible[] = {1, 0, 1, 0, 0, 0, 1, 4, 7};    //x^8+x^6+x^2+4*x+7
        int lhs_pol[] = {1, 0, 3, 0, 0, 4};
        int rhs_pol[] = {6, 0, 0, 4, 0, 0, 0};
        int res_pol[] = {5, 1, 0, 10, 0, 0, 4};
        operation_test(11, 8, irreducible, lhs_pol, 6, rhs_pol, 7, res_pol, 7, &Sub);
    }
    {
        int irreducible[] = {1, 0, 0, 1, 1};
        int lhs[] = {1, 1, 1};
        int rhs[] = {1, 1};
        int res[] = {1, 0, 0};
        operation_test(2, 4, irreducible, lhs, 3, rhs, 2, res, 3, &Add);

    }
    {
        int irreducible[] = {1, 0, 0, 1, 1};
        int lhs[] = {1, 0, 0, 0};
        int rhs[] = {1, 0, 1};
        int res[] = {1, 1, 0, 1};
        operation_test(2, 4, irreducible, lhs, 4, rhs, 3, res, 4, &Add);
    }
    {
        FiniteField GF7 = CreateF_p(7);
        FieldElement sum = GetZero(GF7);
        FieldElement identity = GetIdentity(GF7);
        for (int i = 0; i < 37; i++) {
            mu_check(sum->pol->coeff_size == 1);
            mu_check(sum->pol->coefficients[0] == i % 7);
            FieldElement tmp = sum;
            sum = Add(sum, identity);
            FreeElement(tmp);
        }
        FreeElement(sum);
        FreeElement(identity);
        FreeField(GF7);
    }
    {
        FiniteField GF7 = CreateF_p(7);
        FieldElement sum = GetZero(GF7);
        FieldElement identity = GetIdentity(GF7);
        for (int i = 0; i < 37; i++) {
            mu_check(sum->pol->coeff_size == 1);
            mu_check(sum->pol->coefficients[0] == mod(-i, 7));
            FieldElement tmp = sum;
            sum = Sub(sum, identity);
            FreeElement(tmp);
        }
        FreeElement(sum);
        FreeElement(identity);
        FreeField(GF7);
    }
}

MU_TEST(multiplication_test) {
    {
        int irreducible[] = {1, 0, 0, 0, 1, 1, 0, 1, 1}; //x^8 + x^4 + x^3 + x + 1
        int lhs_pol[] = {1, 0, 1, 0, 0, 1, 1}; // x^6 + x^4 + x + 1
        int rhs_pol[] = {1, 1, 0, 0, 1, 0, 1, 0}; // x^7 + x^6 + x^3 + x
        int res_pol[] = {1}; //little endian
        operation_test(2, 8, irreducible, lhs_pol, 7, rhs_pol, 8, res_pol, 1, &Mult);
    }
    {
        int irreducible[] = {1, 0, 0, 1, 1};
        int lhs[] = {1, 1, 1};
        int rhs[] = {1, 1};
        int res[] = {1, 0, 0, 1};
        operation_test(2, 4, irreducible, lhs, 3, rhs, 2, res, 4, &Mult);
    }
    {
        int irreducible[] = {1, 0, 0, 1, 1};
        int lhs[] = {1, 0, 0, 0};
        int rhs[] = {1, 0, 1};
        int res[] = {1, 1, 1, 0};
        operation_test(2, 4, irreducible, lhs, 4, rhs, 3, res, 4, &Mult);
    }
    {
        int irreducible[] = {1, 0, 0, 1, 1, 1, 0, 1};
        int lhs[] = {1, 1, 0};
        int rhs[] = {1, 0, 1, 0, 1};
        int res[] = {1, 1, 1, 1, 1, 1, 0};
        operation_test(2, 7, irreducible, lhs, 3, rhs, 5, res, 7, &Mult);
    }
    {
        FiniteField GF7 = CreateF_p(7);
        for (int i = 0; i < 37; i++) {
            for (int j = 1; j < 37; j++) {
                int l[] = {i};
                int r[] = {j};
                FieldElement lhs = GetFromArray(GF7, l, 1);
                FieldElement rhs = GetFromArray(GF7, r, 1);
                FieldElement res = Mult(lhs, rhs);
                mu_check(res->pol->coeff_size == 1);
                mu_check(res->pol->coefficients[0] == mod(i * j, 7));
                FreeElement(res);
                FreeElement(lhs);
                FreeElement(rhs);
            }
        }
        FreeField(GF7);
    }


}


MU_TEST(pow_test) {
    FiniteField field = CreateF_p(7);
    FieldElement base = GetZero(field);
    FieldElement ident = GetIdentity(field);
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++) {
            FieldElement tmp = Pow(base, j);
            mu_check(tmp->pol->coeff_size == 1);
            mu_check(tmp->pol->coefficients[0] == mod(int_fast_pow(i, j), 7));
            FreeElement(tmp);
        }
        FieldElement tmp = base;
        base = Add(base, ident);
        FreeElement(tmp);
    }
    FreeElement(base);
    FreeElement(ident);
    FreeField(field);
}

static void inversion(uint8_t p, uint8_t deg, int *irreducible, int *target, int target_size) {
    FiniteField field = CreateF_q(p, deg, irreducible);
    FieldElement elem = GetFromArray(field, target, target_size);
    FieldElement elem_inv = Inv(elem);
    FieldElement res = Mult(elem, elem_inv);
    mu_check(res->pol->coeff_size == 1);
    mu_check(res->pol->coefficients[0] == 1);
    FreeElement(elem);
    FreeElement(elem_inv);
    FreeElement(res);
    FreeField(field);
}

MU_TEST(inv_test) {
    {
        int irreducible[] = {1, 1, 2};
        int lhs_pol[] = {1, 1};
        inversion(5, 2, irreducible, lhs_pol, 2);
    }
    {
        int irreducible[] = {1, 0, 0, 0, 1, 1, 0, 1, 1}; //x^8 + x^4 + x^3 + x + 1
        int lhs_pol[] = {1, 0, 1, 0, 0, 1, 1}; // x^6 + x^4 + x + 1
        int rhs_pol[] = {1, 1, 0, 0, 1, 0, 1, 0}; // x^7 + x^6 + x^3 + x
        inversion(2, 8, irreducible, lhs_pol, 7);
        inversion(2, 8, irreducible, rhs_pol, 8);
    }
    {
        int irreducible[] = {1, 0, 0, 1, 1};
        int lhs[] = {1, 1, 1};
        int rhs[] = {1, 1};
        int res[] = {1, 0, 0, 1};
        inversion(2, 4, irreducible, lhs, 3);
        inversion(2, 4, irreducible, rhs, 2);
        inversion(2, 4, irreducible, res, 4);
    }
    {
        int irreducible[] = {1, 0, 0, 1, 1};
        int lhs[] = {1, 0, 0, 0};
        int rhs[] = {1, 0, 1};
        int res[] = {1, 1, 1, 0};
        inversion(2, 4, irreducible, lhs, 4);
        inversion(2, 4, irreducible, rhs, 3);
        inversion(2, 4, irreducible, res, 4);
    }
    {
        FiniteField f = CreateF_p(7);
        for (int i = 1; i < 7; i++) {
            int arr[] = {1};
            FieldElement elem = GetFromArray(f, arr, 1);
            FieldElement inv_elem = Inv(elem);
            FieldElement res = Mult(elem, inv_elem);
            mu_check(IsIdentity(res));
            FreeElement(elem);
            FreeElement(inv_elem);
            FreeElement(res);
        }
        FreeField(f);
    }

}

MU_TEST(division_test) {
    {
        int irreducible[] = {1, 0, 1, 1};
        int lhs_pol[] = {2, 1, 3};
        int rhs_pol[] = {3, 2, 1};
        int res_pol[] = {1, 6, 2};
        operation_test(7, 3, irreducible, lhs_pol, 3, rhs_pol, 3, res_pol, 3, &Division);
    }

    {
        int irreducible[] = {1, 0, 0, 0, 1, 1, 0, 1, 1}; //x^8 + x^4 + x^3 + x + 1
        int lhs_pol[] = {1, 0, 1, 0, 0, 1, 1}; // x^6 + x^4 + x + 1
        int rhs_pol[] = {1, 1, 0, 0, 1, 0, 1, 0}; // x^7 + x^6 + x^3 + x
        int res_pol[] = {1}; //little endian
        operation_test(2, 8, irreducible, res_pol, 1, rhs_pol, 8, lhs_pol, 7, &Division);
        operation_test(2, 8, irreducible, res_pol, 1, lhs_pol, 7, rhs_pol, 8, &Division);
    }

    {
        int irreducible[] = {1, 0, 0, 1, 1};
        int lhs[] = {1, 1, 1};
        int rhs[] = {1, 1};
        int res[] = {1, 0, 0, 1};
        operation_test(2, 4, irreducible, res, 4, rhs, 2, lhs, 3, &Division);
        operation_test(2, 4, irreducible, res, 4, lhs, 3, rhs, 2, &Division);
    }
    {
        int irreducible[] = {1, 0, 0, 1, 1};
        int lhs[] = {1, 0, 0, 0};
        int rhs[] = {1, 0, 1};
        int res[] = {1, 1, 1, 0};
        operation_test(2, 4, irreducible, res, 4, lhs, 4, rhs, 3, &Division);
        operation_test(2, 4, irreducible, res, 4, rhs, 3, lhs, 4, &Division);
    }
    {
        int irreducible[] = {1, 0, 0, 1, 1, 1, 0, 1};
        int lhs[] = {1, 1, 0};
        int rhs[] = {1, 0, 1, 0, 1};
        int res[] = {1, 1, 1, 1, 1, 1, 0};
        operation_test(2, 7, irreducible, res, 7, rhs, 5, lhs, 3, &Division);
        operation_test(2, 7, irreducible, res, 7, lhs, 3, rhs, 5, &Division);
    }

}

//x^32 + x^28 + x^19 + x^18 + x^16 + x^14 + x^11 + x^10 + x^9 + x^6 + x^5 + x^1 + 1
MU_TEST(test_binary) {
    int arr[] = {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1};
    FiniteField field = CreateF_q(2, 32, arr);
    for (uint32_t i = 0; i < 1000; i++) {
        FieldElement elem = FromUint32(field, i);
        uint32_t res = ToUint32(elem);
        mu_check(res == i);
        FreeElement(elem);
    }
    FreeField(field);
}

MU_TEST_SUITE(operations) {
    MU_RUN_TEST(addition_subtraction_test);
    MU_RUN_TEST(multiplication_test);
    MU_RUN_TEST(pow_test);
    MU_RUN_TEST(inv_test);
    MU_RUN_TEST(division_test);
    MU_RUN_TEST(test_binary);
}


int main() {
    MU_RUN_SUITE(utils);
    MU_RUN_SUITE(operations);
    MU_REPORT();
    return MU_EXIT_CODE;
}
