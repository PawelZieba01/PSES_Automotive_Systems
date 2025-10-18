#include "acutest.h"
#include "fff.h"

#include "../application/SWC_IoHwAbs.c"
#include "Std_Types.h"

DEFINE_FFF_GLOBALS

/* Mock functions declarations */
FAKE_VOID_FUNC(Dio_WriteChannel, Dio_ChannelType, Dio_LevelType);
FAKE_VALUE_FUNC(Std_ReturnType, Rte_Read_P_MyLed, uint32*);


/* Helper macro */
#define SETUP()            \
    RESET_FAKE(Dio_WriteChannel); \
    RESET_FAKE(Rte_Read_P_MyLed); \
    FFF_RESET_HISTORY();

/* Helper variables */

static uint8 MyLed_state = 0U;
static uint8 P_MyLed_state = 0U;

/* Custom fake functions */

void Dio_WriteChannel_Custom(Dio_ChannelType ChannelId, Dio_LevelType Level)
{
    if (ChannelId == DioConf_DioChannel_MY_LED_CYCLIC) {
        MyLed_state = Level;
    }
}

Std_ReturnType Rte_Read_P_MyLed_Custom(uint32* value)
{
    if (value != NULL) {
        *value = P_MyLed_state;
        return E_OK;
    }
    return E_NOT_OK;
}


/* --------------- tests ---------------- */ 
void test_SWC_IoHwAbs_SetDiscreteValue_LED(void)
{
    SETUP();

    RESET_FAKE(Dio_WriteChannel);
    MyLed_state = 0U;

    Dio_WriteChannel_fake.custom_fake = Dio_WriteChannel_Custom;

    SWC_IoHwAbs_SetDiscreteValue(0U, 1U);
    TEST_CHECK(MyLed_state == 1U);

    /* Wyłączenie LED */
    SWC_IoHwAbs_SetDiscreteValue(0U, 0U);
    TEST_CHECK(MyLed_state == 0U);
}

void test_SWC_IoHwAbs_UpdateLED_On(void)
{
    SETUP();

    RESET_FAKE(Dio_WriteChannel);
    MyLed_state = 0U;
    P_MyLed_state = 1U; /* Ustawienie sygnału wejściowego na włączenie LED */

    Dio_WriteChannel_fake.custom_fake = Dio_WriteChannel_Custom;
    Rte_Read_P_MyLed_fake.custom_fake = Rte_Read_P_MyLed_Custom;

    SWC_IoHwAbs_UpdateLED();
    TEST_CHECK(MyLed_state == 1U); /* Sprawdzenie, czy LED został włączony */
}

void test_SWC_IoHwAbs_UpdateLED_Off(void)
{
    SETUP();

    RESET_FAKE(Dio_WriteChannel);
    MyLed_state = 1U;
    P_MyLed_state = 0U; /* Ustawienie sygnału wejściowego na wyłączenie LED */

    Dio_WriteChannel_fake.custom_fake = Dio_WriteChannel_Custom;
    Rte_Read_P_MyLed_fake.custom_fake = Rte_Read_P_MyLed_Custom;

    SWC_IoHwAbs_UpdateLED();
    TEST_CHECK(MyLed_state == 0U); /* Sprawdzenie, czy LED został wyłączony */
}


/* Test list */
TEST_LIST = {
    {"test_SWC_IoHwAbs_SetDiscreteValue_LED", test_SWC_IoHwAbs_SetDiscreteValue_LED},
    {"test_SWC_IoHwAbs_UpdateLED_On", test_SWC_IoHwAbs_UpdateLED_On},
    {"test_SWC_IoHwAbs_UpdateLED_Off", test_SWC_IoHwAbs_UpdateLED_Off},
    {NULL, NULL} /* Must end with NULL */
};