{ VIDEO.PAS - видеорежимы PC 1981-1991 годов: одна и та же картинка
  в каждом режиме, который умеет видеоадаптер.

  Для каждого режима программа сначала рассказывает о нем (разрешение,
  число цветов, сколько бит на точку и сколько байт видеопамяти нужно),
  потом включает режим и рисует картинку: небо, солнце, дом, дерево и две
  полосы внизу - радугу и переход от черного к белому. На полосах лучше
  всего видно, сколько цветов умеет режим. Потом говорит, сколько времени
  рисовала. Если видеоадаптер такого режима не знает, BIOS его не включит,
  и программа так и скажет.

  Сокращения:
    MDA  - Monochrome Display Adapter, монохромный адаптер дисплея
    CGA  - Color Graphics Adapter, цветной графический адаптер
    EGA  - Enhanced Graphics Adapter, улучшенный графический адаптер
    MCGA - Multi-Color Graphics Array, многоцветная графическая матрица
    VGA  - Video Graphics Array, видеографическая матрица
    SVGA - Super VGA: все, что больше и цветнее VGA
    VESA - Video Electronics Standards Association, объединение фирм,
           договорившихся, как включать режимы SVGA (VBE - VESA BIOS
           Extensions, расширения BIOS по стандарту VESA)
    BIOS - Basic Input/Output System, базовая система ввода-вывода;
           режимы включает ее функция INT 10h
    RGB  - Red, Green, Blue: красный, зеленый, синий
    DAC  - Digital-to-Analog Converter, ЦАП: превращает номер цвета
           в напряжения красного, зеленого и синего для монитора

  Часть комплекта pico-class, см. tools/video/README.md. }

program Video;

uses Dos;

type
  TKind = (kMDA, kText, kCGA4, kCGA2, kPlanar, kLin1, kLin8,
           kVBE8, kVBE16, kVBE24);

const
  NMODE = 12;
  Keys: array[1..NMODE] of Char =
    ('1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C');
  ModeNo: array[1..NMODE] of Word =
    ($07, $03, $04, $06, $0D, $10, $11, $12, $13, $100, $10E, $10F);
  Kinds: array[1..NMODE] of TKind =
    (kMDA, kText, kCGA4, kCGA2, kPlanar, kPlanar, kLin1, kPlanar, kLin8,
     kVBE8, kVBE16, kVBE24);
  ResW: array[1..NMODE] of Word = (80, 80, 320, 640, 320, 640, 640, 640, 320, 640, 320, 320);
  ResH: array[1..NMODE] of Word = (25, 25, 200, 200, 200, 350, 480, 480, 200, 400, 200, 200);
  Bytes: array[1..NMODE] of LongInt =
    (4000, 4000, 16000, 16000, 32000, 112000, 38400, 153600, 64000,
     256000, 128000, 192000);
  Title: array[1..NMODE] of String[46] = (
    'MDA, текст 80 x 25, один цвет',
    'CGA, цветной текст 80 x 25',
    'CGA 320 x 200, 4 цвета',
    'CGA 640 x 200, 2 цвета',
    'EGA 320 x 200, 16 цветов',
    'EGA 640 x 350, 16 цветов из 64',
    'MCGA 640 x 480, 2 цвета',
    'VGA 640 x 480, 16 цветов из 262 144',
    'MCGA и VGA 320 x 200, 256 цветов',
    'SVGA 640 x 400, 256 цветов (VESA)',
    'SVGA 320 x 200, 65 536 цветов (VESA)',
    'SVGA 320 x 200, 16,7 миллиона цветов (VESA)');

  { цвета EGA и VGA по умолчанию (0..255 на каждую составляющую) }
  EgaR: array[0..15] of Byte = (0, 0, 0, 0, 170, 170, 170, 170, 85, 85, 85, 85, 255, 255, 255, 255);
  EgaG: array[0..15] of Byte = (0, 0, 170, 170, 0, 0, 85, 170, 85, 85, 255, 255, 85, 85, 255, 255);
  EgaB: array[0..15] of Byte = (0, 170, 0, 170, 0, 170, 0, 170, 85, 255, 85, 255, 85, 255, 85, 255);
  { CGA 320 x 200, палитра 1: черный, голубой, малиновый, серый }
  CgaR: array[0..3] of Byte = (0, 0, 170, 170);
  CgaG: array[0..3] of Byte = (0, 170, 0, 170);
  CgaB: array[0..3] of Byte = (0, 170, 170, 170);
  { порог для узора из точек (дизеринга) 4 x 4 }
  Bayer: array[0..15] of Byte = (0, 8, 2, 10, 12, 4, 14, 6, 3, 11, 1, 9, 15, 7, 13, 5);

