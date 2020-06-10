#pragma once

#include <iostream>
#include <string>
#include <utility>
#include "payload.h"

void log_dump_transport(const Payload &payload, const std::string header);
void log_dump_network(const Payload &payload, const std::string header);
void log_dump_datalink(const Payload &payload, const std::string header);
void log_dump_physical(const Payload &payload, const std::string header);

void log(std::string in);
