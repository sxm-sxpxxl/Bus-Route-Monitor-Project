object MainForm: TMainForm
  Left = 0
  Top = 0
  BorderStyle = bsSingle
  Caption = 'Public transport monitoring system'
  ClientHeight = 600
  ClientWidth = 1056
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object Label3: TLabel
    Left = 639
    Top = 7
    Width = 407
    Height = 19
    Alignment = taCenter
    AutoSize = False
    Caption = 'Information for the selected bus stop'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'GOST type B'
    Font.Style = []
    ParentFont = False
  end
  object Label4: TLabel
    Left = 639
    Top = 127
    Width = 138
    Height = 19
    Alignment = taCenter
    AutoSize = False
    Caption = 'The selected bus'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'GOST type B'
    Font.Style = []
    ParentFont = False
  end
  object Label5: TLabel
    Left = 639
    Top = 160
    Width = 407
    Height = 19
    Alignment = taCenter
    AutoSize = False
    Caption = 'Information for the selected bus'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'GOST type B'
    Font.Style = []
    ParentFont = False
  end
  object MainChart: TChart
    Left = 0
    Top = 0
    Width = 633
    Height = 593
    Legend.Color = clGradientActiveCaption
    Legend.Visible = False
    PrintProportional = False
    Title.Font.Color = clBlack
    Title.Font.Height = -19
    Title.Font.Name = 'GOST type B'
    Title.Font.Style = [fsBold]
    Title.Text.Strings = (
      'The chart of a route of public transport')
    BottomAxis.Visible = False
    LeftAxis.LogarithmicBase = 2.718281828459050000
    LeftAxis.Visible = False
    RightAxis.Automatic = False
    RightAxis.AutomaticMaximum = False
    RightAxis.AutomaticMinimum = False
    RightAxis.Visible = False
    TopAxis.Automatic = False
    TopAxis.AutomaticMaximum = False
    TopAxis.AutomaticMinimum = False
    TopAxis.Visible = False
    View3D = False
    UseDockManager = False
    TabOrder = 0
    OnDblClick = ResetSelectedPoint_MainChartDblClick
    DefaultCanvas = 'TGDIPlusCanvas'
    PrintMargins = (
      15
      18
      15
      18)
    ColorPaletteIndex = 13
    object RouteSeries: TLineSeries
      Brush.BackColor = clDefault
      ClickableLine = False
      Pointer.InflateMargins = True
      Pointer.Style = psRectangle
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Y'
      YValues.Order = loNone
      Data = {
        000A000000000000000000F03F00000000000014400000000000001840000000
        0000000840000000000000104000000000000018400000000000001440000000
        00000024400000000000002040000000000000F03F}
      Detail = {0000000000}
    end
    object BusStopSeries: TPointSeries
      Legend.Text = '1'
      Legend.Visible = False
      LegendTitle = '1'
      Marks.Children = <
        item
          Shape.ShapeStyle = fosRectangle
          Shape.Visible = False
          Shape.TextFormat = ttfHtml
          Shape.Style = smsValue
        end>
      Marks.Frame.Visible = False
      Marks.Callout.Length = 20
      DataSource = RouteSeries
      SeriesColor = clLime
      ShowInLegend = False
      ClickableLine = False
      Pointer.HorizSize = 5
      Pointer.InflateMargins = True
      Pointer.Style = psRectangle
      Pointer.VertSize = 5
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Y'
      YValues.Order = loNone
      OnClickPointer = BusStopSeriesClickPointer
    end
  end
  object BusStopInfoMemo: TMemo
    Left = 639
    Top = 32
    Width = 407
    Height = 89
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'GOST type B'
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    TabOrder = 1
  end
  object BusInfoMemo: TMemo
    Left = 639
    Top = 185
    Width = 407
    Height = 160
    Enabled = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'GOST type B'
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    TabOrder = 2
  end
  object BusesListComboBox: TComboBox
    Left = 783
    Top = 127
    Width = 263
    Height = 26
    Enabled = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'GOST type B'
    Font.Style = []
    ParentFont = False
    TabOrder = 3
    OnChange = BusesListComboBoxChange
  end
  object GroupBox1: TGroupBox
    Left = 639
    Top = 416
    Width = 409
    Height = 176
    TabOrder = 4
    object Label1: TLabel
      Left = 188
      Top = 13
      Width = 218
      Height = 19
      Alignment = taCenter
      AutoSize = False
      Caption = 'Number of simulated buses'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'GOST type B'
      Font.Style = []
      ParentFont = False
    end
    object TimeLabel: TLabel
      Left = 62
      Top = 78
      Width = 47
      Height = 22
      Caption = '0 sec'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'GOST type B'
      Font.Style = []
      ParentFont = False
    end
    object Label2: TLabel
      Left = 67
      Top = 51
      Width = 47
      Height = 22
      Caption = 'Time:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'GOST type B'
      Font.Style = []
      ParentFont = False
    end
    object StartButton: TButton
      Left = 188
      Top = 71
      Width = 218
      Height = 41
      Caption = 'START'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'GOST type B'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      OnClick = StartButtonClick
    end
    object StopButton: TButton
      Left = 188
      Top = 118
      Width = 218
      Height = 41
      Caption = 'STOP'
      Enabled = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'GOST type B'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
      OnClick = StopButtonClick
    end
    object CountOfBusesEdit: TEdit
      Left = 188
      Top = 38
      Width = 218
      Height = 26
      Alignment = taCenter
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'GOST type B'
      Font.Style = []
      ParentFont = False
      TabOrder = 2
      Text = '1'
    end
  end
  object RemoveBusButton: TButton
    Left = 696
    Top = 359
    Width = 305
    Height = 41
    Caption = 'Remove the bus from the route'
    Enabled = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'GOST type B'
    Font.Style = []
    ParentFont = False
    TabOrder = 5
    OnClick = RemoveBusButtonClick
  end
  object UpdatePositionBus: TTimer
    Interval = 100
    OnTimer = UpdatePositionBusTimer
    Left = 8
    Top = 560
  end
end
