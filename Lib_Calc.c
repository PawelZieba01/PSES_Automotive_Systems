/**
 * @file Lib_Calc.c
 * @brief Implementacja funkcjonalności kalkulatora 4-działaniowego z pamięcią.
 * @author pzieba
 * @date 2025-10-09
 * @see Lib_Calc.pdf
 */

/*====================================================================================================================*\
    Załączenie nagłówków
\*====================================================================================================================*/
#include "Lib_Calc.h"
#include "Std_Types.h"
#include "Platform_Types.h"

/*====================================================================================================================*\
    Makra lokalne
\*====================================================================================================================*/

/**
 * @def MAX32
 * Maksymalna wartość dla typu 32-bitowego.
 */
#define MAX32   (2147483647L)

/**
 * @def MIN32
 * Minimalna wartość dla typu 32-bitowego.
 */
#define MIN32   (-2147483648L)

/**
 * @def DIV_POS_BY_ZERO_RESULT
 * Wynik dzielenia przez zero dla liczb dodatnich.
 * Realizuje [P6]
 */
#define DIV_POS_BY_ZERO_RESULT   (0x7FFFFFFF)

/**
 * @def DIV_NEG_BY_ZERO_RESULT
 * Wynik dzielenia przez zero dla liczb ujemnych.
 * Realizuje [P7]
 */
#define DIV_NEG_BY_ZERO_RESULT   (0x80000000)

/**
 * @def DIV_ZERO_BY_ZERO_RESULT
 * Wynik dzielenia przez zero dla zera.
 * Realizuje [P8]
 */
#define DIV_ZERO_BY_ZERO_RESULT  (0x00000000)

/*====================================================================================================================*\
    Typy lokalne
\*====================================================================================================================*/

/*====================================================================================================================*\
    Zmienne globalne
\*====================================================================================================================*/

/*====================================================================================================================*\
    Zmienne lokalne (statyczne)
\*====================================================================================================================*/
static sint32 memory_value;     /**< Reprezentacja pamięci kalkulatora. Realizuje [P11] */

/*====================================================================================================================*\
    Deklaracje funkcji lokalnych
\*====================================================================================================================*/

/*====================================================================================================================*\
    Kod globalnych funkcji inline i makr funkcyjnych
\*====================================================================================================================*/

/*====================================================================================================================*\
    Kod funkcji
\*====================================================================================================================*/


/**
 * @brief Dodawanie dwóch liczb 32-bitowych z kontrolą przepełnienia.
 * @param[in] arg1 Pierwszy argument
 * @param[in] arg2 Drugi argument
 * @param[out] result Wskaźnik na wynik
 * @return E_OK jeśli operacja zakończona sukcesem, E_NOT_OK w przypadku przepełnienia
 * @details Realizuje [P2]
 */
Std_ReturnType Lib_Calc_Add(sint32 arg1, sint32 arg2, sint32* result)
{
    sint64 tmp = arg1 + arg2;
    Std_ReturnType ret = E_OK;
    if ((tmp > MAX32) || (tmp < MIN32)) {
        ret = E_NOT_OK;
    }
    *result = (sint32)tmp;
    return ret;
}

/**
 * @brief Odejmowanie dwóch liczb 32-bitowych z kontrolą przepełnienia.
 * @param[in] arg1 Pierwszy argument
 * @param[in] arg2 Drugi argument
 * @param[out] result Wskaźnik na wynik
 * @return E_OK jeśli operacja zakończona sukcesem, E_NOT_OK w przypadku przepełnienia
 * @details Realizuje [P3]
 */
Std_ReturnType Lib_Calc_Sub(sint32 arg1, sint32 arg2, sint32* result)
{
    sint64 tmp = arg1 - arg2;
    Std_ReturnType ret = E_OK;
    if ((tmp > MAX32) || (tmp < MIN32)) {
        ret = E_NOT_OK;
    }
    *result = (sint32)tmp;
    return ret;
}

/**
 * @brief Mnożenie dwóch liczb 32-bitowych z kontrolą przepełnienia.
 * @param[in] arg1 Pierwszy argument
 * @param[in] arg2 Drugi argument
 * @param[out] result Wskaźnik na wynik
 * @return E_OK jeśli operacja zakończona sukcesem, E_NOT_OK w przypadku przepełnienia
 * @details Realizuje [P4]
 */
Std_ReturnType Lib_Calc_Mul(sint32 arg1, sint32 arg2, sint32* result)
{
    sint64 tmp = arg1 * arg2;
    Std_ReturnType ret = E_OK;
    if ((tmp > MAX32) || (tmp < MIN32)) {
        ret = E_NOT_OK;
    }
    *result = (sint32)tmp;
    return ret;
}

/**
 * @brief Dzielenie dwóch liczb 32-bitowych z kontrolą przepełnienia i obsługą dzielenia przez zero.
 * @param[in] arg1 Licznik
 * @param[in] arg2 Mianownik
 * @param[out] result Wskaźnik na wynik
 * @return E_OK jeśli operacja zakończona sukcesem, E_NOT_OK w przypadku przepełnienia lub dzielenia przez zero
 * @details Realizuje [P5]
 */
Std_ReturnType Lib_Calc_Div(sint32 arg1, sint32 arg2, sint32* result)
{
    Std_ReturnType ret = E_OK;
    sint64 tmp = 0;

    if (arg2 == 0L) {
        if (arg1 > 0L) {
            tmp = DIV_POS_BY_ZERO_RESULT;       /**< Wynik dzielenia przez zero dla liczb dodatnich [P6] */
        } else if (arg1 < 0L) {
             tmp = DIV_NEG_BY_ZERO_RESULT;      /**< Wynik dzielenia przez zero dla liczb ujemnych [P7] */
        } else {
            tmp = DIV_ZERO_BY_ZERO_RESULT;      /**< Wynik dzielenia przez zero dla zera [P8] */
        }
        ret = E_NOT_OK;

    } else {
        tmp = arg1 / arg2;
        if ((tmp > MAX32) || (tmp < MIN32)) {
            ret = E_NOT_OK;
        }
   }
    *result = (sint32)tmp;
    return ret;
}

/**
 * @brief Zapisuje wartość do pamięci kalkulatora.
 * @param[in] mem Wartość do zapisania
 * @details Realizuje [P9]
 */
void Lib_Calc_MemS(sint32 mem)
{
    memory_value = mem;
}

/**
 * @brief Odczytuje wartość z pamięci kalkulatora.
 * @return Zawartość pamięci
 * @details Realizuje [P10]
 */
sint32 Lib_Calc_MemR(void)
{
    return memory_value;
}


