#ifndef PROJECT_PLANECUTTING_H
#define PROJECT_PLANECUTTING_H

#include <sofa/core/objectmodel/BaseObject.h>
#include <SofaBaseTopology/TetrahedronSetTopologyModifier.h>
#include <SofaBaseTopology/TetrahedronSetTopologyContainer.h>
#include "TetrahedronSetTopologyCuttingAlgorithms.h"

class PlaneCutting : public sofa::core::objectmodel::BaseObject
{
public:
    SOFA_CLASS(PlaneCutting, sofa::core::objectmodel::BaseObject);

    typedef sofa::defaulttype::Vec3dTypes DataTypes;
    typedef typename DataTypes::Coord Coord;

    void init() override;

    void reinit() override;

    void handleEvent(sofa::core::objectmodel::Event *event) override;

    void draw(const sofa::core::visual::VisualParams *vparams) override;

protected:
    PlaneCutting();

private:


    // Time step interval at which the topological changes should happen.
    sofa::core::objectmodel::Data<sofa::defaulttype::Vec3d> m_plane_corner_1;
    sofa::core::objectmodel::Data<sofa::defaulttype::Vec3d> m_plane_corner_2;
    sofa::core::objectmodel::Data<sofa::defaulttype::Vec3d> m_plane_corner_3;


    sofa::core::topology::BaseMeshTopology *m_topology;
    sofa::component::topology::TetrahedronSetTopologyModifier *m_topology_modifier;
    sofa::component::topology::TetrahedronSetTopologyContainer *m_topology_container;
    sofa::component::topology::TetrahedronSetTopologyCuttingAlgorithms<DataTypes> *m_topology_algorithms;
    sofa::core::behavior::MechanicalState<DataTypes> *m_state;

    sofa::defaulttype::Vec3d m_plane_center;
    sofa::defaulttype::Vec3d m_plane_normal;
    sofa::defaulttype::Vec3d m_plane_corner_4;

    sofa::helper::vector<sofa::defaulttype::Vector3> intersections_points;
};


#endif //PROJECT_PLANECUTTING_H
