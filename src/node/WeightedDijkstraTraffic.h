/*
########################################################
##           __  __       _____   _____ _             ##
##     /\   |  \/  |     |  __ \ / ____(_)            ##
##    /  \  | \  / | ___ | |  | | (___  _ _ __ ___    ##
##   / /\ \ | |\/| |/ _ \| |  | |\___ \| | '_ ` _ \   ##
##  / ____ \| |  | | (_) | |__| |____) | | | | | | |  ##
## /_/    \_\_|  |_|\___/|_____/|_____/|_|_| |_| |_|  ##
##                                                    ##
## Author:                                            ##
##    Andrea Di Maria                                 ##
##    <andrea.dimaria90@gmail.com>                    ##
########################################################
*/

#ifndef __AMOD_SIMULATOR_MANHATTANROUTING_H_
#define __AMOD_SIMULATOR_MANHATTANROUTING_H_

#include <omnetpp.h>
#include "Pheromone.h"
#include "Traffic.h"

class WeightedDijkstraTraffic : public cSimpleModule
{
private:
    int myAddress;
    int myX;
    int myY;
    int rows;
    int columns;
    double xChannelLength;
    double yChannelLength;
	// Feromone
	double pheromoneDecayTime;
	double pheromoneDecayFactor;
	Pheromone *pheromone;

	Pheromone *pheromoneEmergency;

	// Traffico
	Traffic *traffic;

    double lastUpdateTime;

    cTopology* topo;

    //Feromone related signals
    simsignal_t * signalFeromone;
    //Traffic related signals
    simsignal_t * signalTraffic;

  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual ~WeightedDijkstraTraffic();
    virtual bool checkAvailableGate(int proposal);
};

#endif