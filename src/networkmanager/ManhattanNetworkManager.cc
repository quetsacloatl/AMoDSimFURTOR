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

#include "ManhattanNetworkManager.h"

Define_Module(ManhattanNetworkManager);

void ManhattanNetworkManager::buildSetOfDestroyedNodes() {

    int epicenterAddresses[numberOfEpicenters];

    for (int i = 0; i < numberOfEpicenters; i++) {
        epicenterAddresses[i] = intuniform(0, numberOfNodes - 1);
        EV << "epicenter Address [" << i << " ] " << epicenterAddresses[i]
                  << endl;
        if (disasterRadius > 0)	// Creation of destroyed nodes set
            setOfEpicenters.insert(epicenterAddresses[i]);
	}

}
/*

    cTopology *topo = new cTopology("topo");

       std::vector<std::string> nedTypes;
       nedTypes.push_back("src.node.Node");
       topo->extractByNedTypeName(nedTypes);


       double seed = 0.25;

	for (int i = 0; i < numberOfEpicenters; i++) {

		// PER OGNI EPICENTRO
		cTopology::Node *node = topo->getNode(epicenterAddresses[i]);
		ev << "epicentro: " << node->getModule()->getFullPath() << endl;

		for (int k = 0; k < disasterRadius; k++) {
			// DI RAGGIO

			for (int j = 0; j < node->getNumOutLinks(); j++) {

//				if (uniform(0, 1) < seed) {
					// SCOPPIA I VICINI
					//raggio volte
					cTopology::Node *neighbour = node->getLinkOut(j)->getRemoteNode();

					setOfEpicenters.insert(neighbour->getModule()->getId() - 4);

					EV << neighbour->getModule()->getFullPath() << "   ";
//				}

			}
			ev << endl;


		}


	}




    std::set<int> auxSet = setOfDestroyedNodes;
	for (int i = 1; i < disasterRadius; i++) {
		for (auto elem : auxSet) {
			propagateEarthquakeBetweenNodes(elem);
		}
	auxSet = setOfDestroyedNodes;

	}
delete topo;
	return setOfEpicenters;
}

*/
/**
 * Propagate the earthquake from epicenter node to neighbours.
 */
void ManhattanNetworkManager::propagateEarthquakeBetweenNodes(int epicenterAddress) {
/*
	setOfDestroyedNodes.insert(epicenterAddress);

    cTopology *topo = new cTopology("topo");

    std::vector<std::string> nedTypes;
    nedTypes.push_back("src.node.Node");
    topo->extractByNedTypeName(nedTypes);

    for (int i = 0; i < topo->getNumNodes(); i++) {

        cTopology::Node *node = topo->getNode(epicenterAddress);

        for (int j = 0; j < node->getNumOutLinks(); j++) {
            cTopology::Node *neighbour = node->getLinkOut(j)->getRemoteNode();
            setOfDestroyedNodes.insert(neighbour->getModuleId());

        }

    }
    delete topo;
    return auxSet;*/
}

//void ManhattanNetworkManager::buildSetOfNodesInRedZone(std::set<int> auxSet) {
 /*
	// Creation of red zones nodes set
//    for (auto elem : setOfDestroyedNodes)
//        auxSet = propagateEarthquakeBetweenNodes(elem, auxSet);
//    setOfNodesInRedZone.insert(auxSet.begin(), auxSet.end());

    // Get nodes in red zones
    for (auto elem : setOfDestroyedNodes)
        setOfNodesInRedZone.erase(elem);
    EV<< "nodi in redzone ";
    for (auto elem : setOfNodesInRedZone)
        EV<<elem<< " ";
    EV<<endl;*/
//}

void ManhattanNetworkManager::buildSetOfBorderNodes() {
    for (int i = 0; i < columns; i++) {
        for (int j = 0; j < rows; j++) {
            if (i == 0 || j == 0 || i == (columns - 1) || j == (rows - 1)) {
                setOfBorderNodes.insert(j + i * rows);
            }
        }
    }
}
int ManhattanNetworkManager::pickRandomElemFromSet(std::set<int> s) {
    if (s.empty())
        return -1;
    int rnd = intuniform(0, s.size() - 1);
    std::set<int>::const_iterator it(s.begin());
    advance(it, rnd);
    return *it;
}

