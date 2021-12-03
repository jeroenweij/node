#pragma once

#include <stdint.h>
#include <tools/SStream.h>

namespace NodeLib
{
    enum class Operation : uint8_t
    {
        GET,
        SET,
        SETPWM,
        VALUE,
        ERROR,
        SETMODE,
        POLLNODES,
        HELLOWORLD,
        SENDQ,
        ENDOFQ
    };

    inline std::stringstream& operator<<(std::stringstream& oStrStream, const Operation Operation)
    {
        switch (Operation)
        {
            case Operation::GET:
                oStrStream << "GET";
                break;
            case Operation::SET:
                oStrStream << "SET";
                break;
            case Operation::SETPWM:
                oStrStream << "SETPWM";
                break;
            case Operation::VALUE:
                oStrStream << "VALUE";
                break;
            case Operation::ERROR:
                oStrStream << "ERROR";
                break;
            case Operation::SETMODE:
                oStrStream << "SETMODE";
                break;
            case Operation::POLLNODES:
                oStrStream << "POLLNODES";
                break;
            case Operation::HELLOWORLD:
                oStrStream << "HELLOWORLD";
                break;
            case Operation::SENDQ:
                oStrStream << "SENDQ";
                break;
            case Operation::ENDOFQ:
                oStrStream << "ENDOFQ";
                break;
        }

        return oStrStream;
    }
}
