// SPDX-License-Identifier: MIT

#pragma once

#define VERSION_MAJOR 1
#define VERSION_MINOR 0

// need double macro jump to evaluate macros
#define STRINGIFY_IMPL(x) #x
#define STRINGIFY(x) STRINGIFY_IMPL(x)

#define VERSION_STRING STRINGIFY(VERSION_MAJOR) "." STRINGIFY(VERSION_MINOR)

