#pragma once

// Shared by CadImportModule's NxGeometryReader and RoiModule's
// NxRoiResolver - lives in Shared/NxContracts (not either module's own
// NxBackend) since both NX-dependent modules need it equally.

#include <string>
#include <vector>

#include "Core/Models/BoundingBox3D.h"

namespace NXOpen
{
    class TaggedObject;
    class BasePart;
    class Body;

    namespace Assemblies
    {
        class Component;
    }
}

namespace NxContracts
{
    // Works for any taggable NX entity (Body, Face, ...) via
    // TaggedObject::Tag() - callers just need a pointer to the specific
    // subtype, since Body/Face/etc. all derive from TaggedObject.
    //
    // TODO(office-PC verify): UF_MODL function name/signature (candidates:
    // AskBoundingBox, AskBoundingBoxExact) against the installed NX2406 SDK.
    CadImport::Core::BoundingBox3D ComputeBoundingBoxForTag(NXOpen::TaggedObject* object);

    // --- Assembly-wide Body traversal -----------------------------------
    //
    // Confirmed (by the project owner, not NX headers - see
    // docs/OfficeVerificationChecklist.md Phase 2-A): opening a BLU
    // assembly via bookmark gives a work part that is itself an empty
    // container - it owns no Bodies of its own. Each real Body (Reflector/
    // LGP/Diffuser/Prism/Bezel/...) lives in the individual Part that each
    // Component instances. So `workPart->Bodies()` alone (the pattern both
    // NxGeometryReader and the original NxRoiResolver used) can return an
    // empty list for a typical assembly bookmark - these functions replace
    // that pattern.
    //
    // TODO(office-PC verify): Component::Prototype() is assumed to return
    // (or be safely dynamic_cast-able to) the NXOpen::BasePart* actually
    // referenced by that component instance. If NX2406 requires an extra
    // hop instead (e.g. Prototype()->OwningPart()), adjust BodiesOfComponent
    // only - every other function here is written in terms of it.

    // Bodies owned directly by this component's own referenced Part - NOT
    // recursive into children. Empty if the component has no resolvable
    // referenced Part.
    std::vector<NXOpen::Body*> BodiesOfComponent(NXOpen::Assemblies::Component* component);

    // BodiesOfComponent() for `component` and every descendant, combined.
    std::vector<NXOpen::Body*> CollectAllBodies(NXOpen::Assemblies::Component* component);

    // Depth-first search (including `component` itself) for the first
    // descendant whose Name() == targetName. Returns nullptr if not found.
    NXOpen::Assemblies::Component* FindComponentByName(NXOpen::Assemblies::Component* component, const std::string& targetName);

    // Every Body reachable from a work part, whether it's an assembly
    // (walks ComponentAssembly - see CollectAllBodies) or a single
    // non-assembly part (just Bodies() directly, since then the work part
    // itself IS the part with the actual geometry).
    std::vector<NXOpen::Body*> CollectAllBodiesInWorkPart(NXOpen::BasePart* workPart);
}
