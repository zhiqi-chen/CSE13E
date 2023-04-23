/* 
 * File:   Agent.c
 * Author: barry
 *
 * Created on June 2, 2021, 8:33 PM
 */

#include <xc.h>
#include <stdio.h>
#include "Agent.h"
#include "Oled.h"
#include "Field.h"
#include "Negotiation.h"
#include "FieldOled.h"

/*
 * 
 */

NegotiationData NegotiationHash(NegotiationData secret);
int NegotiationVerify(NegotiationData secret, NegotiationData commitment);
NegotiationOutcome NegotiateCoinFlip(NegotiationData A, NegotiationData B);

Field OwnField;
Field OpponenField;

static AgentState currentState;

static NegotiationData hash;
static NegotiationData A;
static NegotiationData B;

static int turnNumber;

static NegotiationOutcome coinResult;

static GuessData gData;

/**
 * The Init() function for an Agent sets up everything necessary for an agent before the game
 * starts.  At a minimum, this requires:
 *   -setting the start state of the Agent SM.
 *   -setting turn counter to 0
 * If you are using any other persistent data in Agent SM, that should be reset as well.
 * 
 * It is not advised to call srand() inside of AgentInit.  
 *  */
void AgentInit(void) {
    currentState = AGENT_STATE_START;
    A = rand();
    hash = NegotiationHash(A);;
    B = rand();
    turnNumber = 0;

}

/**
 * AgentRun evolves the Agent state machine in response to an event.
 * 
 * @param  The most recently detected event
 * @return Message, a Message struct to send to the opponent. 
 * 
 * If the returned Message struct is a valid message
 * (that is, not of type MESSAGE_NONE), then it will be
 * passed to the transmission module and sent via UART.
 * This is handled at the top level! AgentRun is ONLY responsible 
 * for generating the Message struct, not for encoding or sending it.
 */
