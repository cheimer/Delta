#pragma once
#include "Engine/StreamableManager.h"

namespace DeltaLoad
{
	template<typename T>
	bool LoadComplete(const TSharedPtr<FStreamableHandle>& Handle, const TSoftClassPtr<T>& ClassPtr)
	{
		if (!Handle.IsValid() || !ClassPtr.IsValid()) return false;

		if (Handle->IsLoadingInProgress())
		{
			Handle->WaitUntilComplete();
		}

		return ClassPtr.Get() ? true : false;
	}

	template<typename T>
	bool LoadComplete(const TSharedPtr<FStreamableHandle>& Handle, const TSoftObjectPtr<T>& ObjectPtr)
	{
		if (!Handle.IsValid() || !ObjectPtr.IsValid()) return false;

		if (Handle->IsLoadingInProgress())
		{
			Handle->WaitUntilComplete();
		}

		return ObjectPtr.Get() ? true : false;
	}
}
