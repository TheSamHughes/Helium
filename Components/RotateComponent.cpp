
#include "ComponentsPch.h"
#include "Components/RotateComponent.h"

#include "Components/TransformComponent.h"

#include "Ois/OisSystem.h"

HELIUM_IMPLEMENT_ASSET(Helium::RotateComponentDefinition, Components, 0);

void Helium::RotateComponentDefinition::PopulateComposite( Reflect::Composite& comp )
{
	//comp.AddField(&RotateComponentDefinition::m_RotateBy, "m_TestValue");
}

HELIUM_DEFINE_COMPONENT(Helium::RotateComponent, 16);

void Helium::RotateComponent::PopulateComposite( Reflect::Composite& comp )
{

}

void Helium::RotateComponent::ApplyRotation( TransformComponent *pTransform )
{
    HELIUM_ASSERT(pTransform);

    float fYawChange = 0.0f;
    bool bHasInput = false;

    if (Helium::Input::IsKeyDown(Input::KeyCodes::KC_LEFT))
    {
        fYawChange += 0.1f;
        bHasInput = true;
    }

    if (Helium::Input::IsKeyDown(Input::KeyCodes::KC_RIGHT))
    {
        fYawChange -= 0.1f;
        bHasInput = true;
    }

    if (!bHasInput)
    {
        fYawChange = 0.005f;
    }
        
    Simd::Quat rotation( 0.0f, fYawChange, 0.0f );
    pTransform->SetRotation( pTransform->GetRotation() * rotation );
}