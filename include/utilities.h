#pragma once

#include <iostream>
#include "uartio.h"
#include <string>
#include <utility>

void log_dump_payload(const Payload &payload, const std::string header = "");

void log(std::string in);
