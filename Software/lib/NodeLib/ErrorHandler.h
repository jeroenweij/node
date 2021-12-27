/*************************************************************
* Created by J. Weij
*************************************************************/

#pragma once
namespace NodeLib
{
    class ErrorHandler
    {
      public:
        ErrorHandler(const int ledPin, const int buttonPin = -1);

        void Error(const bool recoverable) const;

      private:
        const int ledPin;
        const int buttonPin;
    };
} // namespace NodeLib
