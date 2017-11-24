#ifndef CUTTING_TETRAHEDRONSETTOPOLOGYCUTTINGALGORITHMS_H
#define CUTTING_TETRAHEDRONSETTOPOLOGYCUTTINGALGORITHMS_H

#include "config.h"

#include <SofaBaseTopology/TetrahedronSetTopologyAlgorithms.h>
#include <SofaBaseTopology/TetrahedronSetTopologyContainer.h>

namespace sofa
{

namespace component
{

namespace topology
{

template<class DataTypes>
class TetrahedronSetTopologyCuttingAlgorithms : public TetrahedronSetTopologyAlgorithms<DataTypes>
{
public:
    SOFA_CLASS(SOFA_TEMPLATE(TetrahedronSetTopologyCuttingAlgorithms, DataTypes),
               SOFA_TEMPLATE(TetrahedronSetTopologyAlgorithms, DataTypes));

    typedef typename DataTypes::Coord Coord;
    typedef typename DataTypes::VecCoord VecCoord;
    typedef core::topology::BaseMeshTopology::Tetra Tetra;

    TetrahedronSetTopologyCuttingAlgorithms() : TetrahedronSetTopologyAlgorithms<DataTypes>()
    {}

    void init() override;

/**
 * @brief Subdivide tetrahedron #tetra_id with the intersection of the parallelogram defined by the triangle p0 p1 p2. The triangle's hypotenuse must be the vector p0p2.
 * @param[in] t Tetrahedron to cut
 * @param[in] p0 First corner of the triangle used to build the parallelogram.
 * @param[in] p1 Second corner of the triangle used to build the parallelogram. This corner must be at the opposite of the hypotenuse.
 * @param[in] p2 Third corner of the triangle used to build the parallelogram.
 * @param[out] tetrahedrons Vector of the newly created tetrahedrons.
 *
 * @return The number of new tetrahedrons created.
 *
 * p1      p2
 *  +------+.
 *   `.      `.
 *     `+--+---+
 *      p0       p3 (automatic)
 */
    unsigned int subDivideTetrahedronWithParallelogram(unsigned int tetra_id,
                                                       const Coord p0,
                                                       const Coord p1,
                                                       const Coord p2,
                                                       std::vector<Coord> &intersections_points) noexcept;

    unsigned int subDivideTetrahedronsWithParallelogram(
                                                       const Coord & p0,
                                                       const Coord & p1,
                                                       const Coord & p2,
                                                       sofa::helper::vector<Coord> &intersections_points) noexcept;

    bool RayIntersectsParallelogram(
        const Coord p0, const Coord p1, const Coord p2,
        const Coord & R0, const Coord & R1,
        Coord & intersection) const;


protected:

    struct intersection_t
    {
        unsigned int tetra_id;
        const sofa::core::topology::Topology::EdgeID edge_id;
        const Coord position;
    };

    sofa::core::behavior::MechanicalState<DataTypes> *m_state;
};

#if defined(SOFA_EXTERN_TEMPLATE) && !defined(SOFA_COMPONENT_FORCEFIELD_PARTICLESREPULSIONFORCEFIELD_CPP)

#ifndef SOFA_FLOAT
extern template class PLANECUTTING_API TetrahedronSetTopologyCuttingAlgorithms<sofa::defaulttype::Vec3dTypes>;
#endif

#endif // defined(SOFA_EXTERN_TEMPLATE) && !defined(SOFA_COMPONENT_FORCEFIELD_PARTICLESREPULSIONFORCEFIELD_CPP)

} // namespace topology

} // namespace component

} // namespace sofa

#endif //CUTTING_TETRAHEDRONSETTOPOLOGYCUTTINGALGORITHMS_H
