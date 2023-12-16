#ifndef __GDT_H
#define __GDT_H

#include "types.h"

//
// GDT class
//
class GDT
{
    public:
        // Segment Descriptor class 
        // Contains base, limit and flags
        class SegmentDescriptor
        {
        private:
            uint16_t seglimit_lo;
            uint16_t base_lo;
            uint8_t  base_hi;
            uint8_t  type;
            uint8_t  flags_limit_h;
            uint8_t  base_vhi;
        
        public:
            SegmentDescriptor(uint32_t base, uint32_t limit, uint8_t flags);
            uint32_t Base();
            uint32_t Limit();
        } __attribute__((packed));

    SegmentDescriptor nullSegmentSelector;
    SegmentDescriptor unusedSegmentSelector;       
    SegmentDescriptor codeSegmentSelector;
    SegmentDescriptor dataSegmentSelector;


public:
    GDT();
    ~GDT();

    uint16_t CodeSegmentSelector();
    uint16_t DataSegmentSelector();
};


#endif