var
  Kind: TKind;
  W, H: Word;                   { разрешение текущего режима }
  BPL: Word;                    { байт на строку экрана }
  Gran: LongInt;                { VESA: шаг окна памяти в байтах }
  Bank: Integer;                { VESA: какой кусок памяти сейчас виден в окне }
  VbeBytes: Byte;               { VESA: байт на точку }
  CR, CG, CB: Byte;             { текущий цвет: красный, зеленый, синий }

{ ---------- клавиатура ---------- }

function KeyPressed: Boolean;   { есть ли клавиша в буфере BIOS (40h:1Ah и 1Ch) }
begin
  KeyPressed := MemW[$40:$1A] <> MemW[$40:$1C];
end;

function ReadKey: Char;
var r: Registers;
begin
  r.ah := 0; Intr($16, r);
  ReadKey := Chr(r.al);
end;

function Ticks: LongInt;        { счетчик BIOS: 18,2 раза в секунду }
begin
  Ticks := MemL[$40:$6C];
end;

{ ---------- BIOS: режимы ---------- }

procedure SetMode(m: Byte);
var r: Registers;
begin
  r.ah := 0; r.al := m; Intr($10, r);
end;

function TextMode: Boolean;     { вернуться в обычный текст 80 x 25 }
begin
  SetMode(3);
  TextMode := True;
end;

function TryMode(i: Integer): Boolean;
{ включить режим; False - видеоадаптер его не умеет }
var r: Registers; info: array[0..255] of Byte;
begin
  TryMode := False;
  if ModeNo[i] < $100 then
  begin
    SetMode(ModeNo[i]);
    TryMode := Mem[$40:$49] = ModeNo[i];     { BIOS запоминает режим здесь }
  end
  else
  begin
    { VESA: сначала спросить о режиме (функция 4F01h), потом включить (4F02h) }
    r.ax := $4F01; r.cx := ModeNo[i]; r.es := Seg(info); r.di := Ofs(info);
    Intr($10, r);
    if r.ax <> $004F then Exit;
    if (info[0] and 1) = 0 then Exit;        { режим есть, но этому монитору не подходит }
    Gran := LongInt(info[4] + 256 * info[5]) * 1024;
    BPL := info[$10] + 256 * info[$11];
    VbeBytes := (info[$19] + 7) div 8;
    if Gran = 0 then Exit;
    r.ax := $4F02; r.bx := ModeNo[i]; Intr($10, r);
    if r.ax <> $004F then Exit;
    Bank := -1;
    TryMode := True;
  end;
end;

{ ---------- точки и линии ---------- }

procedure SelectBank(b: Integer);
var r: Registers;
begin
  if b <> Bank then
  begin
    r.ax := $4F05; r.bx := 0; r.dx := b; Intr($10, r);
    Bank := b;
  end;
end;

procedure VbeByte(addr: LongInt; v: Byte);
{ VESA: память больше окна 64 КБ, поэтому видна кусками - "банками" }
begin
  SelectBank(addr div Gran);
  Mem[$A000:addr mod Gran] := v;
end;

function Nearest16: Byte;       { ближайший из 16 цветов EGA }
var i, best: Integer; d, bd: LongInt;
begin
  bd := MaxLongInt; best := 0;
  for i := 0 to 15 do
  begin
    d := Sqr(LongInt(CR) - EgaR[i]) + Sqr(LongInt(CG) - EgaG[i]) + Sqr(LongInt(CB) - EgaB[i]);
    if d < bd then begin bd := d; best := i; end;
  end;
  Nearest16 := best;
end;

function Nearest4: Byte;        { ближайший из 4 цветов CGA }
var i, best: Integer; d, bd: LongInt;
begin
  bd := MaxLongInt; best := 0;
  for i := 0 to 3 do
  begin
    d := Sqr(LongInt(CR) - CgaR[i]) + Sqr(LongInt(CG) - CgaG[i]) + Sqr(LongInt(CB) - CgaB[i]);
    if d < bd then begin bd := d; best := i; end;
  end;
  Nearest4 := best;
end;

function Index256: Byte;
{ наша палитра: 16..231 - куб 6 x 6 x 6, 232..255 - 24 оттенка серого }
begin
  if (CR = CG) and (CG = CB) then
    Index256 := 232 + Word(CR) * 23 div 255
  else
    Index256 := 16 + ((CR + 25) div 51) * 36 + ((CG + 25) div 51) * 6 + (CB + 25) div 51;
