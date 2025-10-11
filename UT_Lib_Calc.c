/** ==================================================================================================================*\
  @file UT_Lib_Calc.c

  @brief Testy jednostkowe do Lib_Calc
\*====================================================================================================================*/

#include "acutest.h"
#include "Std_Types.h"

#include "Lib_Calc.c"   /* To nie pomyłka - taki include eksponuje zmienne statyczne dla testów */

#define MAX32   (2147483647L)
#define MIN32   (-2147483648L)

#define DIV_POS_BY_ZERO_RESULT   (0x7FFFFFFF)
#define DIV_NEG_BY_ZERO_RESULT   (0x80000000)
#define DIV_ZERO_BY_ZERO_RESULT   (0x00000000)

/**
  @brief Test dodawania

  Funkcja testująca dodawanie w bibliotece. Funkcje testowe acutest nie mogą zwracać ani przyjmować danych!
*/
void Test_Of_Lib_Calc_Add(void)
{
    sint32 result;
    Std_ReturnType retv;

    retv = Lib_Calc_Add(0L, 0L, &result);

    TEST_CHECK(result == 0);       /* Test przejdzie jeżeli wyrażenie w nawiasach jest prawdziwe */
    TEST_CHECK(retv == E_OK);

    retv = Lib_Calc_Add(1L, -1L, &result);

    TEST_CHECK(result == 0);
    TEST_CHECK(retv == E_OK);

    /* TUTAJ DOPISZ WIĘCEJ TESTÓW */
}


/**
  @brief Test odejmowania

  Funkcja testująca odejmowanie w bibliotece.
*/
void Test_Of_Lib_Calc_Sub(void)
{
    sint32 result;
    Std_ReturnType retv;

    retv = Lib_Calc_Sub(0L, 0L, &result);

    TEST_CHECK(result == 0);
    TEST_CHECK(retv == E_OK);

    retv = Lib_Calc_Sub(1L, -1L, &result);

    TEST_CHECK(result == 2L);
    TEST_CHECK(retv == E_OK);

    /* TUTAJ DOPISZ WIĘCEJ TESTÓW */
}


/**
  @brief Test mnożenia

  Funkcja testująca mnożenie w bibliotece.
*/
void Test_Of_Lib_Calc_Mul(void)
{
    sint32 result;
    Std_ReturnType retv;

    retv = Lib_Calc_Mul(0L, 0L, &result);

    TEST_CHECK(result == 0);
    TEST_CHECK(retv == E_OK);

    retv = Lib_Calc_Mul(1L, -1L, &result);

    TEST_CHECK(result == -1L);
    TEST_CHECK(retv == E_OK);

    /* TUTAJ DOPISZ WIĘCEJ TESTÓW */
}


/**
  @brief Test dzielenia

  Funkcja testująca dzielenie w bibliotece.
*/
void Test_Of_Lib_Calc_Div(void)
{
    sint32 result;
    Std_ReturnType retv;

    retv = Lib_Calc_Div(0L, 0L, &result);

    TEST_CHECK(result == DIV_ZERO_BY_ZERO_RESULT);
    TEST_CHECK(retv == E_NOT_OK);

    retv = Lib_Calc_Div(1L, 0L, &result);

    TEST_CHECK(result == DIV_POS_BY_ZERO_RESULT);  
    TEST_CHECK(retv == E_NOT_OK);

    retv = Lib_Calc_Div(-1L, 0L, &result);

    TEST_CHECK(result == DIV_NEG_BY_ZERO_RESULT);  
    TEST_CHECK(retv == E_NOT_OK);

    retv = Lib_Calc_Div(1L, -1L, &result);

    TEST_CHECK(result == -1L);
    TEST_CHECK(retv == E_OK);
}


/**
  @brief Test zapisu i odczytu z pamięci

  Funkcja testująca zapisywanie i odczytywanie pamięci w bibliotece.
*/
void Test_Of_Lib_Calc_MemS_MemR(void)
{
    sint32 retv;

    Lib_Calc_MemS(0L);
    retv = Lib_Calc_MemR();

    TEST_CHECK(retv == 0L);

    Lib_Calc_MemS(MAX32);
    retv = Lib_Calc_MemR();

    TEST_CHECK(retv == MAX32);

    Lib_Calc_MemS(MIN32);
    retv = Lib_Calc_MemR();

    TEST_CHECK(retv == MIN32);
}




/*
  Lista testów - wpisz tutaj wszystkie funkcje które mają być wykonane jako testy.
*/
TEST_LIST = {
    { "Test of Lib_Calc_Add", Test_Of_Lib_Calc_Add },   /* Format to { "nazwa testu", nazwa_funkcji } */
    { "Test of Lib_Calc_Sub", Test_Of_Lib_Calc_Sub },
    { "Test of Lib_Calc_Mul", Test_Of_Lib_Calc_Mul },
    { "Test of Lib_Calc_Div", Test_Of_Lib_Calc_Div },
    { "Test of Lib_Calc_MemS_MemR", Test_Of_Lib_Calc_MemS_MemR },
    { NULL, NULL }                                      /* To musi być na końcu */
};

