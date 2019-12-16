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
	std::vector<double> parasFake;
	std::vector<double> parasMax;
	std::vector<double> parasMin;

	for (int i = 0; i < 10; i++) {
		paras.push_back(i);
		parasMax.push_back(-10);
		parasMin.push_back(10);
	}

	std::cout << "Genetic Solver Test Start." << std::endl << std::endl;

	std::cout << "Case 1 Wrong Size of Parameters" << std::endl;
	
	if (gs.setParamters(parasFake, parasMax, parasMin)){
		std::cout << "Case 1 failed." << std::endl;
		system("PAUSE");
		return 1;
	}
	else {
		std::cout << "Case 1 passed." << std::endl;
		std::cout << "Case 1 Error Message: " << gs.getMsg() << std::endl;
	}
	gs.setParamters(paras, parasMax, parasMin);

	std::cout << std::endl << "Case 2 Null loss function pointer." << std::endl;

	if (gs.setLossFunction(NULL)) {
		std::cout << "Case 2 failed." << std::endl;
		system("PAUSE");
		return 1;
	}
	else {
		std::cout << "Case 2 passed." << std::endl;
		std::cout << "Case 2 Error Message: " << gs.getMsg() << std::endl;
	}

	std::cout << std::endl << "Case 3 Normally Test." << std::endl;
	gs.setLossFunction(lossFunc);
	std::vector<double> result = gs.solve();

	if (result.size() == paras.size()) {
		std::cout << "Final values of parameters: " << std::endl << std::endl;
		for (const auto& value : result) {
			std::cout << value << std::endl;
		}
		std::cout << std::endl << "Loss value: " << gs.getLoss() << std::endl;

		std::cout << "Case 3 passed." << std::endl;

		std::cout << "Case 3 Message: " << gs.getMsg() << std::endl;
	}
	else {
		std::cout << "Case 3 failed." << std::endl;
	}

	std::cout << std::endl << "Genetic Solver Test End." << std::endl;
	system("PAUSE");
}