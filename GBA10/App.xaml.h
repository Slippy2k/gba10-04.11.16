//
// App.xaml.h
// App 类的声明。
//

#pragma once

#include "App.g.h"
#include "DirectXPage.xaml.h"
#include "Database\ROMDatabase.h"
#include "EmulatorSettings.h"
#include "live_connect.h"

namespace GBA10
{
	/// <summary>
	/// 提供特定于应用程序的行为，以补充默认的应用程序类。
	/// </summary>
	ref class App sealed
	{
	protected:
		virtual void OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^ e) override;
		virtual void OnFileActivated(Windows::ApplicationModel::Activation::FileActivatedEventArgs^ args) override;

	internal:
		App();
		static ROMDatabase^ ROMDB;
		static live::live_client* LiveClient;
		static Platform::String^ ExportFolderID;
		static bool HasAds;
		static bool IsPremium;
		static void CheckProductLicense();

	private:
		void OnSuspending(Platform::Object^ sender, Windows::ApplicationModel::SuspendingEventArgs^ e);
		void OnNavigationFailed(Platform::Object ^sender, Windows::UI::Xaml::Navigation::NavigationFailedEventArgs ^e);

		void OnResuming(Platform::Object ^sender, Platform::Object ^args);

		void LaunchApp(Windows::ApplicationModel::Activation::ApplicationExecutionState previousState,
			Windows::ApplicationModel::Activation::FileActivatedEventArgs^ args);

		DirectXPage^ m_directXPage;
	};
}
