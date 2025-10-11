/** ==================================================================================================================*\
  @file UT_Calc_Ab.c

  @brief Testy jednostkowe do Calc_Ab
\*====================================================================================================================*/
#include "Std_Types.h"

#include "acutest.h"
#include "fff.h"

#include "Calc_Ab.c"

#define MAX32   (2147483647L)
#define MIN32   (-2147483648L)

#define A_ERROR_VALUE  (0xFFFFFFFF)

DEFINE_FFF_GLOBALS;

// Mock pamięci kalkulatora
static sint32 memory_value_fake;

FAKE_VALUE_FUNC(Std_ReturnType, Lib_Calc_Add, sint32, sint32, sint32*);
FAKE_VALUE_FUNC(Std_ReturnType, Lib_Calc_Sub, sint32, sint32, sint32*);
FAKE_VALUE_FUNC(Std_ReturnType, Lib_Calc_Mul, sint32, sint32, sint32*);
FAKE_VALUE_FUNC(Std_ReturnType, Lib_Calc_Div, sint32, sint32, sint32*);
FAKE_VOID_FUNC(Lib_Calc_MemS, sint32);
FAKE_VALUE_FUNC(sint32, Lib_Calc_MemR);

// Mocki do custom_fake
static Std_ReturnType mock_add_ok(sint32 a, sint32 b, sint32* r) {
    *r = a + b;
    return E_OK;
}
static Std_ReturnType mock_sub_ok(sint32 a, sint32 b, sint32* r) {
    *r = a - b;
    return E_OK;
}
static Std_ReturnType mock_mul_ok(sint32 a, sint32 b, sint32* r) {
    *r = a * b;
    return E_OK;
}
static Std_ReturnType mock_div_ok(sint32 a, sint32 b, sint32* r) {
    *r = b ? a / b : 0;
    return E_OK;
}

static Std_ReturnType mock_add_err(sint32 a, sint32 b, sint32* r) {
    return E_NOT_OK;
}

static Std_ReturnType mock_sub_err(sint32 a, sint32 b, sint32* r) {
    return E_NOT_OK;
}
static Std_ReturnType mock_mul_err(sint32 a, sint32 b, sint32* r) {
    *r = 0;
    return E_NOT_OK;
}
static Std_ReturnType mock_div_err(sint32 a, sint32 b, sint32* r) {
    *r = 0;
    return E_NOT_OK;
}

static void mock_Lib_Calc_MemS(sint32 mem) {
    memory_value_fake = mem;
}

static sint32 mock_Lib_Calc_MemR(void) {
    return memory_value_fake;
}

// -------------------------------------------------------------------------------------------------

/**
  @brief Test zerowania modułu

  Funkcja testująca reset modułu.
*/
void Test_Of_Calc_Reset(void)
{
    akumulator = 0xFFFFFFFFL;

    Calc_Reset();

    TEST_CHECK(Lib_Calc_MemS_fake.call_count == 1);
    TEST_CHECK(Lib_Calc_MemS_fake.arg0_val == 0L);
    TEST_CHECK(akumulator == 0L);
}

/**
  @brief Test ustawiania wartości akumulatora

  Funkcja testująca ustawianie wartości akumulatora.
*/
void Test_Of_Calc_Set_A(void)
{
    akumulator = 0x00000000L;

    Calc_Set_A(MIN32); 
    TEST_CHECK(akumulator == MIN32);

    akumulator = 0x00000000L;

    Calc_Set_A(MAX32);
    TEST_CHECK(akumulator == MAX32);

    akumulator = A_ERROR_VALUE;

    Calc_Set_A(0L);    
    TEST_CHECK(akumulator == A_ERROR_VALUE);
}

/**
  @brief Test odczytu wartości akumulatora

  Funkcja testująca odczyt wartości akumulatora.
*/
void Test_Of_Calc_Get_A(void)
{
    sint32 a = 0L;
    Std_ReturnType ret;

    akumulator = 0L;
    ret = Calc_Get_A(&a);
    TEST_CHECK(ret == E_OK);
    TEST_CHECK(a == 0L);

    akumulator = MAX32;
    ret = Calc_Get_A(&a);
    TEST_CHECK(ret == E_OK);
    TEST_CHECK(a == MAX32);

    akumulator = MIN32;
    ret = Calc_Get_A(&a);
    TEST_CHECK(ret == E_OK);
    TEST_CHECK(a == MIN32);

    akumulator = A_ERROR_VALUE;
    ret = Calc_Get_A(&a);
    TEST_CHECK(ret == E_NOT_OK);
    TEST_CHECK(a == A_ERROR_VALUE);
}

