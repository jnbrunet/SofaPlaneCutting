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
#ifndef SOFA_FLOAT
    .add<TetrahedronSetTopologyCuttingAlgorithms<Vec3dTypes> >(true) // default template
#endif
;

#ifndef SOFA_FLOAT
template class PLANECUTTING_API TetrahedronSetTopologyCuttingAlgorithms<Vec3dTypes>;
#endif

PointSetTopologyAlgorithms<Vec3dTypes> *a;
} // namespace topology

} // namespace component

} // namespace sofa
