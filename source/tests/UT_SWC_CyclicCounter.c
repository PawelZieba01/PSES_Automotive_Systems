#include "acutest.h"
#include "fff.h"

#include "../application/SWC_CyclicCounter.c"
#include "Std_Types.h"

DEFINE_FFF_GLOBALS

/* Mock functions declarations */
FAKE_VALUE_FUNC(uint8*, Rte_Pim_PimCounterValue);
FAKE_VALUE_FUNC(uint8, Rte_IrvRead_Cyclic_CurrentCounterValue);
FAKE_VOID_FUNC(Rte_IrvWrite_Cyclic_CurrentCounterValue, uint8);
FAKE_VALUE_FUNC(uint8, Rte_IrvRead_SetCounter_CurrentCounterValue);
FAKE_VOID_FUNC(Rte_IrvWrite_SetCounter_CurrentCounterValue, uint8);

FAKE_VALUE_FUNC(Std_ReturnType, Rte_Call_UR_ComMUser_0_RequestComMode, uint8);
FAKE_VALUE_FUNC(Std_ReturnType, Rte_Call_PS_PersistentCounterValue_SetRamBlockStatus, boolean);
FAKE_VALUE_FUNC(Std_ReturnType, Rte_Call_R_CyclicCounterDet_ReportError, uint8, uint8, uint8);
FAKE_VALUE_FUNC(Std_ReturnType, Rte_Call_R_MyLED_SetDiscreteValue, uint8);
FAKE_VALUE_FUNC(Std_ReturnType, Rte_Write_P_CounterOut_CounterValue, uint8);
FAKE_VALUE_FUNC(Std_ReturnType, Rte_Write_P_EchoCalc_CounterValue, uint8);
FAKE_VALUE_FUNC(Std_ReturnType, Rte_Receive_R_SpecialHandling_EventMessage, uint8*);

FAKE_VOID_FUNC(BswM_RequestMode, uint8, uint8);


/* Helper macro */
#define SETUP() \
    RESET_FAKE(Rte_Pim_PimCounterValue); \
    RESET_FAKE(Rte_IrvRead_Cyclic_CurrentCounterValue); \
    RESET_FAKE(Rte_IrvWrite_Cyclic_CurrentCounterValue); \
    RESET_FAKE(Rte_IrvRead_SetCounter_CurrentCounterValue); \
    RESET_FAKE(Rte_IrvWrite_SetCounter_CurrentCounterValue); \
    RESET_FAKE(Rte_Call_UR_ComMUser_0_RequestComMode); \
    RESET_FAKE(Rte_Call_PS_PersistentCounterValue_SetRamBlockStatus); \
    RESET_FAKE(Rte_Call_R_CyclicCounterDet_ReportError); \
    RESET_FAKE(Rte_Call_R_MyLED_SetDiscreteValue); \
    RESET_FAKE(Rte_Write_P_CounterOut_CounterValue); \
    RESET_FAKE(Rte_Write_P_EchoCalc_CounterValue); \
    RESET_FAKE(Rte_Receive_R_SpecialHandling_EventMessage); \
    RESET_FAKE(BswM_RequestMode); \
    FFF_RESET_HISTORY()

/* Helper variables */
static uint8 mock_PimCounterValue = 0U;
static uint8 mock_Irv_Cyclic_CurrentCounterValue = 0U;

uint8* Rte_Pim_PimCounterValue_Custom(void) 
{
    return &mock_PimCounterValue;
}
    
void Rte_IrvWrite_Cyclic_CurrentCounterValue_Custom(uint8 value)
{
    mock_Irv_Cyclic_CurrentCounterValue = value;
}

uint8 Rte_IrvRead_Cyclic_CurrentCounterValue_Custom(void)
{
    return mock_Irv_Cyclic_CurrentCounterValue;
}

Std_ReturnType Rte_Call_PS_PersistentCounterValue_SetRamBlockStatus_Custom(boolean status)
{
    (void)status;
    return E_OK;
}

uint8 Rte_IrvRead_SetCounter_CurrentCounterValue_Custom(void)
{
    return mock_Irv_Cyclic_CurrentCounterValue;
}

void Rte_IrvWrite_SetCounter_CurrentCounterValue_Custom(uint8 value)
{
    mock_Irv_Cyclic_CurrentCounterValue = value;
}


#define MAX_COMMANDS 16
static uint8 command_buffer[MAX_COMMANDS] = {0};
static uint8 command_index = 0;

