/*
  Project   : G RTOS
  Author    : Ali Yasser
  Date      : October 24, 2024
  Version   : 1.0
  Contact   : k4.k4.3li@gmail.com

  Description:
  Handling FIFO operations like enqueue and dequeue.
*/

#include "FIFO.h"

/**
 * @brief Initializes the FIFO buffer.
 *
 * This function sets up the FIFO buffer with a base address, initializes
 * the head and tail pointers, and sets the length and counter.
 *
 * @param fifo Pointer to the OS_tBuffer structure representing the FIFO.
 * @param buff Pointer to the memory buffer used for storing FIFO elements.
 * @param length The maximum number of elements that the FIFO can hold.
 * @return OS_BufferStatus Status of the initialization.
 */
OS_BufferStatus OS_FifoInit(OS_tBuffer* fifo, ELEMENT_TYPE* buff, uint32_t length) {
    // Check if the provided buffer pointer is valid
    if (!buff)
        return FIFO_NULL;

    fifo->base = buff;           // Set the base address of the FIFO buffer
    fifo->head = fifo->base;     // Initialize head pointer to the start of the buffer
    fifo->tail = fifo->base;     // Initialize tail pointer to the start of the buffer
    fifo->length = length;       // Set the length of the FIFO buffer
    fifo->counter = 0;           // Initialize the counter of elements in the FIFO

    return FIFO_NO_ERROR;        // Indicate successful initialization
}

/**
 * @brief Enqueues an item into the FIFO buffer.
 *
 * This function adds an item to the end of the FIFO buffer. If the FIFO
 * is full, it returns an error.
 *
 * @param fifo Pointer to the OS_tBuffer structure representing the FIFO.
 * @param item The item to be added to the FIFO.
 * @return OS_BufferStatus Status of the enqueue operation.
 */
OS_BufferStatus OS_FifoEnqueue(OS_tBuffer* fifo, ELEMENT_TYPE item) {
    // Check if the FIFO is valid
    if (!fifo->base || !fifo->length)
        return FIFO_NULL;

    // Check if the FIFO is full
    if ((fifo->head == fifo->tail) && (fifo->counter == fifo->length))
        return FIFO_FULL;

    *(fifo->tail) = item;        // Add the item at the tail of the FIFO
    fifo->counter++;             // Increment the counter of elements

    // Handle circular enqueue
    if (fifo->tail == (((unsigned int)fifo->base + (4 * fifo->length)) - 4))
        fifo->tail = fifo->base; // Wrap the tail pointer to the start
    else
        fifo->tail++;             // Move the tail pointer forward

    return FIFO_NO_ERROR;        // Indicate successful enqueue
}

/**
 * @brief Dequeues an item from the FIFO buffer.
 *
 * This function removes an item from the front of the FIFO buffer. If the
 * FIFO is empty, it returns an error.
 *
 * @param fifo Pointer to the OS_tBuffer structure representing the FIFO.
 * @param item Pointer to the variable where the dequeued item will be stored.
 * @return OS_BufferStatus Status of the dequeue operation.
 */
OS_BufferStatus OS_FifoDequeue(OS_tBuffer* fifo, ELEMENT_TYPE* item) {
    // Check if the FIFO is valid
    if (!fifo->base || !fifo->length)
        return FIFO_NULL;

    // Check if the FIFO is empty
    if (fifo->head == fifo->tail)
        return FIFO_EMPTY;

    *item = *(fifo->head);       // Retrieve the item at the head of the FIFO
    fifo->counter--;             // Decrement the counter of elements

    // Handle circular dequeue
    if (fifo->head == (((unsigned int)fifo->base + (4 * fifo->length)) - 4))
        fifo->head = fifo->base; // Wrap the head pointer to the start
    else
        fifo->head++;             // Move the head pointer forward

    return FIFO_NO_ERROR;        // Indicate successful dequeue
}

/**
 * @brief Checks if the FIFO buffer is full.
 *
 * This function verifies if the FIFO buffer has reached its maximum capacity.
 *
 * @param fifo Pointer to the OS_tBuffer structure representing the FIFO.
 * @return OS_BufferStatus Status indicating whether the FIFO is full or not.
 */
OS_BufferStatus OS_IsFifoFull(OS_tBuffer* fifo) {
    // Check if the FIFO is valid
    if (!fifo->head || !fifo->base || !fifo->tail)
        return FIFO_NULL;

    // Check if the FIFO is full
    if (fifo->counter == fifo->length)
        return FIFO_FULL;

    return FIFO_NO_ERROR;        // Indicate the FIFO is not full
}
