#ifndef __PORT_H
#define __PORT_H

#include "types.h"

    class Port
    {
        protected:
            uint16_t portnumber;
            Port(uint16_t portnumber);
            ~Port();
    };

    class Port8bit: public Port
    {
        public:
            Port8bit(uint8_t portnumber);
            ~Port8bit();
            virtual void Write(uint8_t portnumber);
            virtual uint8_t Read();
    };

    class SlowPort8bit : public Port8bit
    {
        public:
            SlowPort8bit(uint8_t portnumber);
            ~SlowPort8bit();
            virtual void Write(uint8_t portnumber);
    };    

    class Port16bit : public Port
    {
        public:
            Port16bit(uint16_t portnumber);
            ~Port16bit();
            virtual void Write(uint16_t portnumber);
            virtual uint16_t Read();
    };

    class Port32bit : public Port
    {
        public:
            Port32bit(uint32_t portnumber);
            ~Port32bit();
            virtual void Write(uint32_t portnumber);
            virtual uint32_t Read();
    };

#endif