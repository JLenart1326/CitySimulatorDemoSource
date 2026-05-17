
#include "UserInterface/CDMainHUD.h"
#include <Blueprint/UserWidget.h>
#include <Components/CanvasPanel.h>
#include <Components/Border.h>
#include <Components/BackgroundBlur.h>
#include <Animation/WidgetAnimation.h>
#include "CitySimulatorDemo.h"

DEFINE_LOG_CATEGORY_STATIC(LogCitySimulatorDemoHUD, Log, All);

void UCDMainHUD::PlayFadeInAnimation()
{
	if (!FadeIn) return;

	if (this->IsAnyAnimationPlaying()) {
		UE_LOG(LogCitySimulatorDemoHUD, Log, TEXT("PlayFadeInAnimation: Some Animation is already playing, skipping fade in"));
		return;
	}

	UnbindAllFromAnimationFinished(FadeIn);
	
	FWidgetAnimationDynamicEvent FadeInFinishedEvent;
	FadeInFinishedEvent.BindDynamic(this, &UCDMainHUD::OnFadeInFinishedInternal);
	BindToAnimationFinished(FadeIn, FadeInFinishedEvent);

	PlayAnimationForward(FadeIn, 1.f, false);
}

void UCDMainHUD::PlayFadeOutAnimation()
{
	if (this->IsAnyAnimationPlaying()) {
		UE_LOG(LogCitySimulatorDemoHUD, Log, TEXT("PlayFadeOutAnimation: Some Animation is already playing, skipping fade out"));
		return;
	}

	UnbindAllFromAnimationFinished(FadeOut);

	FWidgetAnimationDynamicEvent FadeOutFinishedEvent;
	FadeOutFinishedEvent.BindDynamic(this, &UCDMainHUD::OnFadeOutFinishedInternal);
	BindToAnimationFinished(FadeOut, FadeOutFinishedEvent);

	PlayAnimationForward(FadeOut, 1.f, false);
}

void UCDMainHUD::OnFadeInFinishedInternal()
{
	OnFadeInAnimationFinished.Broadcast();
	this->SetCoverState(true);

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		this,
		&UCDMainHUD::PlayFadeOutAnimation,
		1.0f,
		false
	);
}

void UCDMainHUD::OnFadeOutFinishedInternal()
{
	OnFadeOutAnimationFinished.Broadcast();
	this->SetCoverState(false);
}

void UCDMainHUD::NativeConstruct()
{
	Super::NativeConstruct();

	SetCoverState(bCoveredOnStartup);
}

void UCDMainHUD::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UCDMainHUD::SetCoverState(bool bIsCovered)
{
	BlurContainer->SetBlurStrength(bIsCovered ? 100.f : 0.f);
	FadeContainer->SetBrushColor(FLinearColor(0.f, 0.f, 0.f, bIsCovered ? 1.f : 0.f));
}
