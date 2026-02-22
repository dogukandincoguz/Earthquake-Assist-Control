#include "QuakeAssistController.h"
#include <iostream>
#include <sstream>

QuakeAssistController::QuakeAssistController()
    : teams(nullptr),
      teamCount(0),
      supplyQueue(4),
      rescueQueue(4) {
}

QuakeAssistController::~QuakeAssistController() {
    delete[] teams;
}

bool QuakeAssistController::parseAndExecute(const std::string& line) {
    //Read the input file line by line and execute realtime.
    // Using stringstream to parse line by line.
    std::stringstream ss(line);
    std::string command;
    ss >> command;
    // Handle different commands
    if(command == "INIT_TEAMS"){
        int numTeams;
        ss >> numTeams;
        initializeTeams(numTeams);
    }
    else if(command == "SET_TEAM_CAPACITY"){
        int teamId, capacity;
        ss >> teamId >> capacity;
        handleSetTeamCapacity(teamId, capacity);
    }
    else if(command == "ADD_SUPPLY"){
        std::string id, cityStr, supplyTypeStr;
        int amount, emergencyLevel;
        ss >> id >> cityStr >> supplyTypeStr >> amount >> emergencyLevel;
        handleAddSupply(id, cityStr, supplyTypeStr, amount, emergencyLevel);
    }
    else if(command == "ADD_RESCUE"){
        std::string id, cityStr, riskStr;
        int numPeople, emergencyLevel;
        ss >> id >> cityStr >> numPeople >> riskStr >> emergencyLevel;
        handleAddRescue(id, cityStr, numPeople, riskStr, emergencyLevel);
    }
    else if(command == "REMOVE_REQUEST"){
        std::string id;
        ss >> id;
        handleRemoveRequest(id);
    }
    else if(command == "HANDLE_EMERGENCY"){
        int teamId, k;
        ss >> teamId >> k;
        handleHandleEmergency(teamId, k);
    }
    else if(command == "DISPATCH_TEAM"){
        int teamId;
        ss >> teamId;
        handleDispatchTeam(teamId);
    }
    else if(command == "PRINT_QUEUES"){
        printQueues();
    }
    else if(command == "PRINT_TEAM"){
        int teamId;
        ss >> teamId;
        printTeam(teamId);
    }
    else if(command == "CLEAR"){
        clear();
    }
    else{
        std::cout << "Error: Unknown command '" << command << "'." << std::endl;
    }
    return true;
}

bool QuakeAssistController::initializeTeams(int numTeams) {
    //Create a team array and initialize it with teams.
    // If already initialized, delete old array first.
    if (teams != nullptr) {
        delete[] teams;
    }
    // Clear queues
    Request temp;
    while (!supplyQueue.isEmpty()) {
        supplyQueue.dequeue(temp);
    }
    while (!rescueQueue.isEmpty()) {
        rescueQueue.dequeue(temp);
    }
    // Inserting new teams
    teams = new Team[numTeams];
    teamCount = numTeams;
    for (int i = 0; i < numTeams; ++i) {
        teams[i].setId(i);
    }
    std::cout << "Initialized " << numTeams << " teams." << std::endl;
    return true;
}

int QuakeAssistController::findTeamIndexById(int teamId) const {
    //Find the index of the team using teamId.
    // Check if teams array is initialized
    if(teams == nullptr){
        return -1;
    }
    // Check for id match
    for(int i = 0; i < teamCount; i++){
        if(teams[i].getId() == teamId){
            return i;
        }
    }
    return -1;
}

bool QuakeAssistController::handleSetTeamCapacity(int teamId, int capacity) {
    //Find the index of team in the array, update the capacity value of the team.
    // Check if teams array is initialized
    if(teams == nullptr){
        std::cout << "Error: Teams not initialized." << std::endl;
        return false;
    }
    int idx = findTeamIndexById(teamId);
    if(idx == -1){
        std::cout << "Error: Team " << teamId << " not found." << std::endl;
        return false;
    }
    teams[idx].setMaxLoadCapacity(capacity);
    std::cout << "Team " << teamId << " capacity set to " << capacity << "." << std::endl;
    return true;
}

