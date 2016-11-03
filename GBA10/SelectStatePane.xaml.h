//
// SelectStatePane.xaml.h
// Declaration of the SelectStatePane class
//

#pragma once

#include "SelectStatePane.g.h"

using namespace Windows::Foundation::Collections;

namespace GBA10
{


	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class SelectStatePane sealed
	{
	public:
		SelectStatePane(int selectedSlot);
	private:
		bool initdone;

		IVector<Platform::String ^> ^savestateVector;

		void Init(void);

		void savestateList_ItemClick(Platform::Object^ sender, Windows::UI::Xaml::Controls::ItemClickEventArgs^ e);
	};
}
