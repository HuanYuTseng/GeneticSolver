#include "..\GeneticSolver.h"
#include <iostream>

double lossFunc(const std::vector<double>& _paras) {
	double sum = 0;

	for (const auto& para : _paras) {
		sum += fabs((para-1) * (para-3));
	}
	return sum;
}

int main() {
	GeneticSolver gs;

	std::vector<double> paras;
	std::vector<double> parasMax;
	std::vector<double> parasMin;

	for (int i = 0; i < 10; i++) {
		paras.push_back(i);
		parasMax.push_back(-10);
		parasMin.push_back(10);
	}

	gs.setLossFunction(lossFunc);
	gs.setParamters(paras, parasMax, parasMin);

	std::vector<double> result = gs.solve();
	std::cout << "The final values: " << std::endl;
	for (const auto& value : result) {
		std::cout << value << std::endl;
	}
	std::cout << std::endl << "Loss value: " << gs.getLoss() << std::endl;
	system("PAUSE");
}