ManhattanNetworkManager::~ManhattanNetworkManager() {
    delete hospitalAddresses;
    delete collectionPointsAddresses;
}

void ManhattanNetworkManager::initialize() {

    cModule* parentModule = getParentModule();
    rows = parentModule->par("width");
    columns = parentModule->par("height");

    numberOfHospitals = par("numberOfHospitals");
    hospitalAddresses = new int[numberOfHospitals];

    numberOfCollectionPoints = par("numberOfCollectionPoints");
    collectionPointsAddresses = new int[numberOfCollectionPoints];

    numberOfStoragePoints = par("numberOfStoragePoints");
    storagePointsAddresses = new int[numberOfStoragePoints];

    numberOfVehicles = par("numberOfVehicles");
    numberOfNodes = par("numberOfNodes");
    numberOfEmergencyVehicles = par("numberOfEmergencyVehicles");
    numberOfTrucks = par("numberOfTrucks");

    ambulanceSpeed = par("ambulanceSpeed");
    truckSpeed = par("truckSpeed");

    disasterRadius = par("disasterRadius");
    numberOfEpicenters = par("numberOfEpicenters");

    xChannelLength = parentModule->par("xNodeDistance");
    yChannelLength = parentModule->par("yNodeDistance");

    additionalTravelTime = setAdditionalTravelTime(parentModule->par("speed"),
            parentModule->par("acceleration"));

    // Creation of destroyed nodes set
    buildSetOfDestroyedNodes();


    // DON'T CHANGE THE ORDER
    buildsetOfAvailableNodes();
    buildSetOfNodesInRedZone();  // Creation of red zones nodes set
    buildSetOfBorderNodes();            // Creation of border zones nodes set
    buildHospitalNodes();
    buildStoragePointNodes();
    buildCollectionPointNodes();


    // Vehicles creation
    for (int i = 0; i < numberOfVehicles; i++) {
        int rand = intuniform(0, numberOfNodes - 1, 4);
        if (!checkHospitalNode(rand)) //Nessun veicolo civile puo' partire dall'ospedale
            vehiclesPerNode[rand] += 1;
    }

    // Ambulances creation
    for (int i = 0; i < numberOfHospitals; i++)
        vehiclesPerNode[hospitalAddresses[i]] = numberOfEmergencyVehicles;

    // initialize truck start node

    for (int i = 0; i < numberOfStoragePoints; i++)
        vehiclesPerNode[storagePointsAddresses[i]] = numberOfTrucks;

}

/**
 * Return the space distance from current node to target one.
 *
 * @param srcAddr
 * @param dstAddress
 * @return
 */
double ManhattanNetworkManager::getSpaceDistance(int srcAddr, int dstAddr) {
    double space_distance = 0;

    int xSource = srcAddr % rows;
    int xDest = dstAddr % rows;

    int ySource = srcAddr / rows;
    int yDest = dstAddr / rows;

    space_distance += abs(xSource - xDest) * xChannelLength;
    space_distance += abs(ySource - yDest) * yChannelLength;

    return space_distance;
}

/**
 * Return the manhattan distance from current node to target one.
 *
 * @param srcAddr
 * @param dstAddress
 * @return
 */
double ManhattanNetworkManager::getManhattanDistance(int srcAddr, int dstAddr) {
    double space_distance = 0;

    int xSource = srcAddr % rows;
    int xDest = dstAddr % rows;

    int ySource = srcAddr / rows;
    int yDest = dstAddr / rows;

    space_distance += abs(xSource - xDest);
    space_distance += abs(ySource - yDest);

    return space_distance;
}

/**
 * Return the X manhattan distance from current node to target one.
 *
 * @param srcAddr
 * @param dstAddress
 * @return
 */
double ManhattanNetworkManager::getManhattanDistanceX(int srcAddr, int dstAddr) {
    double space_distance = 0;

    int xSource = srcAddr % rows;
    int xDest = dstAddr % rows;

    int ySource = srcAddr / rows;
    int yDest = dstAddr / rows;

    space_distance = abs(xSource - xDest);

    return space_distance;
}

/**
 * Return the Y manhattan distance from current node to target one.
 *
 * @param srcAddr
 * @param dstAddress
 * @return
 */
