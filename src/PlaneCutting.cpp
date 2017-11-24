#include "PlaneCutting.h"
#include <sofa/simulation/AnimateBeginEvent.h>
#include <sofa/core/objectmodel/KeypressedEvent.h>
#include <sofa/core/ObjectFactory.h>
#include <sofa/core/visual/VisualParams.h>
#include "TetrahedronSetTopologyCuttingAlgorithms.inl"

PlaneCutting::PlaneCutting()
    : m_plane_corner_1(initData(&m_plane_corner_1, sofa::defaulttype::Vector3(1, 0, 0), "plane_corner_1",
                                "Plane's first corner position. The diagonal must be between this point and the third."))
    , m_plane_corner_2(initData(&m_plane_corner_2, sofa::defaulttype::Vector3(0, 0, 0), "plane_corner_2",
                                "Plane's second corner position.")), m_plane_corner_3(
        initData(&m_plane_corner_3, sofa::defaulttype::Vector3(0, 1, 0), "plane_corner_3",
                 "Plane's third corner position. The diagonal must be between this point and the first."))
{
    this->f_listening.setValue(true);
}

void PlaneCutting::init()
{
    reinit();
}

void PlaneCutting::reinit()
{
    this->f_listening.setValue(true);

    sofa::component::topology::PointSetTopologyAlgorithms<DataTypes> *algo;
    std::string tname;
    m_topology = this->getContext()->getMeshTopology();
    if (!m_topology) {
        serr << "The scene must have a mesh Topology element" << sendl;
        goto error;
    }

    m_topology->getContext()->get(m_topology_container);
    m_topology->getContext()->get(m_topology_modifier);
    m_topology->getContext()->get(m_topology_algorithms);
    m_topology->getContext()->get(algo);
    tname = algo->getTemplateName();
    std::cout << tname << std::endl;
    m_topology->getContext()->get(m_state);

    if (!m_topology_container) {
        serr << "The scene must have at least a TetrahedronSetTopologyContainer element" << sendl;
        goto error;
    }

    if (m_topology_container->getNumberOfTetrahedra() == 0) {
        serr << "No tetrahedron to remove" << sendl;
        goto error;
    }

    if (!m_topology_modifier) {
        serr << "The scene must have at least a TetrahedronSetTopologyModifier element" << sendl;
        goto error;
    }

    if (!m_topology_algorithms) {
        serr << "The scene must have at least a TetrahedronSetTopologyAlgorithms element" << sendl;
        goto error;
    }

    if (!m_state) {
        serr << "The scene must have at least a MechanicalObject element" << sendl;
        goto error;
    }
    // Locate the center of the plane
    m_plane_center = m_plane_corner_1.getValue() + (m_plane_corner_3.getValue() - m_plane_corner_1.getValue()) / 2;
    m_plane_corner_4 = m_plane_corner_2.getValue() + (m_plane_center - m_plane_corner_2.getValue()) * 2;
    m_plane_normal = (m_plane_corner_3.getValue() - m_plane_corner_2.getValue()).cross(
        m_plane_corner_1.getValue() - m_plane_corner_2.getValue());

    m_plane_normal.normalize();

    std::cout << "Corner 1 : " << m_plane_corner_1.getValue() << std::endl;
    std::cout << "Corner 2 : " << m_plane_corner_2.getValue() << std::endl;
    std::cout << "Corner 3 : " << m_plane_corner_3.getValue() << std::endl;
    std::cout << "Corner 4 : " << m_plane_corner_4 << std::endl;
    std::cout << "Center   : " << m_plane_center << std::endl;
    std::cout << "Normal   : " << m_plane_normal << std::endl;

    goto success;

    error:
    printf("An error occured while initialize the PlaneCutting component.\n");
    return;

    success:
    printf("PlaneCutting is initialized.\n");
}