Message AgentRun(BB_Event event) {
    // Display new game message
    Message MessageOut = {MESSAGE_NONE, 0, 0, 0};
    // Check if event is reset
    if (event.type == BB_EVENT_RESET_BUTTON) {
        AgentInit();
        MessageOut.type = MESSAGE_NONE;
        return MessageOut;
    }
    // Check if event is error
    if (event.type == BB_EVENT_ERROR) {
        MessageOut.type = MESSAGE_ERROR;
        return MessageOut;
    }
    // state
    switch (currentState) {

        case AGENT_STATE_START:
            if (event.type == BB_EVENT_START_BUTTON) {
                currentState = AGENT_STATE_CHALLENGING;
                // generate A #a
                A = rand();
                hash = NegotiationHash(A);
                // Send CHA
                MessageOut.type = MESSAGE_CHA;
                MessageOut.param0 = hash;
                // Initialize fields
                FieldInit(&OwnField, &OpponenField);
                // Place own boats
                FieldAIPlaceAllBoats(&OwnField);
                return MessageOut;
            } else if (event.type == BB_EVENT_CHA_RECEIVED) {
                currentState = AGENT_STATE_ACCEPTING;
                // generate B
                B = rand();
                hash = event.param0;
                // Send ADC
                MessageOut.type = MESSAGE_ACC;
                MessageOut.param0 = B;
                // Initialize fields
                FieldInit(&OwnField, &OpponenField);
                // place own boats
                FieldAIPlaceAllBoats(&OwnField);
                return MessageOut;
            }

            break;

        case AGENT_STATE_CHALLENGING:
            // check for ACC_RECEIVED
            if (event.type == BB_EVENT_ACC_RECEIVED) {
                B = event.param0;
                coinResult = NegotiateCoinFlip(A, B);
                if (coinResult != TAILS) { // heads
                    // to waiting to send
                    currentState = AGENT_STATE_WAITING_TO_SEND;
                    FieldOledDrawScreen(&OwnField, &OpponenField, FIELD_OLED_TURN_THEIRS, turnNumber);
                } else { // tails
                    // to defending
                    currentState = AGENT_STATE_DEFENDING;
                    FieldOledDrawScreen(&OwnField, &OpponenField, FIELD_OLED_TURN_THEIRS, turnNumber);
                }
                MessageOut.type = MESSAGE_REV;
                MessageOut.param0 = A;
                return MessageOut;
            }

            break;

        case AGENT_STATE_ACCEPTING:
            // check for REV_RECEIVED
            if (event.type == BB_EVENT_REV_RECEIVED) {
                A = event.param0;
                // cheating detected
                if (NegotiationHash(A) != hash) {
                    // to end screen
                    MessageOut.type = MESSAGE_NONE;
                    currentState = AGENT_STATE_END_SCREEN;
                    return MessageOut;
                }
                coinResult = NegotiateCoinFlip(A, B);
                if (coinResult != TAILS) { // heads
                    // to defending
                    FieldOledDrawScreen(&OwnField, &OpponenField, FIELD_OLED_TURN_THEIRS, turnNumber);
                    MessageOut.type = MESSAGE_NONE;
                    currentState = AGENT_STATE_DEFENDING;
                } else { // tails
                    // decide guess send SHO
                    FieldOledDrawScreen(&OwnField, &OpponenField, FIELD_OLED_TURN_MINE, turnNumber);
                    gData = FieldAIDecideGuess(&OpponenField);
                    MessageOut.type = MESSAGE_SHO;
                    MessageOut.param0 = gData.row;
                    MessageOut.param1 = gData.col;
                    // to attacking
                    currentState = AGENT_STATE_ATTACKING;
                }
                return MessageOut;

            }
            break;

        case AGENT_STATE_ATTACKING:
            // check for RES_RECEIVED
            if (event.type == BB_EVENT_RES_RECEIVED) {
                // update record of enemy field
                gData.row = event.param0;
                gData.col = event.param1;
                gData.result = event.param2;
                FieldUpdateKnowledge(&OpponenField, &gData);
                // if victory
                if (FieldGetBoatStates(&OpponenField) == 0b0000) {
                    // to end screen
                    MessageOut.type = MESSAGE_NONE;
                    currentState = AGENT_STATE_END_SCREEN;
                    return MessageOut;
                } else { // if lose
                    // to defending
                    FieldOledDrawScreen(&OwnField, &OpponenField, FIELD_OLED_TURN_THEIRS, turnNumber);
                    MessageOut.type = MESSAGE_NONE;
                    currentState = AGENT_STATE_DEFENDING;
                    return MessageOut;
                }
            }
            break;

        case AGENT_STATE_DEFENDING:
            // check for SHO_RECEIVED
            if (event.type == BB_EVENT_SHO_RECEIVED) {
                // update own field 
                gData.row = event.param0;
                gData.col = event.param1;
                FieldRegisterEnemyAttack(&OwnField, &gData);
                //send RES
                MessageOut.type = MESSAGE_RES;
                MessageOut.param0 = gData.row;
                MessageOut.param1 = gData.col;
                MessageOut.param2 = gData.result;
                // if defeat
                if (FieldGetBoatStates(&OwnField) == 0b0000) {
                    // to end screen
                    currentState = AGENT_STATE_END_SCREEN;
                    return MessageOut;
                } else { 
                    // to waiting to send
                    currentState = AGENT_STATE_WAITING_TO_SEND;
                    FieldOledDrawScreen(&OwnField, &OpponenField, FIELD_OLED_TURN_THEIRS, turnNumber);
                    return MessageOut;
                }
            }
            break;

        case AGENT_STATE_WAITING_TO_SEND:
            // check for MESSAGE_SENT
            if (event.type == BB_EVENT_MESSAGE_SENT) {
                // increment turn count
                turnNumber++;
                // decide guess
                gData = FieldAIDecideGuess(&OpponenField);
                // send SHO
                MessageOut.type = MESSAGE_SHO;
                MessageOut.param0 = gData.row;
                MessageOut.param1 = gData.col;
                currentState = AGENT_STATE_ATTACKING;
                return MessageOut;
            }
            break;
            
        default:
            break;
    }
    MessageOut.type = MESSAGE_NONE;
    return MessageOut;
}

/** * 
 * @return Returns the current state that AgentGetState is in.  
 * 
 * This function is very useful for testing AgentRun.
 */
AgentState AgentGetState(void) {
    return currentState;
}

/** * 
 * @param Force the agent into the state given by AgentState
 * 
 * This function is very useful for testing AgentRun.
 */
void AgentSetState(AgentState newState) {
    currentState = newState;
}