double ManhattanNetworkManager::getManhattanDistanceY(int srcAddr, int dstAddr) {
    double space_distance = 0;

    int xSource = srcAddr % rows;
    int xDest = dstAddr % rows;

    int ySource = srcAddr / rows;
    int yDest = dstAddr / rows;

    space_distance = abs(ySource - yDest);

    return space_distance;
}

/**
 * Return the hop distance from current node to target one.
 *
 * @param srcAddr
 * @param dstAddress
 * @return
 */
double ManhattanNetworkManager::getHopDistance(int srcAddr, int dstAddr) {

    cTopology* topo = new cTopology("topo");
    std::vector<std::string> nedTypes;
    nedTypes.push_back("src.node.Node");
    topo->extractByNedTypeName(nedTypes);

    cTopology::Node *node = topo->getNode(srcAddr);
    cTopology::Node *targetnode = topo->getNode(dstAddr);
    topo->calculateUnweightedSingleShortestPathsTo(targetnode);

    delete topo;

    return node->getDistanceToTarget();
}

/**
 * Return the time distance from current node to target one.
 *
 * @param dstAddress
 * @return
 */
double ManhattanNetworkManager::getTimeDistance(int srcAddr, int dstAddr) {
    double time_distance = 0;

    int xSource = srcAddr % rows;
    int xDest = dstAddr % rows;

    int ySource = srcAddr / rows;
    int yDest = dstAddr / rows;

    time_distance = abs(xSource - xDest) * xTravelTime;
    double yTime = abs(ySource - yDest) * yTravelTime;
    time_distance += yTime;

    if (time_distance != 0)
        time_distance += additionalTravelTime;

    return time_distance;
}

/**
 * Return the vehicles started from nodeAddr.
 *
 * @param nodeAddr
 * @return
 */
int ManhattanNetworkManager::getVehiclesPerNode(int nodeAddr) {
    int nVehicles = 0;
    std::map<int, int>::iterator it;

    it = vehiclesPerNode.find(nodeAddr);
    if (it != vehiclesPerNode.end())
        nVehicles = it->second;

    return nVehicles;
}



/**
 * Check if the specified address is valid.
 *
 * @param dstAddress
 * @return
 */
bool ManhattanNetworkManager::isValidAddress(int nodeAddr) {
    if (nodeAddr >= 0 && nodeAddr < numberOfNodes)
        return true;
    return false;
}

/**
 * Return the outputGate index.
 *
 * @param dstAddress
 * @return
 */
int ManhattanNetworkManager::getOutputGate(int srcAddr, int dstAddr) {
    return -1;
}

/**
 * Return the length of the channel connected to the specified gate.
 *
 * @param dstAddress
 * @param gateIndex
 * @return
 */
double ManhattanNetworkManager::getChannelLength(int nodeAddr, int gateIndex) {
    return -1;
}

double ManhattanNetworkManager::getXChannelLength() {
    return xChannelLength;
}

void ManhattanNetworkManager::handleMessage(cMessage *msg) {

}

/** Check if current node is destroyed.
 *
 * @param addr
 * @return
 **/
bool ManhattanNetworkManager::checkDestroyedNode(int addr) {

    for (auto elem : setOfDestroyedNodes) {
        if (elem == addr)
            return true;
    }
    return false;
}

bool ManhattanNetworkManager::checkBorderNode(int addr) {
    for (auto elem : setOfBorderNodes) {
        if (elem == addr)
            return true;
    }
    return false;
}
bool ManhattanNetworkManager::checkRedZoneNode(int addr) {
    for (auto elem : setOfNodesInRedZone) {
        if (elem == addr)
            return true;
    }
    return false;
}

void ManhattanNetworkManager::buildHospitalNodes() {
    int safeHospital;

    do {
        safeHospital = intuniform(0, numberOfNodes - 1);
    } while (setOfEpicenters.find(safeHospital) != setOfEpicenters.end());
    hospitalAddresses[0] = safeHospital;

    EV << "HOSPITAL Safe: " << hospitalAddresses[0] << endl;

    for (int i = 1; i < numberOfHospitals; i++) {
        hospitalAddresses[i] = intuniform(0, numberOfNodes - 1);
        EV << "HOSPITAL" << hospitalAddresses[i] << endl;
    }

}