end;

function Dither(x, y: Word): Byte;   { 2 цвета: светлее порога - белая точка }
var lum: Word;
begin
  lum := (Word(CR) * 3 + Word(CG) * 6 + Word(CB)) div 10;
  if lum > Bayer[(y and 3) * 4 + (x and 3)] * 16 + 8 then Dither := 1 else Dither := 0;
end;

var
  DevColor: Byte;               { номер цвета для текущего режима (кроме 2 цветов и VESA 16/24) }

procedure SetColor(r, g, b: Byte);
begin
  CR := r; CG := g; CB := b;
  case Kind of
    kCGA4: DevColor := Nearest4;
    kPlanar: DevColor := Nearest16;
    kLin8, kVBE8: DevColor := Index256;
  end;
end;

procedure PutPixel(x, y: Word);
var ofs: Word; sh: Byte; a: LongInt; c: Word;
begin
  case Kind of
    kCGA4:
      begin                     { четные строки с B800:0000, нечетные с B800:2000 }
        ofs := (y and 1) * $2000 + (y shr 1) * 80 + x shr 2;
        sh := (3 - (x and 3)) * 2;
        Mem[$B800:ofs] := (Mem[$B800:ofs] and not (3 shl sh)) or (DevColor shl sh);
      end;
    kCGA2:
      begin
        ofs := (y and 1) * $2000 + (y shr 1) * 80 + x shr 3;
        sh := 7 - (x and 7);
        Mem[$B800:ofs] := (Mem[$B800:ofs] and not (1 shl sh)) or (Dither(x, y) shl sh);
      end;
    kLin1:
      begin
        ofs := y * 80 + x shr 3;
        sh := 7 - (x and 7);
        Mem[$A000:ofs] := (Mem[$A000:ofs] and not (1 shl sh)) or (Dither(x, y) shl sh);
      end;
    kPlanar:
      begin                     { 4 плоскости: маска бита, чтение в защелки, запись цвета }
        ofs := y * BPL + x shr 3;
        Port[$3CE] := 8; Port[$3CF] := $80 shr (x and 7);
        sh := Mem[$A000:ofs];
        Mem[$A000:ofs] := DevColor;
      end;
    kLin8:
      Mem[$A000:y * 320 + x] := DevColor;
    kVBE8:
      VbeByte(LongInt(y) * BPL + x, DevColor);
    kVBE16:
      begin                     { 5 бит красного, 6 зеленого, 5 синего }
        c := (Word(CR shr 3) shl 11) or (Word(CG shr 2) shl 5) or (CB shr 3);
        a := LongInt(y) * BPL + LongInt(x) * 2;
        VbeByte(a, Lo(c)); VbeByte(a + 1, Hi(c));
      end;
    kVBE24:
      begin                     { по байту: синий, зеленый, красный }
        a := LongInt(y) * BPL + LongInt(x) * VbeBytes;
        VbeByte(a, CB); VbeByte(a + 1, CG); VbeByte(a + 2, CR);
      end;
  end;
end;

var
  PixB: array[0..3] of Byte;    { VESA: байты одной точки }

procedure VbePixBytes;          { байты точки текущего цвета для VESA 16 и 24 бит }
var c: Word;
begin
  if Kind = kVBE16 then
  begin                         { 5 бит красного, 6 зеленого, 5 синего }
    c := (Word(CR shr 3) shl 11) or (Word(CG shr 2) shl 5) or (CB shr 3);
    PixB[0] := Lo(c); PixB[1] := Hi(c);
  end
  else
  begin                         { по байту: синий, зеленый, красный }
    PixB[0] := CB; PixB[1] := CG; PixB[2] := CR; PixB[3] := 0;
  end;
end;

