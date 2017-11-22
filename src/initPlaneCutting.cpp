#include <sofa/helper/system/config.h>

#ifndef WIN32
#define SOFA_EXPORT_DYNAMIC_LIBRARY
#define SOFA_IMPORT_DYNAMIC_LIBRARY
#define SOFA_PLANECUTTINGPLUGIN_API
#else
#ifdef SOFA_BUILD_PLANECUTTINGPLUGIN
#define SOFA_PLANECUTTINGPLUGIN_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#define SOFA_PLANECUTTINGPLUGIN_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif
#endif

namespace sofa
{

namespace component
{

//Here are just several convenient functions to help user to know what contains the plugin

extern "C" {
SOFA_PLANECUTTINGPLUGIN_API void initExternalModule();
SOFA_PLANECUTTINGPLUGIN_API const char *getModuleName();
SOFA_PLANECUTTINGPLUGIN_API const char *getModuleVersion();
SOFA_PLANECUTTINGPLUGIN_API const char *getModuleLicense();
SOFA_PLANECUTTINGPLUGIN_API const char *getModuleDescription();
SOFA_PLANECUTTINGPLUGIN_API const char *getModuleComponentList();
}

void initExternalModule()
{
    static bool first = true;
    if (first) {
        first = false;
    }
}

const char *getModuleName()
{
    return "Plugin Plane Cutting";
}

const char *getModuleVersion()
{
    return "alpa 1.0";
}

const char *getModuleLicense()
{
    return "LGPL";
}

const char *getModuleDescription()
{
    return "Cutting with SOFA Framework's connectivity";
}

const char *getModuleComponentList()
{
    return "controllers ";
}

}

}