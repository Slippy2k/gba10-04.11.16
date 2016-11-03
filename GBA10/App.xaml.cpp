//
// App.xaml.cpp
// App 类的实现。
//

#include "pch.h"
#include "App.xaml.h"
#include "DirectXPage.xaml.h"
#include "EmulatorSettings.h"
#include "live_connect.h"

using namespace GBA10;

using namespace Platform;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::UI::ViewManagement;
using namespace Windows::ApplicationModel::Store;
using namespace Windows::System::Profile;

ROMDatabase^ GBA10::App::ROMDB = nullptr;
live::live_client* GBA10::App::LiveClient = nullptr;
Platform::String^ GBA10::App::ExportFolderID = "";
bool GBA10::App::HasAds = true;
bool GBA10::App::IsPremium = false;

/// <summary>
/// 初始化单一实例应用程序对象。这是执行的创作代码的第一行，
/// 已执行，逻辑上等同于 main() 或 WinMain()。
/// </summary>
App::App()
{
	//determine theme
	if (EmulatorSettings::Current->Theme == 0)
		this->RequestedTheme = ApplicationTheme::Light;
	else if (EmulatorSettings::Current->Theme == 1)
		this->RequestedTheme = ApplicationTheme::Dark;

	InitializeComponent();
	
	AnalyticsVersionInfo^ deviceFamily = AnalyticsInfo::VersionInfo;
	if (deviceFamily->DeviceFamily == "Windows.Xbox")
		RequiresPointerMode = ApplicationRequiresPointerMode::WhenRequested;

	Suspending += ref new SuspendingEventHandler(this, &App::OnSuspending);
	Resuming += ref new EventHandler<Object^>(this, &App::OnResuming);

	ROMDB = ref new ROMDatabase();
	LiveClient = new live::live_client();
}

void App::CheckProductLicense()
{
	HasAds = false;
	IsPremium = true;

	////bool IsActive = CurrentApp::LicenseInformation->IsActive;
	//bool IsTrial = CurrentApp::LicenseInformation->IsTrial;
	////auto expiredate = CurrentApp::LicenseInformation->ExpirationDate.UniversalTime;

	//if (CurrentApp::LicenseInformation->ProductLicenses->Lookup("noads_premium")->IsActive)
	//{
	//	HasAds = false;
	//	IsPremium = true;
	//	return; //no need to check for other 2 licenses
	//}

	//if (CurrentApp::LicenseInformation->ProductLicenses->Lookup("removeads")->IsActive)
	//	HasAds = false;

	//if (CurrentApp::LicenseInformation->ProductLicenses->Lookup("premiumfeatures")->IsActive)
	//	IsPremium = true;


}

/// <summary>
/// 在最终用户正常启动应用程序时调用。	将在启动应用程序
/// 将在启动应用程序以打开特定文件等情况下使用。
/// </summary>
/// <param name="e">有关启动请求和过程的详细信息。</param>
void App::OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^ e)
{
	Windows::UI::ViewManagement::ApplicationView::GetForCurrentView()->SetDesiredBoundsMode
	(Windows::UI::ViewManagement::ApplicationViewBoundsMode::UseCoreWindow);

	LaunchApp(e->PreviousExecutionState, nullptr);
}

