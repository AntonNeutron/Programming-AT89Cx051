unit U_ComWake;

interface

uses
  SysUtils, Classes, CPort;

type
  TWakeCommandEvent = procedure(Sender: TObject; cmd: byte; const Data :array of byte; Count: Integer) of object;
  TState = (  WST_IDLE, //состояние ожидания
  //WST_ADD,  //прием адреса
  WST_CMD,  //прием команды
  WST_LNG,  //прием длины пакета
  WST_DATA, //прием/передача данных
  WST_CRC,  //прием/передача CRC
  WST_DONE);  //состояние готовности

  TComWake = class(TComponent)
  private
    FRxData : array of byte;
    FRxState: TState;
    FRxStuff: Boolean;
    FRxCmd  : Byte;
    FRxLen  : Byte;

    FTxData : array of byte;
   // FTxState: TState;


    FComLink: TComLink;
    FComPort: TCustomComPort;
    FMaxFrameSize: byte;
    FOnCommand: TWakeCommandEvent;
    procedure RxBuf(Sender: TObject; const Buffer; Count: Integer);
    procedure SetComPort(const Value: TCustomComPort);
    { Private declarations }
  protected
    procedure Notification(AComponent: TComponent; Operation: TOperation); override;
    procedure DoCommand(cmd: byte; const Data :array of byte; Count: Integer); dynamic;
    { Protected declarations }
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    procedure SendCommand(cmd: byte; Data :array of byte);
    procedure Reset;
    { Public declarations }
  published
    property ComPort: TCustomComPort read FComPort write SetComPort;
    property OnCommand: TWakeCommandEvent read FOnCommand write FOnCommand;
    property MaxFrameSize: byte read FMaxFrameSize write FMaxFrameSize default 255;
    { Published declarations }
  end;

procedure Register;

implementation

procedure Register;
begin
  RegisterComponents('CPortLib', [TComWake]);
end;

const
  FEND  = $C0;
  FESC  = $DB;
  TFEND = $DC;
  TFESC = $DD;

  CRC_INIT = $DE; //????????? ???????? ??????????? ?????
  CRC_FEND = $82; //????????? ???????? ??????????? ????? ? ?????? FEND


  CrcTable:array[0..255] of Byte = (

    $00, $5E, $BC, $E2, $61, $3F, $DD, $83,
    $C2, $9C, $7E, $20, $A3, $FD, $1F, $41,
    $9D, $C3, $21, $7F, $FC, $A2, $40, $1E,
    $5F, $01, $E3, $BD, $3E, $60, $82, $DC,
    $23, $7D, $9F, $C1, $42, $1C, $FE, $A0,
    $E1, $BF, $5D, $03, $80, $DE, $3C, $62,
    $BE, $E0, $02, $5C, $DF, $81, $63, $3D,
    $7C, $22, $C0, $9E, $1D, $43, $A1, $FF,
    $46, $18, $FA, $A4, $27, $79, $9B, $C5,
    $84, $DA, $38, $66, $E5, $BB, $59, $07,
    $DB, $85, $67, $39, $BA, $E4, $06, $58,
    $19, $47, $A5, $FB, $78, $26, $C4, $9A,
    $65, $3B, $D9, $87, $04, $5A, $B8, $E6,
    $A7, $F9, $1B, $45, $C6, $98, $7A, $24,
    $F8, $A6, $44, $1A, $99, $C7, $25, $7B,
    $3A, $64, $86, $D8, $5B, $05, $E7, $B9,
    $8C, $D2, $30, $6E, $ED, $B3, $51, $0F,
    $4E, $10, $F2, $AC, $2F, $71, $93, $CD,
    $11, $4F, $AD, $F3, $70, $2E, $CC, $92,
    $D3, $8D, $6F, $31, $B2, $EC, $0E, $50,
    $AF, $F1, $13, $4D, $CE, $90, $72, $2C,
    $6D, $33, $D1, $8F, $0C, $52, $B0, $EE,
    $32, $6C, $8E, $D0, $53, $0D, $EF, $B1,
    $F0, $AE, $4C, $12, $91, $CF, $2D, $73,
    $CA, $94, $76, $28, $AB, $F5, $17, $49,
    $08, $56, $B4, $EA, $69, $37, $D5, $8B,
    $57, $09, $EB, $B5, $36, $68, $8A, $D4,
    $95, $CB, $29, $77, $F4, $AA, $48, $16,
    $E9, $B7, $55, $0B, $88, $D6, $34, $6A,
    $2B, $75, $97, $C9, $4A, $14, $F6, $A8,
    $74, $2A, $C8, $96, $15, $4B, $A9, $F7,
    $B6, $E8, $0A, $54, $D7, $89, $6B, $35
  );

var
  iRxPos : integer;

{
procedure DowCRC(b: Byte; var crc:Byte);
var
  temp_crc :integer;
begin
 temp_crc:=crc + b;
 crc:=temp_crc xor (temp_crc shr 2);
end;

procedure ByteDeStuff(var Str: string; var ind:byte; var b:byte);
begin
  b := ord(Str[ind]);
  if(b = FESC) then begin
    Inc(ind);
    b := ord(Str[ind]);
    if(b = TFEND) then
      b := FEND
    else if(b = TFESC) then
      b := FESC
    else
      raise Exception.Create('WAKE destuffing error.');
  end;
  Inc(ind);
end;

}


procedure Do_Crc8(b: Byte; var crc:Byte);

