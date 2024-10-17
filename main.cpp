#include "EventManager.h"
#include "fluid_solver.h"
#include <iostream>
#include <vector>
#include <algorithm>

#define SIZE 42

#define IX(i, j, k) ((i) + (M + 2) * (j) + (M + 2) * (N + 2) * (k))

// Globals for the grid size
static const int M = SIZE;
static const int N = SIZE;
static const int O = SIZE;
static const float dt = 0.1f;      // Time delta
static const float diff = 0.0001f; // Diffusion constant
static const float visc = 0.0001f; // Viscosity constant

// Fluid simulation arrays
static std::vector<float> u, v, w, u_prev, v_prev, w_prev;
static std::vector<float> dens, dens_prev;

// Function to allocate simulation data
bool allocate_data() {
  int size = (M + 2) * (N + 2) * (O + 2);
  u.resize(size);
  v.resize(size);
  w.resize(size);
  u_prev.resize(size);
  v_prev.resize(size);
  w_prev.resize(size);
  dens.resize(size);
  dens_prev.resize(size);
  
  return true; // Always returns true since vector handles allocation
}

// Function to clear the data (set all to zero)
void clear_data() {
  std::fill(u.begin(), u.end(), 0.0f);
  std::fill(v.begin(), v.end(), 0.0f);
  std::fill(w.begin(), w.end(), 0.0f);
  std::fill(u_prev.begin(), u_prev.end(), 0.0f);
  std::fill(v_prev.begin(), v_prev.end(), 0.0f);
  std::fill(w_prev.begin(), w_prev.end(), 0.0f);
  std::fill(dens.begin(), dens.end(), 0.0f);
  std::fill(dens_prev.begin(), dens_prev.end(), 0.0f);
}

// Apply events (source or force) for the current timestep
void apply_events(const std::vector<Event> &events) {
  int i = M / 2, j = N / 2, k = O / 2; // Compute the index once
  for (const auto &event : events) {
    if (event.type == ADD_SOURCE) {
      dens[IX(i, j, k)] = event.density;
    } else if (event.type == APPLY_FORCE) {
      u[IX(i, j, k)] = event.force.x;
      v[IX(i, j, k)] = event.force.y;
      w[IX(i, j, k)] = event.force.z;
    }
  }
}

// Function to sum the total density
float sum_density() {
  return std::accumulate(dens.begin(), dens.end(), 0.0f);
}

// Simulation loop
void simulate(EventManager &eventManager, int timesteps) {
  for (int t = 0; t < timesteps; t++) {
    // Get the events for the current timestep
    std::vector<Event> events = eventManager.get_events_at_timestamp(t);

    // Apply events to the simulation
    apply_events(events);

    // Perform the simulation steps
    vel_step(M, N, O, u.data(), v.data(), w.data(), u_prev.data(), v_prev.data(), w_prev.data(), visc, dt);
    dens_step(M, N, O, dens.data(), dens_prev.data(), u.data(), v.data(), w.data(), diff, dt);
  }
}

int main() {
  // Initialize EventManager
  EventManager eventManager;
  eventManager.read_events("events.txt");

  // Get the total number of timesteps from the event file
  int timesteps = eventManager.get_total_timesteps();

  // Allocate and clear data
  if (!allocate_data())
    return -1;
  clear_data();

  // Run simulation with events
  simulate(eventManager, timesteps);

  // Print total density at the end of simulation
  float total_density = sum_density();
  std::cout << "Total density after " << timesteps
            << " timesteps: " << total_density << std::endl;

  return 0;
}