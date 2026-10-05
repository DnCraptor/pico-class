{ SOUNDS.PAS - one tune on the sound devices of the PC, 1981-1992.
  The same melody ("Ode to Joy", Beethoven) plays on:
    1  PC speaker            - one square wave from the timer chip
    2  Tandy / PCjr          - SN76489 tone generator (PSG), 3 voices
    3  AdLib                 - Yamaha OPL2 FM synthesizer
    4  Covox Sound Master    - AY-3-8930 tone generator (PSG), at 240h
    5  Roland MPU-401        - General MIDI notes for a synthesizer
    6  Covox Speech Thing    - 8-bit DAC on LPT2 (278h)
    7  Disney Sound Source   - DAC with a 16-byte FIFO on LPT1 (378h)
    8  Sound Blaster         - DAC fed by DMA (220h, DMA 1)
  For the DACs (6-8) the program computes the sound wave itself, sample by
  sample, and then says a sentence from SPEECH.RAW (8000 Hz, 8 bit).
  No Crt unit: its delay loop crashes on fast machines; the timer chip is
  used for all timing instead.
  Part of the pico-class kit, see tools/sound/README.md. }

program Sounds;

{$F+}  { far calls everywhere: the note procedures are passed as parameters }

uses Dos;

const
  PIT_HZ   = 1193182;
  RATE     = 8000;              { DAC sample rate for Covox and Sound Blaster }
  DSS_RATE = 7000;              { the Disney Sound Source plays at a fixed 7 kHz }
  QUARTER  = 220;               { milliseconds }
  NNOTES   = 30;
  { the melody: MIDI note numbers (60 = C4) and lengths in eighths }
  MelNote: array[1..NNOTES] of Byte = (
    64,64,65,67, 67,65,64,62, 60,60,62,64, 64,62,62,
    64,64,65,67, 67,65,64,62, 60,60,62,64, 62,60,60);
  MelLen: array[1..NNOTES] of Byte = (
    2,2,2,2, 2,2,2,2, 2,2,2,2, 3,1,4,
    2,2,2,2, 2,2,2,2, 2,2,2,2, 3,1,4);
  { the bass: one note a bar (8 eighths) }
  NBARS = 8;
  BassNote: array[1..NBARS] of Byte = (48, 43, 48, 43, 48, 43, 48, 48);

type
  TBuf = array[0..65000] of Byte;
  PBuf = ^TBuf;

var
  Wave: PBuf;                   { computed melody for the DACs }
  WaveLen: Word;
  Speech: PBuf;
  SpeechLen: Word;
  SinTab: array[0..255] of ShortInt;
  Stop: Boolean;

{ ---------- timer, keyboard ---------- }

function PitCount: Word;        { channel 0 counter, counts down at 1.19 MHz }
var lo, hi: Byte;
begin
  Port[$43] := $00;             { latch channel 0 }
  lo := Port[$40];
  hi := Port[$40];
  PitCount := lo or (Word(hi) shl 8);
end;

procedure PitMode2;
{ Channel 0 in mode 2 counts down by 1 per tick (the BIOS uses mode 3,
  which counts by 2); the reload value stays 65536, 18.2 interrupts/s. }
begin
  Port[$43] := $34; Port[$40] := 0; Port[$40] := 0;
end;

procedure PitMode3;
begin
  Port[$43] := $36; Port[$40] := 0; Port[$40] := 0;
end;

function KeyPressed: Boolean;
var r: Registers;
begin
  r.ah := 1; Intr($16, r);
  KeyPressed := (r.flags and FZero) = 0;
end;

function ReadKey: Char;
var r: Registers;
begin
  r.ah := 0; Intr($16, r);
  ReadKey := Chr(r.al);
end;

procedure WaitTicks(ticks: LongInt);   { wait this many 1.19 MHz ticks }
var last, now: Word;
begin
  last := PitCount;
  while ticks > 0 do
  begin
    now := PitCount;
    ticks := ticks - Word(last - now);
    last := now;
  end;
end;

procedure WaitMs(ms: Word);
begin
  if KeyPressed then if ReadKey = #27 then Stop := True;
  if not Stop then WaitTicks(LongInt(ms) * 1193);
