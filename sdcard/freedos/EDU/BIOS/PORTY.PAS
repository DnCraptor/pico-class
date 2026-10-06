{ Порты: часы и динамик без BIOS и DOS }
program Porty;

function Hex2(b: Byte): string;           { байт -> две шестнадцатеричные цифры }
const Cifry: string[16] = '0123456789ABCDEF';
begin
  Hex2 := Cifry[b div 16 + 1] + Cifry[b mod 16 + 1];
end;

function Chasy(reg: Byte): Byte;          { прочитать регистр микросхемы часов }
begin
  Port[$70] := reg;                       { какой регистр нужен }
  Chasy := Port[$71];                     { его значение }
end;

var t: Word;
begin
  WriteLn('Часы (порты 70h, 71h): ',
          Hex2(Chasy(4)), ':', Hex2(Chasy(2)), ':', Hex2(Chasy(0)));
  { динамик: таймер (порты 42h, 43h) и порт 61h }
  Port[$43] := $B6;                       { канал 2 таймера, режим "меандр" }
  Port[$42] := $A9;                       { 1193182 / 1000 = 1193 = 04A9h: }
  Port[$42] := $04;                       { сначала младший байт, потом старший }
  Port[$61] := Port[$61] or 3;            { включить динамик }
  t := MemW[$0040:$006C];                { счетчик тиков таймера: 18,2 в секунду }
  repeat until MemW[$0040:$006C] - t >= 6;   { ждем 6 тиков - треть секунды }
  Port[$61] := Port[$61] and $FC;         { выключить }
  WriteLn('Звук 1000 Гц - через порты');
end.