//#ifdef TABLE_CRC
//  #define TABLE_READ(i) pgm_read_word(&CrcTable[i])

begin
  crc := CrcTable[crc xor b]; //????????? ??????????
end;





constructor TComWake.Create(AOwner: TComponent);
begin
  inherited Create(AOwner);
  FComLink := TComLink.Create;
  FComLink.OnRxBuf := RxBuf;
  FMaxFrameSize := 255;
  FRxStuff := False;
  FRxState := WST_IDLE;
end;

// destroy component
destructor TComWake.Destroy;
begin
  ComPort := nil;
  FRxData := nil;
  FTxData := nil;
  FComLink.Free;
  inherited Destroy;
end;

procedure TComWake.Reset;
begin
  FRxStuff := False;
  FRxState := WST_IDLE;
end;

procedure TComWake.SetComPort(const Value: TCustomComPort);
begin
  if Value <> FComPort then
  begin
    if FComPort <> nil then
      FComPort.UnRegisterLink(FComLink);
    FComPort := Value;
    if FComPort <> nil then
    begin
      FComPort.FreeNotification(Self);
      FComPort.RegisterLink(FComLink);
    end;
  end;
end;

// remove ComPort property if being destroyed
procedure TComWake.Notification(AComponent: TComponent;
  Operation: TOperation);
begin
  inherited Notification(AComponent, Operation);
  if (AComponent = FComPort) and (Operation = opRemove) then
    ComPort := nil;
end;

procedure TComWake.DoCommand(cmd: byte; const Data :array of byte; Count: Integer);
begin
  if Assigned(FOnCommand) then
    FOnCommand(Self, cmd, Data,Count);
end;

procedure TComWake.RxBuf(Sender: TObject; const Buffer; Count: Integer);
var
  Buf:AnsiString;
  i:integer;
  error_flags :TComErrors;
  data : byte;
begin

  if(FRxState = WST_DONE) then Exit;
  error_flags := FComPort.LastErrors;
  {if(error_flags ) then begin                  //если ошибка,
     RxState = WST_IDLE;   //переход к поиску FEND
     Exit;
  end;}

  SetLength(Buf, Count);
  Move(Buffer, Buf[1], Count); // Переносим в переменную

  for i := 1 to Count do begin
    data := ord(Buf[i]);                 //чтение данных
    if(data = FEND)then begin                //обнаружен FEND (из любого состояния)
     FRxState := WST_CMD;             //переход к приему адреса
     FRxStuff := False;
     Continue;
    end;

    if(data = FESC)then begin                 //принят FESC,
     FRxStuff := True;           //начало дестафинга
     Continue;
    end;

    if(FRxStuff) then begin                      //если идет дестафинг,
      if(data = TFESC)then               //если принят TFESC,
        data := FESC                 //замена его на FESC
      else if(data = TFEND) then         //если принят TFEND,
              data := FEND               //замена его на FEND
           else begin
             FRxState := WST_IDLE; //иначе ошибка стаффинга
             Continue;
           end;
      FRxStuff := false;                   //дестаффинг закончен
    end;

    case FRxState of
    WST_CMD: begin                   //прием кода команды
          FRxState := WST_LNG;           //далее - прием длины пакета
          FRxCmd   := data;
        end;
    WST_LNG: begin                   //идет прием длины пакета
          FRxState := WST_DATA;          //далее - прием данных
          if(data > FMaxFrameSize)then data := FMaxFrameSize; //ограничение длины пакета
          FRxLen := data;
          SetLength(FRxData,FRxLen);
          iRxPos := 0;
        end;
    WST_DATA:begin                   //идет прием данных
          if(iRxPos = (FRxLen))then begin          //если все данные приняты, CRC = data
            FRxState := WST_DONE;        //прием окончен
            DoCommand(FRxCmd,FRxData,FRxLen);
            FRxState := WST_IDLE;
            Continue;
          end;
          FRxData[iRxPos] := data;
          iRxPos := iRxPos + 1;
        end;
    end;

  end;


end;

procedure ByteStuff(b:byte;var Buff :array of byte; var Index:integer);
begin
  if (b = FEND) or (b = FESC) then begin
    buff[Index] := FESC;
    Index := Index + 1;
    if (b = FEND) then
      buff[Index] := TFEND
    else
      buff[Index] := TFESC;
  end else
   buff[Index] := b;
  Index := Index + 1;
end;

procedure TComWake.SendCommand(cmd: byte; Data :array of byte);
var
  crc : Byte;
  i, len, index, count : integer;
begin
  if not Assigned(FComPort) then Exit;
  if not FComPort.Connected then Exit;
  len := length(Data);
  count := FMaxFrameSize;
  SetLength(FTxData,count);
  FTxData[0] := FEND;
  index := 1;
  crc := CRC_FEND;
  ByteStuff(cmd,FTxData,index);
  Do_Crc8(cmd,crc);
  ByteStuff(len,FTxData,index);
  Do_Crc8(len,crc);
  for i := 1 to len+1 do begin
   if index >= count-2 then begin // остался свободный 1 элемент
    Inc(count,2);
    SetLength(FTxData,count);
   end;
   if i <= len then begin
    Do_Crc8(Data[i-1],crc);
    ByteStuff(Data[i-1],FTxData,index)
   end else
    ByteStuff(crc,FTxData,index)
  end;
  FComPort.Write(FTxData[0],index);
end;

end.
