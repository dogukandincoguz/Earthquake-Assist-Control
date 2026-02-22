# QuakeAssist — Earthquake Emergency Response System

A C++ simulation of a disaster response coordination system. The program manages incoming supply and rescue requests during an earthquake, assigns them to response teams according to emergency priority, and handles workload balancing with automatic rollback support.

## Overview

The system processes a series of commands from standard input and coordinates two types of emergency requests:

- **Supply Requests** — Delivery of critical resources (water, food, tent, etc.) to affected cities.
- **Rescue Requests** — Deployment of rescue personnel to cities based on the number of people at risk and danger level.

Requests are held in separate priority queues and assigned to teams based on emergency score. If a team's workload capacity would be exceeded during assignment, the system performs a full rollback and returns unassigned requests to their original queues.

## Data Structures

| Class | Structure | Purpose |
|---|---|---|
| `RequestQueue` | Circular array queue | Stores pending supply/rescue requests in FIFO order |
| `MissionStack` | Dynamic array stack | Holds requests assigned to a team for the current mission |
| `Team` | — | Represents a response team with a workload capacity |
| `Request` | — | Encapsulates a single supply or rescue request with all metadata |
| `QuakeAssistController` | — | Parses commands and orchestrates all components |

## Supported Commands

| Command | Description |
|---|---|
| `INIT_TEAMS <n>` | Initialize `n` response teams |
| `SET_TEAM_CAPACITY <id> <cap>` | Set maximum workload capacity for a team |
| `ADD_SUPPLY <id> <city> <type> <amount> <lvl>` | Add a supply request to the supply queue |
| `ADD_RESCUE <id> <city> <people> <risk> <lvl>` | Add a rescue request to the rescue queue |
| `REMOVE_REQUEST <id>` | Remove a pending request by ID |
| `HANDLE_EMERGENCY <teamId> <maxRequests>` | Assign up to N requests to a team (with rollback on overflow) |
| `DISPATCH_TEAM <teamId>` | Mark team's current mission as complete and clear it |
| `PRINT_QUEUES` | Print current contents of both queues |
| `PRINT_TEAM <teamId>` | Print current mission stack of a team |
| `CLEAR` | Clear all queues and team missions |

## Building & Running

Requires **g++** with C++11 support or later.

```bash
# Compile
g++ -std=c++11 -Wall -o quake_assist main.cpp MissionStack.cpp QuakeAssistController.cpp Request.cpp RequestQueue.cpp Team.cpp

# Run with input file
./quake_assist < example_input.txt
```

## Project Structure

```
.
├── main.cpp                    # Entry point — reads commands from stdin
├── Request.h / Request.cpp     # Request data model (supply & rescue)
├── RequestQueue.h / .cpp       # Circular array queue for requests
├── MissionStack.h / .cpp       # Dynamic array stack for team missions
├── Team.h / Team.cpp           # Team with workload tracking and rollback
├── QuakeAssistController.h/.cpp# Command parser and system orchestrator
└── example_input.txt           # Sample input for testing
```
## License

This project is part of the BBM203 Data Structures course.