procedure HLine(x1, x2, y: Integer);
{ горизонтальная линия одного цвета; для каждого вида памяти свой быстрый путь }
var x, n, k, bp: Integer; a, off: LongInt; ofs, e1, e2: Word; m, pat: Byte;
begin
  if y < 0 then Exit;
  if y >= H then Exit;
  if x1 < 0 then x1 := 0;
  if x2 >= W then x2 := W - 1;
  if x1 > x2 then Exit;
  case Kind of
    kLin8:
      FillChar(Mem[$A000:y * 320 + x1], x2 - x1 + 1, DevColor);
    kVBE8:
      begin                     { кусками, не пересекая границу банка }
        a := LongInt(y) * BPL + x1; n := x2 - x1 + 1;
        while n > 0 do
        begin
          SelectBank(a div Gran);
          off := a mod Gran;
          k := n;
          if off + k > Gran then k := Gran - off;
          FillChar(Mem[$A000:off], k, DevColor);
          a := a + k; n := n - k;
        end;
      end;
    kVBE16, kVBE24:
      begin
        VbePixBytes;
        if Kind = kVBE16 then bp := 2 else bp := VbeBytes;
        a := LongInt(y) * BPL + LongInt(x1) * bp;
        SelectBank(a div Gran); off := a mod Gran;
        for x := x1 to x2 do
          for k := 0 to bp - 1 do
          begin
            Mem[$A000:off] := PixB[k];
            Inc(off);
            if off = Gran then begin SelectBank(Bank + 1); off := 0; end;
          end;
      end;
    kPlanar:
      begin                     { края - по точке, середина - целыми байтами }
        e1 := (x1 + 7) and not 7; e2 := (x2 + 1) and not 7;
        if e1 >= e2 then
          for x := x1 to x2 do PutPixel(x, y)
        else
        begin
          for x := x1 to e1 - 1 do PutPixel(x, y);
          Port[$3CE] := 8; Port[$3CF] := $FF;
          ofs := y * BPL + e1 shr 3;
          for x := 0 to (e2 - e1) shr 3 - 1 do
          begin
            m := Mem[$A000:ofs]; Mem[$A000:ofs] := DevColor; Inc(ofs);
          end;
          for x := e2 to x2 do PutPixel(x, y);
        end;
      end;
    kCGA4:
      begin                     { 4 точки в байте: байт середины - цвет 4 раза подряд }
        e1 := (x1 + 3) and not 3; e2 := (x2 + 1) and not 3;
        if e1 >= e2 then
          for x := x1 to x2 do PutPixel(x, y)
        else
        begin
          for x := x1 to e1 - 1 do PutPixel(x, y);
          ofs := (y and 1) * $2000 + (y shr 1) * 80 + e1 shr 2;
          FillChar(Mem[$B800:ofs], (e2 - e1) shr 2, DevColor * $55);
          for x := e2 to x2 do PutPixel(x, y);
        end;
      end;
    kLin1, kCGA2:
      begin                     { узор повторяется через 4 точки: байт середины один и тот же }
        e1 := (x1 + 7) and not 7; e2 := (x2 + 1) and not 7;
        if e1 >= e2 then
          for x := x1 to x2 do PutPixel(x, y)
        else
        begin
          for x := x1 to e1 - 1 do PutPixel(x, y);
          pat := 0;
          for x := 0 to 7 do pat := (pat shl 1) or Dither(x, y);
          if Kind = kLin1 then ofs := y * 80 + e1 shr 3
          else ofs := (y and 1) * $2000 + (y shr 1) * 80 + e1 shr 3;
          if Kind = kLin1 then FillChar(Mem[$A000:ofs], (e2 - e1) shr 3, pat)
          else FillChar(Mem[$B800:ofs], (e2 - e1) shr 3, pat);
          for x := e2 to x2 do PutPixel(x, y);
        end;
      end;
  else
    for x := x1 to x2 do PutPixel(x, y);
  end;
end;

{ Полосы внизу: цвет меняется от точки к точке, поэтому цвета для всех x
  считаются один раз, а строки потом только копируются. }
var
  SR, SG, SB, SD: array[0..639] of Byte;   { цвет каждой точки строки и его номер }

procedure StripRow(y: Integer);
var x, k, bp: Integer; a, off: LongInt;
begin
  if Kind >= kVBE8 then
  begin                         { VESA: адрес растет по строке, банк меняется на границе }
    if Kind = kVBE8 then bp := 1
    else if Kind = kVBE16 then bp := 2
    else bp := VbeBytes;
    a := LongInt(y) * BPL;
    SelectBank(a div Gran); off := a mod Gran;
    for x := 0 to W - 1 do
    begin
      CR := SR[x]; CG := SG[x]; CB := SB[x];
      if Kind = kVBE8 then PixB[0] := SD[x] else VbePixBytes;
      for k := 0 to bp - 1 do
      begin
        Mem[$A000:off] := PixB[k];
        Inc(off);
        if off = Gran then begin SelectBank(Bank + 1); off := 0; end;
      end;
    end;
  end
  else
    for x := 0 to W - 1 do
    begin
      CR := SR[x]; CG := SG[x]; CB := SB[x]; DevColor := SD[x];
      PutPixel(x, y);
    end;
