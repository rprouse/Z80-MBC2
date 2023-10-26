In this folder there are the source files (CP/M 2.2 "core" + Custom BIOS for the Z80-MBC2) used to create:


* the cpm22.bin file (CP/M 2.2 complete load file)

* the binary file used for the track 0 of the boot disk (DS0N00.DSK)

* the complete executable in Intel-Hex format of CP/M 2.2 to be loaded with iLoad (for testing/development)



Note 1: all the previous revisions of the Custom BIOS (CBIOS) are present here (see the Changelog inside 
        each file).



Assemble with the TASM 3.2 assembler:   https://www.ticalc.org/archives/files/fileinfo/250/25051.html
                         TASM manual:   http://www.cpcalive.com/docs/TASMMAN.HTM



--> To assemble cpm22.bin set "iLoadMode .equ 2" inside the cpm22_Z80MBC2.asm file and run the command:

                    tasm -s -h -c -g3 -80  cpm22_Z80MBC2.asm cpm22.bin
                    
                    
                    
--> To assemble the track 0 binary file set "iLoadMode .equ 0" inside the cpm22_Z80MBC2.asm file and 
    run the command:

                    tasm -s -h -c -g3 -80  cpm22_Z80MBC2.asm cpm22Track0.bin
                    
                    The cpm22Track0.bin file must be inserted in the track 0 of the boot disk (DS0N00.DSK) 
                    with CPMtoolsGUI or an equivalent tool, choosing z80-mbc2-d0 as disk format.
                    
                    
                    
--> To assemble in Intel-Hex format for iLoad load set "iLoadMode .equ 1" inside the cpm22_Z80MBC2.asm file
    and run the command:

                    tasm -s -h -c -g0 -80 cpm22_Z80MBC2.asm cpm22.hex




Note 2: in the cpm22_Z80MBC2.asm (at the bottom) there is an include directive for the needed Custom BIOS (last version).

Note 3: since the initial load is done by the Atmega using the cpm22.bin file (instaed of loading the full 
        track 0), it is not really needed to update the track 0 of disk 0 when you are only changing the 
        Bios part.
        When a warm boot is performed only the CP/M 2.2 "core" (Bios excluded) is loaded using the track 0,
        so the sectors of the track 0 storing the Bios are never loaded.