end;

function Freq(note: Byte): Real;       { MIDI note -> Hz, A4 = 440 }
begin
  Freq := 440.0 * Exp((note - 69) * Ln(2.0) / 12.0);
end;

{ ---------- 1. PC speaker ---------- }

procedure SpeakerOn(f: Real);
var d: Word;
begin
  d := Round(PIT_HZ / f);
  Port[$43] := $B6;             { channel 2, square wave }
  Port[$42] := Lo(d); Port[$42] := Hi(d);
  Port[$61] := Port[$61] or 3;  { gate the timer to the speaker }
end;

procedure SpeakerOff;
begin
  Port[$61] := Port[$61] and $FC;
end;

procedure PlaySpeaker;
var i: Integer;
begin
  i := 1;
  while (i <= NNOTES) and not Stop do
  begin
    SpeakerOn(Freq(MelNote[i]));
    WaitMs(MelLen[i] * QUARTER div 2 - 20);
    SpeakerOff;
    WaitMs(20);
    Inc(i);
  end;
  SpeakerOff;
end;

{ ---------- chip players: melody + bass on two voices ---------- }

type
  TNoteProc = procedure(voice: Byte; note: Byte);   { note 0 = silence }

procedure PlayChip(play: TNoteProc);
{ walks the melody in eighths and starts the bass at every bar }
var i, k, eighth: Integer;
begin
  i := 1; eighth := 0;
  while (i <= NNOTES) and not Stop do
  begin
    if eighth mod 8 = 0 then play(1, BassNote[eighth div 8 + 1]);
    play(0, MelNote[i]);
    for k := 1 to MelLen[i] do
    begin
      if (k > 1) and (eighth mod 8 = 0) then play(1, BassNote[eighth div 8 + 1]);
      if k = MelLen[i] then
      begin
        WaitMs(QUARTER div 2 - 25); play(0, 0); WaitMs(25);
      end
      else WaitMs(QUARTER div 2);
      Inc(eighth);
    end;
    Inc(i);
  end;
  play(0, 0); play(1, 0);
end;

{ ---------- 2. Tandy / PCjr: SN76489 at 0C0h ---------- }

procedure TandyNote(voice: Byte; note: Byte);
var n: Word;
begin
  if note = 0 then
    Port[$C0] := $90 or (voice shl 5) or $0F       { volume off }
  else
  begin
    n := Round(3579545.0 / (32.0 * Freq(note)));   { 10-bit period }
    Port[$C0] := $80 or (voice shl 5) or (n and $0F);
    Port[$C0] := n shr 4;
    Port[$C0] := $90 or (voice shl 5) or (voice * 3);  { bass a bit softer }
  end;
end;

procedure PlayTandy;
var v: Byte;
begin
  for v := 0 to 3 do Port[$C0] := $9F or (v shl 5);  { all four voices off }
  PlayChip(TandyNote);
end;

{ ---------- 3. AdLib: OPL2 at 388h ---------- }

procedure OplWrite(reg, val: Byte);
var i: Integer; b: Byte;
begin
  Port[$388] := reg;
  for i := 1 to 6 do b := Port[$388];
  Port[$389] := val;
  for i := 1 to 35 do b := Port[$388];
end;

const
  { operator offsets of channels 0 and 1: modulator, carrier }
  OplMod: array[0..1] of Byte = ($00, $01);
  OplCar: array[0..1] of Byte = ($03, $04);

procedure OplSetup;
var r: Integer; ch: Byte;
begin
  for r := $01 to $F5 do OplWrite(r, 0);
  OplWrite($01, $20);           { allow waveform select }
  for ch := 0 to 1 do
  begin
    { a soft electric-piano-like sound: two operators, modulator feeds the carrier }
    OplWrite($20 + OplMod[ch], $01);  OplWrite($20 + OplCar[ch], $01);
    OplWrite($40 + OplMod[ch], $10);  OplWrite($40 + OplCar[ch], $00);
    OplWrite($60 + OplMod[ch], $F2);  OplWrite($60 + OplCar[ch], $F3);
    OplWrite($80 + OplMod[ch], $77);  OplWrite($80 + OplCar[ch], $75);
    OplWrite($E0 + OplMod[ch], $00);  OplWrite($E0 + OplCar[ch], $00);
    OplWrite($C0 + ch, $06);          { feedback 3, FM }
  end;
