// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      queue.c
//      Purpose :   Input Queue for keyboard
//      Date :      3rd July 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "input_module.h"
#include "input_module_local.h"

#define INPQUEUESIZE    (128)                                                       // So we can unpack function keys !

uint8_t  queueEntryCount = 0;
uint16_t keyboardQueue[INPQUEUESIZE];

/**
 * @brief      Empty the keyboard queue
 */
void INPResetQueue(void) {
    queueEntryCount = 0;
}
/**
 * @brief      Insert a charactr into the keyboard queue
 *
 * @param[in]  key   Code of character to insert
 */
void INPInsertIntoQueue(int16_t key) {
    if (queueEntryCount < INPQUEUESIZE) {                                           // Add to end of queue if space.
        keyboardQueue[queueEntryCount++] = key;
        // LOG("Added %d to queue",key);
    }
}

/**
 * @brief      Get the next key in the queue.
 *
 * @return     Key at the front of the queue, zero if there is none.
 */
int16_t INPGetKey(void) {
    if (queueEntryCount == 0) return 0;                                             // Queue empty
    int key = keyboardQueue[0];
    for (int i = 0;i < queueEntryCount-1;i++) {                                     // Remove head of the queue
        keyboardQueue[i] = keyboardQueue[i+1];
    }
    queueEntryCount--;                                                              // Fix up the count.
    return key;
}

/**
 * @brief      Check if a key is available in the queue
 *
 * @return     true if there is a key.
 */
bool INPIsKeyAvailable(void) {
    return queueEntryCount != 0;
}

