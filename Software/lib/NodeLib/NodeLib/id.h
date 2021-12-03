#pragma once

#include <stdint.h>

#include "EOperation.h"

using Value = uint8_t;

namespace NodeLib
{
    struct __attribute__((packed)) Id
    {
        uint8_t node;
        uint8_t channel;
        Operation operation;

        Id() :
            node(0),
            channel(0),
            operation(Operation::GET)
        {
        }

        Id(const uint8_t node, const uint8_t channel) :
            node(node),
            channel(channel),
            operation(Operation::GET)
        {
        }

        Id(const uint8_t node, const uint8_t channel, const Operation operation) :
            node(node),
            channel(channel),
            operation(operation)
        {
        }

        bool operator<(const Id& other) const;
        bool operator==(const Id& other) const;

        inline bool operator!=(const Id& other) const
        {
            return !(*this == other);
        }
    };

    struct __attribute__((packed)) Message
    {
        Id id;
        Value value;

        Message() :
            id(0, 0, Operation::GET),
            value(0)
        {
        }

        Message(int node, const Operation op) :
            id(node, 0, op),
            value(0)
        {
        }

        Message(const uint8_t node, const uint8_t channel, const Operation operation, const Value value) :
            id(node, channel, operation),
            value(value)
        {
        }

        Message(const Id& id, const Value value) :
            id(id),
            value(value)
        {
        }
    };
}
