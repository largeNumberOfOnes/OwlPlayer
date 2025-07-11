/*
 * This is an interface that implements all platform-specific functions.
 * The implementation of these functions can be overridden from outside,
 *    so it is compiled into an object file separately from the rest. No
 *    other files can be included in the main code.
 */

#pragma once



#include "had_interface.h"
#include "had_logger.h"
#include "had_types.h"
#include "had_audio.h"
#include "had_keys.h"
