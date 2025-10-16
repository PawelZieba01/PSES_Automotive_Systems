#include "acutest.h"
#include "fff.h"

#include "../application/SWC_ModifyEcho.c"

DEFINE_FFF_GLOBALS

/* Mock functions declarations */
FAKE_VALUE_FUNC(Std_ReturnType, Rte_Read_R_NewCounterIn_CounterValue, uint8*);
FAKE_VALUE_FUNC(Std_ReturnType, Rte_Read_R_EchoResult_CounterValue, uint8*);
FAKE_VALUE_FUNC(Std_ReturnType, Rte_Send_P_SpecialRequest_EventMessage, uint8);
FAKE_VOID_FUNC(Rte_IWrite_ModifyEcho_P_EchoOut_CounterValue, uint8);
FAKE_VALUE_FUNC(Std_ReturnType, Rte_Call_R_EchoResult_SetCounterAndAdd, uint8, uint8*);

/* Helper macro */
#define SETUP() \
    RESET_FAKE(Rte_Read_R_NewCounterIn_CounterValue); \
    RESET_FAKE(Rte_Read_R_EchoResult_CounterValue); \
    RESET_FAKE(Rte_Send_P_SpecialRequest_EventMessage); \
    RESET_FAKE(Rte_IWrite_ModifyEcho_P_EchoOut_CounterValue); \
    RESET_FAKE(Rte_Call_R_EchoResult_SetCounterAndAdd); \
    FFF_RESET_HISTORY()

/* Helper variables + C-style fake functions for FFF custom_fake */
static uint8 g_Rte_Read_NewCounterIn_value = 0U;
static uint8 g_Rte_Read_EchoResult_value = 0U;

static Std_ReturnType Rte_Read_R_NewCounterIn_Custom(uint8* data) {
    if (data != NULL) {
        *data = g_Rte_Read_NewCounterIn_value;
    }
    return (Std_ReturnType)0U;
}

static Std_ReturnType Rte_Read_R_EchoResult_Custom(uint8* out) {
    if (out != NULL) {
        *out = g_Rte_Read_EchoResult_value;
    }
    return (Std_ReturnType)0U;
}

/* Special request: input 0xF1..0xFF -> send event with lower 4 bits and write same masked value to EchoOut */
void test_SWC_ModifyEcho_SpecialRequest(void) {
    SETUP();

    g_Rte_Read_NewCounterIn_value = 0xF5U;
    Rte_Read_R_NewCounterIn_CounterValue_fake.custom_fake = Rte_Read_R_NewCounterIn_Custom;

    SWC_ModifyEcho_ModifyEcho();

    TEST_CHECK(Rte_Send_P_SpecialRequest_EventMessage_fake.call_count == 1);
    TEST_CHECK(Rte_Send_P_SpecialRequest_EventMessage_fake.arg0_val == (uint8)(0xF5U & 0x0FU));
    TEST_CHECK(Rte_IWrite_ModifyEcho_P_EchoOut_CounterValue_fake.call_count == 1);
    TEST_CHECK(Rte_IWrite_ModifyEcho_P_EchoOut_CounterValue_fake.arg0_val == (uint8)(0xF5U & 0x0FU));
}

/* Decrement case: input 0x01..0xF0 -> write input-1 to EchoOut */
void test_SWC_ModifyEcho_Decrement(void) {
    SETUP();

    g_Rte_Read_NewCounterIn_value = 0x10U;
    Rte_Read_R_NewCounterIn_CounterValue_fake.custom_fake = Rte_Read_R_NewCounterIn_Custom;

    SWC_ModifyEcho_ModifyEcho();

    TEST_CHECK(Rte_IWrite_ModifyEcho_P_EchoOut_CounterValue_fake.call_count == 1);
    TEST_CHECK(Rte_IWrite_ModifyEcho_P_EchoOut_CounterValue_fake.arg0_val == (uint8)(0x10U - 1U));
}

/* EchoResult read case: input 0x00 -> read from R_EchoResult and forward to EchoOut */
void test_SWC_ModifyEcho_EchoResultRead(void) {
    SETUP();

    g_Rte_Read_NewCounterIn_value = 0x00U;
    Rte_Read_R_NewCounterIn_CounterValue_fake.custom_fake = Rte_Read_R_NewCounterIn_Custom;

    g_Rte_Read_EchoResult_value = 0xABU;
    Rte_Read_R_EchoResult_CounterValue_fake.custom_fake = Rte_Read_R_EchoResult_Custom;

    SWC_ModifyEcho_ModifyEcho();

    TEST_CHECK(Rte_Read_R_EchoResult_CounterValue_fake.call_count == 1);
    TEST_CHECK(Rte_IWrite_ModifyEcho_P_EchoOut_CounterValue_fake.call_count == 1);
    TEST_CHECK(Rte_IWrite_ModifyEcho_P_EchoOut_CounterValue_fake.arg0_val == 0xABU);
}