Std_ReturnType Rte_Receive_R_SpecialHandling_EventMessage_Custom(uint8* out)
{
    if (command_index >= MAX_COMMANDS || command_buffer[command_index] == 0x00) {
        *out = 0x00;
        return E_NOT_OK;
    }

    *out = command_buffer[command_index++];
    return E_OK;
}


/* Helper do ustawienia jednej komendy w buforze */
void set_single_command(uint8 cmd)
{
    for (int i = 0; i < MAX_COMMANDS; i++) command_buffer[i] = 0x00;
    command_buffer[0] = cmd;
    command_index = 0;
}

/* Helper do ustawienia kilku komend w buforze (kopiowanie tablicy) */
void set_commands(uint8* cmds, uint8 count)
{
    for (int i = 0; i < MAX_COMMANDS; i++) command_buffer[i] = 0x00;
    for (int i = 0; i < count && i < MAX_COMMANDS; i++)
        command_buffer[i] = cmds[i];
    command_index = 0;
}


/* --------------- tests ---------------- */ 


void test_SWC_CyclicCounter_Init(void) 
{
    SETUP();
    mock_PimCounterValue = 42U;
    Rte_Pim_PimCounterValue_fake.custom_fake = Rte_Pim_PimCounterValue_Custom;
    Rte_IrvWrite_Cyclic_CurrentCounterValue_fake.custom_fake = Rte_IrvWrite_Cyclic_CurrentCounterValue_Custom;

    SWC_CyclicCounter_Init();

    TEST_CHECK(Rte_IrvWrite_Cyclic_CurrentCounterValue_fake.call_count == 1);
    TEST_CHECK(Rte_IrvWrite_Cyclic_CurrentCounterValue_fake.arg0_val == 42U);
    TEST_CHECK(Rte_Call_UR_ComMUser_0_RequestComMode_fake.call_count == 1);
    TEST_CHECK(mock_Irv_Cyclic_CurrentCounterValue == 42U);
    TEST_CHECK(Rte_Call_UR_ComMUser_0_RequestComMode_fake.arg0_val == FULL_COMMUNICATION);
    
    
#ifdef BASE_COMM_ENABLED
    TEST_CHECK(BswM_RequestMode_fake.call_count == 1);
    TEST_CHECK(BswM_RequestMode_fake.arg0_val == EB_INTGR_BSWM_SWC_CYCLIC);
    TEST_CHECK(BswM_RequestMode_fake.arg1_val == EB_INTGR_BSWM_SWC_CYCLIC_RUN);
#endif
}


void test_SWC_CyclicCounter_Shutdown(void) 
{
    SETUP();
    mock_Irv_Cyclic_CurrentCounterValue = 84U;
    Rte_IrvRead_Cyclic_CurrentCounterValue_fake.custom_fake = Rte_IrvRead_Cyclic_CurrentCounterValue_Custom;
    mock_PimCounterValue = 0U;
    Rte_Pim_PimCounterValue_fake.custom_fake = Rte_Pim_PimCounterValue_Custom;

    SWC_CyclicCounter_Shutdown();

    TEST_CHECK(Rte_IrvRead_Cyclic_CurrentCounterValue_fake.call_count == 1);
    TEST_CHECK(Rte_Pim_PimCounterValue_fake.call_count == 1);
    TEST_CHECK(mock_PimCounterValue == 84U);
    TEST_CHECK(Rte_Call_PS_PersistentCounterValue_SetRamBlockStatus_fake.call_count == 1);
    TEST_CHECK(Rte_Call_PS_PersistentCounterValue_SetRamBlockStatus_fake.arg0_val == TRUE);
}


