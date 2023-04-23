/* 
 * File:   AgentTest.c
 * Author: barry
 *
 * Created on June 4, 2021, 11:19 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include "Agent.h"
#include "BattleBoats.h"

/*
 * 
 */

static BB_Event testing;
int pass_1;
int pass_2;
int pass_3;

int main(int argc, char** argv) {
    // Testing for Agent
    printf("This is the test for Agent.c\n");
    printf("\n");

    // Testing for AgentInit
    printf("    Test for AgentInit:\n");
    AgentInit();
    if (AgentGetState() == AGENT_STATE_START) {
        printf("    AgentInit test: SUCCESS!\n");
    } else {
        printf("    AgentInit test: SUCCESS!\n");
    }
    printf("\n");
    /*******************************************************/

    // Testing for AgentRun
    printf("    Test for AgentRun:\n");
    testing.type = BB_EVENT_RESET_BUTTON;
    AgentRun(testing);
    if (AgentGetState() == AGENT_STATE_START) {
        pass_1++;
    }
    if (pass_1 == 1) {
        printf("    AgentRun test: SUCCESS!\n");
    } else {
        printf("    AgentRun test: FAILED!\n");
    }
    printf("\n");
    /*******************************************************/

    // Testing for AgentGetState
    printf("    Test for AgentGetState:\n");
    AgentSetState(AGENT_STATE_CHALLENGING);
    if (AgentGetState() == AGENT_STATE_CHALLENGING) {
        pass_2++;
    }
    if (pass_2 == 1) {
        printf("    AgentGetState: SUCCESS!\n");
    } else {
        printf("    AgentGetState: FAILED!\n");
    }
    printf("\n");
    /*******************************************************/
    
    // Testing for AgentSetState
    printf("    Test for AgentSetState:\n");
    AgentSetState(AGENT_STATE_END_SCREEN);
    if (AgentGetState() == AGENT_STATE_END_SCREEN) {
        pass_3++;
    }
    if (pass_3 == 1) {
        printf("    AgentSetState: SUCCESS!\n");
    } else {
        printf("    AgentSetState: FAILED!\n");
    }
    printf("\n");
    
    while (1);
}

