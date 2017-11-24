#ifndef SOFA_COMPONENT_TOPOLOGY_TETRAHEDRONSETTOPOLOGYCUTTINGALGORITHMS_INL
#define SOFA_COMPONENT_TOPOLOGY_TETRAHEDRONSETTOPOLOGYCUTTINGALGORITHMS_INL

#include "TetrahedronSetTopologyCuttingAlgorithms.h"
#include <SofaBaseTopology/TetrahedronSetTopologyAlgorithms.inl>

namespace sofa
{

namespace component
{

namespace topology
{

template<class DataTypes>
void TetrahedronSetTopologyCuttingAlgorithms<DataTypes>::init()
{
    TetrahedronSetTopologyAlgorithms<DataTypes>::init();
    m_state = static_cast<sofa::core::behavior::MechanicalState<DataTypes> *> (this->getContext()->getMechanicalState());
    if (!m_state) {
        serr << "No mechanical state found while initialize the TetrahedronSetTopologyCuttingAlgorithms component"
             << sendl;
    }
}

template<class DataTypes>
unsigned int TetrahedronSetTopologyCuttingAlgorithms<DataTypes>::subDivideTetrahedronWithParallelogram(
    unsigned int tetra_id,
    const Coord p0, const Coord p1,
    const Coord p2,
    std::vector<Coord> &intersections_points) noexcept
{
/*
 *     p1   (e2)   p2
 *      +---------+.
 *  (e1) `.         `.
 *         `+---------+
 *          p0        p3 (automatic)
 */

    // Locate the last corner of the parallelogram
    const Coord
        e1 = p0 - p1, // Unit vector base e1 of parallelogram
        e2 = p2 - p1, // Unit vector base e2 of parallelogram
        n = (e1.cross(e2)).normalized(); // normal = e1 X e2

    if (this->m_state && this->m_container && this->m_modifier) {
        const auto &positions = m_state->readPositions();
        std::list<intersection_t> intersections;

        sofa::core::topology::BaseMeshTopology::EdgesInTetrahedron edges = this->m_container->getEdgesInTetrahedron(
            tetra_id);

        for (sofa::core::topology::Topology::EdgeID edge_id : edges) {
            const Coord
                &edge_p0 = positions[this->m_container->getEdge(edge_id)[0]], // Tetrahedron's edge first vertex
                &edge_p1 = positions[this->m_container->getEdge(edge_id)[1]], // Tetrahedron's edge second vertex
                edge = edge_p1 - edge_p0; // Tetrahedron's edge

            Coord intersection;
            if (RayIntersectsParallelogram(p0, p1, p2, edge_p0, edge_p1, intersection)) {
                intersections.push_back({tetra_id, edge_id, intersection});
                intersections_points.push_back(intersection);
            }
        }

        // Only register the intersection points if the parallelogram cut the entire tetrahedron
        if (intersections.size() > 2) {
            VecCoord intersectedPointsInTetra;
            sofa::helper::vector<sofa::core::topology::Topology::EdgeID> intersectedEdgeIDInTetra;
            std::cout << "\n------------------------------\nCutting tetrahedron #" << tetra_id << " [ ";
            for (const auto &point_id : this->m_container->getTetrahedron(tetra_id)) {
                std::cout << "(" << positions[point_id] << ") ";
            }
            std::cout << "]\nIntersected points [ ";
            for (const intersection_t &t : intersections) {
                intersectedPointsInTetra.push_back(t.position);
                intersectedEdgeIDInTetra.push_back(t.edge_id);
                std::cout << "(" << t.position << ") ";
            }
            std::cout << "]\n------------------------------\n";
            this->subDivideRestTetrahedronsWithPlane(intersectedPointsInTetra, intersectedEdgeIDInTetra, p1, n);
        }
    }

    return 0;
}

template<class DataTypes>
unsigned int TetrahedronSetTopologyCuttingAlgorithms<DataTypes>::subDivideTetrahedronsWithParallelogram(
    const Coord & p0, const Coord & p1,
    const Coord & p2,
    sofa::helper::vector<Coord> &intersections) noexcept
{
    if (!this->m_state || !this->m_container || !this->m_modifier)
        return 0;

    // Locate the last corner of the parallelogram
    const Coord
        e1 = p0 - p1, // Unit vector base e1 of parallelogram
        e2 = p2 - p1, // Unit vector base e2 of parallelogram
        n = (e1.cross(e2)).normalized(); // normal = e1 X e2

    VecCoord intersectedPoints;
    sofa::helper::vector<sofa::core::topology::Topology::EdgeID> intersectedEdges;
    const auto &positions = m_state->readPositions();

    for (unsigned int edge_id = 0; edge_id < this->m_container->getNumberOfEdges(); ++edge_id) {

        const Coord
            &edge_p0 = positions[this->m_container->getEdge(edge_id)[0]], // Tetrahedron's edge first vertex
            &edge_p1 = positions[this->m_container->getEdge(edge_id)[1]], // Tetrahedron's edge second vertex
            edge = edge_p1 - edge_p0; // Tetrahedron's edge

        Coord intersection;
        if (RayIntersectsParallelogram(p0, p1, p2, edge_p0, edge_p1, intersection)) {
            intersections.push_back(intersection);
            intersectedEdges.push_back(edge_id);
        }
    }

    if (intersections.size() > 0 && intersections.size() == intersectedEdges.size()) {
        this->subDivideTetrahedronsWithPlane(intersections, intersectedEdges, p1, n);
        return 1;
    } else {
        msg_error() << "Mismatch";
        return 0;
    }
}

template<class DataTypes>
bool TetrahedronSetTopologyCuttingAlgorithms<DataTypes>::RayIntersectsParallelogram(
    const Coord p0, const Coord p1, const Coord p2,
    const Coord & R0, const Coord & R1,
    Coord & intersection) const
{
    const Coord
        Rd = (R1-R0).normalized(),
        A = p1,
        AB = p0-p1,
        AC = p2-p1,
        distV = A-R0,
        N =  AB.cross(AC);

    const double nr = N * Rd;

    if (fabs(nr) < 0.01) return false;
    const double ir = (N * distV) / nr;
    if (ir < 0.0 || ir > (R1-R0).norm()) return false;
    const double iu = -((distV.cross(AC)) * Rd) / nr;
    const double iv = -((AB.cross(distV)) * Rd) / nr;
    intersection = R0 + Rd*ir;
    return (iu >= 0 && iv >= 0 && iu <= 1 && iv <= 1);
}

} // namespace topology

} // namespace component

} // namespace sofa

#endif // SOFA_COMPONENT_TOPOLOGY_TETRAHEDRONSETTOPOLOGYCUTTINGALGORITHMS_INL