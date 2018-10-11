// ---------------------------------------------------------------------------

#ifndef ViewH
#define ViewH
// ---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <VCLTee.Chart.hpp>
#include <VCLTee.Series.hpp>
#include <VclTee.TeeGDIPlus.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>

#include <algorithm>

#include "BusWorker.h"

// ---------------------------------------------------------------------------
class TMainForm : public TForm {
__published: // IDE-managed Components
	TChart *MainChart;
	TLineSeries *RouteSeries;
	TPointSeries *BusStopSeries;
	TButton *StartButton;
	TButton *StopButton;
	TTimer *UpdatePositionBus;
	TMemo *BusStopInfoMemo;
	TEdit *CountOfBusesEdit;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *TimeLabel;
	TLabel *Label3;
	TLabel *Label4;
	TMemo *BusInfoMemo;
	TComboBox *BusesListComboBox;
	TLabel *Label5;
	TGroupBox *GroupBox1;
	TButton *RemoveBusButton;

	void __fastcall StartButtonClick(TObject *Sender);
	void __fastcall UpdatePositionBusTimer(TObject *Sender);
	void __fastcall StopButtonClick(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall ResetSelectedPoint_MainChartDblClick(TObject *Sender);
	void __fastcall BusStopSeriesClickPointer(TCustomSeries *Sender,
		int ValueIndex, int X, int Y);
	void __fastcall BusSeriesGetMarkText(TChartSeries *Sender, int ValueIndex,
		UnicodeString &MarkText);
	void __fastcall BusesListComboBoxChange(TObject *Sender);
	void __fastcall RemoveBusButtonClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);

private: // User declarations
		std::vector<double>xVec;
	std::vector<double>yVec;

	std::vector<BusWorker*>busWorkerVec;
	std::vector<TPointSeries*>busSeriesVec;

	BusWorker* selectedBusWorker;
	TPointSeries* selectedBusSeries;
	TColor selectedBusColor;

	int startModelingTime;
	int indexSelectedPoint;
	bool isPointSelect;

	int CreateNewBus(int ID);
	void SetPosition(TPointSeries* point, double x, double y);
	void SetEnabledVCL(bool state);
	void TMainForm::SelectBusInBusesList(int index);

public: // User declarations
	__fastcall TMainForm(TComponent* Owner);
};

// ---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
// ---------------------------------------------------------------------------
#endif