end;

{ ---------- картинка ---------- }

{ Картинка задана на поле 1000 x 750 (это 4:3, как экран) и растягивается
  на любое разрешение: точки у CGA и EGA не квадратные, а круг остается
  кругом. }
function SX(v: Integer): Integer; begin SX := LongInt(v) * W div 1000; end;
function SY(v: Integer): Integer; begin SY := LongInt(v) * H div 750; end;

procedure Box(x1, y1, x2, y2: Integer);
var y: Integer;
begin
  for y := SY(y1) to SY(y2) - 1 do HLine(SX(x1), SX(x2) - 1, y);
end;

procedure Disc(cx, cy, r: Integer);   { круг радиуса r }
var y, y1, y2, dx: Integer; dy: Real;
begin
  y1 := SY(cy - r); y2 := SY(cy + r);
  for y := y1 to y2 do
  begin
    dy := (y + 0.5) * 750.0 / H - cy;
    if Sqr(dy) < Sqr(1.0 * r) then
    begin
      dx := Round(Sqrt(Sqr(1.0 * r) - Sqr(dy)));
      HLine(SX(cx - dx), SX(cx + dx), y);
    end;
  end;
end;

procedure Roof(x1, x2, ytop, ybot: Integer);   { треугольник }
var y, half, mid: Integer;
begin
  mid := (x1 + x2) div 2;
  for y := SY(ytop) to SY(ybot) - 1 do
  begin
    half := LongInt(y - SY(ytop)) * ((x2 - x1) div 2) div (SY(ybot) - SY(ytop));
    HLine(SX(mid - half), SX(mid + half), y);
  end;
end;

procedure Hue(t: Integer);      { радуга: t = 0..1530 }
begin
  case t div 255 of
    0: SetColor(255, t, 0);
    1: SetColor(510 - t, 255, 0);
    2: SetColor(0, 255, t - 510);
    3: SetColor(0, 1020 - t, 255);
    4: SetColor(t - 1020, 0, 255);
  else SetColor(255, 0, 1530 - t);
  end;
end;

procedure Picture;
var y, x, y1, y2, v: Integer;
begin
  { небо: сверху синее, к горизонту светлее }
  y2 := SY(450);
  for y := 0 to y2 - 1 do
  begin
    v := LongInt(y) * 160 div y2;
    SetColor(40 + v div 2, 90 + v, 255 - v div 4);
    HLine(0, W - 1, y);
  end;
  SetColor(255, 220, 0);  Disc(820, 120, 70);            { солнце }
  { трава: темнее к низу }
  y1 := SY(450); y2 := SY(620);
  for y := y1 to y2 - 1 do
  begin
    v := LongInt(y - y1) * 100 div (y2 - y1);
    SetColor(40, 200 - v, 40);
    HLine(0, W - 1, y);
  end;
  SetColor(210, 160, 100); Box(150, 300, 450, 520);      { стена }
  SetColor(200, 30, 30);   Roof(120, 480, 170, 300);      { крыша }
  SetColor(255, 255, 255); Box(220, 350, 320, 450);      { окно }
  SetColor(60, 120, 255);  Box(230, 360, 310, 440);
  SetColor(120, 70, 30);   Box(360, 380, 420, 520);      { дверь }
  SetColor(110, 60, 20);   Box(640, 330, 680, 520);      { ствол }
  SetColor(0, 140, 0);     Disc(660, 280, 90);           { крона }
  { полоса-радуга: все оттенки подряд }
  for x := 0 to W - 1 do
  begin
    Hue(LongInt(x) * 1530 div W);
    SR[x] := CR; SG[x] := CG; SB[x] := CB; SD[x] := DevColor;
  end;
  for y := SY(630) to SY(690) - 1 do StripRow(y);
  if KeyPressed then Exit;
  { полоса от черного к белому }
  for x := 0 to W - 1 do
  begin
    v := LongInt(x) * 255 div (W - 1);
    SetColor(v, v, v);
    SR[x] := CR; SG[x] := CG; SB[x] := CB; SD[x] := DevColor;
  end;
  for y := SY(690) to H - 1 do StripRow(y);
end;

