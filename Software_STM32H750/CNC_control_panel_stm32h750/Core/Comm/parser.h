/*
 * parser.h
 *
 *  Created on: Sep 19, 2023
 *      Author: AvatarBg111
 */

#ifndef COMM_PARSER_H_
#define COMM_PARSER_H_

// Includes
#include "main.h"


// Exported macros and typedefs


// Exported variables


// Exported function declarations
/**
  * @brief Check whether MPG mode is enabled by parsing GRBL message
  */
bool mpg_enabled(uint8_t*,uint8_t);

/**
  * @brief Check whether MPG mode is disabled by parsing GRBL message
  */
bool mpg_disabled(uint8_t*,uint8_t);

#endif /* COMM_PARSER_H_ */
