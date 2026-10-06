{ Массив: что будет, если выйти за его границу? }
{$R-}   { проверка границ выключена; поставьте $R+ и соберите снова }
program Massiv;
var                       { переменные лежат в памяти одна за другой }
  a: array[1..5] of Integer;
  b: Integer;
  i: Integer;
begin
  b := 77;
  for i := 1 to 6 do
  begin
    a[i] := i * 10;       { при i = 6 элемента a[6] нет! }
    WriteLn('a[', i, '] = ', a[i], '   b = ', b);
  end;
end.
