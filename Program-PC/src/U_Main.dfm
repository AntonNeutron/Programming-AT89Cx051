object Form1: TForm1
  Left = 1687
  Top = 198
  Width = 640
  Height = 581
  Color = clBtnFace
  Constraints.MaxWidth = 640
  Constraints.MinHeight = 500
  Constraints.MinWidth = 640
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  OnClose = FormClose
  OnCreate = FormCreate
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 632
    Height = 89
    Align = alTop
    TabOrder = 0
    object btn5: TSpeedButton
      Left = 224
      Top = 48
      Width = 129
      Height = 33
      Action = act_LockBit
    end
    object btn_Connected: TBitBtn
      Left = 8
      Top = 8
      Width = 137
      Height = 73
      Caption = #1055#1086#1076#1082#1083#1102#1095#1080#1090#1100#1089#1103
      TabOrder = 0
      OnClick = btn_ConnectedClick
    end
    object btn2: TBitBtn
      Left = 152
      Top = 8
      Width = 65
      Height = 33
      Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1080
      TabOrder = 1
      OnClick = btn2Click
    end
    object btn1: TBitBtn
      Left = 224
      Top = 8
      Width = 129
      Height = 33
      Action = act_Signatura
      Caption = #1057#1080#1075#1085#1072#1090#1091#1088#1072' '#1095#1080#1087#1072
      TabOrder = 2
    end
    object btnSignatura: TBitBtn
      Left = 496
      Top = 48
      Width = 121
      Height = 33
      Action = act_Erase
      Caption = #1054#1095#1080#1089#1090#1082#1072' '#1095#1080#1087#1072
      TabOrder = 3
    end
    object btnErase: TBitBtn
      Left = 360
      Top = 8
      Width = 129
      Height = 33
      Action = act_ChipRead
      Caption = #1063#1090#1077#1085#1080#1077' '#1095#1080#1087#1072
      TabOrder = 4
    end
    object btnChipVerif: TBitBtn
      Left = 496
      Top = 8
      Width = 121
      Height = 33
      Action = act_ChipVerif
      Caption = #1042#1077#1088#1080#1092#1080#1082#1072#1094#1080#1103
      TabOrder = 5
    end
  end
  object mmo_log: TMemo
    Left = 0
    Top = 433
    Width = 632
    Height = 102
    Align = alBottom
    Color = clBtnFace
    ReadOnly = True
    TabOrder = 1
  end
  object btnErase1: TBitBtn
    Left = 360
    Top = 48
    Width = 129
    Height = 33
    Action = act_ChipWrite
    Caption = #1057#1090#1077#1088#1077#1090#1100' '#1080' '#1079#1072#1087#1080#1089#1072#1090#1100
    TabOrder = 2
  end
  object sGrid_MemFileGrid_ChipMem: TStringGrid
    Left = 0
    Top = 89
    Width = 433
    Height = 344
    Align = alLeft
    ColCount = 17
    DefaultColWidth = 20
    DefaultRowHeight = 20
    RowCount = 2
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 3
    ColWidths = (
      53
      20
      20
      20
      20
      20
      20
      20
      20
      20
      20
      20
      20
      20
      20
      20
      20)
  end
  object stat1: TStatusBar
    Left = 0
    Top = 535
    Width = 632
    Height = 19
    Panels = <
      item
        Width = 300
      end
      item
        Style = psOwnerDraw
        Width = 50
      end>
  end
  object pb1: TProgressBar
    Left = 120
    Top = 408
    Width = 300
    Height = 17
    TabOrder = 5
  end
  object btn3: TBitBtn
    Left = 440
    Top = 96
    Width = 129
    Height = 33
    Caption = #1054#1090#1082#1088#1099#1090#1100' '#1087#1088#1086#1096#1080#1074#1082#1091
    TabOrder = 6
    OnClick = btn3Click
  end
  object btn4: TBitBtn
    Left = 440
    Top = 136
    Width = 129
    Height = 33
    Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100' '#1074' '#1092#1072#1081#1083
    TabOrder = 7
    OnClick = btn4Click
  end
  object ComPort: TComPort
    BaudRate = br19200
    Port = 'COM13'
    Parity.Bits = prNone
    StopBits = sbOneStopBit
    DataBits = dbEight
    Events = [evRxChar, evTxEmpty, evRxFlag, evRing, evBreak, evCTS, evDSR, evError, evRLSD, evRx80Full]
    FlowControl.OutCTSFlow = False
    FlowControl.OutDSRFlow = False
    FlowControl.ControlDTR = dtrDisable
    FlowControl.ControlRTS = rtsDisable
    FlowControl.XonXoffOut = False
    FlowControl.XonXoffIn = False
    StoredProps = [spBasic]
    TriggersOnRxChar = False
    OnAfterOpen = ComPortAfterOpen
    OnAfterClose = ComPortAfterClose
    OnBeforeClose = ComPortBeforeClose
    Left = 112
    Top = 488
  end
  object ComPKG: TComWake
    ComPort = ComPort
    OnCommand = ComPKGCommand
    Left = 64
    Top = 488
  end
  object frmstrg1: TFormStorage
    IniSection = 'Programmator'
    UseRegistry = False
    StoredProps.Strings = (
      'ComPort.Port'
      'ComPort.BaudRate')
    StoredValues = <>
    Left = 16
    Top = 488
  end
  object tmr: TTimer
    Enabled = False
    Interval = 500
    OnTimer = tmrTimer
    Left = 112
    Top = 440
  end
  object actlst1: TActionList
    Left = 488
    Top = 200
    object act_Signatura: TAction
      Caption = #1057#1080#1075#1085#1072#1090#1091#1088#1072' '#1095#1080#1087#1072
      OnExecute = act_SignaturaExecute
      OnUpdate = act_SignaturaUpdate
    end
    object act_Erase: TAction
      Caption = #1054#1095#1080#1089#1090#1082#1072' '#1095#1080#1087#1072
      OnExecute = act_EraseExecute
      OnUpdate = act_EraseUpdate
    end
    object act_ChipRead: TAction
      Caption = #1063#1090#1077#1085#1080#1077' '#1095#1080#1087#1072
      OnExecute = act_ChipReadExecute
      OnUpdate = act_EraseUpdate
    end
    object act_ChipVerif: TAction
      Caption = #1042#1077#1088#1080#1092#1080#1082#1072#1094#1080#1103
      OnExecute = act_ChipVerifExecute
      OnUpdate = act_EraseUpdate
    end
    object act_ChipWrite: TAction
      Caption = #1057#1090#1077#1088#1077#1090#1100' '#1080' '#1079#1072#1087#1080#1089#1072#1090#1100
      OnExecute = act_ChipWriteExecute
      OnUpdate = act_EraseUpdate
    end
    object act_LockBit: TAction
      Caption = #1041#1080#1090#1099' '#1073#1083#1086#1082#1080#1088#1086#1074#1082#1080
      OnExecute = act_LockBitExecute
      OnUpdate = act_EraseUpdate
    end
  end
  object dlgOpen: TOpenDialog
    DefaultExt = 'hex'
    Filter = 'Hex files (*.hex)|*.hex'
    Left = 16
    Top = 440
  end
  object dlgSave1: TSaveDialog
    DefaultExt = 'hex'
    Filter = 'Hex files (*.hex)|*.hex'
    Options = [ofOverwritePrompt, ofHideReadOnly, ofEnableSizing]
    Left = 64
    Top = 440
  end
end