procedure Palette256;           { цвета 16..255 записываются в DAC }
var r, g, b: Integer;
begin
  Port[$3C8] := 16;
  for r := 0 to 5 do
    for g := 0 to 5 do
      for b := 0 to 5 do
      begin
        Port[$3C9] := r * 63 div 5;  { у DAC по 64 уровня на каждый цвет }
        Port[$3C9] := g * 63 div 5;
        Port[$3C9] := b * 63 div 5;
      end;
  for r := 0 to 23 do           { 232..255: серые }
  begin
    Port[$3C9] := r * 63 div 23; Port[$3C9] := r * 63 div 23; Port[$3C9] := r * 63 div 23;
  end;
end;

{ ---------- текстовые режимы ---------- }

procedure PutText(seg: Word; x, y: Integer; s: String; attr: Byte);
var i: Integer;
begin
  for i := 1 to Length(s) do
  begin
    Mem[seg:(y * 80 + x + i - 1) * 2] := Ord(s[i]);
    Mem[seg:(y * 80 + x + i - 1) * 2 + 1] := attr;
  end;
end;

procedure ShowMDA;
{ MDA: у каждой буквы байт атрибута, но цвет один - атрибут делает буквы
  ярче, подчеркивает, мигает или меняет местами букву и фон.
  В режиме 7 шрифт 8 x 14 без русских букв - только латиница. }
begin
  PutText($B000, 2, 1, 'IBM Monochrome Display Adapter, 1981: text only, 80 x 25', $0F);
  PutText($B000, 2, 3, 'Normal text            (attribute 07h)', $07);
  PutText($B000, 2, 4, 'Bright text            (attribute 0Fh)', $0F);
  PutText($B000, 2, 5, 'Underlined text        (attribute 01h)', $01);
  PutText($B000, 2, 6, 'Inverse text           (attribute 70h)', $70);
  PutText($B000, 2, 7, 'Blinking text          (attribute 87h)', $87);
  PutText($B000, 2, 9, 'Every character takes 2 bytes: the code and the attribute.', $07);
  PutText($B000, 2, 10, '80 x 25 x 2 = 4000 bytes for the whole screen.', $07);
  PutText($B000, 2, 23, 'Press any key', $70);
end;

procedure ShowText;             { CGA: 16 цветов букв на 8 цветах фона }
var fg, bg: Integer; s: String[4];
begin
  PutText($B800, 2, 1, 'CGA, цветной текст: атрибут = цвет фона (4 бита) + цвет буквы (4 бита)', $0F);
  for bg := 0 to 7 do
    for fg := 0 to 15 do
    begin
      Str(fg, s);
      if fg < 10 then s := ' ' + s;
      PutText($B800, 4 + fg * 4, 3 + bg * 2, ' ' + s + ' ', bg * 16 + fg);
    end;
  PutText($B800, 2, 20, 'Строки - 8 цветов фона, столбцы - 16 цветов букв.', $07);
  PutText($B800, 2, 21, 'На знак - 2 байта: 80 x 25 x 2 = 4000 байт; у CGA 16 КБ - 4 страницы текста.', $07);
  PutText($B800, 2, 23, 'Нажмите любую клавишу', $70);
end;

{ ---------- меню ---------- }

procedure AdapterInfo;          { что за видеоадаптер, по ответу BIOS }
var r: Registers; name: String[12]; mem: Integer; buf: array[0..511] of Byte;
begin
  name := 'CGA или MDA';
  mem := 0;
  r.ax := $1A00; Intr($10, r);  { функция 1Ah есть у MCGA и VGA }
  if r.al = $1A then
    case r.bl of
      1: name := 'MDA';
      2: name := 'CGA';
      4, 5: name := 'EGA';
      7, 8: name := 'VGA';
      $0B, $0C: name := 'MCGA';
    end
  else
  begin
    r.ah := $12; r.bl := $10; Intr($10, r);   { функция 12h/10h есть у EGA }
    if r.bl <> $10 then name := 'EGA';
  end;
  if name = 'MCGA' then mem := 64
  else if (name = 'EGA') or (name = 'VGA') then
  begin
    r.ah := $12; r.bl := $10; Intr($10, r);
    if r.bl <= 3 then mem := 64 * (r.bl + 1);
  end;
  Write('  Видеоадаптер по ответу BIOS: ', name);
  if mem > 0 then Write(', видеопамять ', mem, ' КБ');
  r.ax := $4F00; r.es := Seg(buf); r.di := Ofs(buf); Intr($10, r);
  if r.ax = $004F then Writeln(', есть VESA BIOS') else Writeln(', VESA BIOS нет');
end;

