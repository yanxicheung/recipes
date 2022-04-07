#ifndef INCL_OOT_InterfaceDef_20131101170006_H
#define INCL_OOT_InterfaceDef_20131101170006_H

namespace details
{
    template <typename T>
    struct Interface
    {
        virtual ~Interface() {}
    };
}

#define DEFINE_INTERFACE(role)  struct role : ::details::Interface<role>

#endif

