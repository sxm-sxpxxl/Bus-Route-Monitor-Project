// ---------------------------------------------------------------------------

#pragma hdrstop

#include "BusWorker.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

HANDLE BusWorker::hEvent = INVALID_HANDLE_VALUE;
double BusWorker::timeStartNewBus = 0;

BusWorker::BusWorker(int id) : ID(id) {
	hThread = INVALID_HANDLE_VALUE;
	hStopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	hUpdateEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	isForwardDirection = true;
	isServiceable = true;
}

BusWorker::~BusWorker() {
	stop();
	CloseHandle(hStopEvent);
	CloseHandle(hUpdateEvent);
}

bool BusWorker::start(std::vector<double>xVec, std::vector<double>yVec,
	double timeIntervalInMilliseconds, int waitingTimeInMilliseconds) {
	if (hThread != INVALID_HANDLE_VALUE)
		return FALSE;

	this->xVec = xVec;
	this->yVec = yVec;
	this->timeIntervalInMilliseconds = timeIntervalInMilliseconds;
	this->waitingTimeInMilliseconds = waitingTimeInMilliseconds;

	ResetEvent(hStopEvent);
	ResetEvent(hUpdateEvent);

	hThread = CreateThread(NULL, NULL, ThreadProc, (LPVOID) this, NULL, NULL);
	if (hEvent == INVALID_HANDLE_VALUE)
		hEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
	return TRUE;
}

bool BusWorker::stop() {
	if (hThread == INVALID_HANDLE_VALUE)
		return FALSE;

	SetEvent(hStopEvent);
	if (WaitForSingleObject(hThread, 100) == WAIT_TIMEOUT) {
		TerminateThread(hThread, 1);
	}
	CloseHandle(hThread);
	hThread = INVALID_HANDLE_VALUE;

	if (hEvent != INVALID_HANDLE_VALUE) {
		CloseHandle(hEvent);
		hEvent = INVALID_HANDLE_VALUE;
	}

	return TRUE;
}

int BusWorker::getID() {
	return ID;
}

int BusWorker::getCurrentSiteNumber() {
	return currentSiteNumber;
}

double BusWorker::getCurrentSpeed() {
	return speed;
}

bool BusWorker::getServiceable() {
	return isServiceable;
}

double BusWorker::getXValue() {
	return xCurrent;
}

double BusWorker::getYValue() {
	return yCurrent;
}

void BusWorker::resetUpdate() {
	ResetEvent(hUpdateEvent);
}

bool BusWorker::isDataUpdate() {
	return WaitForSingleObject(hUpdateEvent, 0) == WAIT_OBJECT_0;
}

double BusWorker::random(int min, int max) {
	srand(time(0));
	return min + rand() % ((max + 1) - min);
}

DWORD WINAPI BusWorker::ThreadProc(LPVOID pvParam) {
	BusWorker* _this = (BusWorker*) pvParam;
	if (!_this)
		return 1;

	WaitForSingleObject(hEvent, INFINITE);

	while (true) {
		_this->speed = _this->random(20, 100) / 100; // optional

		// Определение количества начальных шагов для первого участка маршрута и вычисление самого шага по оси абсцисс
		_this->step = _this->getDistance(_this->xVec[1],
			_this->yVec[1] - _this->yVec[0]) /
			(_this->speed * _this->timeIntervalInMilliseconds / 1000);
		double deltaX = _this->xVec[1] / _this->step;

		_this->isForwardDirection = true;
		_this->currentSiteNumber = 1;

		// Прямое прохождение маршрута, начиная со 2-го узла (2 эл-т с индексом 1),
		// поскольку в дальнейшем происходит обращение к эл-ту с индексом i - 1. Да, знаю, надо исправить.
		for (int i = 1; i < _this->xVec.size(); i++) {
			// Установка начального положения координаты абсцисс и ординат
			_this->xCurrent = _this->xVec[i - 1];
			_this->yCurrent = _this->yVec[i - 1];

			// Переопределение шага по выявленному соотношению для сохранения постоянства скорости в прохождении
			// участков маршрута разной длины. Условие служит для того, чтобы обойти итерацию на которой уже известны
			// значения количества шагов и самого шага
			if (i != 1)
				deltaX *= _this->getDistance(_this->xVec[1],
				_this->yVec[i - 1] - _this->yVec[i - 2]) / _this->getDistance
					(_this->xVec[1], _this->yVec[i] - _this->yVec[i - 1]);
			// Осуществление итеративного прохождения участка маршрута. Возвращает TRUE в случае успешного прохождения и FALSE
			// в случае, если работа исполняемого потока была прервана
			if (!_this->followTheRoute(i, deltaX))
				return 0;
		}

		_this->isForwardDirection = false;

		// Аналогичный участок кода, что и выше для обратного прохождения маршрута, также начиная со 2-го узла с конца
		for (int i = (_this->xVec.size() - 1) - 1; i >= 0; i--) {
			_this->xCurrent = _this->xVec[i + 1];
			_this->yCurrent = _this->yVec[i + 1];

			if (i != (_this->xVec.size() - 1) - 1)
				deltaX *= _this->getDistance(_this->xVec[1],
				_this->yVec[i + 1] - _this->yVec[i + 2]) / _this->getDistance
					(_this->xVec[1], _this->yVec[i] - _this->yVec[i + 1]);

			if (!_this->followTheRoute(i, -deltaX))
				return 0;
		}
	}
}

