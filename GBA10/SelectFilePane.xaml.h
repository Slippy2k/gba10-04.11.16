//
// SelectStatePane.xaml.h
// Declaration of the SelectStatePane class
//

#pragma once

#include "SelectFilePane.g.h"



namespace GBA10
{

	public delegate void FileSelectedDelegate(int selectedIndex);

	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class SelectFilePane sealed
	{
	public:
		SelectFilePane(Windows::Foundation::Collections::IVector<Platform::String^>^ list, Platform::String^ Title);
		property FileSelectedDelegate ^FileSelectedCallback;
	private:
		bool initdone;

		//Platform::Collections::Vector<Platform::String^>^ fileVector;

		void fileList_ItemClick(Platform::Object^ sender, Windows::UI::Xaml::Controls::ItemClickEventArgs^ e);
	};
}
