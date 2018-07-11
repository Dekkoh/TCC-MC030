#include <vector>

#include "Values.h"

#ifndef __ALL_VALUES_H__

#define __ALL_VALUES_H__

using namespace std;

class AllValues {

private:

	vector<Values*> valuesCollection;

	static AllValues* instance;

	AllValues() {}

public:

	static AllValues* getInstance() {

		if (instance == NULL)

			instance = new AllValues;

		return instance;

	}

	void add(Values* m) {

		valuesCollection.push_back(m);

	}

	Values* get(long i) {

		return valuesCollection[i];

	}

	long size() {

		return valuesCollection.size();

	}

};

#endif
