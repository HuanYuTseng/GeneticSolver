#include "GeneticSolver.h"
#include <random>

GeneticSolver::GeneticSolver(){
	;
}

GeneticSolver::~GeneticSolver() {
	;
}

std::vector<double> GeneticSolver::solve()
{
	std::vector<double> result;

	if (m_LossFunction == NULL) {

		m_Msg = "Set loss function first.";
		return result;
	}
	if (m_Paras.size() <= 0)
	{
		m_Msg = "Set paramters first.";
		return result;
	}

	std::random_device randomDevice;
	std::default_random_engine randomEngine(randomDevice());

	initailization();
	calculateOnce();

	for (int i = 0; i < m_IterTimes; i++) {
		nextGeneration();
		calculateOnce();
	}

	for (const auto& para : m_BestParas) {
		result.push_back(para);
	}

	m_Msg = "Success";
	return result;
}

bool GeneticSolver::setParamters(const std::vector<double>& _Paras, const std::vector<double>& _ParasMax, const std::vector<double>& _ParasMin) {
	if (_Paras.size() != _ParasMax.size() || _Paras.size() != _ParasMin.size() || _Paras.size() == 0) {

		m_Msg = "The size of parameters or limitation of paramters is not valid.";
		return false;
	}

	m_ParasMax.clear();
	for (const auto& para : _ParasMax)
		m_ParasMax.push_back(para);

	m_ParasMin.clear();
	for (const auto& para : _ParasMin)
		m_ParasMin.push_back(para);

	bool isValid = true;
	m_Paras.clear();
	for (unsigned int i = 0; i < _Paras.size(); i++) {
		double para = _Paras[i];

		if (para > m_ParasMax[i] || para < m_ParasMin[i])
			isValid = false;

		m_Paras.push_back(para);
	}
	if (!isValid){
		m_Msg = "Value(is) in parameters or limitation of paramters is(are) not valid.";
		return false;
	}

	m_Msg = "Success";
	return true;
}

bool GeneticSolver::setLossFunction(const GeneticLossFunction _LossFunction){
	if (_LossFunction == NULL){
		m_Msg = "The function pointer is NULL.";
		return false;
	}
	m_LossFunction = _LossFunction;

	m_Msg = "Success";
	return true;
}

bool GeneticSolver::setGroupSize(const int _GroupSize){
	if (_GroupSize <= 0) {
		m_Msg = "The group size should be greater than 0.";
		return false;
	}
	m_GroupSize = _GroupSize;

	m_Msg = "Success";
	return true;
}

int GeneticSolver::getGroupSize(){
	return m_GroupSize;
}

bool GeneticSolver::setIterationTimes(const int _IterTimes){
	if (_IterTimes <= 0) {
		m_Msg = "The iteration times should be greater than 0.";
		return false;
	}
	m_IterTimes = _IterTimes;

	m_Msg = "Success";
	return true;
}

int GeneticSolver::getIterationTimes(){
	return m_IterTimes;
}

bool GeneticSolver::setMutationRate(const double _MutationRate){
	if (_MutationRate < 0.0 || _MutationRate > 1.0) {
		m_Msg = "The mutation rate should be in the range 0.0 ~ 1.0";
		return false;
	}
	m_MutationRate = _MutationRate;

	m_Msg = "Success";
	return true;
}

double GeneticSolver::getMutationRate(){
	return m_MutationRate;
}

bool GeneticSolver::setMatingRate(const int _MatingRate){
	if (_MatingRate < 0.0 || _MatingRate > 1.0) {
		m_Msg = "The mating rate should be in the range 0.1 ~ 1.0";
		return false;
	}
	m_MatingRate = _MatingRate;

	m_Msg = "Success";
	return true;
}

double GeneticSolver::getMatingRate(){
	return m_MatingRate;
}

double GeneticSolver::getLoss(){
	return m_BestLoss;
}

std::string GeneticSolver::getMsg(){
	return m_Msg;
}

void GeneticSolver::initailization(){
	m_BestLoss = m_LossFunction(m_Paras);

	m_ParasGroup.clear();
	m_ParasGroup.push_back(m_Paras);
	for (int i = 1; i < m_GroupSize / 2; i++) {
		std::vector<double> individual = modifiedIndividual(m_Paras, 2.0);
		m_ParasGroup.push_back(individual);
	}
	for (int i = 0; i < m_GroupSize / 2; i++) {
		std::vector<double> individual = generateRandomIndividual();
		m_ParasGroup.push_back(individual);
	}
}

