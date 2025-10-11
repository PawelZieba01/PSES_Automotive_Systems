/** ==================================================================================================================*\
  @file Calc_Ab.c

  @brief Implementacja kalkulatora
  
  Implementacja kalkukatora 4-działaniowego z pamięcią.

  @see Calc_Ab.pdf
\*====================================================================================================================*/

/*====================================================================================================================*\
    Załączenie nagłówków
\*====================================================================================================================*/
#include "Calc_Ab.h"
#include "Lib_Calc.h"

/*====================================================================================================================*\
    Makra lokalne
\*====================================================================================================================*/
#define A_ERROR_VALUE  (0xFFFFFFFF)  /**< Wartość ignorowana w akumulatorze. Realizuje [P3] */
/*====================================================================================================================*\
    Typy lokalne
\*====================================================================================================================*/

/*====================================================================================================================*\
    Zmienne globalne
\*====================================================================================================================*/

/*====================================================================================================================*\
    Zmienne lokalne (statyczne)
\*====================================================================================================================*/
static sint32 akumulator;     /**< Akumulator kalkulatora [P2] */

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
  @brief Zerowanie modułu

  Funkcja zeruje wartość akumulatora i pamięci.
  Realizuje [P9]
*/
void Calc_Reset(void)
{
    akumulator = 0L;
    Lib_Calc_MemS(0L);
}


/**
 * @brief Ustawia wartość akumulatora.
 * @param[in] value Wartość do ustawienia w akumulatorze
 * @details Realizuje [P3]
 */
void Calc_Set_A(sint32 value)
{
    if (akumulator != A_ERROR_VALUE) {
        akumulator = value;
    }
}


/**
 * @brief Odczytuje wartość z akumulatora.
 * @param[out] a Wskaźnik na zmienną do przechowania wartości akumulatora
 * @return E_OK jeśli operacja zakończona sukcesem, E_NOT_OK jeśli akumulator ma wartość błędu
 * @details Realizuje [P3]
 */
Std_ReturnType Calc_Get_A(sint32* a)
{
    Std_ReturnType ret = E_OK;
    sint32 tmp = akumulator;

    if (tmp == A_ERROR_VALUE) {
        ret = E_NOT_OK;
    }

    *a = tmp;
    return ret;
}


/**
 * @brief Wykonuje operację arytmetyczną na akumulatorze i drugim argumencie.
 * @param[in] op Typ operacji do wykonania (dodawanie, odejmowanie, mnożenie, dzielenie)
 * @param[in] arg2 Drugi argument operacji
 * @details Realizuje [P5]
 */
void Calc_Oper(Calc_Operation_T op, sint32 arg2)
{
    Std_ReturnType ret = E_OK;
    sint32 result = 0L;

    if (akumulator == A_ERROR_VALUE) {
        ret = E_NOT_OK;
    } else {
        switch (op)
        {
        case CALC_ADD:
            ret = Lib_Calc_Add(akumulator, arg2, &result);
            break;
        
        case CALC_SUB:
            ret = Lib_Calc_Sub(akumulator, arg2, &result);
            break;

        case CALC_MUL:  
            ret = Lib_Calc_Mul(akumulator, arg2, &result);
            break;

        case CALC_DIV:  
            ret = Lib_Calc_Div(akumulator, arg2, &result);
            break;

        default:
            ret = E_NOT_OK;
            break;
        }
    }   

    if (ret == E_OK) {
        akumulator = result;
    } else {
        akumulator = A_ERROR_VALUE;
    }
}


void Calc_Mem(Calc_MemOp_T op)
{
    Std_ReturnType ret = E_OK;
    sint32 result = 0L;

    if (akumulator == A_ERROR_VALUE) {
        ret = E_NOT_OK;
    } else {
        sint32 mem = Lib_Calc_MemR();
        
        switch (op)
        {
        case CALC_MEM_ADD:
            ret = Lib_Calc_Add(mem, akumulator, &result);
            break;
        
        case CALC_MEM_SUB:
            ret = Lib_Calc_Sub(mem, akumulator, &result);
            break;

        case CALC_MEM_STO:  
            result = akumulator;
            break;

        case CALC_MEM_ZERO:  
            result = 0L;
            break;

        default:
            ret = E_NOT_OK;
            break;
        }
    }

    if (ret == E_OK) {
        Lib_Calc_MemS(result);
    } else {
        akumulator = A_ERROR_VALUE;
    }
}
