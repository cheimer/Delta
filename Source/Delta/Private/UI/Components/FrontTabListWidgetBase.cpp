// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Components/FrontTabListWidgetBase.h"

#include "Editor/WidgetCompilerLog.h"
#include "UI/Components/FrontCommonButtonBase.h"

void UFrontTabListWidgetBase::RequestRegisterTab(const FName& InTabID, const FText& InTabDisplayName)
{
	RegisterTab(InTabID, TabButtonEntryWidgetClass, nullptr);

	if (UFrontCommonButtonBase* FoundButton = Cast<UFrontCommonButtonBase>(GetTabButtonBaseByID(InTabID)))
	{
		FoundButton->SetButtonText(InTabDisplayName);
	}
}

#if WITH_EDITOR	
void UFrontTabListWidgetBase::ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledDefaults(CompileLog);

	if (!TabButtonEntryWidgetClass)
	{
		CompileLog.Error(FText::FromString("TabButtonEntryWidgetClass is not valid. " + GetClass()->GetName() + " need valid entry widget class"));
	}
}
#endif
