// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "UMGPrivatePCH.h"

#define LOCTEXT_NAMESPACE "UMG"

/////////////////////////////////////////////////////
// UButtonComponent

UButtonComponent::UButtonComponent(const FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	SButton::FArguments ButtonDefaults;

	ButtonStyle = NULL;
	HorizontalAlignment = ButtonDefaults._HAlign;
	VerticalAlignment = ButtonDefaults._VAlign;
	ContentPadding = ButtonDefaults._ContentPadding.Get();

	bDisplayText = true;
	ButtonText = LOCTEXT("ButtonDefaultValue", "This is a button");
	ButtonTextStyle = NULL;

	DesiredSizeScale = ButtonDefaults._DesiredSizeScale.Get();
	ContentScale = ButtonDefaults._ContentScale.Get();

	ButtonColorAndOpacity = ButtonDefaults._ButtonColorAndOpacity.Get();
	ForegroundColor = ButtonDefaults._ForegroundColor.Get();
}

TSharedRef<SWidget> UButtonComponent::RebuildWidget()
{
	SButton::FArguments ButtonDefaults;

	const FButtonStyle* StylePtr = (ButtonStyle != NULL) ? ButtonStyle->GetStyle<FButtonStyle>() : NULL;
	if (StylePtr == NULL)
	{
		StylePtr = ButtonDefaults._ButtonStyle;
	}

	const FTextBlockStyle* TextStylePtr = ( ButtonTextStyle != NULL ) ? ButtonTextStyle->GetStyle<FTextBlockStyle>() : NULL;
	if ( TextStylePtr == NULL )
	{
		TextStylePtr = ButtonDefaults._TextStyle;
	}

	if (bDisplayText)
	{
		TextAttribute = BIND_UOBJECT_ATTRIBUTE(FText, GetButtonText);
	}
	else
	{
		TAttribute<FText> NotSet;
		TextAttribute = NotSet;
	}

	TSharedRef<SButton> NewButton = SNew(SButton)
		.ButtonStyle(StylePtr)
		.TextStyle(TextStylePtr)
		.HAlign(HorizontalAlignment)
		.VAlign(VerticalAlignment)
		.ContentPadding(BIND_UOBJECT_ATTRIBUTE(FMargin, GetContentPadding))
		.Text(TextAttribute)
		.OnClicked(BIND_UOBJECT_DELEGATE(FOnClicked, SlateOnClickedCallback))
		.DesiredSizeScale(DesiredSizeScale)
		.ContentScale(ContentScale)
		.ButtonColorAndOpacity(BIND_UOBJECT_ATTRIBUTE(FSlateColor, GetButtonColor))
		.ForegroundColor(BIND_UOBJECT_ATTRIBUTE(FSlateColor, GetForegroundColor));

	if ( Content )
	{
		NewButton->SetContent(Content->GetWidget());
	}
	
	MyButton = NewButton;
	return NewButton;
}

void UButtonComponent::SetContent(USlateWrapperComponent* InContent)
{
	Super::SetContent(InContent);

	TSharedPtr<SButton> Button = MyButton.Pin();
	if (Button.IsValid())
	{
		if ( InContent )
		{
			Button->SetContent(InContent->GetWidget());
		}
		else
		{
			// Removed a child; replace the text if desired
			if (bDisplayText)
			{
				Button->SetContent(SNew(STextBlock)
					.Text(TextAttribute)
					.TextStyle( ButtonTextStyle ) );
			}
			else
			{
				Button->SetContent(SNullWidget::NullWidget);
			}
		}
	}
}

FMargin UButtonComponent::GetContentPadding() const
{
	return ContentPadding;
}

FText UButtonComponent::GetButtonText() const
{
	return ButtonText;
}

FText UButtonComponent::GetText_Implementation() const
{
	return ButtonText;
}

FReply UButtonComponent::SlateOnClickedCallback()
{
	OnClicked.Broadcast();
	return FReply::Handled();
}

FSlateColor UButtonComponent::GetButtonColor() const
{
	return ButtonColorAndOpacity;
}

FSlateColor UButtonComponent::GetForegroundColor() const
{
	return ForegroundColor;
}

/////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE
