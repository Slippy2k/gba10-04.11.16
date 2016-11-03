//
// SelectStatePane.xaml.cpp
// Implementation of the SelectStatePane class
//

#include "pch.h"
#include "SelectFilePane.xaml.h"
#include "DirectXPage.xaml.h"

using namespace GBA10;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Platform::Collections;

// The User Control item template is documented at http://go.microsoft.com/fwlink/?LinkId=234236


SelectFilePane::SelectFilePane(IVector<Platform::String^>^ list, Platform::String^ title):initdone(false)
{
	InitializeComponent();
	//this->fileVector = ref new Vector<Platform::String^>();
	
	
	//for (int i = 0; i < list->Size; i++)
	//	this->fileVector->Append(list->GetAt(i));
	this->txtTitle->Text = title;

	this->FileListvs->Source = list;
	//this->fileList->SelectedItem = nullptr;
	if (fileList->Items->Size > 0)
	{
		fileList->SelectedIndex = 0;
		//fileList->SelectedItem = romList->Items->GetAt(0);
	}

	if (list->Size > 0)
	{
		this->fileList->Visibility = Windows::UI::Xaml::Visibility::Visible;
		this->txtNoFile->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	}
	else
	{
		this->fileList->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		this->txtNoFile->Visibility = Windows::UI::Xaml::Visibility::Visible;
	}
	initdone = true;
}


void SelectFilePane::fileList_ItemClick(Platform::Object^ sender, Windows::UI::Xaml::Controls::ItemClickEventArgs^ e)
{
	if (initdone)
	{
		//StorageFile ^file = (StorageFile ^)(this->fileList->SelectedItem);
		int selectedIndex = this->fileList->SelectedIndex;

		//close the pane
		auto dp = this->Parent;
		Popup^ pop = (Popup^)dp;
		pop->IsOpen = false;

		//return the file to whatever windows that call it
		if (this->FileSelectedCallback)
			FileSelectedCallback(selectedIndex);


	}
}
