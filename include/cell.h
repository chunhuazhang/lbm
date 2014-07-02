#pragma once

#include "collision.h"
#include <memory>

namespace lbm
{

template<typename lattice_model> class Collision;

template<typename lattice_model>
class Cell
{
    double_array<lattice_model::Q> pdf;
    Collision<lattice_model>* collision;
public:
    explicit Cell(Collision<lattice_model>* collision);

    /**
     * Indicates whether this cell is a fluid cell.
     */
    bool is_fluid() const;

    /**
     * Returns i-th distribution function of the current cell.
     */
    double& operator[](size_t index);

    /**
     * Returns i-th distribution function of the current cell (const version)
     */
    const double& operator[](size_t index) const;

    /**
     * Performs a collision step for the current cell.
     * @lattice_position (x,y,z) position of this cell.
     */
    void collide(const uint_array<lattice_model::D>& lattice_position);

    /**
     * Computes density of this cell.
     */
    double density() const;

    /**
     * Computes velocity of this cell.
     */
    double_array<lattice_model::D> velocity(double density) const;

    /**
     * Computes equilibrium function of this cell.
     */
    double_array<lattice_model::Q> equilibrium(double density, const double_array<lattice_model::D>& velocity) const;

    /**
     * Sets the collision handler for this cell.
     * This may be used to apply a boundary condition on this cell.
     */
    void set_collision_handler(Collision<lattice_model>* collision);

    /**
     * Currently installed collision handler.
     */
    auto get_collision_handler() const -> decltype(collision);

    bool has_fluid_vicinity(const Domain<lattice_model>& domain, const uint_array<lattice_model::D>& position) const {
        auto x = position[0];
        auto y = position[1];
        auto z = position[2];

        for (auto q = 0u; q < lattice_model::Q; ++q) {
            auto dx = lattice_model::velocities[q][0];
            auto dy = lattice_model::velocities[q][1];
            auto dz = lattice_model::velocities[q][2];
            if (domain.in_bounds(x + dx, y + dy, z + dz)
                    && domain.cell(x + dx, y + dy, z + dz).is_fluid()) {
                return true;
            }
        }
        return false;
    }
};

} //namespace lbm

#include "cell.hpp"