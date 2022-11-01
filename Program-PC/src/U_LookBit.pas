unit U_LookBit;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, Buttons, ExtCtrls;

type
  TF_LookBit = class(TForm)
    rg_LookBit: TRadioGroup;
    btn1: TBitBtn;
    btn2: TBitBtn;
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  F_LookBit: TF_LookBit;

implementation

{$R *.dfm}

end.