procedure Menu;
var i: Integer;
begin
  Writeln;
  Writeln('VIDEO - видеорежимы PC: одна картинка в разных режимах');
  AdapterInfo;
  Writeln;
  for i := 1 to NMODE do
    Writeln('  ', Keys[i], '  ', Title[i]);
  Writeln;
  Writeln('  Esc - выход. Видеоадаптер выбирается в настройках murm386 (Win+F11).');
  Write('Выбор: ');
end;

function Num(n: LongInt): String;   { 153600 -> '153 600' }
var s: String;
begin
  Str(n, s);
  if n >= 10000 then
  begin
    if Length(s) > 6 then Insert(' ', s, Length(s) - 5);
    Insert(' ', s, Length(s) - 2);
  end;
  Num := s;
end;

function BitWord(n: LongInt): String;   { 1 бит, 2 бита, 8 бит, 24 бита }
begin
  if (n mod 100 >= 11) and (n mod 100 <= 14) then BitWord := 'бит'
  else if (n mod 10 >= 2) and (n mod 10 <= 4) then BitWord := 'бита'
  else BitWord := 'бит';
end;

procedure About(i: Integer);    { рассказ о режиме перед картинкой }
var bits, bpp: LongInt;
begin
  Writeln;
  Writeln(Title[i]);
  case i of
    1: begin
         Writeln('IBM PC, 1981. Только текст, никаких рисунков. Монитор зеленый или янтарный.');
         Writeln('У каждого знака 2 байта: код буквы и атрибут - яркая, подчеркнутая,');
         Writeln('мигающая или инверсная.');
       end;
    2: begin
         Writeln('IBM PC, 1981. Тот же текст 80 x 25, но в цвете: 16 цветов букв, 8 цветов фона.');
         Writeln('Атрибут знака: 4 бита - цвет буквы, 4 бита - цвет фона.');
       end;
    3: begin
         Writeln('CGA, 1981. 4 цвета - это 2 бита на точку: в одном байте 4 точки.');
         Writeln('Цвета не любые: черный и одна из двух готовых палитр. Здесь палитра 1:');
         Writeln('голубой, малиновый, серый. Зеленую траву нарисовать нечем!');
       end;
    4: begin
         Writeln('CGA, 1981. 1 бит на точку: в байте 8 точек, только черный и белый.');
         Writeln('Оттенки получаются узором из точек - это называется дизеринг.');
       end;
    5: begin
         Writeln('EGA, 1984. 16 цветов - это 4 бита на точку. Память разложена на 4 слоя');
         Writeln('(плоскости): в каждом по одному биту цвета каждой точки.');
       end;
    6: begin
         Writeln('EGA, 1984. 16 цветов, каждый можно выбрать из 64. Нужна карта со 128 КБ:');
         Writeln('базовая EGA с 64 КБ показывала в этом режиме только 4 цвета.');
       end;
    7: begin
         Writeln('MCGA, 1987 (IBM PS/2 модели 30). Точки квадратные: 640 x 480 - это 4:3,');
         Writeln('как сам экран. Но цвета всего два - снова узор из точек.');
       end;
    8: begin
         Writeln('VGA, 1987 (IBM PS/2 модели 50 и старше). 16 цветов, каждый из 262 144.');
         Writeln('Это больше 128 КБ: в murm386 нужен адаптер VGA 256K.');
       end;
    9: begin
         Writeln('MCGA и VGA, 1987. Ровно 1 байт на точку - проще всего для программиста,');
         Writeln('поэтому этот режим так любили игры. 64 000 байт - как раз одно окно');
         Writeln('памяти 64 КБ. Каждый из 256 цветов выбирается из 262 144.');
       end;
    10: begin
         Writeln('SVGA, конец 1980-х: у каждой фирмы свои карты, а включать режимы');
         Writeln('договорились по стандарту VESA (1989). 256 000 байт не помещаются в окно');
         Writeln('64 КБ: программа видит память кусками ("банками") и переключает их.');
       end;
    11: begin
         Writeln('SVGA, "High Color": 16 бит на точку - 5 бит красного, 6 зеленого,');
         Writeln('5 синего. Номеров цветов больше нет: в памяти сразу яркость каждой части.');
       end;
    12: begin
         Writeln('SVGA, "True Color": 24 бита на точку - по байту на красный, зеленый и синий.');
         Writeln('Ступенек между оттенками глаз уже не видит.');
       end;
  end;
  if i <= 2 then
    Writeln('Память: ', ResW[i], ' x ', ResH[i], ' знаков x 2 байта = ', Num(Bytes[i]), ' байт.')
  else
  begin
    bits := Bytes[i] * 8;
    bpp := bits div (LongInt(ResW[i]) * ResH[i]);
    Writeln('Память: ', ResW[i], ' x ', ResH[i], ' точек x ', bpp, ' ', BitWord(bpp),
            ' = ', Num(bits), ' бит = ', Num(Bytes[i]), ' байт.');
  end;
  Writeln;
  Write('Любая клавиша - показать, Esc - назад: ');