end;

type
  TPatch = record
    name: String[12];
    m20, m40, m60, m80, mE0, c20, c40, c60, c80, cE0, fbC0: Byte;
  end;
const
  NPATCH = 4;
  { the same two operators, set four different ways: the modulator's
    loudness (40h) and frequency multiple (20h) decide how many overtones
    the carrier gets - that is the whole trick of FM synthesis }
  Patches: array[1..NPATCH] of TPatch = (
    (name: 'flute';     m20: $21; m40: $3F; m60: $F0; m80: $F0; mE0: 0;
                        c20: $21; c40: $00; c60: $A1; c80: $15; cE0: 0; fbC0: $00),
    (name: 'piano';     m20: $01; m40: $10; m60: $F2; m80: $77; mE0: 0;
                        c20: $01; c40: $00; c60: $F3; c80: $75; cE0: 0; fbC0: $06),
    (name: 'trumpet';   m20: $21; m40: $16; m60: $71; m80: $AE; mE0: 0;
                        c20: $21; c40: $00; c60: $81; c80: $9E; cE0: 0; fbC0: $0E),
    (name: 'bell';      m20: $05; m40: $14; m60: $F2; m80: $13; mE0: 0;
                        c20: $01; c40: $00; c60: $F2; c80: $14; cE0: 0; fbC0: $00));

procedure OplPatch(ch: Byte; var pt: TPatch);
begin
  OplWrite($20 + OplMod[ch], pt.m20);  OplWrite($20 + OplCar[ch], pt.c20);
  OplWrite($40 + OplMod[ch], pt.m40);  OplWrite($40 + OplCar[ch], pt.c40);
  OplWrite($60 + OplMod[ch], pt.m60);  OplWrite($60 + OplCar[ch], pt.c60);
  OplWrite($80 + OplMod[ch], pt.m80);  OplWrite($80 + OplCar[ch], pt.c80);
  OplWrite($E0 + OplMod[ch], pt.mE0);  OplWrite($E0 + OplCar[ch], pt.cE0);
  OplWrite($C0 + ch, pt.fbC0);
end;

procedure AdlibNote(voice: Byte; note: Byte);
var block: Byte; f: Real; fnum: Word;
begin
  OplWrite($B0 + voice, 0);     { key off }
  if note = 0 then Exit;
  block := note div 12 - 1;     { octave }
  if block > 7 then block := 7;
  f := Freq(note);
  fnum := Round(f * 1048576.0 / 49716.0 / (1 shl block));
  OplWrite($A0 + voice, Lo(fnum));
  OplWrite($B0 + voice, $20 or (block shl 2) or (Hi(fnum) and 3));   { key on }
end;

procedure PlayAdlib;
begin
  OplSetup;
  PlayChip(AdlibNote);
  OplWrite($B0, 0); OplWrite($B1, 0);
end;

procedure PlayAdlibPatches;
{ A: the first bar of the tune with four FM instruments, one after another }
const Scale: array[1..8] of Byte = (64, 64, 65, 67, 67, 65, 64, 62);
var p, i: Integer;
begin
  OplSetup;
  p := 1;
  while (p <= NPATCH) and not Stop do
  begin
    Writeln('  ', Patches[p].name);
    OplPatch(0, Patches[p]);
    i := 1;
    while (i <= 8) and not Stop do
    begin
      AdlibNote(0, Scale[i]);
      WaitMs(QUARTER);
      Inc(i);
    end;
    AdlibNote(0, 0);
    WaitMs(400);
    Inc(p);
  end;
  OplWrite($B0, 0);
end;

{ ---------- 4. Covox Sound Master: AY-3-8930 at 240h ---------- }

const SM_BASE = $240;

procedure AyWrite(reg, val: Byte);
begin
  Port[SM_BASE] := reg;         { select a register }
  Port[SM_BASE + 1] := val;     { write it }
end;

