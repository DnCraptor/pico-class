{ Запись с вариантами: та же идея, что union в Си }
program Variant;
type
  Chislo = record
    case Integer of
      0: (i: Integer);                 { целое: 2 байта }
      1: (bajt: array[0..1] of Byte);  { те же 2 байта по одному }
      2: (l: LongInt);                 { длинное: 4 байта }
  end;
var
  c: Chislo;
begin
  WriteLn('размер записи: ', SizeOf(c), ' байта (как самый большой вариант)');
  c.l := 0;
  c.i := 258;                          { 258 = 1 * 256 + 2 }
  WriteLn('i = ', c.i, ', bajt[0] = ', c.bajt[0], ', bajt[1] = ', c.bajt[1]);
  c.bajt[1] := 3;                      { меняем старший байт }
  WriteLn('i = ', c.i);                { 3 * 256 + 2 = 770 }
end.
