
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
        // 32-bit address space
        // Now we have to squeeze the (32-bit) limit into 2.5 regiters (20-bit).
        // This is done by discarding the 12 least significant bits, but this
        // is only legal, if they are all ==1, so they are implicitly still there

        // so if the last bits aren't all 1, we have to set them to 1, but this
        // would increase the limit (cannot do that, because we might go beyond
        // the physical limit or get overlap with other segments) so we have to
        // compensate this by decreasing a higher bit (and might have up to
        // 4095 wasted bytes behind the used memory)
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
    // Type
    target[5] = flags;
}

//
// Get the Base from the Target
//
uint32_t GDT::SegmentDescriptor::Base()
{
    uint8_t* target = (uint8_t*) this;

    uint32_t result = target[7];
    result = (result << 8) + target[4];
    result = (result << 8) + target[3];
    result = (result << 8) + target[2];

    return result;
}

//
// Get the Limit from the Target
//
uint32_t GDT::SegmentDescriptor::Limit()
{
    uint8_t* target = (uint8_t*) this;
    
    uint32_t result = target[6] & 0xF;
    result = (result << 8) + target[1];
    result = (result << 8) + target[0];

    if( target[6] & 0xC0 == 0xC0 )
        result = (result << 12) | 0xFFF;

    return result;
}