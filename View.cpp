// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "View.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMainForm *MainForm;
// ---------------------------------------------------------------------------

__fastcall TMainForm::TMainForm(TComponent* Owner) : TForm(Owner) {
	selectedBusWorker = NULL;
	selectedBusSeries = NULL;
	selectedBusColor = 0;
	indexSelectedPoint = -1;
	isPointSelect = false;
}
// ---------------------------------------------------------------------------

void TMainForm::SetPosition(TPointSeries* point, double x, double y) {
	point->Clear();
	point->AddXY(x, y, (UnicodeString)x, point->Color);
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::StartButtonClick(TObject *Sender) {
	for (int i = 0; i < CountOfBusesEdit->Text.ToInt(); i++) {
		int ID = i + 100;
		CreateNewBus(ID);
	}
	SelectBusInBusesList(0);

	startModelingTime = clock();
	SetEnabledVCL(true);
}
// ---------------------------------------------------------------------------

int TMainForm::CreateNewBus(int ID) {
	BusWorker* busWorker = new BusWorker(ID);
	busWorkerVec.push_back(busWorker);

	TPointSeries* busSeries = new TPointSeries(this);
	busSeriesVec.push_back(busSeries);
	busSeries->ParentChart = MainChart;

	// Bus series parameters
	busSeries->Tag = ID;
	busSeries->Pointer->HorizSize = 8;
	busSeries->Pointer->VertSize = 8;
	busSeries->Pointer->Style = psCircle;
	busSeries->Marks->Visible = true;
	busSeries->OnGetMarkText = BusSeriesGetMarkText;

	SetPosition(busSeries, xVec[0], yVec[0]);
	if (busWorker->start(xVec, yVec, UpdatePositionBus->Interval, 1000))
		// функция возвращает индекс в BusesListComboBox для удобства
			return BusesListComboBox->Items->Add
			(L"Bus #" + IntToStr(busSeries->Tag));
	else
		return -1;
}

void TMainForm::SetEnabledVCL(bool state) {
	StartButton->Enabled = !state;
	CountOfBusesEdit->Enabled = !state;
	StopButton->Enabled = state;
	BusInfoMemo->Enabled = state;
	BusesListComboBox->Enabled = state;
	RemoveBusButton->Enabled = state;
}

void __fastcall TMainForm::UpdatePositionBusTimer(TObject *Sender) {
	// Time Update
	if (StartButton->Enabled == false)
		TimeLabel->Caption =
			IntToStr((int)((clock() - startModelingTime) / CLOCKS_PER_SEC)) +
			L" sec";

	// Если какая-нибудь остановка выбрана, но моделирование трафика не запущено, в BusStopInfoMemo выводится
	// информация о том, что на маршруте нет ни одного автобуса
	if (busWorkerVec.size() == 0) {
		if (isPointSelect) {
			BusStopInfoMemo->Lines->Clear();
			BusStopInfoMemo->Lines->Add(L"There aren't buses on the route.");
		}
		return;
	}

	// Обновление позиции каждого автобуса на графике движения
	for (int i = 0; i < CountOfBusesEdit->Text.ToInt(); i++) {
		if (busWorkerVec[i]->isDataUpdate()) {
			SetPosition(busSeriesVec[i], busWorkerVec[i]->getXValue(),
				busWorkerVec[i]->getYValue());
			busWorkerVec[i]->resetUpdate();
		}
	}

	if (isPointSelect) {
		// Обновление информации в BusStopInfoMemo о времени прихода ближайшего автобуса к выбранной остановке
		int bestTimeToArrival = 0;
		for (int i = 0; i < busWorkerVec.size(); i++) {
			int time = busWorkerVec[i]->getTimeToArrival(indexSelectedPoint);
			if (time < bestTimeToArrival || bestTimeToArrival == 0)
				bestTimeToArrival = time;
		}
		BusStopInfoMemo->Lines->Clear();
		BusStopInfoMemo->Lines->Add(L"Bus stop number: " +
			IntToStr(indexSelectedPoint));
		BusStopInfoMemo->Lines->Add(L"Time before the arrival of the bus: " +
			IntToStr(bestTimeToArrival) + L" sec");
	}
	// Если ни одна остановка не выбрана, BusStopInfoMemo просто поддерживается не заполненым
	else
		BusStopInfoMemo->Lines->Clear();

	// В случае, если выбран хотя бы один автобус, что возможно только при запуске системы моделирования,
	// в BusInfoMemo выводятся характеристики выбранного автобуса
	if (selectedBusWorker != NULL) {
		BusInfoMemo->Clear();
		BusInfoMemo->Lines->Add(
			L"Bus number: " + IntToStr(selectedBusWorker->getID()));
		BusInfoMemo->Lines->Add(L"Current speed: " +
			FloatToStr(selectedBusWorker->getCurrentSpeed()));

		int numberNextBusStop = selectedBusWorker->getCurrentSiteNumber();
		BusInfoMemo->Lines->Add(L"Next bus stop: " +
			IntToStr(numberNextBusStop));
		BusInfoMemo->Lines->Add(L"Time to arrival at next bus stop: " +
			IntToStr(selectedBusWorker->getTimeToArrival(numberNextBusStop)) +
			L" sec");
		BusInfoMemo->Lines->Add(L"Service status: " +
			BoolToStr(selectedBusWorker->getServiceable(), true));
	}
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::StopButtonClick(TObject *Sender) {
	TimeLabel->Caption = L"0 sec";

	BusesListComboBox->Clear();
	BusesListComboBox->ItemIndex = -1;
	BusInfoMemo->Clear();

	selectedBusSeries = NULL;
	selectedBusWorker = NULL;

	std::for_each(busWorkerVec.begin(), busWorkerVec.end(),
		std::default_delete<BusWorker>());
	std::for_each(busSeriesVec.begin(), busSeriesVec.end(),
		std::default_delete<TPointSeries>());

	busWorkerVec.clear();
	busSeriesVec.clear();

	SetEnabledVCL(false);
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::FormDestroy(TObject *Sender) {
	if (busWorkerVec.size() == 0)
		return;
	std::for_each(busWorkerVec.begin(), busWorkerVec.end(),
		std::default_delete<BusWorker>());
	busWorkerVec.clear();
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::ResetSelectedPoint_MainChartDblClick(TObject *Sender)
{
	if (indexSelectedPoint != -1)
		BusStopSeries->Pointer->operator[](indexSelectedPoint)->Color = clLime;
	indexSelectedPoint = -1;
	BusStopInfoMemo->Lines->Clear();
	isPointSelect = false;
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::BusStopSeriesClickPointer(TCustomSeries *Sender,
	int ValueIndex, int X, int Y) {
	// При выборе остановки, необходимо снять выделение с предыдущей выбранной
	if (indexSelectedPoint != -1)
		ResetSelectedPoint_MainChartDblClick(this);

	indexSelectedPoint = ValueIndex;
	if (BusStopSeries->Pointer->operator[](indexSelectedPoint)->Color == clLime)
	{
		BusStopSeries->Pointer->operator[](indexSelectedPoint)->Color = clRed;
		isPointSelect = true;
	}
	else {
		BusStopSeries->Pointer->operator[](indexSelectedPoint)->Color = clLime;
		isPointSelect = false;
	}
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::BusSeriesGetMarkText(TChartSeries *Sender,
	int ValueIndex, UnicodeString &MarkText) {
	MarkText = IntToStr(Sender->Tag);
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::BusesListComboBoxChange(TObject *Sender) {
	SelectBusInBusesList(BusesListComboBox->ItemIndex);
}
// ---------------------------------------------------------------------------

void TMainForm::SelectBusInBusesList(int index) {
	BusesListComboBox->ItemIndex = index;
	UnicodeString selectedBusStr = BusesListComboBox->Items->Strings[index];
	int indexStartNumber = selectedBusStr.Pos(L"#") + 1;
	int numberBus = selectedBusStr.SubString(indexStartNumber,
		selectedBusStr.Length() - (indexStartNumber - 1)).ToInt();

	for (int i = 0; i < busSeriesVec.size(); i++) {
		if (busWorkerVec[i]->getID() == numberBus)
			selectedBusWorker = busWorkerVec[i];
		if (busSeriesVec[i]->Tag == numberBus) {
			if (selectedBusSeries != NULL) {
				selectedBusSeries->Pointer->Color = selectedBusColor;
			}
			selectedBusSeries = busSeriesVec[i];
			selectedBusColor = busSeriesVec[i]->Pointer->Color;
			busSeriesVec[i]->Pointer->Color = clRed;
		}
	}
}

void __fastcall TMainForm::RemoveBusButtonClick(TObject *Sender) {
	int ID = 0;
	for (int i = 0; i < busWorkerVec.size(); i++) {
		if (busWorkerVec[i] == selectedBusWorker) {
			ID = busWorkerVec[i]->getID();
			busWorkerVec[i]->stop();
			delete busWorkerVec[i];
			busWorkerVec.erase(busWorkerVec.begin() + i);
			selectedBusWorker = NULL;
		}

		if (busSeriesVec[i] == selectedBusSeries) {
			busSeriesVec[i]->Clear();
			delete busSeriesVec[i];
			busSeriesVec.erase(busSeriesVec.begin() + i);
			selectedBusSeries = NULL;
		}
	}

	BusesListComboBox->Items->Delete(BusesListComboBox->ItemIndex);
	SelectBusInBusesList(CreateNewBus(ID));
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::FormCreate(TObject *Sender) {
	for (int i = 0; i < RouteSeries->XValues->Count; i++) {
		xVec.push_back(RouteSeries->XValue[i]);
		yVec.push_back(RouteSeries->YValue[i]);
	}
}
// ---------------------------------------------------------------------------
