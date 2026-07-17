#include "NxGeometryUtils.h"

#include <NXOpen/BasePart.hxx>
#include <NXOpen/Body.hxx>
#include <NXOpen/Part.hxx>
#include <NXOpen/TaggedObject.hxx>
#include <NXOpen/UF/UFSession.hxx>
#include <NXOpen/Assemblies/Component.hxx>
#include <NXOpen/Assemblies/ComponentAssembly.hxx>

using namespace CadImport::Core;

namespace NxContracts
{
    BoundingBox3D ComputeBoundingBoxForTag(NXOpen::TaggedObject* object)
    {
        BoundingBox3D box;

        if (object == nullptr)
        {
            return box;
        }

        try
        {
            NXOpen::UF::UFSession* ufSession = NXOpen::UF::UFSession::GetUFSession();
            double range[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
            ufSession->Modl.AskBoundingBox(object->Tag(), range);

            box.min = { range[0], range[1], range[2] };
            box.max = { range[3], range[4], range[5] };
            box.valid = true;
        }
        catch (...)
        {
            box.valid = false;
        }

        return box;
    }

    std::vector<NXOpen::Body*> BodiesOfComponent(NXOpen::Assemblies::Component* component)
    {
        std::vector<NXOpen::Body*> bodies;

        if (component == nullptr)
        {
            return bodies;
        }

        // TODO(office-PC verify): see the header's "Assembly-wide Body
        // traversal" note - Prototype() is assumed to give (or be
        // dynamic_cast-able to) the BasePart this component instances.
        NXOpen::BasePart* referencedPart = dynamic_cast<NXOpen::BasePart*>(component->Prototype());
        if (referencedPart != nullptr)
        {
            bodies = referencedPart->Bodies();
        }

        return bodies;
    }

    std::vector<NXOpen::Body*> CollectAllBodies(NXOpen::Assemblies::Component* component)
    {
        std::vector<NXOpen::Body*> bodies = BodiesOfComponent(component);

        if (component == nullptr)
        {
            return bodies;
        }

        for (NXOpen::Assemblies::Component* child : component->GetChildren())
        {
            std::vector<NXOpen::Body*> childBodies = CollectAllBodies(child);
            bodies.insert(bodies.end(), childBodies.begin(), childBodies.end());
        }

        return bodies;
    }

    NXOpen::Assemblies::Component* FindComponentByName(NXOpen::Assemblies::Component* component, const std::string& targetName)
    {
        if (component == nullptr)
        {
            return nullptr;
        }

        if (component->Name() == targetName)
        {
            return component;
        }

        for (NXOpen::Assemblies::Component* child : component->GetChildren())
        {
            NXOpen::Assemblies::Component* found = FindComponentByName(child, targetName);
            if (found != nullptr)
            {
                return found;
            }
        }

        return nullptr;
    }

    std::vector<NXOpen::Body*> CollectAllBodiesInWorkPart(NXOpen::BasePart* workPart)
    {
        if (workPart == nullptr)
        {
            return {};
        }

        // TODO(office-PC verify): ComponentAssembly() is exposed on Part
        // (not BasePart) in most NX versions - same caveat already noted
        // in NxAssemblyReader::ReadTree.
        NXOpen::Part* part = dynamic_cast<NXOpen::Part*>(workPart);
        NXOpen::Assemblies::ComponentAssembly* assembly = (part != nullptr) ? part->ComponentAssembly() : nullptr;
        NXOpen::Assemblies::Component* root = (assembly != nullptr) ? assembly->RootComponent() : nullptr;

        if (root != nullptr)
        {
            return CollectAllBodies(root);
        }

        // Non-assembly (single-part) bookmark - the work part IS the part
        // with the actual geometry.
        return workPart->Bodies();
    }
}