void test_SWC_CyclicCounter_SetCounter(void) 
{
    /* Reset FFF call counters */
    RESET_FAKE(Rte_IrvRead_SetCounter_CurrentCounterValue);
    RESET_FAKE(Rte_IrvWrite_SetCounter_CurrentCounterValue);

    /* Init mock data */
    uint8 returnSum = 0U;
    mock_PimCounterValue = 0U;
    mock_Irv_Cyclic_CurrentCounterValue = 0U;

    /* Assign custom fakes */
    Rte_IrvRead_SetCounter_CurrentCounterValue_fake.custom_fake = Rte_IrvRead_SetCounter_CurrentCounterValue_Custom;
    Rte_IrvWrite_SetCounter_CurrentCounterValue_fake.custom_fake = Rte_IrvWrite_SetCounter_CurrentCounterValue_Custom;

    /* Call the function under test */
    SWC_CyclicCounter_SetCounter(2U, &returnSum);

    /* Checks */
    TEST_CHECK(Rte_IrvRead_SetCounter_CurrentCounterValue_fake.call_count == 1);
    TEST_MSG("Rte_IrvRead_SetCounter_CurrentCounterValue_fake.call_count: %d", Rte_IrvRead_SetCounter_CurrentCounterValue_fake.call_count);

    TEST_CHECK(Rte_IrvWrite_SetCounter_CurrentCounterValue_fake.call_count == 1);
    TEST_CHECK(mock_Irv_Cyclic_CurrentCounterValue == 2U);
    TEST_CHECK(mock_PimCounterValue == 0U);
    TEST_CHECK(returnSum == 2U);
}


void test_single_commands(void)
{
    SETUP();
    Rte_IrvWrite_Cyclic_CurrentCounterValue_fake.custom_fake = Rte_IrvWrite_Cyclic_CurrentCounterValue_Custom;
    Rte_IrvRead_Cyclic_CurrentCounterValue_fake.custom_fake = Rte_IrvRead_Cyclic_CurrentCounterValue_Custom;
    Rte_Receive_R_SpecialHandling_EventMessage_fake.custom_fake = Rte_Receive_R_SpecialHandling_EventMessage_Custom;
    Rte_Call_R_MyLED_SetDiscreteValue_fake.return_val = E_OK;
    Rte_Write_P_CounterOut_CounterValue_fake.return_val = E_OK;

    mock_Irv_Cyclic_CurrentCounterValue = 0;

    /* Testujemy komendę 0x01 - LED ON, CounterOut = 0xF1 */
    set_single_command(0x01);
    SWC_CyclicCounter_Cyclic();
    TEST_CHECK(Rte_Call_R_MyLED_SetDiscreteValue_fake.arg0_val == 1U);
    TEST_CHECK(Rte_Write_P_CounterOut_CounterValue_fake.arg0_val == 0xF1U);

    /* Testujemy komendę 0x02 - LED OFF, CounterOut = 0xF2 */
    set_single_command(0x02);
    SWC_CyclicCounter_Cyclic();
    TEST_CHECK(Rte_Call_R_MyLED_SetDiscreteValue_fake.arg0_val == 0U);
    TEST_CHECK(Rte_Write_P_CounterOut_CounterValue_fake.arg0_val == 0xF2U);

    /* Testujemy komendę 0x03 - LED ON, CounterOut = 0x01 */
    set_single_command(0x03);
    SWC_CyclicCounter_Cyclic();
    TEST_CHECK(Rte_Call_R_MyLED_SetDiscreteValue_fake.arg0_val == 1U);
    TEST_CHECK(Rte_Write_P_CounterOut_CounterValue_fake.arg0_val == 0x01U);

    /* Testujemy komendę 0x04 - LED OFF, CounterOut = 0x02 */
    set_single_command(0x04);
    SWC_CyclicCounter_Cyclic();
    TEST_CHECK(Rte_Call_R_MyLED_SetDiscreteValue_fake.arg0_val == 0U);
    TEST_CHECK(Rte_Write_P_CounterOut_CounterValue_fake.arg0_val == 0x02U);

    /* Testujemy komendę 0x05 - LED ON, CounterOut = 0x03 */
    set_single_command(0x05);
    SWC_CyclicCounter_Cyclic();
    TEST_CHECK(Rte_Call_R_MyLED_SetDiscreteValue_fake.arg0_val == 1U);

    /* Testujemy komendę 0x06 - LED OFF, CounterOut = 0x04 */
    set_single_command(0x06);
    SWC_CyclicCounter_Cyclic();
    TEST_CHECK(Rte_Call_R_MyLED_SetDiscreteValue_fake.arg0_val == 0U);

    /* Testujemy komendę 0x07 - inkrementacja licznika */
    set_single_command(0x07);
    mock_Irv_Cyclic_CurrentCounterValue = 0U;
    SWC_CyclicCounter_Cyclic();
    TEST_CHECK(mock_Irv_Cyclic_CurrentCounterValue == 1U);

    /* Testujemy komendę 0x08 - dekrementacja licznika */
    set_single_command(0x08);
    mock_Irv_Cyclic_CurrentCounterValue = 1U;
    SWC_CyclicCounter_Cyclic();
    TEST_CHECK(mock_Irv_Cyclic_CurrentCounterValue == 0U);

    /* Testujemy komendę 0x0F - reset licznika */
    mock_Irv_Cyclic_CurrentCounterValue = 5U;
    set_single_command(0x0F);
    SWC_CyclicCounter_Cyclic();
    TEST_CHECK(mock_Irv_Cyclic_CurrentCounterValue == 0U);

    /* Testujemy komendę 0x07 - inkrementacja licznika, skrajna wartość */
    set_single_command(0x07);
    mock_Irv_Cyclic_CurrentCounterValue = 0x0FU;
    SWC_CyclicCounter_Cyclic();
    TEST_CHECK(mock_Irv_Cyclic_CurrentCounterValue == 0x00U);

    /* Testujemy komendę 0x08 - dekrementacja licznika, skrajna wartość */
    set_single_command(0x08);
    mock_Irv_Cyclic_CurrentCounterValue = 0x00U;
    SWC_CyclicCounter_Cyclic();
    TEST_CHECK(mock_Irv_Cyclic_CurrentCounterValue == 0x0FU);
}


