#include "Team.h"

Team::Team()
    : teamID(-1),
      maxLoadCapacity(0),
      currentWorkload(0),
      missionStack(4) {
}

Team::Team(int id, int maxLoad)
    : teamID(id),
      maxLoadCapacity(maxLoad),
      currentWorkload(0),
      missionStack(4) {
}

int Team::getId() const {
    return teamID;
}

int Team::getMaxLoadCapacity() const {
    return maxLoadCapacity;
}

int Team::getCurrentWorkload() const {
    return currentWorkload;
}

void Team::setId(int id) {
    teamID = id;
}

void Team::setMaxLoadCapacity(int maxLoad) {
    maxLoadCapacity = maxLoad;
}

bool Team::hasActiveMission() const {
    return !missionStack.isEmpty();
}

bool Team::tryAssignRequest(const Request& req) {
    // Check if adding this request would cause overload before pushing
    int workloadContribution = req.computeWorkloadContribution();
    if(currentWorkload + workloadContribution > maxLoadCapacity){
        return false;
    }
    missionStack.push(req);
    currentWorkload += workloadContribution;
    return true;
}

void Team::rollbackMission(RequestQueue& supplyQueue, RequestQueue& rescueQueue) {
    int stackSize = missionStack.size();
    if (stackSize == 0) return;
    
    // Use temporary array to preserve original queue order
    Request* tempArray = new Request[stackSize];
    
    // Pop all requests from stack
    for(int i = 0; i < stackSize; i++){
        missionStack.pop(tempArray[i]);
        currentWorkload -= tempArray[i].computeWorkloadContribution();
    }
    
    // Enqueue in reverse order to maintain original FIFO order in queues
    for(int i = stackSize - 1; i >= 0; i--){
        if(tempArray[i].getType() == "SUPPLY"){
            supplyQueue.enqueue(tempArray[i]);
        } else {
            rescueQueue.enqueue(tempArray[i]);
        }
    }
    
    delete[] tempArray;
}

void Team::clearMission() {
    missionStack.clear();
    currentWorkload = 0;
}

const MissionStack& Team::getMissionStack() const {
    return missionStack;
}
