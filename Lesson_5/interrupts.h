#ifndef __INTERRUPTS_H
#define __INTERRUPTS_H

#include "gdt.h"
#include "port.h"

    class InterruptManager
    {

        protected:
            // Each entry of the IDT is a GateDescriptor
            struct GateDescriptor
            {
                uint16_t handleAddressLowBits;
                uint16_t gdt_codeSegmentSelector;
                uint8_t reserved;
                uint8_t access;
                uint16_t handlerAddressHighBits;
            } __attribute__((packed));
            
            // The IDT
            static GateDescriptor idt[256];
            
            // IDT Pointer
            struct IDTPointer
            {
                uint16_t size;
                uint32_t base;
            } __attribute__((packed));

            // Set up an IDT entry
            static void SetIDTEntry(
                uint8_t interruptNumber,
                uint16_t codeSegmentSelectorOffset,
                void (*handler)(),
                uint8_t DescriptorPrivilegeLevel,
                uint8_t DescriptorType
            );
            
            static void IgnoreInterruptRequest();

            static void HandleInterruptRequest0x00();
            static void HandleInterruptRequest0x01();

            static uint32_t HandleInterrupt(uint8_t interruptNunmber, uint32_t esp);

            SlowPort8bit picMasterCommand;
            SlowPort8bit picMasterData;
            SlowPort8bit picSlaveCommand;
            SlowPort8bit picSlaveData; 
            

        public:
            InterruptManager(GDT* gdt);
            ~InterruptManager();

            void Activate();
    };


#endif
