
#include "interrupts.h"


void printf(char* str);

InterruptManager::GateDescriptor InterruptManager::idt[256];

// Set up an IDT entry
void InterruptManager::SetIDTEntry(
    uint8_t interruptNumber,
    uint16_t codeSegmentSelectorOffset,
    void (*handler)(),
    uint8_t DescriptorPrivilegeLevel,
    uint8_t DescriptorType)
{
    const uint8_t IDT_DESC_PRESENT = 0x80;

    idt[interruptNumber].handleAddressLowBits = (((uint32_t) handler) & 0xFFFF);
    idt[interruptNumber].handlerAddressHighBits = ((((uint32_t) handler) >> 16) & 0xFFFF);
    idt[interruptNumber].gdt_codeSegmentSelector = codeSegmentSelectorOffset;
    idt[interruptNumber].access = IDT_DESC_PRESENT | ((DescriptorPrivilegeLevel & 3) << 5) | DescriptorType;
    idt[interruptNumber].reserved = 0;

};

        
InterruptManager::InterruptManager(GDT* gdt)
: picMasterCommand(0x20), 
  picMasterData(0x21),
  picSlaveCommand(0xA0), 
  picSlaveData(0xA1)
{
    uint32_t CodeSegment = gdt->CodeSegmentSelector();
    const uint8_t IDT_INTERRUPT_GATE = 0xE;

    for (uint16_t i = 0; i < 256; i++){
        SetIDTEntry(i, CodeSegment, &IgnoreInterruptRequest, 0, IDT_INTERRUPT_GATE);
    }

    SetIDTEntry(0, CodeSegment, &IgnoreInterruptRequest, 0, IDT_INTERRUPT_GATE);
    
    SetIDTEntry(0x20, CodeSegment, &HandleInterruptRequest0x00, 0, IDT_INTERRUPT_GATE);
    SetIDTEntry(0x21, CodeSegment, &HandleInterruptRequest0x01, 0, IDT_INTERRUPT_GATE);
    
    picMasterCommand.Write(0x11);
    picSlaveCommand.Write(0x11);

    picMasterData.Write(0x20);
    picSlaveData.Write(0x28);

    picMasterData.Write(0x04); 
    picSlaveData.Write(0x02);

    picMasterData.Write(0x01);
    picSlaveData.Write(0x01);

    picMasterData.Write(0x00);
    picSlaveData.Write(0x00);

    IDTPointer idt_pointer;
    idt_pointer.size = 256 * sizeof(GateDescriptor) - 1;
    idt_pointer.base = (uint32_t) idt;

    asm volatile("lidt %0" : : "m" (idt_pointer));
};

InterruptManager::~InterruptManager()
{

};

void InterruptManager::Activate()
{
    asm("sti");
}

uint32_t InterruptManager::HandleInterrupt(uint8_t interruptNunmber, uint32_t esp)
{
    printf("INTERRUPT");

    return esp;
}

void InterruptManager::IgnoreInterruptRequest()
{
}

void InterruptManager::HandleInterruptRequest0x00()
{

}

void InterruptManager::HandleInterruptRequest0x01()
{

}

