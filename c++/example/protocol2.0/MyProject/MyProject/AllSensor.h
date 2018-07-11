#include <vector>

#include "Sensor.h"

#ifndef __ALL_SENSOR_H__

#define __ALL_SENSOR_H__

using namespace std;

class AllSensor {

private:

	vector<Sensor*> sensorCollection;

	static AllSensor* instance;

	AllSensor() {}

public:

	static AllSensor* getInstance() {

		if (instance == NULL)

			instance = new AllSensor;

		return instance;

	}

	void add(Sensor* m) {

		sensorCollection.push_back(m);

	}

	Sensor* get(long i) {

		return sensorCollection[i];

	}

	long size() {

		return sensorCollection.size();

	}

};

#endif
