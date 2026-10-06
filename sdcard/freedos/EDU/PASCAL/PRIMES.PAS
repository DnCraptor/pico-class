{ Простые числа от 2 до M с замером времени - как PRIMES.BAS в модуле 19 }
program Primes;
uses Dos;
var
  m, n, d, c: Integer;
  t1, t2: LongInt;

{ время в сотых долях секунды с начала суток }
function Now: LongInt;
var
  h, mi, s, hs: Word;
begin
  GetTime(h, mi, s, hs);
  Now := ((LongInt(h) * 60 + mi) * 60 + s) * 100 + hs;
end;

begin
  WriteLn('Сколько простых чисел от 2 до M?');
  Write('M (не больше 32000): ');
  ReadLn(m);
  t1 := Now;
  c := 0;
  for n := 2 to m do
  begin
    d := 2;
    while (d * d <= n) and (n mod d <> 0) do
      d := d + 1;
    if d * d > n then
      c := c + 1;
  end;
  t2 := Now;
  WriteLn('Найдено простых: ', c);
  WriteLn('Секунд: ', (t2 - t1) / 100 : 0 : 2);
end.