void PlaneCutting::handleEvent(sofa::core::objectmodel::Event *event)
{
    // If the event is an KeypressedEvent
    if (sofa::core::objectmodel::KeypressedEvent::checkEventType(event)) {
        sofa::component::topology::TetrahedronSetTopologyCuttingAlgorithms<sofa::defaulttype::Vec3dTypes> *algo =
            static_cast<sofa::component::topology::TetrahedronSetTopologyCuttingAlgorithms<sofa::defaulttype::Vec3dTypes> *>(m_topology_algorithms);
        sofa::core::objectmodel::KeypressedEvent *e = static_cast<sofa::core::objectmodel::KeypressedEvent *> (event);
        switch (e->getKey()) {
            case 18: // left
                m_plane_corner_1.setValue(m_plane_corner_1.getValue() + sofa::defaulttype::Vector3(-0.1, 0, 0));
                m_plane_corner_2.setValue(m_plane_corner_2.getValue() + sofa::defaulttype::Vector3(-0.1, 0, 0));
                m_plane_corner_3.setValue(m_plane_corner_3.getValue() + sofa::defaulttype::Vector3(-0.1, 0, 0));
                m_plane_corner_4 += sofa::defaulttype::Vector3(-0.1, 0, 0);
                break;
            case 19: // up
                m_plane_corner_1.setValue(m_plane_corner_1.getValue() + sofa::defaulttype::Vector3(0, 0.1, 0));
                m_plane_corner_2.setValue(m_plane_corner_2.getValue() + sofa::defaulttype::Vector3(0, 0.1, 0));
                m_plane_corner_3.setValue(m_plane_corner_3.getValue() + sofa::defaulttype::Vector3(0, 0.1, 0));
                break;
            case 20: // right
                m_plane_corner_1.setValue(m_plane_corner_1.getValue() + sofa::defaulttype::Vector3(0.1, 0, 0));
                m_plane_corner_2.setValue(m_plane_corner_2.getValue() + sofa::defaulttype::Vector3(0.1, 0, 0));
                m_plane_corner_3.setValue(m_plane_corner_3.getValue() + sofa::defaulttype::Vector3(0.1, 0, 0));
                break;
            case 21: // down
                m_plane_corner_1.setValue(m_plane_corner_1.getValue() + sofa::defaulttype::Vector3(0, -0.1, 0));
                m_plane_corner_2.setValue(m_plane_corner_2.getValue() + sofa::defaulttype::Vector3(0, -0.1, 0));
                m_plane_corner_3.setValue(m_plane_corner_3.getValue() + sofa::defaulttype::Vector3(0, -0.1, 0));
                break;
            case 67: // c
                intersections_points.clear();
                algo->subDivideTetrahedronsWithParallelogram(
                    m_plane_corner_1.getValue(),
                    m_plane_corner_2.getValue(),
                    m_plane_corner_3.getValue(),
                    intersections_points);
                break;
            case 74: // j
                m_plane_corner_1.setValue(m_plane_corner_1.getValue() + sofa::defaulttype::Vector3(0, 0, -0.1));
                m_plane_corner_2.setValue(m_plane_corner_2.getValue() + sofa::defaulttype::Vector3(0, 0, -0.1));
                m_plane_corner_3.setValue(m_plane_corner_3.getValue() + sofa::defaulttype::Vector3(0, 0, -0.1));
                break;
            case 75: // k
                m_plane_corner_1.setValue(m_plane_corner_1.getValue() + sofa::defaulttype::Vector3(0, 0, 0.1));
                m_plane_corner_2.setValue(m_plane_corner_2.getValue() + sofa::defaulttype::Vector3(0, 0, 0.1));
                m_plane_corner_3.setValue(m_plane_corner_3.getValue() + sofa::defaulttype::Vector3(0, 0, 0.1));
                break;
        }
        m_plane_center = m_plane_corner_1.getValue() + (m_plane_corner_3.getValue() - m_plane_corner_1.getValue()) / 2;
        m_plane_corner_4 = m_plane_corner_2.getValue() + (m_plane_center - m_plane_corner_2.getValue()) * 2;
    }

}

void PlaneCutting::draw(const sofa::core::visual::VisualParams *vparams)
{
    double size = (m_plane_corner_1.getValue() - m_plane_corner_2.getValue()).norm();

    vparams->drawTool()->setLightingEnabled(true); //Enable lightning
    vparams->drawTool()->drawQuads(
        {m_plane_corner_1.getValue(), m_plane_corner_2.getValue(), m_plane_corner_3.getValue(), m_plane_corner_4},
        sofa::defaulttype::Vec<4, float>(1, 0, 0, 1));
    vparams->drawTool()->drawArrow(m_plane_center, m_plane_center + m_plane_normal * size / 4.0, (float) (size * 0.01),
                                   sofa::defaulttype::Vec<4, float>(0, 0, 1, 1));

    vparams->drawTool()->drawPoints(intersections_points, 10.f, sofa::defaulttype::Vec4f(0, 1, 0, 1));

    vparams->drawTool()->setLightingEnabled(false); //Disable lightning
}

SOFA_DECL_CLASS(PlaneCutting)
int PlaneCuttingClass = sofa::core::RegisterObject("Simple plane cutting component")
    .add<PlaneCutting>();