procedure SoundMasterNote(voice: Byte; note: Byte);
var n: Word;
begin
  if note = 0 then
    AyWrite(8 + voice, 0)       { volume 0 }
  else
  begin
    n := Round(1773400.0 / (16.0 * Freq(note)));   { 12-bit period }
    AyWrite(voice * 2, Lo(n));
    AyWrite(voice * 2 + 1, Hi(n) and $0F);
    AyWrite(8 + voice, 15 - voice * 3);
  end;
end;

procedure PlaySoundMaster;
begin
  AyWrite(7, $3C);              { tone on voices A and B; C and noise off }
  PlayChip(SoundMasterNote);
  AyWrite(8, 0); AyWrite(9, 0); AyWrite(10, 0);
end;

{ ---------- 5. Roland MPU-401: MIDI at 330h ---------- }

procedure MidiOut(b: Byte);
var t: Word;
begin
  t := 0;
  while ((Port[$331] and $40) <> 0) and (t < 60000) do Inc(t);
  Port[$330] := b;
end;

var MidiLast: array[0..1] of Byte;

procedure MidiNote(voice: Byte; note: Byte);
begin
  if MidiLast[voice] <> 0 then
  begin
    MidiOut($80 + voice); MidiOut(MidiLast[voice]); MidiOut(0);   { note off }
  end;
  MidiLast[voice] := note;
  if note <> 0 then
  begin
    MidiOut($90 + voice); MidiOut(note); MidiOut(100 - voice * 20);  { note on }
  end;
end;

procedure PlayMidi;
var t: Word;
begin
  Port[$331] := $FF;            { reset }
  for t := 1 to 30000 do ;
  Port[$331] := $3F;            { UART mode: bytes go straight to MIDI }
  for t := 1 to 30000 do ;
  MidiOut($C0); MidiOut(0);     { channel 1: General MIDI program 1, piano }
  MidiOut($C1); MidiOut(32);    { channel 2: program 33, acoustic bass }
  MidiLast[0] := 0; MidiLast[1] := 0;
  PlayChip(MidiNote);
end;

{ ---------- the computed wave for the DACs ---------- }

procedure MakeWave(rate: Word);
{ Two voices: the melody as a sine with a fading "piano" envelope, the bass
  as a quieter square wave. Every sample is 128 + the sum of the voices. }
var i: Integer;
    n, len, bar: Word;
    ph1, inc1, ph2, inc2: LongInt;
    amp, v: Integer;
    ampFix, ampDec: Word;
    eighthLen: Word;
begin
  eighthLen := LongInt(rate) * QUARTER div 2000;
  WaveLen := 0;
  ph1 := 0; ph2 := 0; inc2 := 0;
  for i := 1 to NNOTES do
  begin
    len := MelLen[i] * eighthLen;
    inc1 := Round(Freq(MelNote[i]) * 256.0 * 256.0 / rate);   { 8.8 fixed }
    ampFix := 70 * 256;                     { 8.8 fixed: fades from 70 to 15 }
    ampDec := 55 * 256 div len;
    for n := 0 to len - 1 do
    begin
      if (WaveLen mod (8 * eighthLen)) = 0 then
      begin
        bar := WaveLen div (8 * eighthLen) + 1;
        if bar > NBARS then bar := NBARS;
        inc2 := Round(Freq(BassNote[bar]) * 256.0 * 256.0 / rate);
      end;
      amp := ampFix shr 8;
      ampFix := ampFix - ampDec;
      if n > len - rate div 50 then amp := 0;    { short gap before the next note }
      v := SinTab[(ph1 shr 8) and 255] * amp div 128;
      if (ph2 shr 15) and 1 = 0 then v := v + 18 else v := v - 18;
      Wave^[WaveLen] := 128 + v;
      Inc(WaveLen);
      ph1 := ph1 + inc1;
      ph2 := ph2 + inc2;
    end;
  end;
end;

{ ---------- 6. Covox Speech Thing: DAC on LPT2 (278h) ---------- }

