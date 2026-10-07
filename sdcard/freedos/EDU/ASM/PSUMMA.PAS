{ Функция на ассемблере в программе на Паскале }
program PSumma;
{$L SUMMAP.OBJ}                               { подключить объектный модуль }
function Summa(a, b: Integer): Integer; external;   { тело - в SUMMAP.OBJ }
begin
  WriteLn('Паскаль: Summa(30, 12) = ', Summa(30, 12));
end.
