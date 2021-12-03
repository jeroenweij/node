#pragma once

#include <stdint.h>

enum class Operation : uint8_t
{
    GET,
    SET,
    SETPWM,
    SENDQ
};

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

        Id(uint8_t node, uint8_t channel, Operation operation) :
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
        uint8_t value;

        Message() :
            id(0, 0, Operation::GET),
            value(0)
        {
        }
    };
}