void App::LaunchApp(ApplicationExecutionState previousState, FileActivatedEventArgs^ args)
{
#if _DEBUG
	if (IsDebuggerPresent())
	{
		//DebugSettings->EnableFrameRateCounter = true;
	}
#endif

	//change minimum suze to 320 px
	Windows::Foundation::Size minsize = { 320.0f, 320.0f };
	ApplicationView::GetForCurrentView()->SetPreferredMinSize(minsize);

	//apply theme to title bar
	auto titleBar = ApplicationView::GetForCurrentView()->TitleBar;

	// set up our brushes
	SolidColorBrush^ bkgColor = (SolidColorBrush^)(Current->Resources->Lookup("TitleBarBackgroundThemeBrush"));
	SolidColorBrush^ btnHoverColor = (SolidColorBrush^)(Current->Resources->Lookup("TitleBarButtonHoverThemeBrush"));
	SolidColorBrush^ btnPressedColor = (SolidColorBrush^)(Current->Resources->Lookup("TitleBarButtonPressedThemeBrush"));
	Windows::UI::Color foregroundColor = (Windows::UI::Color)(Current->Resources->Lookup("TitleBarForegroundColor"));
	Windows::UI::Color inactiveForegroundColor = (Windows::UI::Color)(Current->Resources->Lookup("TitleBarInactiveForegroundColor"));

	// override colors!
	titleBar->BackgroundColor = bkgColor->Color;
	titleBar->ForegroundColor = foregroundColor;
	titleBar->ButtonBackgroundColor = bkgColor->Color;
	titleBar->ButtonForegroundColor = foregroundColor;
	titleBar->ButtonHoverBackgroundColor = btnHoverColor->Color;
	titleBar->ButtonHoverForegroundColor = foregroundColor;
	titleBar->ButtonPressedBackgroundColor = btnPressedColor->Color;
	titleBar->ButtonPressedForegroundColor = foregroundColor;

	titleBar->InactiveBackgroundColor = bkgColor->Color;
	titleBar->InactiveForegroundColor = inactiveForegroundColor;
	titleBar->ButtonInactiveBackgroundColor = bkgColor->Color;
	titleBar->ButtonInactiveForegroundColor = inactiveForegroundColor;



	//check license
	CheckProductLicense();

	App::ROMDB->Initialize().then([this, previousState, args] {
		if (m_directXPage == nullptr)
		{
			m_directXPage = ref new DirectXPage();
		}


		if (previousState == ApplicationExecutionState::Terminated)
		{
			m_directXPage->LoadInternalState(ApplicationData::Current->LocalSettings->Values);
		}

		// Place the page in the current window and ensure that it is active.
		Window::Current->Content = m_directXPage;
		Window::Current->Activate();

		//import file 
		if (args != nullptr)
		{
			m_directXPage->ImportRomFromFile(args);
		}
	}, task_continuation_context::use_current());
}

void App::OnFileActivated(FileActivatedEventArgs^ args)
{
	LaunchApp(args->PreviousExecutionState, args);



}

/// <summary>
/// 在挂起应用程序执行时调用。	保存应用程序状态
/// 无需知道应用程序会被终止还是会恢复，
/// 并让内存内容保持不变。
/// </summary>
/// <param name="sender">挂起的请求的源。</param>
/// <param name="e">有关挂起请求的详细信息。</param>
void App::OnSuspending(Object^ sender, SuspendingEventArgs^ e)
{
	(void)sender;	// Unused parameter

	auto deferral = e->SuspendingOperation->GetDeferral();

	create_task(m_directXPage->SaveInternalState(ApplicationData::Current->LocalSettings->Values)).then([deferral]
	{
		deferral->Complete();
	});
}

/// <summary>
/// Invoked when application execution is being resumed.
/// </summary>
/// <param name="sender">The source of the resume request.</param>
/// <param name="args">Details about the resume request.</param>
void App::OnResuming(Object ^sender, Object ^args)
{
	(void)sender; // Unused parameter
	(void)args; // Unused parameter

	m_directXPage->LoadInternalState(ApplicationData::Current->LocalSettings->Values);
}

/// <summary>
/// 导航到特定页失败时调用
/// </summary>
///<param name="sender">导航失败的框架</param>
///<param name="e">有关导航失败的详细信息</param>
void App::OnNavigationFailed(Platform::Object ^sender, Windows::UI::Xaml::Navigation::NavigationFailedEventArgs ^e)
{
	throw ref new FailureException("Failed to load Page " + e->SourcePageType.Name);
}