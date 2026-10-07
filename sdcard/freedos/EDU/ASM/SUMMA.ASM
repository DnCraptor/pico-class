; SUMMA.ASM - одна функция в двух соглашениях о вызовах
; для Си:      jwasmr summa.asm             -> SUMMA.OBJ
; для Паскаля: jwasmr -DPASKAL -Fo summap.obj summa.asm

IFDEF PASKAL
CODE    SEGMENT BYTE PUBLIC         ; Turbo Pascal ждет сегмент CODE
        ASSUME  CS:CODE
ELSE
_TEXT   SEGMENT BYTE PUBLIC 'CODE'  ; Turbo C ждет сегмент _TEXT
        ASSUME  CS:_TEXT
ENDIF

        PUBLIC  SUMMA
; ---- соглашение Паскаля: function Summa(a, b: Integer): Integer
; параметры кладутся в стек слева направо (a, потом b),
; стек чистит сама функция (RET 4), имя - заглавными буквами
SUMMA   PROC    NEAR
        push    bp
        mov     bp,sp           ; [bp+2] - адрес возврата
        mov     ax,[bp+6]       ; a - положен первым, лежит глубже
        add     ax,[bp+4]       ; b
        pop     bp
        ret     4               ; убрать из стека 4 байта параметров
SUMMA   ENDP

IFNDEF PASKAL
        PUBLIC  _summa_c
; ---- соглашение Си: int summa_c(int a, int b)
; параметры кладутся справа налево (b, потом a),
; стек чистит тот, кто вызвал, имя - с подчеркиванием впереди
_summa_c PROC   NEAR
        push    bp
        mov     bp,sp
        mov     ax,[bp+4]       ; a - положен последним, лежит ближе
        add     ax,[bp+6]       ; b
        pop     bp
        ret                     ; параметры уберет вызывающий
_summa_c ENDP
ENDIF

IFDEF PASKAL
CODE    ENDS
ELSE
_TEXT   ENDS
ENDIF
        END
