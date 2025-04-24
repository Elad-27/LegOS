gdtr DW 0 ; For limit storage
     DD 0 ; For base storage

global setGdt
global reloadSegments
extern gdt_descriptor
section .text
   setGdt:
      MOV   AX, [esp + 4]
      MOV   [gdtr], AX
      MOV   EAX, [ESP + 8]
      MOV   [gdtr + 2], EAX
      LGDT  [gdtr]
      RET
      
   reloadSegments:
      ; Reload data segment registers:
      LGDT [gdt_descriptor]
      
      MOV   AX, 0x10 ; 0x10 is a stand-in for your data segment
      MOV   DS, AX
      MOV   ES, AX
      MOV   FS, AX
      MOV   GS, AX
      MOV   SS, AX
      jmp 08h:.flush
   .flush:
      ret