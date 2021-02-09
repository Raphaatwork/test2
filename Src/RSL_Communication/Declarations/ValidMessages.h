#ifndef __VALID_MESSAGES_H
#define __VALID_MESSAGES_H

// Reception validities

/** @brief This method will return the pointer to the reception 2D-byte array
 *  @return Pointer to the reception 2D-byte array
 */
uint8_t** messageParamValidity_reception_getPointerToBuffer();

/** @brief This method will return the overall byte count of the 2D-byte array
 *  @return Overall byte count of the 2D-byte array
 */
uint32_t messageParamValidity_reception_getOverallSize();

/** @brief This method will return the column count of the 2D-byte array
 *  @return Column count of the 2D-byte array
 */
uint32_t messageParamValidity_reception_getColCount();

/** @brief This method will return the row count of the 2D-byte array
 *  @return Row count of the 2D-byte array
 */
uint32_t messageParamValidity_reception_getRowCount();

/** @brief This method will return the element size in the 2D-byte array
 *  @return Element size in the 2D-byte array
 */
uint32_t messageParamValidity_reception_getElementSize();

// Transmission validities

/** @brief This method will return the pointer to the reception 2D-byte array
 *  @return Pointer to the reception 2D-byte array
 */
uint8_t** messageParamValidity_transmission_getPointerToBuffer();

/** @brief This method will return the overall byte count of the 2D-byte array
 *  @return Overall byte count of the 2D-byte array
 */
uint32_t messageParamValidity_transmission_getOverallSize();

/** @brief This method will return the column count of the 2D-byte array
 *  @return Column count of the 2D-byte array
 */
uint32_t messageParamValidity_transmission_getColCount();

/** @brief This method will return the row count of the 2D-byte array
 *  @return Row count of the 2D-byte array
 */
uint32_t messageParamValidity_transmission_getRowCount();

/** @brief This method will return the element size in the 2D-byte array
 *  @return Element size in the 2D-byte array
 */
uint32_t messageParamValidity_transmission_getElementSize();

#endif