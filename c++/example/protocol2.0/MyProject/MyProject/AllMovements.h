#include <vector>

#include "Movement.h"

#ifndef __ALL_MOVEMENTS_H__

#define __ALL_MOVEMENTS_H__

using namespace std;

class AllMovements {

private:

	vector<Movement*> movementCollection;

	static AllMovements* instance;

	AllMovements() {}

public:

	static AllMovements* getInstance() {
		if (instance == NULL) {
			instance = new AllMovements;
		}
		return instance;
	}

	void add(Movement* m) {
		movementCollection.push_back(m);
	}

	Movement* get(long i) {
		return movementCollection[i];
	}

	long size() {
		return movementCollection.size();
	}

	void clearList() {
		movementCollection.clear();
	}

	void deleteIndex(int index) {
		movementCollection.erase(movementCollection.begin() + index);
	}

};

#endif