/* Edge: SpecialRequest lower bound 0xF1 -> expect send with 0x01 and IWrite 0x01 */
void test_SWC_ModifyEcho_SpecialRequest_Min(void) {
    SETUP();
    g_Rte_Read_NewCounterIn_value = 0xF1U;
    Rte_Read_R_NewCounterIn_CounterValue_fake.custom_fake = Rte_Read_R_NewCounterIn_Custom;

    SWC_ModifyEcho_ModifyEcho();

    TEST_CHECK(Rte_Send_P_SpecialRequest_EventMessage_fake.call_count == 1);
    TEST_CHECK(Rte_Send_P_SpecialRequest_EventMessage_fake.arg0_val == (uint8)(0xF1U & 0x0FU));
    TEST_CHECK(Rte_IWrite_ModifyEcho_P_EchoOut_CounterValue_fake.call_count == 1);
    TEST_CHECK(Rte_IWrite_ModifyEcho_P_EchoOut_CounterValue_fake.arg0_val == (uint8)(0xF1U & 0x0FU));
}

/* Edge: SpecialRequest upper bound 0xFF -> expect send with 0x0F and IWrite 0x0F */
void test_SWC_ModifyEcho_SpecialRequest_Max(void) {
    SETUP();
    g_Rte_Read_NewCounterIn_value = 0xFFU;
    Rte_Read_R_NewCounterIn_CounterValue_fake.custom_fake = Rte_Read_R_NewCounterIn_Custom;

    SWC_ModifyEcho_ModifyEcho();

    TEST_CHECK(Rte_Send_P_SpecialRequest_EventMessage_fake.call_count == 1);
    TEST_CHECK(Rte_Send_P_SpecialRequest_EventMessage_fake.arg0_val == (uint8)(0xFFU & 0x0FU));
    TEST_CHECK(Rte_IWrite_ModifyEcho_P_EchoOut_CounterValue_fake.call_count == 1);
    TEST_CHECK(Rte_IWrite_ModifyEcho_P_EchoOut_CounterValue_fake.arg0_val == (uint8)(0xFFU & 0x0FU));
}

/* Edge: Decrement lower bound 0x01 -> expect EchoOut = 0x00 */
void test_SWC_ModifyEcho_Decrement_Min(void) {
    SETUP();
    g_Rte_Read_NewCounterIn_value = 0x01U;
    Rte_Read_R_NewCounterIn_CounterValue_fake.custom_fake = Rte_Read_R_NewCounterIn_Custom;

    SWC_ModifyEcho_ModifyEcho();

    TEST_CHECK(Rte_IWrite_ModifyEcho_P_EchoOut_CounterValue_fake.call_count == 1);
    TEST_CHECK(Rte_IWrite_ModifyEcho_P_EchoOut_CounterValue_fake.arg0_val == (uint8)(0x01U - 1U));
}

/* Edge: Decrement upper bound 0xF0 -> expect EchoOut = 0xEF */
void test_SWC_ModifyEcho_Decrement_Max(void) {
    SETUP();
    g_Rte_Read_NewCounterIn_value = 0xF0U;
    Rte_Read_R_NewCounterIn_CounterValue_fake.custom_fake = Rte_Read_R_NewCounterIn_Custom;

    SWC_ModifyEcho_ModifyEcho();

    TEST_CHECK(Rte_IWrite_ModifyEcho_P_EchoOut_CounterValue_fake.call_count == 1);
    TEST_CHECK(Rte_IWrite_ModifyEcho_P_EchoOut_CounterValue_fake.arg0_val == (uint8)(0xF0U - 1U));
}

/* Test list */
TEST_LIST = {
    {"Special Request Test", test_SWC_ModifyEcho_SpecialRequest},
    {"Special Request Min", test_SWC_ModifyEcho_SpecialRequest_Min},
    {"Special Request Max", test_SWC_ModifyEcho_SpecialRequest_Max},
    {"Decrement Test", test_SWC_ModifyEcho_Decrement},
    {"Decrement Min", test_SWC_ModifyEcho_Decrement_Min},
    {"Decrement Max", test_SWC_ModifyEcho_Decrement_Max},
    {"EchoResult Read Test", test_SWC_ModifyEcho_EchoResultRead},
    {NULL, NULL} /* Must end with NULL */
};
