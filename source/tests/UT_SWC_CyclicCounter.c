#include "acutest.h"
#include "fff.h"

#include "../application/SWC_CyclicCounter.c"

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
    RESET_FAKE(Rte_Receive_R_SpecialHandling_EventMessage); \
    RESET_FAKE(BswM_RequestMode); \
    FFF_RESET_HISTORY()

/* Helper variables + C-style fake functions for FFF custom_fake */
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



/* --------------- tests ---------------- */ 


void test_SWC_CyclicCounter_Init(void) {
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


void test_SWC_CyclicCounter_Shutdown(void) {
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


void test_SWC_CyclicCounter_SetCounter(void) {
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


/* Test list */
TEST_LIST = {
    {"test_SWC_CyclicCounter_Init", test_SWC_CyclicCounter_Init},
    {"test_SWC_CyclicCounter_Shutdown", test_SWC_CyclicCounter_Shutdown},
    {"test_SWC_CyclicCounter_SetCounter", test_SWC_CyclicCounter_SetCounter},
    {NULL, NULL} /* Must end with NULL */
};