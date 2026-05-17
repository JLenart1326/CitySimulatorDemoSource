#pragma once

#include "CoreMinimal.h"
#include <Blueprint/UserWidget.h>
#include "CDMainHUD.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFadeAnimationFinished);

class UCanvasPanel;
class UBorder;
class UBackgroundBlur;
class UWidgetAnimation;
/**
 * Main UI Widget that handles Fade animations
 */
UCLASS()
class CITYSIMULATORDEMO_API UCDMainHUD : public UUserWidget
{
	GENERATED_BODY()

protected:
	//Sub widgets
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> MainCanvasPanel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> FadeContainer;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBackgroundBlur> BlurContainer;

	//Animations
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FadeIn;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FadeOut;

public:
	//Delegates
	UPROPERTY(BlueprintAssignable, Category = "UI|Animation")
	FOnFadeAnimationFinished OnFadeInAnimationFinished;

	UPROPERTY(BlueprintAssignable, Category = "UI|Animation")
	FOnFadeAnimationFinished OnFadeOutAnimationFinished;

public:
	bool bCoveredOnStartup = false;

private:
	UFUNCTION()
	void OnFadeInFinishedInternal();

	UFUNCTION()
	void OnFadeOutFinishedInternal();

public:
	UFUNCTION(BlueprintCallable, Category = "UI|Animation")
	void PlayFadeInAnimation();

	UFUNCTION(BlueprintCallable, Category = "UI|Animation")
	void PlayFadeOutAnimation();

protected:
	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;

public:
	void SetCoverState(bool bIsCovered);

	
};