/**
  @brief Test wykonywania operacji arytmetycznych

  Funkcja testująca wykonywanie operacji arytmetycznych.
*/
void Test_of_Calc_Oper(void)
{
    // Dodawanie OK
    akumulator = 0L;
    Lib_Calc_Add_fake.custom_fake = mock_add_ok;
    Calc_Oper(CALC_ADD, MIN32);
    TEST_CHECK(akumulator == MIN32);

    // Odejmowanie OK
    akumulator = 0L;
    Lib_Calc_Sub_fake.custom_fake = mock_sub_ok;
    Calc_Oper(CALC_SUB, MAX32);
    TEST_CHECK(akumulator == MIN32+1);                  // +1 bo 0 - 2147483647 = -2147483647 a nie -2147483648 

    // Mnożenie OK
    akumulator = 2L;
    Lib_Calc_Mul_fake.custom_fake = mock_mul_ok;
    Calc_Oper(CALC_MUL, 3L);
    TEST_CHECK(akumulator == 6L);

    // Dzielenie OK
    akumulator = 2L;
    Lib_Calc_Div_fake.custom_fake = mock_div_ok;
    Calc_Oper(CALC_DIV, 2L);
    TEST_CHECK(akumulator == 1L);

    // Dzielenie przez zero (błąd)
    akumulator = 1L;
    Lib_Calc_Div_fake.custom_fake = mock_div_err;
    Calc_Oper(CALC_DIV, 0L); 
    TEST_CHECK(akumulator == A_ERROR_VALUE);

    // Gdy poprzednia operacja zakończyła się błędem, Dodawanie
    akumulator = A_ERROR_VALUE;
    Lib_Calc_Add_fake.custom_fake = mock_add_ok;
    Calc_Oper(CALC_ADD, 1L); 
    TEST_CHECK(akumulator == A_ERROR_VALUE);

    // Gdy poprzednia operacja zakończyła się błędem, Odejmowanie
    akumulator = A_ERROR_VALUE;
    Lib_Calc_Sub_fake.custom_fake = mock_sub_ok;
    Calc_Oper(CALC_SUB, 1L);
    TEST_CHECK(akumulator == A_ERROR_VALUE);

    // Gdy poprzednia operacja zakończyła się błędem, Mnożenie
    akumulator = A_ERROR_VALUE;
    Lib_Calc_Mul_fake.custom_fake = mock_mul_ok;
    Calc_Oper(CALC_MUL, 2L);
    TEST_CHECK(akumulator == A_ERROR_VALUE);

    //Gdy poprzednia operacja zakończyła się błędem, Dzielenie
    akumulator = A_ERROR_VALUE;
    Lib_Calc_Div_fake.custom_fake = mock_div_ok;
    Calc_Oper(CALC_DIV, 2L);
    TEST_CHECK(akumulator == A_ERROR_VALUE);

    // Wyłącz custom_fake po teście
    Lib_Calc_Add_fake.custom_fake = NULL;
    Lib_Calc_Sub_fake.custom_fake = NULL;
    Lib_Calc_Mul_fake.custom_fake = NULL;
    Lib_Calc_Div_fake.custom_fake = NULL;
}


/**
  @brief Test operacji na pamięci kalkulatora

  Funkcja testująca operacje na pamięci kalkulatora.
*/
void Test_of_Calc_Mem(void)
{
    Lib_Calc_MemR_fake.custom_fake = mock_Lib_Calc_MemR;
    Lib_Calc_MemS_fake.custom_fake = mock_Lib_Calc_MemS;
    Lib_Calc_Add_fake.custom_fake = mock_add_ok;
    Lib_Calc_Sub_fake.custom_fake = mock_sub_ok;

    // Dodawanie OK
    memory_value_fake = 0L;
    akumulator = MAX32;
    Calc_Mem(CALC_MEM_ADD);
    TEST_CHECK(memory_value_fake == MAX32);

    // Odejmowanie OK
    memory_value_fake = 0L;
    akumulator = MAX32;
    Calc_Mem(CALC_MEM_SUB);
    TEST_CHECK(memory_value_fake == MIN32+1);                  // +1 bo 0 - 2147483647 = -2147483647 a nie -2147483648 

    // Zapis do pamięci OK
    memory_value_fake = 0L;
    akumulator = MAX32;
    Calc_Mem(CALC_MEM_STO);
    TEST_CHECK(memory_value_fake == MAX32);

    // Wyzerowanie pamięci OK
    memory_value_fake = MAX32;
    akumulator = 0L;
    Calc_Mem(CALC_MEM_ZERO);
    TEST_CHECK(memory_value_fake == 0L);

    // Gdy poprzednia operacja zakończyła się błędem, Dodawanie
    memory_value_fake = 0L;
    akumulator = A_ERROR_VALUE;
    Calc_Mem(CALC_MEM_ADD); 
    TEST_CHECK(akumulator == A_ERROR_VALUE);
    TEST_CHECK(memory_value_fake == 0L);

    // Gdy poprzednia operacja zakończyła się błędem, Odejmowanie
    memory_value_fake = 0L;
    akumulator = A_ERROR_VALUE;
    Calc_Mem(CALC_MEM_SUB);
    TEST_CHECK(akumulator == A_ERROR_VALUE);
    TEST_CHECK(memory_value_fake == 0L);
    
    // Wyłącz custom_fake po teście
    Lib_Calc_Add_fake.custom_fake = NULL;
    Lib_Calc_Sub_fake.custom_fake = NULL;
    Lib_Calc_MemR_fake.custom_fake = NULL;
    Lib_Calc_MemS_fake.custom_fake = NULL;

}

/*
  Lista testów - wpisz tutaj wszystkie funkcje które mają być wykonane jako testy.
*/
TEST_LIST = {
    { "Test of Calc_Reset", Test_Of_Calc_Reset },         /* Format to { "nazwa testu", nazwa_funkcji } */
    { "Test of Calc_Set_A", Test_Of_Calc_Set_A },
    { "Test of Calc_Get_A", Test_Of_Calc_Get_A },
    { "Test of Calc_Oper", Test_of_Calc_Oper },
    { "Test of Calc_Mem", Test_of_Calc_Mem },
    { NULL, NULL }                                        /* To musi być na końcu */
};