bool QuakeAssistController::handleAddSupply(const std::string& id,
                                            const std::string& cityStr,
                                            const std::string& supplyTypeStr,
                                            int amount,
                                            int emergencyLevel) {
    //Create new supply request, and add it to the SUPPLY queue.
    Request req(id, cityStr, supplyTypeStr, amount, emergencyLevel); // Creating new supply request
    supplyQueue.enqueue(req); // Enqueueing to supply queue
    std::cout << "Request " << id << " added to SUPPLY queue." << std::endl;
    return true;
}

bool QuakeAssistController::handleAddRescue(const std::string& id,
                                            const std::string& cityStr,
                                            int numPeople,
                                            const std::string& riskStr,
                                            int emergencyLevel) {
    //Create new rescue request, and add it to the RESCUE queue.
    Request req(id, cityStr, numPeople, riskStr, emergencyLevel); // Creating new rescue request
    rescueQueue.enqueue(req);
    std::cout << "Request " << id << " added to RESCUE queue." << std::endl;
    return true;
}

bool QuakeAssistController::handleRemoveRequest(const std::string& id) {
    //Remove request using request ID from request(SUPPLY, RESCUE) queue.
    // Try to remove from supply queue first
    if(supplyQueue.removeById(id)){
        std::cout << "Request " << id << " removed from queues." << std::endl;
        return true;
    }
    // If not found in supply, try rescue queue
    if(rescueQueue.removeById(id)){
        std::cout << "Request " << id << " removed from queues." << std::endl;
        return true;
    }
    // Not found in either queue
    std::cout << "Error: Request " << id << " not found." << std::endl;
    return true;
}

bool QuakeAssistController::handleHandleEmergency(int teamId, int k) {
    // TODO: Implement:
    // 1) Find team by id.
    // 2) For up to k steps:
    //    - Look at front of supplyQueue and rescueQueue using peek().
    //    - Use Request::computeEmergencyScore() to decide which to take.
    //    - If both empty -> break.
    //    - Try teams[teamIdx].tryAssignRequest(chosenRequest).
    //       * If this returns false, print overload message and
    //         call teams[teamIdx].rollbackMission(supplyQueue, rescueQueue),
    //         then break.
    //       * Else, dequeue chosen request from its queue and continue.

    // Find team index by ID
    int idx = findTeamIndexById(teamId);
    if (idx == -1) {
        std::cout << "Error: Team " << teamId << " not found." << std::endl;
        return false;
    }
    bool overloaded = false; // Flag for overload
    // Initialize counters
    int totalAssigned = 0;
    int supplyCount = 0;
    int rescueCount = 0;
    int totalWorkload = 0;
    // Loop for up to k steps to assign requests to the team
    for(int step = 0; step < k; step++){
        Request supplyReq, rescueReq;
        bool supplyAvailable = supplyQueue.peek(supplyReq);
        bool rescueAvailable = rescueQueue.peek(rescueReq);
        if(!supplyAvailable && !rescueAvailable){
            break; //Both queues are empty
        }
        Request* chosenReq = nullptr;
        // If both available, compare scores
        if(supplyAvailable && rescueAvailable){
            int supplyScore = supplyReq.computeEmergencyScore();
            int rescueScore = rescueReq.computeEmergencyScore();
            if(supplyScore > rescueScore){
                chosenReq = &supplyReq;
            }
            else{
                chosenReq = &rescueReq; //Tie or rescue wins
            }
        }
        else if(supplyAvailable){
            chosenReq = &supplyReq;
        }
        else{
            chosenReq = &rescueReq;
        }
        Team& t = teams[idx];
        // Check if team can take the chosen request
        if(!t.tryAssignRequest(*chosenReq)){
            std::cout << "Overload on Team " << teamId << ": rolling back mission." << std::endl;
            t.rollbackMission(supplyQueue, rescueQueue);
            overloaded = true;
            break;
        }
        else{
            totalAssigned++;
            totalWorkload += chosenReq->computeWorkloadContribution();
            if(chosenReq->getType() == "SUPPLY"){
                supplyCount++;
                Request dequeuedReq;
                supplyQueue.dequeue(dequeuedReq);
            }
            else{
                rescueCount++;
                Request dequeuedReq;
                rescueQueue.dequeue(dequeuedReq);
            }
        }
    }
    if(!overloaded){ // Only print if not overloaded
        std::cout << "Team " << teamId << " assigned " << totalAssigned << " requests (" 
                << supplyCount << " SUPPLY, " << rescueCount << " RESCUE), total workload " 
                << totalWorkload << "." << std::endl;
        }
    return true;
}

