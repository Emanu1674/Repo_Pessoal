object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'Ds_test'
  ClientHeight = 222
  ClientWidth = 384
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 96
    Top = 24
    Width = 138
    Height = 29
    Caption = 'XM PLAYER'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -24
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label2: TLabel
    Left = 128
    Top = 187
    Width = 132
    Height = 13
    Caption = 'mailto:sfengtfw@gmail.com'
  end
  object Button1: TButton
    Left = 96
    Top = 127
    Width = 75
    Height = 25
    Caption = 'Play!'
    TabOrder = 0
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 191
    Top = 127
    Width = 75
    Height = 25
    Caption = 'Stop'
    TabOrder = 1
    OnClick = Button2Click
  end
  object Button3: TButton
    Left = 280
    Top = 77
    Width = 75
    Height = 31
    Caption = 'Browse'
    TabOrder = 2
    OnClick = Button3Click
  end
  object Edit1: TEdit
    Left = 32
    Top = 82
    Width = 234
    Height = 21
    TabOrder = 3
    Text = 'XM filename...'
    OnClick = Edit1Click
  end
end
