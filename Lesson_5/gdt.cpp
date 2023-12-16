
#include "gdt.h"

GDT::GDT()
: nullSegmentSelector(0, 0, 0),
  unusedSegmentSelector(0, 0, 0),
  codeSegmentSelector(0, 64*1024*1024, 0x9A),
  dataSegmentSelector(0, 64*1024*1024, 0x92)
{
    uint32_t i[2];
    // GDT memory address
    i[1] = (uint32_t) this;
    // Size of the GDT (High bytes)
    i[0] = sizeof(GDT) << 16;

    asm volatile("lgdt (%0)": :"p" (((uint8_t *) i) + 2));
}

GDT::~GDT()
{
}


uint16_t GDT::DataSegmentSelector()
{
    return (uint8_t *)&dataSegmentSelector - (uint8_t *)this;
}


uint16_t GDT::CodeSegmentSelector()
{
    return (uint8_t *)&codeSegmentSelector - (uint8_t *)this;
}

//
// Get the Segment Descriptor
//
GDT::SegmentDescriptor::SegmentDescriptor(uint32_t base, uint32_t limit, uint8_t flags)
{
    uint8_t* target = (uint8_t*) this;

    if(limit <= 65536){
        target[6] = 0x40;
    }else{
        if((limit & 0xFFF) != 0XFFF)
            limit = (limit >> 12) - 1;
        else
            limit = limit >> 12;
        
        target[6] = 0xC0;  
    }

    // Assign bits of the limit
    target[0] = limit & 0xFF;
    target[1] = (limit >> 8) & 0xFF;
    target[6] |= (limit >> 16) & 0xF;

    // Assign bits of the base address
    target[2] = base & 0xFF;
    target[3] = (base >> 8) & 0xFF;
    target[4] = (base >> 16) & 0xFF;
    target[7] = (base >> 24) & 0xFF;

    // Assign bits of the flag
    target[5] = flags;
}

//
// Get the Base from the Target
//
uint32_t GDT::SegmentDescriptor::Base()
{
    uint8_t* target = (uint8_t*) this;
    uint32_t result = target[7];
    result |= (target[4] & 0x0000FF00);
    result |= (target[3] & 0x00FF0000);
    result |= (target[2] & 0xFF000000);

    return result;
}

//
// Get the Limit from the Target
//
uint32_t GDT::SegmentDescriptor::Limit()
{
    uint8_t* target = (uint8_t*) this;
    uint32_t result = target[6] & 0xF;
    
    result |= (target[1] & 0x000FF0);
    result |= (target[0] & 0x0FF000);

    if( target[6] & 0xC0 == 0xC0 )
        result = (result << 12) | 0xFFF;

    return result;

    return result;
}