end;

{$IFDEF DUMP}
procedure DumpScreen(i: Integer);   { только для проверки на ПК: видеопамять в файл }
var f: file; p, n: Integer; a, total: LongInt; buf: array[0..1023] of Byte; k: Word;
begin
  Assign(f, 'DUMP' + Keys[i] + '.BIN'); Rewrite(f, 1);
  case Kind of
    kCGA4, kCGA2: BlockWrite(f, Mem[$B800:0], 16384);
    kMDA: BlockWrite(f, Mem[$B000:0], 4000);
    kText: BlockWrite(f, Mem[$B800:0], 4000);
    kLin1: BlockWrite(f, Mem[$A000:0], 38400);
    kLin8: BlockWrite(f, Mem[$A000:0], 64000);
    kPlanar:
      for p := 0 to 3 do
      begin
        Port[$3CE] := 4; Port[$3CF] := p;
        BlockWrite(f, Mem[$A000:0], BPL * H);
      end;
  else
    begin
      total := LongInt(BPL) * H; a := 0;
      while a < total do
      begin
        for k := 0 to 1023 do
        begin
          SelectBank((a + k) div Gran);
          buf[k] := Mem[$A000:(a + k) mod Gran];
        end;
        BlockWrite(f, buf, 1024);
        a := a + 1024;
      end;
    end;
  end;
  Close(f);
end;
{$ENDIF}

procedure Show(i: Integer);
var t0, t: LongInt; c: Char; ok: Boolean; r: Registers;
begin
  About(i);
{$IFDEF DUMP}
  c := #13;
{$ELSE}
  c := ReadKey;
{$ENDIF}
  Writeln;
  if c = #27 then Exit;
  if not TryMode(i) then
  begin
    ok := TextMode;
    Writeln;
    Writeln('Этот видеоадаптер такой режим не умеет: BIOS его не включил.');
    Writeln('Нужно ', Num(Bytes[i]), ' байт видеопамяти. Другой адаптер выбирается');
    Writeln('в настройках murm386: Win+F11, строка Video adapter.');
    Exit;
  end;
  Kind := Kinds[i];
  W := ResW[i]; H := ResH[i];
  t0 := Ticks;
  case Kind of
    kMDA: ShowMDA;
    kText: ShowText;
  else
    begin
      if Kind = kCGA4 then
      begin                     { палитра 1: голубой, малиновый, серый (BIOS, функция 0Bh) }
        r.ah := $0B; r.bh := 1; r.bl := 1; Intr($10, r);
      end;
      if Kind = kPlanar then
      begin
        BPL := W div 8;
        Port[$3CE] := 5; Port[$3CF] := 2;    { режим записи 2: байт - это цвет }
      end;
      if (Kind = kLin8) or (Kind = kVBE8) then Palette256;
      Picture;
      if Kind = kPlanar then
      begin
        Port[$3CE] := 5; Port[$3CF] := 0;
        Port[$3CE] := 8; Port[$3CF] := $FF;
      end;
    end;
  end;
  t := Ticks - t0;
{$IFDEF DUMP}
  DumpScreen(i);
  c := #13;
{$ELSE}
  c := ReadKey;
{$ENDIF}
  ok := TextMode;
  Writeln;
  Writeln(Title[i]);
  if Kind >= kCGA4 then
    Writeln('Картинка нарисована за ', t div 18, ',', (t mod 18) * 10 div 18, ' с.');
  Writeln('Видеопамяти на экран: ', Num(Bytes[i]), ' байт.');
end;

var c: Char; i, sel: Integer; ok: Boolean;
begin
  ok := TextMode;
{$IFDEF DUMP}
  for i := 1 to NMODE do Show(i);
  Exit;
{$ENDIF}
  repeat
    Menu;
    c := UpCase(ReadKey);
    Writeln(c);
    sel := 0;
    for i := 1 to NMODE do
      if Keys[i] = c then sel := i;
    if sel > 0 then Show(sel);
  until c = #27;
  ok := TextMode;
end.
