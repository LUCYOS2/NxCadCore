#include "NxBackend/NxGeometryReader.h"

#include <vector>

#include <NXOpen/Session.hxx>
#include <NXOpen/BasePart.hxx>
#include <NXOpen/Part.hxx>
#include <NXOpen/Body.hxx>
#include <NXOpen/Face.hxx>
#include <NXOpen/Edge.hxx>
#include <NXOpen/NXException.hxx>
#include <NXOpen/Assemblies/Component.hxx>
#include <NXOpen/Assemblies/ComponentAssembly.hxx>

#include "NxContracts/NxGeometryUtils.h"

using namespace CadImport::Core;

namespace CadImport::NxBackend
{
    NxGeometryReader::NxGeometryReader(NxConnector* connector, Core::ILogger* logger)
        : m_connector(connector), m_logger(logger)
    {
    }

    OperationResult<GeometryInfo> NxGeometryReader::ReadGeometry(const std::string& componentName)
    {
        if (m_connector == nullptr || !m_connector->IsAvailable())
        {
            return OperationResult<GeometryInfo>::Fail("NxConnector is not connected - call Connect() first");
        }

        try
        {
            NXOpen::Session* session = m_connector->RawSession();
            NXOpen::BasePart* workPart = session->Parts()->Work();
            if (workPart == nullptr)
            {
                return OperationResult<GeometryInfo>::Fail("No Work Part is currently loaded");
            }

            GeometryInfo geometryInfo;
            geometryInfo.componentName = componentName;

            // The work part itself is typically an empty container in an
            // assembly - each named Component's real Bodies live in the
            // Part it instances, not in workPart directly (see
            // NxContracts::CollectAllBodiesInWorkPart / docs/
            // OfficeVerificationChecklist.md Phase 2-A). So: find the
            // Component matching componentName and read only its own
            // Bodies, rather than pulling every Body up front and filtering
            // by Body::OwningComponent() (which assumed workPart->Bodies()
            // already covered the whole assembly - it doesn't).
            std::vector<NXOpen::Body*> bodies;
            NXOpen::Part* part = dynamic_cast<NXOpen::Part*>(workPart);
            NXOpen::Assemblies::ComponentAssembly* assembly = (part != nullptr) ? part->ComponentAssembly() : nullptr;
            NXOpen::Assemblies::Component* root = (assembly != nullptr) ? assembly->RootComponent() : nullptr;

            if (root != nullptr)
            {
                NXOpen::Assemblies::Component* match = NxContracts::FindComponentByName(root, componentName);
                bodies = NxContracts::BodiesOfComponent(match);
            }
            else
            {
                // Non-assembly (single-part) bookmark - the work part IS
                // the part with the actual geometry.
                bodies = workPart->Bodies();
            }

            for (NXOpen::Body* body : bodies)
            {
                BodyInfo bodyInfo;
                bodyInfo.bodyId = body->JournalIdentifier();
                bodyInfo.faceCount = static_cast<int>(body->GetFaces().size());
                bodyInfo.edgeCount = static_cast<int>(body->GetEdges().size());
                bodyInfo.boundingBox = ComputeBoundingBox(body);

                geometryInfo.bodies.push_back(bodyInfo);
            }

            if (m_logger)
            {
                m_logger->Info("GeometryReader: component=" + componentName +
                                " bodies=" + std::to_string(geometryInfo.bodies.size()));
            }

            return OperationResult<GeometryInfo>::Ok(geometryInfo);
        }
        catch (const NXOpen::NXException& ex)
        {
            const std::string msg = std::string("NXException while reading geometry: ") + ex.Message();
            if (m_logger) m_logger->Error(msg);
            return OperationResult<GeometryInfo>::Fail(msg);
        }
    }

    BoundingBox3D NxGeometryReader::ComputeBoundingBox(NXOpen::Body* body)
    {
        // Shared with RoiModule's NxRoiResolver - see
        // Shared/NxContracts/NxGeometryUtils.h/.cpp for the actual UF call
        // and its TODO(office-PC verify) notes.
        return NxContracts::ComputeBoundingBoxForTag(body);
    }
}
