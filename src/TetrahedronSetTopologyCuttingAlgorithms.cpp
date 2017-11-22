#include <sofa/core/ObjectFactory.h>
#include "TetrahedronSetTopologyCuttingAlgorithms.inl"

namespace sofa
{

namespace component
{

namespace topology
{
using namespace sofa::defaulttype;
SOFA_DECL_CLASS(TetrahedronSetTopologyCuttingAlgorithms)
int TetrahedronSetTopologyCuttingAlgorithmsClass = core::RegisterObject("Tetrahedron set topology cutting algorithms")
    .add<TetrahedronSetTopologyCuttingAlgorithms<Vec3dTypes> >(true) // default template
;

template
class SOFA_BASE_TOPOLOGY_API TetrahedronSetTopologyCuttingAlgorithms<Vec3dTypes>;

PointSetTopologyAlgorithms<Vec3dTypes> *a;
} // namespace topology

} // namespace component

} // namespace sofa