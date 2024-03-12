#include "minunit.h"
#include <finite_fields.h>
#include "string.h"

MU_TEST(zero_identity_different_fields) {
    FiniteField field2 = CreateF_p(2);
    FieldElement ident_2 = GetIdentity(field2);
    FieldElement zero_2 = GetZero(field2);

    mu_check(ident_2->coeff_size == 1);
    mu_check(zero_2->coeff_size == 1);
    mu_check(ident_2->coefficients[0] == 1);
    mu_check(zero_2->coefficients[0] == 0);

    uint8_t polynom[] = {1, 0, 0, 0, 2};
    FiniteField field5 = CreateF_q(5, 4, polynom);
    FieldElement ident_5_4 = GetIdentity(field5);
    FieldElement zero_5_4 = GetZero(field5);

    mu_check(ident_5_4->coeff_size == 1);
    mu_check(zero_5_4->coeff_size == 1);
    mu_check(ident_5_4->coefficients[0] == 1);
    mu_check(zero_5_4->coefficients[0] == 0);

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
//https://math.stackexchange.com/questions/3738494/showing-x4-2-is-irreducible-in-f-5x
MU_TEST(neg_test) {
    int polynom[] = {1, 0, 0, 0, 2};
    FiniteField field = CreateF_q(5, 4, polynom);
    int target[] = {4, 3, 0, 1, 3};
    FieldElement elem = GetFromArray(field, target, 5);
    mu_check(elem != NULL);
    FieldElement neg_elem = Neg(elem);
    mu_check(neg_elem != NULL);
    FieldElement res = Add(elem, neg_elem);
    mu_check(res->coeff_size == 1);
    mu_check(res->coefficients[0] == 0);

    FreeField(field);
    FreeElement(elem);
    FreeElement(neg_elem);
    FreeElement(res);
}

MU_TEST(copy_test) {
    uint8_t polynom[] = {1, 0, 0, 0, 2};
    FiniteField field = CreateF_q(5, 4, polynom);
    uint8_t target[] = {4, 3, 0, 1, 3};
    FieldElement elem = GetFromArray(field, target, 5);
    mu_check(elem != NULL);
    FieldElement cpy_elem = Copy(elem);
    mu_check(cpy_elem != NULL);
    FieldElement res = Sub(elem, cpy_elem);
    mu_check(res->coeff_size == 1);
    mu_check(res->coefficients[0] == 0);

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

MU_TEST(addition_subtraction_test_F_q) {
    int polynom[] = {1, 0, 0, 0, 2};
    FiniteField field = CreateF_q(5, 4, polynom);

    int target[] = {4, 3, 0, 1, 3};
    FieldElement lhs = GetFromArray(field, target, 5);

    FieldElement add_res = Add(lhs, lhs);

    mu_check(add_res->coeff_size == 5);
    uint8_t res_pol[] = {1, 2, 0, 1, 3}; //little - endian
    for (int i = 0; i < 5; i++) {
        //printf("\n\n%d\n\n", add_res->coefficients[i]);
        mu_check(add_res->coefficients[i] == res_pol[i]);
    }

    FieldElement sub_res = Sub(lhs, add_res);
    mu_check(sub_res->coeff_size == 5);
    uint8_t res2_pol[] = {2, 4, 0, 2, 1};
    for (int i = 0; i < 5; i++) {
        mu_check(sub_res->coefficients[i] == res2_pol[i]);
    }
    FreeElement(sub_res);

    FiniteField GF7 = CreateF_p(7);
    FieldElement sum = GetZero(GF7);
    FieldElement identity = GetIdentity(GF7);
    for (int i = 0; i < 37; i++) {
        mu_check(sum->coeff_size == 1);
        mu_check(sum->coefficients[0] == i % 7);
        FieldElement tmp = sum;
        sum = Add(sum, identity);
        FreeElement(tmp);
    }
    FreeElement(sum);
    FreeElement(identity);
    FreeField(GF7);
    FreeElement(lhs);
    FreeElement(add_res);
    FreeField(field);
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
    MU_RUN_TEST(addition_subtraction_test_F_q);
    MU_RUN_TEST(test_binary);
}

int main() {
    MU_RUN_SUITE(utils);
    MU_RUN_SUITE(operations);
    MU_REPORT();
    return MU_EXIT_CODE;
}
