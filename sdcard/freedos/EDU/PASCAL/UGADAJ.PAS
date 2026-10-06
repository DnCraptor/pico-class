{ Угадай число: та же игра, что на Бейсике в модуле 19, но без GOTO }
program Ugadaj;
var
  x, g, n: Integer;
begin
  Randomize;
  x := Random(100) + 1;          { случайное целое от 1 до 100 }
  n := 0;
  WriteLn('Угадай число от 1 до 100');
  repeat
    Write('Твой ответ: ');
    ReadLn(g);
    n := n + 1;
    if g < x then
      WriteLn('Больше')
    else if g > x then
      WriteLn('Меньше');
  until g = x;
  WriteLn('Угадал! Ходов: ', n);
end.