void test_two_identical_commands(void)
{
    SETUP();
    Rte_IrvWrite_Cyclic_CurrentCounterValue_fake.custom_fake = Rte_IrvWrite_Cyclic_CurrentCounterValue_Custom;
    Rte_IrvRead_Cyclic_CurrentCounterValue_fake.custom_fake = Rte_IrvRead_Cyclic_CurrentCounterValue_Custom;
    Rte_Receive_R_SpecialHandling_EventMessage_fake.custom_fake = Rte_Receive_R_SpecialHandling_EventMessage_Custom;
    Rte_Call_R_MyLED_SetDiscreteValue_fake.return_val = E_OK;
    Rte_Write_P_CounterOut_CounterValue_fake.return_val = E_OK;

    mock_Irv_Cyclic_CurrentCounterValue = 0;

    uint8 cmds[2] = {0x07, 0x07}; // inkrementacja licznika
    set_commands(cmds, 2);

    /* Pierwsza komenda powinna zadziałać */
    mock_Irv_Cyclic_CurrentCounterValue = 0U;
    SWC_CyclicCounter_Cyclic();
    TEST_CHECK(mock_Irv_Cyclic_CurrentCounterValue == 1U);

    /* Druga taka sama - powinna być zignorowana, licznik nie zmieni się */
    SWC_CyclicCounter_Cyclic();
    TEST_CHECK(mock_Irv_Cyclic_CurrentCounterValue == 1U);
}


void test_buffer_overflow(void)
{
    SETUP();
    Rte_IrvWrite_Cyclic_CurrentCounterValue_fake.custom_fake = Rte_IrvWrite_Cyclic_CurrentCounterValue_Custom;
    Rte_IrvRead_Cyclic_CurrentCounterValue_fake.custom_fake = Rte_IrvRead_Cyclic_CurrentCounterValue_Custom;
    Rte_Receive_R_SpecialHandling_EventMessage_fake.custom_fake = Rte_Receive_R_SpecialHandling_EventMessage_Custom;
    Rte_Call_R_MyLED_SetDiscreteValue_fake.return_val = E_OK;
    Rte_Write_P_CounterOut_CounterValue_fake.return_val = E_OK;

    mock_Irv_Cyclic_CurrentCounterValue = 0;

    uint8 cmds[12] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0F, 0x0A,0x0B}; // 12 komend, przekracza bufor
    set_commands(cmds, 12);

    for (int i = 0; i < 10; i++) {
        SWC_CyclicCounter_Cyclic();
    }

    /* Sprawdzenie wartości licznika po operacjach inkrementacji/dekrementacji/reset 
    Licznik powinien byćrówny 0 po resecie */
    TEST_CHECK(mock_Irv_Cyclic_CurrentCounterValue == 0U);
}



/* Test list */
TEST_LIST = {
    {"test_SWC_CyclicCounter_Init", test_SWC_CyclicCounter_Init},
    {"test_SWC_CyclicCounter_Shutdown", test_SWC_CyclicCounter_Shutdown},
    {"test_SWC_CyclicCounter_SetCounter", test_SWC_CyclicCounter_SetCounter},
    {"test_single_commands", test_single_commands},
    {"test_two_identical_commands", test_two_identical_commands},
    {"test_buffer_overflow", test_buffer_overflow},
    {NULL, NULL} /* Must end with NULL */
};