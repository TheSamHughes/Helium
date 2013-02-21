#include "EditorPch.h"

#include "EditorEngine.h"

#include "RenderingD3D9/D3D9Renderer.h"
#include "Graphics/DynamicDrawer.h"
#include "Framework/WorldManager.h"
#include "Reflect/Object.h"


using namespace Helium;
using namespace Helium::Editor;


EditorEngine::EditorEngine()
: m_SceneManager( NULL )
{
}

EditorEngine::~EditorEngine()
{

}

bool EditorEngine::Initialize( SceneGraph::SceneManager* sceneManager, HWND hwnd )
{
    HELIUM_ASSERT( sceneManager );

    m_SceneManager = sceneManager;

    InitRenderer( hwnd );

    m_SceneManager->e_SceneAdded.AddMethod( this, &EditorEngine::OnSceneAdded );
    m_SceneManager->e_SceneRemoving.AddMethod( this, &EditorEngine::OnSceneRemoving );

    return true;
}

void EditorEngine::Shutdown()
{
    m_SceneManager->e_SceneAdded.RemoveMethod( this, &EditorEngine::OnSceneAdded );
    m_SceneManager->e_SceneRemoving.RemoveMethod( this, &EditorEngine::OnSceneRemoving );

    m_PrimaryWorldProxy.Release();

    DynamicDrawer::DestroyStaticInstance();
    RenderResourceManager::DestroyStaticInstance();
    Renderer::DestroyStaticInstance();
}

void EditorEngine::InitRenderer( HWND hwnd )
{
    HELIUM_VERIFY( D3D9Renderer::CreateStaticInstance() );

    Renderer* pRenderer = Renderer::GetStaticInstance();
    HELIUM_ASSERT( pRenderer );
    pRenderer->Initialize();

    Renderer::ContextInitParameters mainCtxInitParams;
    mainCtxInitParams.pWindow = hwnd;
    mainCtxInitParams.bFullscreen = false;
    mainCtxInitParams.bVsync = true;
    mainCtxInitParams.displayWidth = 64;
    mainCtxInitParams.displayHeight = 64;

    HELIUM_VERIFY( pRenderer->CreateMainContext( mainCtxInitParams ) );

    RenderResourceManager& rRenderResourceManager = RenderResourceManager::GetStaticInstance();
    rRenderResourceManager.Initialize();
    rRenderResourceManager.UpdateMaxViewportSize( wxSystemSettings::GetMetric(wxSYS_SCREEN_X), wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) );

    HELIUM_VERIFY( DynamicDrawer::GetStaticInstance().Initialize() );
}

void EditorEngine::OnViewCanvasPaint()
{
    WorldManager& rWorldManager = WorldManager::GetStaticInstance();
    rWorldManager.Update();
}

Reflect::ObjectPtr EditorEngine::CreateProxyFor( SceneGraph::Scene* scene )
{
    HELIUM_ASSERT( scene->GetType() == SceneGraph::Scene::SceneTypes::World );

    switch ( scene->GetType() )
    {
        case SceneGraph::Scene::SceneTypes::World:
            return CreateWorldProxy( scene );
        default:
            return NULL;
    }
}

WorldProxyPtr EditorEngine::CreateWorldProxy( SceneGraph::Scene* scene )
{
    Package* pWorldDefinitionPackage = WorldManager::GetStaticInstance().GetWorldDefinitionPackage();

    tstring newWorldDefaultNameString( TXT( "NewWorld" ) );
    Name newWorldName( newWorldDefaultNameString.c_str() );
    int attempt = 1;
    do
    {
        if ( ! pWorldDefinitionPackage->FindChild( newWorldName ) )
        {
            break;
        }

        tstringstream newWorldNameStringStream;
        newWorldNameStringStream << newWorldDefaultNameString << TXT("_") << attempt;
        tstring newWorldNameString = newWorldNameStringStream.str();
        newWorldName = Name( newWorldNameString.c_str() );

        ++attempt;
    } while (attempt < 100);

    WorldDefinitionPtr spWorldDefinition;
    bool success = WorldDefinition::Create( spWorldDefinition, newWorldName, WorldManager::GetStaticInstance().GetWorldDefinitionPackage() );

    if (!success)
    {
        wxMessageBox( TXT( "Failed to create new world." ) );
        return NULL;
    }

    HELIUM_ASSERT( spWorldDefinition );

    scene->SetDefinition( spWorldDefinition );

    //////////////////////////////

    WorldProxyPtr spWorldProxy = Reflect::AssertCast<WorldProxy>( WorldProxy::CreateObject() );
    HELIUM_ASSERT( spWorldProxy );
    spWorldProxy->Initialize( spWorldDefinition );

    scene->SetProxy( spWorldProxy );

    return spWorldProxy;
}

void EditorEngine::OnSceneAdded( const SceneGraph::SceneChangeArgs& args )
{
    SceneGraph::Scene* scene = args.m_Scene;

    if ( m_SceneToProxyMap.Find( scene ) == m_SceneToProxyMap.End() )
    {
        m_SceneToProxyMap[scene] = CreateProxyFor( scene );
//      m_SceneToDefinitionMap[scene] = 
    }
    else
    {
        HELIUM_ASSERT( false );
    }
}

void EditorEngine::OnSceneRemoving( const SceneGraph::SceneChangeArgs& args )
{
    SceneGraph::Scene* scene = args.m_Scene;

    HELIUM_VERIFY( m_SceneToProxyMap.Remove( scene ) );
//  HELIUM_VERIFY( m_SceneToDefinitionMap.Remove( scene ) );
}