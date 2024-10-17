#include "EventManager.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <cstdio>  // Para usar sscanf

// Method to read events from a file and store them
void EventManager::read_events(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    std::string line;

    // Read the first line to get the total number of timesteps
    std::getline(file, line);
    std::istringstream first_line(line);
    first_line >> total_timesteps;

    // Optional: Pre-allocate memory for events if the number of events can be estimated
    // This is a placeholder, replace `estimated_number_of_events` with an actual estimate
    events.reserve(100);  // Adjust this value based on the dataset size or calculate dynamically

    // Read the remaining lines for events
    while (std::getline(file, line)) {
        std::string event_type = line.substr(0, line.find(' '));

        int x, y, z, density, timestep;

        if (event_type == "source") {
            // Parse the density and timestep for source event using sscanf
            sscanf(line.c_str(), "source %d %d", &density, &timestep);
            events.emplace_back(density, timestep);
        } else if (event_type == "force") {
            // Parse the vector and timestep for force event using sscanf
            sscanf(line.c_str(), "force %d %d %d %d", &x, &y, &z, &timestep);
            events.emplace_back(x, y, z, timestep);
        } else {
            std::cerr << "Unknown event type: " << event_type << std::endl;
        }
    }

    file.close();
}

// Method to get all events for a specific timestamp
const std::vector<Event> EventManager::get_events_at_timestamp(int timestamp) const {
    std::vector<Event> result;
    for (const auto &event : events) {
        if (event.timestep == timestamp) {
            result.push_back(event);
        }
    }
    return result;  // Consider returning a reference if you can, or use std::move if copying is necessary
}