void GeneticSolver::calculateOnce(){
	m_LossMap.clear();
	for(int i = 0; i < m_GroupSize; i++){
		std::vector<double> paras = m_ParasGroup[i];
		double loss = m_LossFunction(paras);
		m_LossMap.insert(std::make_pair(loss, i));
	}
}

void GeneticSolver::nextGeneration(){
	std::random_device randomDevice;
	std::default_random_engine randomEngine(randomDevice());
	const int lossMapSize = m_LossMap.size();
	std::uniform_int_distribution<int> pickOne(0, lossMapSize-1);
	std::uniform_int_distribution<unsigned long> uniformDist(0, m_RandomResolution);

	std::map<double, int>::iterator lossIter = m_LossMap.begin();
	
	int firstIndex = lossIter->second;
	m_BestLoss = lossIter->first;
	m_BestParas.clear();
	for (const auto& para : m_ParasGroup[firstIndex]) {
		m_BestParas.push_back(para);
	}
	
	std::vector<std::vector<double>> newParasGroup;
	for (int count = 0; count < lossMapSize; count++) {
		int index = lossIter->second;
		if(count <= lossMapSize * 0.05)
			newParasGroup.push_back(m_ParasGroup[index]);
		else if (count < lossMapSize * m_MatingRate) {
			std::vector<double> individual = m_ParasGroup[index];
			std::vector<double> newIndividual;
			for (unsigned int paraIndex = 0; paraIndex < individual.size(); paraIndex++) {
				double mutationRandom = (uniformDist(randomEngine) * 1.0) / m_RandomResolution;
				if (mutationRandom < m_MutationRate) {
					double randomPara = ( (uniformDist(randomEngine) * 1.0) / m_RandomResolution )
										* (m_ParasMax[paraIndex] - m_ParasMin[paraIndex]) + m_ParasMin[paraIndex];
					newIndividual.push_back(randomPara);
				}
				else {
					double usingOtherRandom = (uniformDist(randomEngine) * 1.0) / m_RandomResolution;
					if (usingOtherRandom >= 0.5) {
						int otherIndex = 0;
						for (int retry = 0; retry < 10; retry++) {
							int tempIndex = pickOne(randomEngine);
							int tempIndexCheck = pickOne(randomEngine);
							if (tempIndex < tempIndexCheck) {
								otherIndex = tempIndex;
								break;
							}
						}
						newIndividual.push_back(m_ParasGroup[otherIndex][paraIndex]);
					}
					else {
						newIndividual.push_back(individual[paraIndex]);
					}
				}
			}
			newParasGroup.push_back(newIndividual);
		}
		else {
			newParasGroup.push_back(generateRandomIndividual());
		}
		lossIter++;
	}

	m_ParasGroup.clear();
	for (const auto& newIndividual : newParasGroup) {
		m_ParasGroup.push_back(newIndividual);
	}
}

std::vector<double> GeneticSolver::generateRandomIndividual(){
	std::vector<double> result;
	std::random_device randomDevice;
	std::default_random_engine randomEngine(randomDevice());

	for (unsigned int i = 0; i < m_Paras.size(); i++) {
		std::uniform_int_distribution<unsigned long> uniformDist(0, m_RandomResolution);
		double para = ( ( uniformDist(randomEngine) * 1.0 ) / m_RandomResolution) * ( m_ParasMax[i] - m_ParasMin[i] ) + m_ParasMin[i];

		result.push_back(para);
	}

	return result;
}

std::vector<double> GeneticSolver::modifiedIndividual(const std::vector<double>& _Paras, const double _Factor){
	std::vector<double> result;
	std::random_device randomDevice;
	std::default_random_engine randomEngine(randomDevice());

	for (unsigned int i = 0; i < m_Paras.size(); i++) {
		std::uniform_int_distribution<unsigned long> uniformDist(0, m_RandomResolution);
		double para = ( ( uniformDist(randomEngine) - (m_RandomResolution / 2.0) ) / m_RandomResolution ) 
						* 2.0 * _Factor * _Paras[i] + _Paras[i] ;

		if (para > m_ParasMax[i]) {
			para = m_ParasMax[i];
		}
		if (para < m_ParasMin[i]) {
			para = m_ParasMin[i];
		}

		result.push_back(para);
	}

	return result;
}