bool ManhattanNetworkManager::checkHospitalNode(int addr) {
    for (int i = 0; i < numberOfHospitals; i++) {
        if (hospitalAddresses[i] == addr)
            return true;
    }
    return false;

}

/*
 * Assign random nodes from border to the collection points.
 * These nodes aren't destroyed.
 */
void ManhattanNetworkManager::buildCollectionPointNodes() {
    bool aux;
    for (int i = 0; i < numberOfCollectionPoints; i++) {

        do {
            aux=false;
            collectionPointsAddresses[i] = pickRandomElemFromSet(setOfAvailableNodes);
            for (int j=0; j<numberOfStoragePoints;j++){
                if(storagePointsAddresses[j]==collectionPointsAddresses[i])
                    aux=true;
            }
        } while (aux);

        EV << "Collection Point: " << collectionPointsAddresses[i] << endl;
    }

}

int ManhattanNetworkManager::pickClosestHospitalFromNode(int addr) {
    int destAddress = hospitalAddresses[0];
    int min = numberOfNodes;
    for (int i = 0; i < numberOfHospitals; i++) {
        if (getHopDistance(addr, hospitalAddresses[i]) < min) {
            destAddress = hospitalAddresses[i];
            min = getHopDistance(addr, hospitalAddresses[i]);
        }
    }

    return destAddress;

}

int ManhattanNetworkManager::pickRandomNodeInRedZone() {
    return pickRandomElemFromSet(setOfNodesInRedZone);
}

bool ManhattanNetworkManager::checkCollectionPointNode(int addr) {
    for (int i = 0; i < numberOfCollectionPoints; i++) {
        if (collectionPointsAddresses[i] == addr)
            return true;
    }
    return false;

}

int ManhattanNetworkManager::pickClosestCollectionPointFromNode(int addr) {
    int destAddress = collectionPointsAddresses[0];
    int min = numberOfNodes;
    for (int i = 0; i < numberOfCollectionPoints; i++) {
        if (getHopDistance(addr, collectionPointsAddresses[i]) < min) {
            destAddress = collectionPointsAddresses[i];
            min = getHopDistance(addr, collectionPointsAddresses[i]);
        }
    }

    return destAddress;

}

void ManhattanNetworkManager::buildsetOfAvailableNodes() {
    EV << "Nodi available ";
    for (int i = 0; i < numberOfNodes; i++) {
        if (setOfEpicenters.find(i) == setOfEpicenters.end()) {
            setOfAvailableNodes.insert(i);
            EV << i << " ";

        }
    }
    EV << endl;
}

void ManhattanNetworkManager::buildStoragePointNodes() {
    for (int i = 0; i < numberOfStoragePoints; i++) {
        do {
            storagePointsAddresses[i] = pickRandomElemFromSet(setOfBorderNodes);
        } while (setOfEpicenters.find(storagePointsAddresses[i])!= setOfEpicenters.end());

        EV << "StoragePoint: " << storagePointsAddresses[i] << endl;
    }
}

int ManhattanNetworkManager::checkStoragePointNode(int addr) {
    for (int i = 0; i < numberOfStoragePoints; i++) {
        if (storagePointsAddresses[i] == addr)
            return true;
    }
    return false;
}

int ManhattanNetworkManager::pickRandomStoragePointNode() {
    if (numberOfStoragePoints<1)
        return -1;
    return storagePointsAddresses[intuniform(0,numberOfStoragePoints-1)];
}

void ManhattanNetworkManager::insertRedZoneNode(int addr) {
	setOfNodesInRedZone.insert(addr);
}

void ManhattanNetworkManager::insertDestroyedNode(int addr) {
	setOfDestroyedNodes.insert(addr);
	setOfAvailableNodes.erase(addr);
}

void ManhattanNetworkManager::removeRedZoneNode(int addr) {
	setOfNodesInRedZone.erase(addr);
}

void ManhattanNetworkManager::buildSetOfNodesInRedZone() {
//	setOfNodesInRedZone.insert(-1);
}

int ManhattanNetworkManager::pickRandomCollectionPointNode() {
    return collectionPointsAddresses[intuniform(0, numberOfCollectionPoints - 1)];
}