bool BusWorker::followTheRoute(int i, double deltaX) {
	// Вычисление коэффициента угла наклона касательной
	double angleFactor;
	if (isForwardDirection)
		angleFactor = (yVec[i] - yVec[i - 1]) / (xVec[i] - xVec[i - 1]);
	else
		angleFactor = (yVec[i] - yVec[i + 1]) / (xVec[i] - xVec[i + 1]);

	// Запутанный способ универсализировать содержимое цикла
	// посредством введения управляющей переменной состояния is
	bool is = true;
	currentSiteNumber = i;

	while (is) {
		xCurrent += deltaX;
		yCurrent = angleFactor * xCurrent + (yVec[i] - angleFactor * xVec[i]);

		if (deltaX > 0)
			is = xCurrent < xVec[i];
		else
			is = xCurrent > xVec[i];

		double time = clock();

		// FILETIME createTime;
		// FILETIME exitTime;
		// FILETIME kernelTime;
		// FILETIME userTime;
		// if ( GetProcessTimes( GetCurrentProcess( ),
		// &createTime, &exitTime, &kernelTime, &userTime ) != -1 )
		// {
		// SYSTEMTIME userSystemTime;
		// if ( FileTimeToSystemTime( &userTime, &userSystemTime ) != -1 )
		// time = (double)userSystemTime.wSecond + (double)userSystemTime.wMilliseconds / 1000;
		// }

		// clock() or SYSTEMTIME?

		if (time - timeStartNewBus > 5000) {

			timeStartNewBus = time;
			SetEvent(hEvent);
		}

		if (random(0, 100) < 3) {
			isServiceable = false;
		}

		if (!isServiceable) {
			timeStartNewBus = time;
			SetEvent(hEvent);
			return FALSE;
		}

		SetEvent(hUpdateEvent);
		while (WaitForSingleObject(hUpdateEvent,
			0) == WAIT_OBJECT_0 && WaitForSingleObject(hStopEvent,
			0) != WAIT_OBJECT_0) {
		}
		if (WaitForSingleObject(hStopEvent, 0) == WAIT_OBJECT_0) {
			return FALSE;
		}
	}

	Sleep(waitingTimeInMilliseconds);
	return TRUE;
}

double BusWorker::getDistance(double x, double y) {
	return sqrt(pow(x, 2) + pow(y, 2));
}

int BusWorker::getTimeToArrival(int indexOfBusStop) {
	double distance;
	if (xCurrent < indexOfBusStop) {
		if (isForwardDirection) {
			distance =
				getDistance(xVec[currentSiteNumber] -
				xVec[currentSiteNumber - 1],
				yVec[currentSiteNumber] - yVec[currentSiteNumber - 1]) -
				getDistance(xCurrent - xVec[currentSiteNumber - 1],
				yCurrent - yVec[currentSiteNumber - 1]);
			for (int i = currentSiteNumber; i < indexOfBusStop; i++) {
				distance += getDistance(xVec[i + 1] - xVec[i],
					yVec[i + 1] - yVec[i]);
			}
		}
		else {
			distance =
				getDistance(xVec[currentSiteNumber] -
				xVec[currentSiteNumber + 1],
				yVec[currentSiteNumber] - yVec[currentSiteNumber + 1]) -
				getDistance(xCurrent - xVec[currentSiteNumber + 1],
				yCurrent - yVec[currentSiteNumber + 1]);

			for (int i = currentSiteNumber; i > 0; i--) {
				distance += getDistance(xVec[i] - xVec[i - 1],
					yVec[i] - yVec[i - 1]);
			}

			for (int i = 0; i < indexOfBusStop; i++) {
				distance += getDistance(xVec[i + 1] - xVec[i],
					yVec[i + 1] - yVec[i]);
			}
		}
	}
	else {

		if (isForwardDirection) {
			distance =
				getDistance(xVec[currentSiteNumber] -
				xVec[currentSiteNumber - 1],
				yVec[currentSiteNumber] - yVec[currentSiteNumber - 1]) -
				getDistance(xCurrent - xVec[currentSiteNumber - 1],
				yCurrent - yVec[currentSiteNumber - 1]);

			for (int i = currentSiteNumber; i < xVec.size() - 1; i++) {
				distance += getDistance(xVec[i + 1] - xVec[i],
					yVec[i + 1] - yVec[i]);
			}

			for (int i = xVec.size() - 1; i > indexOfBusStop; i--) {
				distance += getDistance(xVec[i] - xVec[i - 1],
					yVec[i] - yVec[i - 1]);
			}
		}
		else {
			distance =
				getDistance(xVec[currentSiteNumber] -
				xVec[currentSiteNumber + 1],
				yVec[currentSiteNumber] - yVec[currentSiteNumber + 1]) -
				getDistance(xCurrent - xVec[currentSiteNumber + 1],
				yCurrent - yVec[currentSiteNumber + 1]);

			for (int i = currentSiteNumber; i > indexOfBusStop; i--) {
				distance += getDistance(xVec[i] - xVec[i - 1],
					yVec[i] - yVec[i - 1]);
			}
		}

	}

	int timeToArrival = (int)(distance / speed) +
		((abs(indexOfBusStop - currentSiteNumber) - 0)
		* waitingTimeInMilliseconds / 1000);

	return timeToArrival;
}
