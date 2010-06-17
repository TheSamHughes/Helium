#include "Precompile.h"
#include "AssetPreferences.h"

using namespace Luna;

REFLECT_DEFINE_CLASS( AssetPreferences );

void AssetPreferences::EnumerateClass( Reflect::Compositor<AssetPreferences>& comp )
{
  Reflect::ElementField* elemAssetEditorWindowSettings = comp.AddField( &AssetPreferences::m_AssetEditorWindowSettings, "m_AssetEditorWindowSettings" );
  Reflect::ElementField* elemAnimGroupDlgWindowSettings = comp.AddField( &AssetPreferences::m_AnimGroupDlgWindowSettings, "m_AnimGroupDlgWindowSettings" );
  Reflect::ElementField* elemAttrChooserDlgWindowSettings = comp.AddField( &AssetPreferences::m_ComponentChooserDlgWindowSettings, "m_ComponentChooserDlgWindowSettings" );
  Reflect::Field* fieldAttrChooserTab = comp.AddField( &AssetPreferences::m_ComponentChooserTab, "m_ComponentChooserTab" );
  Reflect::EnumerationField* enumFilePathOption = comp.AddEnumerationField( &AssetPreferences::m_FilePathOption, "m_FilePathOption" );
  Reflect::ElementField* elemMRU = comp.AddField( &AssetPreferences::m_MRU, "m_MRU" );
  Reflect::Field* fieldDisplayPreviewAxis = comp.AddField( &AssetPreferences::m_DisplayPreviewAxis, "m_DisplayPreviewAxis" );
}


///////////////////////////////////////////////////////////////////////////////
// Globals, statics, etc.
///////////////////////////////////////////////////////////////////////////////

// Pointer to the global Asset Editor preferences
AssetPreferencesPtr g_AssetEditorPreferences = NULL;

// Increment this value to invalidate all previously saved preferences
const static std::string s_PreferencesVersion( "1" );

// Increment this value to invalidate just the window settings for the Asset Editor
const static std::string s_WindowSettingsVersion( "5" );

// Increment this value to invalidate just the window settings for the Anim Group dialog
const static std::string s_AnimGroupDlgVersion( "1" );

// Increment this value to invalidate just the window settings for the Anim Group dialog
const static std::string s_ComponentChooserDlgVersion( "1" );

///////////////////////////////////////////////////////////////////////////////
// Static initialization.
// 
void AssetPreferences::InitializeType()
{
  Reflect::RegisterClass<AssetPreferences>( "AssetPreferences" );

  g_AssetEditorPreferences = new AssetPreferences();
  g_AssetEditorPreferences->LoadPreferences();
}

///////////////////////////////////////////////////////////////////////////////
// Static cleanup.
// 
void AssetPreferences::CleanupType()
{
  g_AssetEditorPreferences = NULL;
  Reflect::UnregisterClass<AssetPreferences>();
}

///////////////////////////////////////////////////////////////////////////////
// Returns the global Asset Editor preferences.  You must call 
// AssetPreferences::InitializeType first.
// 
AssetPreferences* Luna::GetAssetEditorPreferences()
{
  if ( !g_AssetEditorPreferences.ReferencesObject() )
  {
    throw Nocturnal::Exception( "AssetEditorPreferences is not initialized, must call AssetPreferences::InitializeType first." );
  }

  return g_AssetEditorPreferences;
}

///////////////////////////////////////////////////////////////////////////////
// Constructor
// 
AssetPreferences::AssetPreferences()
: m_AssetEditorWindowSettings( new WindowSettings( s_WindowSettingsVersion ) )
, m_AnimGroupDlgWindowSettings( new WindowSettings( s_AnimGroupDlgVersion ) )
, m_ComponentChooserDlgWindowSettings( new WindowSettings( s_ComponentChooserDlgVersion, wxDefaultPosition, wxSize( 700, 400 ) ) )
, m_ComponentChooserTab( "" )
, m_FilePathOption( FilePathOptions::PartialPath )
, m_MRU( new MRUData() )
, m_DisplayPreviewAxis( false )
{
}

///////////////////////////////////////////////////////////////////////////////
// Overridden to verify settings after load.
// 
void AssetPreferences::PostDeserialize()
{
  __super::PostDeserialize();

  WindowSettings::CheckWindowSettings( m_AssetEditorWindowSettings, s_WindowSettingsVersion );

  WindowSettings::CheckWindowSettings( m_AnimGroupDlgWindowSettings, s_AnimGroupDlgVersion );

  if ( !m_MRU.ReferencesObject() )
  {
    m_MRU = new MRUData();
  }
}

///////////////////////////////////////////////////////////////////////////////
// Returns the current version of the preferences.  Changing this value 
// invalidates all previously saved preferences.  You can also invalidate
// separate aspects of the preferences.  See the globals section at the top
// of this file.
// 
const std::string& AssetPreferences::GetCurrentVersion() const 
{
  return s_PreferencesVersion;
}

///////////////////////////////////////////////////////////////////////////////
// Load preferences.
// 
std::string AssetPreferences::GetPreferencesPath() const
{
#pragma TODO( "reimplement" )
  return "";
}

///////////////////////////////////////////////////////////////////////////////
// This field represents the name of the tab that was last selected in the
// Component Chooser Dialog.
// 
const Reflect::Field* AssetPreferences::ComponentChooserTab() const
{
  return GetClass()->FindField( &AssetPreferences::m_ComponentChooserTab );
}

///////////////////////////////////////////////////////////////////////////////
// Returns the field for the file path option.
// 
const Reflect::Field* AssetPreferences::FilePathOption() const
{
  return GetClass()->FindField( &AssetPreferences::m_FilePathOption );
}

///////////////////////////////////////////////////////////////////////////////
// Returns the Asset Editor's window settings.
// 
WindowSettings* AssetPreferences::GetAssetEditorWindowSettings()
{
  return m_AssetEditorWindowSettings.Ptr();
}

///////////////////////////////////////////////////////////////////////////////
// Returns the Animation Group Dialog's window settings.
// 
WindowSettings* AssetPreferences::GetAnimGroupDlgWindowSettings()
{
  return m_AnimGroupDlgWindowSettings.Ptr();
}

///////////////////////////////////////////////////////////////////////////////
// Returns the Component Chooser Dialog's window settings.
// 
WindowSettings* AssetPreferences::GetComponentChooserDlgWindowSettings()
{
  return m_ComponentChooserDlgWindowSettings.Ptr();
}

///////////////////////////////////////////////////////////////////////////////
// Returns the MRU settings.
// 
MRUData* AssetPreferences::GetMRU()
{
  return m_MRU;
}

///////////////////////////////////////////////////////////////////////////////
// Returns the display Axis settings.
// 
bool AssetPreferences::DisplayPreviewAxis() const
{
  return m_DisplayPreviewAxis;
}

///////////////////////////////////////////////////////////////////////////////
// Sets the display Axis settings and raises an event if it changed.
// 
void AssetPreferences::SetDisplayPreviewAxis( bool display )
{
  if ( m_DisplayPreviewAxis != display )
  {
    m_DisplayPreviewAxis = display;
    RaiseChanged( DisplayPreviewAxisField() );
  }
}

///////////////////////////////////////////////////////////////////////////////
// Gets the display preview Axis field
// 
const Reflect::Field* AssetPreferences::DisplayPreviewAxisField() const
{
  return GetClass()->FindField( &AssetPreferences::m_DisplayPreviewAxis );
}