bool QuakeAssistController::handleDispatchTeam(int teamId) {
    int idx = findTeamIndexById(teamId);
    // check if team exists and has active mission
    if (idx == -1) {
        std::cout << "Error: Team " << teamId << " not found." << std::endl;
        return true;
    }
    Team& t = teams[idx];
    if (!t.hasActiveMission()) {
        std::cout << "Error: Team " << teamId << " has no active mission." << std::endl;
        return true;
    }
    int workload = t.getCurrentWorkload();
    std::cout << "Team " << teamId << " dispatched with workload " << workload << "." << std::endl;
    t.clearMission();
    return true;
}

void QuakeAssistController::printQueues() const {
    //Print queues.
    std::cout << "SUPPLY QUEUE:" << std::endl;
    for(int i = 0; i < supplyQueue.getCount(); i++){
        // Setting variables
        Request* reqs = supplyQueue.getData();
        int frontIdx = supplyQueue.getFrontIndex();
        int capacity = supplyQueue.getCapacity();
        int idx = (frontIdx + i) % capacity;
        const Request& req = reqs[idx];
        std::cout << req.getId() << " " << req.getCity() << " " << req.getSupplyType()
                  << " " << req.getAmount() << " " << req.getEmergencyLevel() << std::endl;
    }
    std::cout << "RESCUE QUEUE:" << std::endl;
    for(int i = 0; i < rescueQueue.getCount(); i++){
        Request* reqs = rescueQueue.getData();
        int frontIdx = rescueQueue.getFrontIndex();
        int capacity = rescueQueue.getCapacity();
        int idx = (frontIdx + i) % capacity;
        const Request& req = reqs[idx];
        std::cout << req.getId() << " " << req.getCity() << " " << req.getNumPeople()
                  << " " << req.getRisk() << " " << req.getEmergencyLevel() << std::endl;
    }
}

void QuakeAssistController::printTeam(int teamId) const {
    //Print team data using teamId.
    int idx = findTeamIndexById(teamId);
    // Check if empty or not found
    if (idx == -1) {
        std::cout << "Error: Team " << teamId << " not found." << std::endl;
        return;
    }
    const Team& t = teams[idx];
    std::cout << "TEAM " << teamId << " STACK:" << std::endl;
    const MissionStack& ms = t.getMissionStack();
    for (int i = ms.size() - 1; i >= 0; i--) {
        const Request& req = ms.getAt(i);
        // Check for request type
        if (req.getType() == "SUPPLY") {
            std::cout << req.getId() << " " << req.getCity() << " " << req.getSupplyType()
                      << " " << req.getAmount() << " " << req.getEmergencyLevel() << std::endl;
        } else if (req.getType() == "RESCUE") {
            std::cout << req.getId() << " " << req.getCity() << " " << req.getNumPeople()
                      << " " << req.getRisk() << " " << req.getEmergencyLevel() << std::endl;
        }
    }
}

void QuakeAssistController::clear() {
    //Clear data.
    // Clear all team missions
    for (int i = 0; i < teamCount; i++) {
        teams[i].clearMission();
    }
    
    // Clear both queues manually
    Request temp;
    while (!supplyQueue.isEmpty()) {
        supplyQueue.dequeue(temp);
    }
    while (!rescueQueue.isEmpty()) {
        rescueQueue.dequeue(temp);
    }
    std::cout << "System cleared." << std::endl;
}
