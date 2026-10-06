{ Дроби: десять раз по 0.1 - это ровно 1? }
program Drobi;
var
  s: Real;
  i: Integer;
begin
  s := 0;
  for i := 1 to 10 do
    s := s + 0.1;
  WriteLn('s = ', s:0:10);
  if s = 1 then
    WriteLn('s равно 1')
  else
    WriteLn('s НЕ равно 1');
  WriteLn('s - 1 = ', s - 1);
  { сравнивать дроби надо "с допуском" }
  if Abs(s - 1) < 0.000001 then
    WriteLn('с допуском: равно');
end.