procedure CovoxPlay(buf: PBuf; len: Word);
var i: Word; last, now: Word;
const STEP = PIT_HZ div RATE;
begin
  last := PitCount;
  i := 0;
  while (i < len) and not Stop do
  begin
    Port[$278] := buf^[i];
    repeat
      now := PitCount;
    until Word(last - now) >= STEP;
    last := last - STEP;
    if (i and 1023) = 0 then
      if KeyPressed then if ReadKey = #27 then Stop := True;
    Inc(i);
  end;
  Port[$278] := 128;
end;

procedure PlayCovox;
begin
  Writeln('  computing the wave...');
  MakeWave(RATE);
  Writeln('  playing: the computer sends 8000 numbers a second to the DAC');
  CovoxPlay(Wave, WaveLen);
  if not Stop then
  begin
    WaitMs(400);
    CovoxPlay(Speech, SpeechLen);
  end;
end;

{ ---------- 7. Disney Sound Source: DAC with FIFO on LPT1 (378h) ---------- }

procedure DssPlay(buf: PBuf; len: Word; step: Word);
{ step: source samples per output sample in 8.8 fixed point }
var src: LongInt; t: Word;
begin
  src := 0;
  Port[$37A] := $04;
  while (src shr 8) < len do
  begin
    t := 0;
    while ((Port[$379] and $40) <> 0) and (t < 50000) do Inc(t);   { FIFO full }
    Port[$378] := buf^[src shr 8];
    Port[$37A] := $0C;          { strobe (select line): the byte goes into the FIFO }
    Port[$37A] := $04;
    src := src + step;
    if (src and $FFFF) < step then
      if KeyPressed then if ReadKey = #27 then begin Stop := True; Exit; end;
  end;
end;

procedure PlayDisney;
begin
  Writeln('  computing the wave...');
  MakeWave(DSS_RATE);
  Writeln('  playing: the Sound Source takes the numbers at its own 7000 a second');
  DssPlay(Wave, WaveLen, 256);
  if not Stop then
  begin
    WaitMs(400);
    DssPlay(Speech, SpeechLen, 256 * RATE div DSS_RATE);   { 8000 -> 7000 }
  end;
end;

{ ---------- 8. Sound Blaster: DAC + DMA at 220h ---------- }

const SB = $220;
var DmaBuf: PBuf;               { 64000 bytes: holds a DMA-safe 32000-byte window }
    DmaOfs: Word;

procedure DspWrite(b: Byte);
var t: Word;
begin
  t := 0;
  while ((Port[SB + $C] and $80) <> 0) and (t < 60000) do Inc(t);
  Port[SB + $C] := b;
end;

function DspReset: Boolean;
var t: Word;
begin
  Port[SB + 6] := 1;
  for t := 1 to 1000 do ;
  Port[SB + 6] := 0;
  t := 0;
  while ((Port[SB + $E] and $80) = 0) and (t < 60000) do Inc(t);
  DspReset := Port[SB + $A] = $AA;
end;

procedure FindDmaWindow;
{ A DMA transfer must not cross a 64 KB boundary of physical memory. }
var phys, edge: LongInt;
begin
  phys := LongInt(Seg(DmaBuf^)) * 16 + Ofs(DmaBuf^);
  edge := (phys or $FFFF) + 1;  { next 64 KB boundary }
  if edge - phys >= 32000 then DmaOfs := 0
  else DmaOfs := edge - phys;
end;

procedure SbPlayChunk(src: PBuf; from, len: Word);
var phys: LongInt; i: Word;
begin
  for i := 0 to len - 1 do DmaBuf^[DmaOfs + i] := src^[from + i];
  phys := LongInt(Seg(DmaBuf^)) * 16 + Ofs(DmaBuf^) + DmaOfs;
  { the DMA controller, channel 1: memory -> Sound Blaster, single pass }
  Port[$0A] := $05;                       { mask channel 1 }
  Port[$0C] := 0;                         { reset the byte flip-flop }
  Port[$0B] := $49;                       { single, read memory, channel 1 }
  Port[$02] := Lo(Word(phys)); Port[$02] := Hi(Word(phys));
  Port[$83] := phys shr 16;               { page }
  Port[$03] := Lo(len - 1); Port[$03] := Hi(len - 1);
  Port[$0A] := $01;                       { unmask channel 1 }
  { the Sound Blaster: 8-bit single-cycle DMA output }
  DspWrite($14); DspWrite(Lo(len - 1)); DspWrite(Hi(len - 1));
  { wait as long as the chunk plays, then acknowledge the interrupt }
  for i := 1 to len div 80 do
    if not Stop then WaitTicks(PIT_HZ div 100);
  i := Port[SB + $E];
