// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Options/KeyRemapScreenWidget.h"

#include "CommonInputSubsystem.h"
#include "CommonRichTextBlock.h"
#include "CommonUITypes.h"
#include "ICommonInputModule.h"
#include "Framework/Application/IInputProcessor.h"

DECLARE_DELEGATE_OneParam(FOnInputPreProcessorKeyPressed, const FKey& /*PressedKey*/)
DECLARE_DELEGATE_OneParam(FOnInputPreProcessorKeySelectCanceled, const FString& /*CanceledReason*/)

class FKeyRemapScreenInputProcessor : public IInputProcessor
{
public:
	FKeyRemapScreenInputProcessor(ECommonInputType InInputTypeToListenTo, ULocalPlayer* InLocalPlayer)
	{
		CachedInputTypeToListenTo = InInputTypeToListenTo;
		CachedLocalPlayer = InLocalPlayer;
	}

	FOnInputPreProcessorKeyPressed OnInputPreProcessorKeyPressed;
	FOnInputPreProcessorKeySelectCanceled OnInputPreProcessorKeySelectCanceled;
	
protected:
	virtual void Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor) override
	{
		
	}

	virtual bool HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override
	{
		ProcessPressedKey(InKeyEvent.GetKey());
		
		return true;
	}

	virtual bool HandleMouseButtonDownEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override
	{
		ProcessPressedKey(MouseEvent.GetEffectingButton());
		
		return true;
	}

	void ProcessPressedKey(const FKey& InPressedKey)
	{
		if (InPressedKey == EKeys::Escape)
		{
			OnInputPreProcessorKeySelectCanceled.ExecuteIfBound(TEXT("Cancel Key Remap"));
			return;
		}

		UCommonInputSubsystem* CommonInputSubsystem = UCommonInputSubsystem::Get(CachedLocalPlayer.Get());
		check(CommonInputSubsystem);

		ECommonInputType CurrentInputType = CommonInputSubsystem->GetCurrentInputType();

		switch (CachedInputTypeToListenTo)
		{
		case ECommonInputType::MouseAndKeyboard:
			if (InPressedKey.IsGamepadKey() || CurrentInputType == ECommonInputType::Gamepad)
			{
				OnInputPreProcessorKeySelectCanceled.ExecuteIfBound(TEXT("It's for Keyboard & Mouse"));
				return;
			}
			break;
		case ECommonInputType::Gamepad:
			/**
			 *  over 5.4 engine, when click confirm button(A Button) call mouse left button too.
			 *  so, if using Gamepad but LeftMouseButton clicked, find InputActionData.
			 */
			if (CurrentInputType == ECommonInputType::Gamepad && InPressedKey == EKeys::LeftMouseButton)
			{
				FCommonInputActionDataBase* InputActionData = ICommonInputModule::GetSettings().GetDefaultClickAction().GetRow<FCommonInputActionDataBase>(TEXT(""));
				check(InputActionData);
				
				OnInputPreProcessorKeyPressed.ExecuteIfBound(InputActionData->GetDefaultGamepadInputTypeInfo().GetKey());

				return;
			}
			
			if (!InPressedKey.IsGamepadKey())
			{
				OnInputPreProcessorKeySelectCanceled.ExecuteIfBound(TEXT("It's for Gamepad"));
				return;
			}
			break;
		default:
			break;
		}

		OnInputPreProcessorKeyPressed.ExecuteIfBound(InPressedKey);
	}

private:
	ECommonInputType CachedInputTypeToListenTo;

	TWeakObjectPtr<ULocalPlayer> CachedLocalPlayer;
	
};

void UKeyRemapScreenWidget::NativeOnActivated()
{
	Super::NativeOnActivated();

	CachedInputPreProcessor = MakeShared<FKeyRemapScreenInputProcessor>(CachedDesiredInputType, GetOwningLocalPlayer());
	CachedInputPreProcessor->OnInputPreProcessorKeyPressed.BindUObject(this, &ThisClass::OnValidKeyPressedDetected);
	CachedInputPreProcessor->OnInputPreProcessorKeySelectCanceled.BindUObject(this, &ThisClass::OnKeySelectCanceled);

	FSlateApplication::Get().RegisterInputPreProcessor(CachedInputPreProcessor, -1);

	FString InputDeviceName = "";

	switch (CachedDesiredInputType)
	{
	case ECommonInputType::MouseAndKeyboard:
		InputDeviceName = TEXT("Mouse & Keyboard");
		break;
	case ECommonInputType::Gamepad:
		InputDeviceName = TEXT("Gamepad");
		break;
	default:
		break;
	}

	const FString DisplayRichMessage = FString::Printf(
		TEXT("<KeyRemapDefault>Press any</> <KeyRemapHighlight>%s</> <KeyRemapDefault>Key</>"), *InputDeviceName);

	RemapMessageCommonRichText->SetText(FText::FromString(DisplayRichMessage));
	
}

void UKeyRemapScreenWidget::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();

	if (CachedInputPreProcessor)
	{
		FSlateApplication::Get().UnregisterInputPreProcessor(CachedInputPreProcessor);

		CachedInputPreProcessor.Reset();
	}
	
}

void UKeyRemapScreenWidget::OnValidKeyPressedDetected(const FKey& PressedKey)
{
	RequestDeactivateWidget(
		[this, PressedKey]()
		{
			OnKeyRemapScreenKeyPressed.ExecuteIfBound(PressedKey);
		}
	);
}

void UKeyRemapScreenWidget::OnKeySelectCanceled(const FString& CanceledReason)
{
	RequestDeactivateWidget(
		[this, CanceledReason]()
		{
			OnKeyRemapScreenKeySelectCanceled.ExecuteIfBound(CanceledReason);
		}
	);
}

void UKeyRemapScreenWidget::RequestDeactivateWidget(TFunction<void()> PreDeactivateCallback)
{
	FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda(
		[PreDeactivateCallback, this](float DeltaTime)->bool
		{
			PreDeactivateCallback();

			DeactivateWidget();

			return false;
		}
	));
}

void UKeyRemapScreenWidget::SetDesiredInputTypeToFilter(ECommonInputType InDesiredInputType)
{
	CachedDesiredInputType = InDesiredInputType;
}
