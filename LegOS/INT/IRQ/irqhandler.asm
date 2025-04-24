extern timer_callback
extern irq1_handler

;irq0 - Programmable Interrupt Timer Interrupt
irq_stub_0:
    push eax 
    call timer_callback
    mov al, 20h
    out 20h, al
    pop eax
    iret

;irq1 - keyboard
irq_stub_1: 
    push eax
    call  irq1_handler
    pop eax
    iret

;irq2 - Cascade (used internally by the two PICs. never raised)
irq_stub_2:
    cli
    hlt
    iret

;irq3 - COM2 (if enabled)
irq_stub_3:
    cli
    hlt
    iret

;irq4 - COM1 (if enabled)
irq_stub_4:
    cli
    hlt
    iret

;irq5 - LPT2 (if enabled)
irq_stub_5:
    cli
    hlt
    iret

;irq6 - Floppy Disk
irq_stub_6:
    cli
    hlt
    iret

;irq7 - LPT1 / Unreliable "spurious" interrupt (usually)
irq_stub_7:
    cli
    hlt
    iret

;irq8 - CMOS real-time clock (if enabled)
irq_stub_8:
    cli
    hlt
    iret

;irq9 - Free for peripherals / legacy SCSI / NIC 
irq_stub_9:
    cli
    hlt
    iret

;irq10 - Free for peripherals / SCSI / NIC
irq_stub_10:
    cli
    hlt
    iret

;irq11 - Free for peripherals / SCSI / NIC
irq_stub_11:
    cli
    hlt
    iret

;irq12 - PS2 mouse
irq_stub_12:
    cli
    hlt
    iret

;irq13 - FPU / Coprocessor / Inter-processor
irq_stub_13:
    cli
    hlt
    iret

;irq14 - Primary ATA Hard Disk 
irq_stub_14:
    cli
    hlt
    iret

;irq15 - Secondary ATA Hard Disk 
irq_stub_15:
    cli
    hlt
    iret

;stub table: (used to prevent excessive code reuse, and not related to actual function)
global irq_stub_table
irq_stub_table:
%assign i 0 
%rep    16 
    dd irq_stub_%+i 
%assign i i+1 
%endrep