end;

procedure SbPlay(src: PBuf; len: Word);
var from, n: Word;
begin
  from := 0;
  while (from < len) and not Stop do
  begin
    n := len - from;
    if n > 28160 then n := 28160;         { half the melody: 4 bars }
    SbPlayChunk(src, from, n);
    from := from + n;
  end;
end;

procedure PlaySoundBlaster;
begin
  if not DspReset then
  begin
    Writeln('  no Sound Blaster at 220h');
    Exit;
  end;
  DspWrite($D1);                          { speaker on }
  DspWrite($40); DspWrite(256 - 1000000 div RATE);   { time constant: 8000 Hz }
  Writeln('  computing the wave...');
  MakeWave(RATE);
  Writeln('  playing: the DMA controller feeds the DAC, the processor is free');
  SbPlay(Wave, WaveLen);
  if not Stop then
  begin
    WaitMs(400);
    SbPlay(Speech, SpeechLen);
  end;
  DspWrite($D3);                          { speaker off }
end;

{ ---------- menu ---------- }

procedure LoadSpeech;
var f: file; n: Word;
begin
  SpeechLen := 0;
  Assign(f, 'SPEECH.RAW');
  {$I-} Reset(f, 1); {$I+}
  if IOResult <> 0 then Exit;
  BlockRead(f, Speech^, 32000, n);
  Close(f);
  SpeechLen := n;
end;

procedure Menu;
begin
  Writeln;
  Writeln('SOUNDS - one tune on the sound devices of the PC');
  Writeln;
  Writeln('  1  PC speaker (1981)            one square wave, one voice');
  Writeln('  2  Tandy / PCjr (1984)          PSG SN76489: square waves, 3 voices');
  Writeln('  3  AdLib (1987)                 OPL2 FM synthesis: 9 voices');
  Writeln('  4  Covox Sound Master (1989)    PSG AY-3-8930, port 240h');
  Writeln('  5  Roland MPU-401 (1984)        MIDI notes for a synthesizer');
  Writeln('  6  Covox Speech Thing (1986)    8-bit DAC on LPT2: any wave');
  Writeln('  7  Disney Sound Source (1990)   DAC with a buffer on LPT1');
  Writeln('  8  Sound Blaster (1989)         DAC fed by DMA, plus OPL2');
  Writeln('  A  AdLib: four FM instruments   flute, piano, trumpet, bell');
  Writeln;
  Writeln('  Esc - stop / quit.  Turn the device on in the emulator settings first.');
  Writeln('  (SOUNDS 3 plays device 3 and quits.)');
  Write('Your choice: ');
end;

procedure PlayDevice(c: Char);
begin
  Stop := False;
  case c of
    '1': PlaySpeaker;
    '2': PlayTandy;
    '3': PlayAdlib;
    '4': PlaySoundMaster;
    '5': PlayMidi;
    '6': PlayCovox;
    '7': PlayDisney;
    '8': PlaySoundBlaster;
    'a', 'A': PlayAdlibPatches;
  end;
end;

var c: Char; i: Integer; Arg: String;
begin
  for i := 0 to 255 do SinTab[i] := Round(127 * Sin(i * 2 * Pi / 256));
  GetMem(Wave, 65000);
  GetMem(Speech, 32000);
  GetMem(DmaBuf, 64000);
  FindDmaWindow;
  LoadSpeech;
  PitMode2;
  if ParamCount > 0 then
  begin
    Arg := ParamStr(1);
    PlayDevice(Arg[1]);         { SOUNDS 3 - play one device and quit }
  end
  else
    repeat
      Menu;
      c := ReadKey;
      Writeln(c);
      PlayDevice(c);
    until c = #27;
  SpeakerOff;
  PitMode3;
end.
