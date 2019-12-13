#pragma once
#include <stdlib.h>
#include <vector>
#include <string>
#include <map>

//The loss function type of Genetic Solver.
typedef double (*GeneticLossFunction)(const std::vector<double>&);

//A class using genetic algorithm for solving optmization problems.
class GeneticSolver {
public:
	//Constructor
	GeneticSolver();
	//*Distructor
	~GeneticSolver();

	/* Calculate the parameters by the loss got from loss function. User(s) should set parameters & loss function first.*/
	std::vector<double>					solve();

	//-------------------------------Set loss function and paramters for the problem----------------------------

	/*  Set paramters and limitation of paramters.
	*	param[in] _Paras The paramters of the problem which will be the arguments of loss function.
	*	param[in] _ParasMax The maximum limitation of parameters.
	*	param[in] _ParasMin The minimum limitation of parameters.
	*/
	bool								setParamters(const std::vector<double>& _Paras, const std::vector<double>& _ParasMax, const std::vector<double>& _ParasMin);

	/*	Set loss function for the problem.
	 *	params[in] _LossFunction The loss function for the problem.
	 */
	bool								setLossFunction(const GeneticLossFunction _LossFunction);

	//--------------------------------Set & get paramters used in genetic algorithm-----------------------------

	/* Set the group size used for genetic algorithm.
	 * param[in] _GroupSize The group size used for genetic algorithm.
	 */
	bool								setGroupSize(const int _GroupSize);

	/* Get the group size used for genetic algorithm.*/
	int									getGroupSize();

	/* Set the iteration times of genetic algorithm.
	 * param[in] _IterTimes The iteration times of genetic algorithm.
	 */
	bool								setIterationTimes(const int _IterTimes);

	/* Get the iteration times of genetic algorithm.*/
	int									getIterationTimes();

	/* Set the mutation rate of genetic algorithm.
	 * param[in] _MutationRate The mutation rate of genetic algorithm.
	 */
	bool								setMutationRate(const double _MutationRate);

	/* Get the mutation rate of genetic algorithm.*/
	double								getMutationRate();

	/* Set the mating rate of genetic algorithm.
	 * param[in] _MatingRate The mating rate of genetic algorithm.
	 */
	bool								setMatingRate(const int _MatingRate);

	/* Get the mating rate of genetic algorithm.*/
	double								getMatingRate();

	/* Get the loss of the solved result. If solve() has not been called, it will return -1.*/
	double								getLoss();

	/* Get the message from Genetic Solver. If there no error(s), the message will be "Success". Or it will return the error message.*/
	std::string							getMsg();

private:

	//----------------------------------------Functions for calculating genetic algorithm-------------------------

	/* Initialize the calculation.*/
	void								initailization();

	/* Calculate the loss of parameters group for one time.*/
	void								calculateOnce();

	/* Generate the next generation according to loss, mating rate and mutation rate.*/
	void								nextGeneration();

	/* Generate new individual randomly.*/
	std::vector<double>					generateRandomIndividual();

	/* Modify the input individual by the factor 
	 * param[in] _Paras The individual user(s) want(s) to modify.
	 * param[in] _Factor This means each parameter in the individual's changed range. (1-Factor) * V < V < (1+Factor) * V
	 */
	std::vector<double>					modifiedIndividual(const std::vector<double>& _Paras, const double _Factor);

	//-------------------Loss function and paramters for the problem user(s) want(s) to solve----------------------

	//Loss function for the problem
	GeneticLossFunction					m_LossFunction = NULL;
	//Initial parameters.
	std::vector<double>					m_Paras;
	//Maximum limitation of parameters.
	std::vector<double>					m_ParasMax;
	//Minimum limitation of parameters.
	std::vector<double>					m_ParasMin;
	//Paramters group for genetic algorithm.
	std::vector<std::vector<double>>	m_ParasGroup;

	//----------------------------------------Paramters for genetic algorithm---------------------------------------

	//Group size
	int									m_GroupSize = 20;
	//Iteration times;
	int									m_IterTimes = 20000;
	//Mutation rate
	double								m_MutationRate = 0.1;
	//Mating rate
	double								m_MatingRate = 0.9;
	//Random resolution
	unsigned long						m_RandomResolution = 1000000;
	//Multimap of loss & index
	std::multimap<double, int>			m_LossMap;
	//Best loss
	double								m_BestLoss = - 1;
	//The paras of best loss
	std::vector<double>					m_BestParas;


	//Error message
	std::string							m_Msg = "Success";
};