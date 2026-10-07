; --- ZAPLATKA 1: gde bufer dlya DMA ---
        jmp     Z1_DALSHE
BUFER:                             ; vdvoe bolshe nuzhnogo: ischem okno bez granicy 64 KB
        times 16384 db 0
Z1_DALSHE:
        mov     ax,cs
        mov     [N_SG],ax          ; segment nashej programmy
        mov     dx,ax
        mov     cl,4
        shl     ax,cl              ; segment * 16: mladshie 16 bit
        mov     cl,12
        shr     dx,cl              ; starshie 4 bita - nomer stranicy 64 KB
        mov     bx,OFFSET BUFER    ; smeschenie bufera (bez OFFSET BASM molcha dast 0!)
        add     ax,bx
        jnc     Z1_BEZ_PERENOSA
        inc     dx                 ; perenos v starshuyu chast (BASM ne znaet ADC)
Z1_BEZ_PERENOSA:                   ; fizicheskij adres bufera = DX:AX
        mov     si,ax
        add     si,8192
        jnc     Z1_OK              ; blok ne peresekaet granicu 64 KB
        neg     ax                 ; skolko bajt do granicy
        add     bx,ax              ; sdvigaem okno rovno na granicu
        xor     ax,ax
        inc     dx
Z1_OK:
        mov     [N_OF],bx          ; otkuda Bejsiku pisat (POKE)
        mov     [N_PG],dx          ; stranica - dlya porta 83h
        mov     bl,al
        xor     bh,bh
        mov     [N_LO],bx          ; mladshij bajt adresa
        mov     bl,ah
        mov     [N_HI],bx          ; starshij bajt adresa
; --- konec zaplatki 1 ---
