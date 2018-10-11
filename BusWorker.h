// ---------------------------------------------------------------------------

#ifndef BusWorkerH
#define BusWorkerH
// ---------------------------------------------------------------------------
#include <vector>
#include <math.h>
#include <cmath>
#include <stdlib.h>
#include <time.h>
#include <ctime>
#include <windows.h>

class BusWorker {
private:
	HANDLE hThread;
	HANDLE hStopEvent;
	HANDLE hUpdateEvent;
	static HANDLE hEvent;

	std::vector<double> xVec;
	std::vector<double> yVec;

	double xCurrent;
	double yCurrent;

	int ID;
	int currentSiteNumber;
	bool isForwardDirection;
	bool isServiceable;
	double speed;
	double step;

	double timeIntervalInMilliseconds;
	int waitingTimeInMilliseconds;
	static double timeStartNewBus;

	double random(int min, int max);
	double getDistance(double x, double y);
	bool followTheRoute(int i, double deltaX);
	static DWORD WINAPI ThreadProc(LPVOID pvParam);

public:
	BusWorker(int ID);
	~BusWorker();

	bool start(std::vector<double>xVec, std::vector<double>yVec,
		double timeInterval, int waitingTimeInMilliseconds);
	bool stop();

	int getID();
	double getXValue();
	double getYValue();
	bool isDataUpdate();
	void resetUpdate();

	int getTimeToArrival(int indexOfBusStop);
	double getCurrentSpeed();
	int getCurrentSiteNumber();
	bool getServiceable();
};

#endif
