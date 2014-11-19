#pragma once
class Particle;
#include "Particle.h"
#include <random>
#include <iostream>
#include <math.h>
#include <limits>
#include <fstream>
#include <string>
#include "test_func.h"
#include <SFML/Graphics.hpp>

enum UpdateStrategy
{
	PSO,
	HPSO,
	HPSOv2,
	HPSOv3,
	HPSORand,
	HPSONoVel,
	DE
};

class Swarm
{
public:
	Swarm(void);
	~Swarm(void);
	void setUpdateStrategy(UpdateStrategy ups);
	double run();
	std::string toString();
	
	int dimension;
	int min_x, max_x, max_velocity;
	int iterations;
	int size;

	int currentIteration;

	double inertiaWeight;
	double cognitiveWeight;
	double socialWeight;
	double mutationWeight;

	double crossoverRatio;

	double* swarmBest;
	double swarmBestFitness;
	int lastBestFoundAt;

	Particle* particles;

	int functionNumber;


private:
	void preRun();
	void postRun();

	double getFitness();
	double getFitness(Particle& p);
	double getFitness(double* position);
	double getRandomPosition();
	double getRandomVelocity();
	double getRandomFactor();
	int getRandomIndex();
	int getRandomDimensionIndex();
	void setNewBest(double);

	void render();
	sf::RenderWindow window;
	sf::Texture texture;
	sf::Sprite sprite;
	sf::Vector2f scale;
	bool ignorePlot;

	void drawPlot();
	void updatePlot();
	void drawParticle(sf::RenderWindow &window, float x, float y, sf::Color color);
	float Swarm::convertX(float x);
	float Swarm::convertY(float y);

	void copyArray(double* src, double* dest);
	void initParticle(Particle& p);
	void updateBest();
	void updateBest(Particle& p);
	void (Swarm::*updateStrategy)();

	double* fitness;
	double* trialPSOParticlePosition;
	double* trialPSOParticleVelocity;
	double* trialDEParticle;
	double* trialPosition;
	double* trialVelocity;

	int currentParticleIndex;

	std::random_device random;
    std::mt19937 randomGenerator;

	//update strategies
	std::string updateStrategyName;
	void PSO();
	void HPSO();
	void HPSOv2();
	void HPSOv3();
	void HPSORand();
	void reweight();
	void DE();
	void HPSONoVel();
};