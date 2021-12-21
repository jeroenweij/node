/*************************************************************
* Created by J. Weij
*************************************************************/

#pragma once

#include <stdint.h>

#include "EChannelId.h"
#include "EOperation.h"

using Value = uint8_t;

namespace NodeLib
{
    struct __attribute__((packed)) Id
    {
        uint8_t   node;
        ChannelId channel;
        Operation operation;

        Id() :
            node(0),
            channel(ChannelId::INTERNAL_MSG),
            operation(Operation::GET)
        {
        }

        Id(const uint8_t node, const ChannelId channel) :
            node(node),
            channel(channel),
            operation(Operation::GET)
        {
        }

        Id(const uint8_t node, const ChannelId channel, const Operation operation) :
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
        Id    id;
        Value value;

        Message() :
            id(0, ChannelId::INTERNAL_MSG, Operation::GET),
            value(0)
        {
        }

        Message(int node, const Operation op) :
            id(node, ChannelId::INTERNAL_MSG, op),
            value(0)
        {
        }

        Message(const uint8_t node, const ChannelId channel, const Operation operation, const Value value) :
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
} // namespace NodeLib
